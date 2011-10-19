

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include "wxraytracer.h"
#include "main.xpm"
#include "bg.xpm"

DEFINE_EVENT_TYPE(wxEVT_THREAD)

/******************************************************************************/
/********************* wxraytracerapp *****************************************/
/******************************************************************************/


BEGIN_EVENT_TABLE(wxraytracerapp, wxApp)
END_EVENT_TABLE()

IMPLEMENT_APP(wxraytracerapp)

bool wxraytracerapp::OnInit()
{
   wxInitAllImageHandlers();

   
   frame = new wxraytracerFrame(wxPoint(200,200), wxSize(700,700)); 
   frame->Centre();
   frame->Show(TRUE);
   SetTopWindow(frame);
   return TRUE;
}

int wxraytracerapp::OnExit()
{
   return 0;
}

void wxraytracerapp::SetStatusText(const wxString&  text, int number)
{
   frame->SetStatusText(text, number);
}

/******************************************************************************/
/********************* wxraytracerFrame ***************************************/
/******************************************************************************/

BEGIN_EVENT_TABLE( wxraytracerFrame, wxFrame )     
   EVT_MENU( Menu_Render_Start, wxraytracerFrame::OnRenderStart )
   EVT_MENU( Menu_Render_Pause, wxraytracerFrame::OnRenderPause )
   EVT_MENU( Menu_Render_Resume, wxraytracerFrame::OnRenderResume )
   EVT_MENU( Menu_File_Save, wxraytracerFrame::OnSaveFile )
   EVT_MENU( Menu_File_Open, wxraytracerFrame::OnOpenFile )
   EVT_MENU( Menu_File_Quit, wxraytracerFrame::OnQuit )
   EVT_MENU( Menu_Thread_Default, wxraytracerFrame::OnThreadDefault )
   EVT_MENU( Menu_Thread_Single, wxraytracerFrame::OnThreadSingle )
   EVT_MENU( Menu_Thread_Dual, wxraytracerFrame::OnThreadDual )
   EVT_MENU( Menu_Thread_Quad, wxraytracerFrame::OnThreadQuad )
   EVT_MENU( Menu_Division_Default, wxraytracerFrame::OnDivisionDefault )
   EVT_MENU( Menu_Division_Single, wxraytracerFrame::OnDivisionSingle )
   EVT_MENU( Menu_Division_Dual, wxraytracerFrame::OnDivisionDual )
   EVT_MENU( Menu_Division_Quad, wxraytracerFrame::OnDivisionQuad )
   EVT_MENU( Menu_Division_64, wxraytracerFrame::OnDivision64 )
   EVT_COMMAND(ID_RENDER_COMPLETED, wxEVT_RENDER, wxraytracerFrame::OnRenderCompleted)
END_EVENT_TABLE()

wxraytracerFrame::wxraytracerFrame(const wxPoint& pos, const wxSize& size)
   : wxFrame((wxFrame *)NULL, -1, wxT( "Ray Tracer" ), pos, size)
{
   wxMenu *menuFile = new wxMenu;
   
   menuFile->Append(Menu_File_Open, wxT("&Open..."   ));
   menuFile->Append(Menu_File_Save, wxT("&Save As..."));
   menuFile->AppendSeparator();
   menuFile->Append(Menu_File_Quit, wxT("E&xit"));
   
   menuFile->Enable(menuFile->FindItem(wxT("&Save As...")), FALSE);
   
   wxMenu *menuRender = new wxMenu;
   
   menuRender->Append(Menu_Render_Start , wxT("&Start" ));
   menuRender->Append(Menu_Render_Pause , wxT("&Pause" ));
   menuRender->Append(Menu_Render_Resume, wxT("&Resume"));
   
   menuRender->Enable(menuRender->FindItem(wxT("&Start" )), TRUE );
   menuRender->Enable(menuRender->FindItem(wxT("&Pause" )), FALSE);
   menuRender->Enable(menuRender->FindItem(wxT("&Resume")), FALSE);


   //---------------------------------------- Multithread menu
   wxMenu *menuMultithread = new wxMenu;

   menuMultithread->AppendRadioItem(Menu_Thread_Default , wxT("&Thread per system core" ));
   menuMultithread->AppendRadioItem(Menu_Thread_Single , wxT("&Single Thread" ));
   menuMultithread->AppendRadioItem(Menu_Thread_Dual, wxT("&Dual Threads"));
   menuMultithread->AppendRadioItem(Menu_Thread_Quad, wxT("&Quad Threads"));

   menuMultithread->Check(menuMultithread->FindItem(wxT("&Thread per system core" )), TRUE );   
 

   //---------------------------------------- Divisions Menu

   wxMenu *menuDivisions = new wxMenu;

   menuDivisions->AppendRadioItem(Menu_Division_Default , wxT("&8x8 Grid" ));
   menuDivisions->AppendRadioItem(Menu_Division_Single , wxT("&1x1 Grid" ));
   menuDivisions->AppendRadioItem(Menu_Division_Dual, wxT("&2x2 Grid"));
   menuDivisions->AppendRadioItem(Menu_Division_Quad, wxT("&4x4 Grid"));
   menuDivisions->AppendRadioItem(Menu_Division_64, wxT("&64x64 Grid"));

   menuDivisions->Check(menuDivisions->FindItem(wxT("&8x8 Grid" )), TRUE );   


   //-------------------------------------- Create the MenuBar

   wxMenuBar *menuBar = new wxMenuBar;
   menuBar->Append(menuFile  , wxT("&File"  ));
   menuBar->Append(menuRender, wxT("&Render"));
   menuBar->Append(menuMultithread, wxT("&Multithreading"));
   menuBar->Append(menuDivisions, wxT("&Divisions"));

   
   SetMenuBar( menuBar );

   canvas = new RenderCanvas(this);
   
   CreateStatusBar();
   SetStatusText(wxT("Ready"));
   
   wxIcon icon(main_xpm);
   SetIcon(icon);   
   
   wxStatusBar* statusBar = GetStatusBar();
   int widths[] = {150,300};
   statusBar->SetFieldsCount(2, widths);
}

void wxraytracerFrame::OnQuit( wxCommandEvent& WXUNUSED( event ) )
{

   if(canvas != NULL)
   {
	   canvas->OnQuit();
	   delete canvas;
   }
   Close();
}

void wxraytracerFrame::OnClose( wxCommandEvent& WXUNUSED( event ) )
{

   if(canvas != NULL)
   {
	   canvas->OnQuit();
	   delete canvas;
   }
   Close();
}

void wxraytracerFrame::OnSaveFile( wxCommandEvent& WXUNUSED( event ) )
{
   wxString caption = wxT("Save File");
   
   wxString wildcard = wxT("JPEG files (*.jpg)|*.jpg|TIFF files (*.tif)|*.tif|PNG files (*.png)|*.png|BMP files (*.bmp)|*.bmp");
   
   wxString defaultDir = wxEmptyString;
   wxString defaultFilename = wxT("render.jpg");
   
   wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard,
                        wxFD_SAVE|wxFD_OVERWRITE_PROMPT);
   dialog.SetPath(currentPath);
   
   if (dialog.ShowModal() == wxID_OK)
   {
      currentPath = dialog.GetPath();
      
      wxImage theImage = canvas->GetImage();
	  theImage.SetOption(wxIMAGE_OPTION_QUALITY, 100); // Best Quality
      theImage.SaveFile(currentPath);
   }
}

void wxraytracerFrame::OnOpenFile( wxCommandEvent& WXUNUSED( event ) )
{
   wxString caption = wxT("Choose a file");
   
   wxString wildcard = wxT("JPEG files (*.jpg)|*.jpg|TIFF files (*.tif)|*.tif|PNG files (*.png)|*.png|BMP files (*.bmp)|*.bmp");

   
   wxString defaultDir = wxEmptyString;
   wxString defaultFilename = wxEmptyString;
   
   wxFileDialog dialog(this, caption, defaultDir, defaultFilename, wildcard,
                        wxFD_OPEN|wxFD_FILE_MUST_EXIST);
   dialog.SetPath(currentPath);
   
   if (dialog.ShowModal() == wxID_OK)
   {
      currentPath = dialog.GetPath();
      wxImage* theImage = new wxImage();
      theImage->LoadFile(currentPath);
      
      if (!theImage->IsOk())
      {
         delete theImage;
         wxMessageBox(wxT("Sorry, could not load file."));
      }
      else
      {
         canvas->SetImage(*theImage);
         wxMenu* menuFile = GetMenuBar()->GetMenu(0);
         menuFile->Enable(menuFile->FindItem(wxT( "&Save As..."  )) , TRUE);
      }
   }
}

void wxraytracerFrame::OnThreadDefault( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(2);
   menu->Check(menu->FindItem(wxT("&Thread per system core" )), TRUE); 
   
   canvas->threadNumber = 0;
}

void wxraytracerFrame::OnThreadSingle( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(2);   
   menu->Check(menu->FindItem(wxT("&Single Thread" )), TRUE);   
   
   canvas->threadNumber = 1;
}

void wxraytracerFrame::OnThreadDual( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(2);
   menu->Check(menu->FindItem(wxT("&Dual Threads")), TRUE);
   
   canvas->threadNumber = 2;
}

void wxraytracerFrame::OnThreadQuad( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(2);
   menu->Check(menu->FindItem(wxT("&Quad Threads")), TRUE);
   
   canvas->threadNumber = 4;
}



//------------------------------- Divisions

void wxraytracerFrame::OnDivisionDefault( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&8x8 Grid")), TRUE);
   
   canvas->divisionsNumber = 0;
}

void wxraytracerFrame::OnDivisionSingle( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&1x1 Grid")), TRUE);
   
   canvas->divisionsNumber = 1;
}

void wxraytracerFrame::OnDivisionDual( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&2x2 Grid")), TRUE);
   
   canvas->divisionsNumber = 2;
}


void wxraytracerFrame::OnDivisionQuad( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&4x4 Grid")), TRUE);
   
   canvas->divisionsNumber = 4;
}

void wxraytracerFrame::OnDivision64( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&64x64 Grid")), TRUE);
   
   canvas->divisionsNumber = 64;
}





void wxraytracerFrame::OnRenderStart( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(1);
   menu->Enable(menu->FindItem(wxT("&Start" )), FALSE);
   menu->Enable(menu->FindItem(wxT("&Pause" )), TRUE );
   menu->Enable(menu->FindItem(wxT("&Resume")), FALSE);
   menu->Enable(menu->FindItem(wxT("&Stop")), TRUE);
   
   canvas->renderStart();
   
   wxMenu* menuFile = GetMenuBar()->GetMenu(0);
   menuFile->Enable(menuFile->FindItem(wxT( "&Open..."   )), FALSE);
   menuFile->Enable(menuFile->FindItem(wxT( "&Save As...")), TRUE );

   wxMenu* menuThread = GetMenuBar()->GetMenu(2);
   menuThread->Enable(menuThread->FindItem(wxT("&Thread per system core" )), FALSE );
   menuThread->Enable(menuThread->FindItem(wxT("&Single Thread" )), FALSE);
   menuThread->Enable(menuThread->FindItem(wxT("&Dual Threads")), FALSE);
   menuThread->Enable(menuThread->FindItem(wxT("&Quad Threads")), FALSE);

   wxMenu* menuDivision = GetMenuBar()->GetMenu(3);
   menuDivision->Enable(menuDivision->FindItem(wxT("&8x8 Grid" )), FALSE );
   menuDivision->Enable(menuDivision->FindItem(wxT("&1x1 Grid" )), FALSE);
   menuDivision->Enable(menuDivision->FindItem(wxT("&2x2 Grid")), FALSE);
   menuDivision->Enable(menuDivision->FindItem(wxT("&4x4 Grid")), FALSE);
   menuDivision->Enable(menuDivision->FindItem(wxT("&64x64 Grid")), FALSE);
}

void wxraytracerFrame::OnRenderCompleted( wxCommandEvent& event )
{
   wxMenu* menu = GetMenuBar()->GetMenu(1);
   menu->Enable(menu->FindItem(wxT("&Start" )), TRUE );
   menu->Enable(menu->FindItem(wxT("&Pause" )), FALSE);
   menu->Enable(menu->FindItem(wxT("&Resume")), FALSE);
   menu->Enable(menu->FindItem(wxT("&Stop")), FALSE);

   wxMenu* menuThread = GetMenuBar()->GetMenu(2);
   menuThread->Enable(menuThread->FindItem(wxT("&Thread per system core" )), TRUE );
   menuThread->Enable(menuThread->FindItem(wxT("&Single Thread" )), TRUE);
   menuThread->Enable(menuThread->FindItem(wxT("&Dual Threads")), TRUE);
    menuThread->Enable(menuThread->FindItem(wxT("&Quad Threads")), TRUE);

   wxMenu* menuDivision = GetMenuBar()->GetMenu(3);
   menuDivision->Enable(menuDivision->FindItem(wxT("&8x8 Grid" )), TRUE );
   menuDivision->Enable(menuDivision->FindItem(wxT("&1x1 Grid" )), TRUE );
   menuDivision->Enable(menuDivision->FindItem(wxT("&2x2 Grid")), TRUE );
   menuDivision->Enable(menuDivision->FindItem(wxT("&4x4 Grid")), TRUE );
   menuDivision->Enable(menuDivision->FindItem(wxT("&64x64 Grid")), TRUE );
   
   wxMenu* menuFile = GetMenuBar()->GetMenu(0);
   menuFile->Enable(menuFile->FindItem(wxT("&Open...")), TRUE);

   
   wxGetApp().SetStatusText(wxT("Rendering complete"));
}

void wxraytracerFrame::OnRenderPause( wxCommandEvent& event )
{
   wxMenu* menu = GetMenuBar()->GetMenu(1);
   menu->Enable(menu->FindItem(wxT("&Start" )), FALSE);
   menu->Enable(menu->FindItem(wxT("&Pause" )), FALSE);
   menu->Enable(menu->FindItem(wxT("&Resume")), TRUE );
   
   canvas->renderPause();
   
   wxGetApp().SetStatusText( wxT( "Rendering paused" ) );
}

void wxraytracerFrame::OnRenderResume( wxCommandEvent& event )
{
   wxMenu* menu = GetMenuBar()->GetMenu(1);
   menu->Enable(menu->FindItem(wxT("&Start" )), FALSE);
   menu->Enable(menu->FindItem(wxT("&Pause" )), TRUE );
   menu->Enable(menu->FindItem(wxT("&Resume")), FALSE);
   
   canvas->renderResume();
   
   wxGetApp().SetStatusText(wxT("Rendering..."));
}



/******************************************************************************/
/********************* Queue *******************************************/
/******************************************************************************/

void Queue::AddJob(const tJOB& job, const tPRIORITY& priority) // push a job with given priority class onto the FIFO
    {
      wxMutexLocker lock(mutexQueue); // lock the queue
      jobs.insert(std::make_pair(priority, job)); // insert the prioritized entry into the multimap
      queueCount.Post(); // new job has arrived: increment semaphore counter
    } // void AddJob(const tJOB& job, const tPRIORITY& priority=eNORMAL)

tJOB Queue::Pop()
    {
      tJOB element;
      queueCount.Wait(); // wait for semaphore (=queue count to become positive)
      mutexQueue.Lock(); // lock queue
      element = (jobs.begin())->second; // get the first entry from queue (higher priority classes come first)
      jobs.erase(jobs.begin()); // erase it
      mutexQueue.Unlock(); // unlock queue
      return element; // return job entry
    } // tJOB Pop()

void Queue::Report(const tJOB::tCOMMANDS& cmd, const wxString& sArg, int iArg) // report back to parent
    {
      wxCommandEvent evt(wxEVT_THREAD, cmd); // create command event object
      evt.SetString(sArg); // associate string with it
      evt.SetInt(iArg);
      parent->AddPendingEvent(evt); // and add it to parent's event queue
    } // void Report(const tJOB::tCOMMANDS& cmd, const wxString& arg=wxEmptyString)

size_t Queue::Stacksize() // helper function to return no of pending jobs
    {
      wxMutexLocker lock(mutexQueue); // lock queue until the size has been read
      return jobs.size();
    }

/******************************************************************************/
/********************* WorkerThread *******************************************/
/******************************************************************************/

wxThread::ExitCode WorkerThread::Entry()
    {
      tJOB::tCOMMANDS iErr;	  		
      queue->Report(tJOB::eID_THREAD_STARTED, wxEmptyString, id); // tell main thread that worker thread has successfully started
      try { while(true) OnJob(); } // this is the main loop: process jobs until a job handler throws
      catch(tJOB::tCOMMANDS& i) 
	  { queue->Report(iErr=i, wxEmptyString, id); } // catch return value from error condition
      return (wxThread::ExitCode)iErr; // and return exit code
    } // virtual wxThread::ExitCode Entry()

void WorkerThread::OnJob()
    {
      tJOB job = queue->Pop(); // pop a job from the queue. this will block the worker thread if queue is empty
      switch(job.cmd)
      {
      case tJOB::eID_THREAD_EXIT: // thread should exit	
		queue->Report(tJOB::eID_THREAD_JOB, wxString::Format(wxT("Ending #%s Thread."), job.arg.c_str()), id);
		//Sleep(1000); // wait a while
        throw tJOB::eID_THREAD_EXIT; // confirm exit command
      case tJOB::eID_THREAD_JOB: // process a standard job	
		if(world->camera_ptr != NULL)
			world->camera_ptr->render_scene(*world, job.theJob); // Render Camera based scene		
		else
			world->render_scene(job.theJob);					 // Orthographic render:
        queue->Report(tJOB::eID_THREAD_JOB, wxString::Format(wxT("Job #%s done."), job.arg.c_str()), id); // report successful completion
        break;
      case tJOB::eID_THREAD_JOBERR: // process a job that terminates with an error
        queue->Report(tJOB::eID_THREAD_JOB, wxString::Format(wxT("Job #%s errorneous."), job.arg.c_str()), id);
        //Sleep(1000);
        throw tJOB::eID_THREAD_EXIT; // report exit of worker thread
        break;
      case tJOB::eID_THREAD_NULL: // dummy command
	  case tJOB::eID_THREAD_STARTED:
      default: break; // default
      } // switch(job.cmd)
    } // virtual void OnJob()

/******************************************************************************/
/********************* RenderCanvas *******************************************/
/******************************************************************************/

RenderCanvas::RenderCanvas(wxWindow *parent)
   : wxScrolledWindow(parent), theImage(NULL), w(NULL), timer(NULL), updateTimer(this, ID_RENDER_UPDATE), totalThreads(1),
   threads(NULL), queue(NULL), threadNumber(0), divisions(0), divisionsNumber(0), manager(NULL), theThreads(NULL)
{
   SetOwnBackgroundColour(wxColour(143,144,150));
   queue=new Queue(this);
}

RenderCanvas::~RenderCanvas(void)
{	// Double check we are not leaving anything behind in memory

    renderPause(); 
	if(queue != NULL)
	{   if(threads.size() != 0)
			OnQuit();
		wxThread::Sleep(500);
		delete queue;
		queue = NULL; }	
		
	if(manager != NULL)
    {	delete manager;
		manager = NULL; 
	}

	if(theImage != NULL)
    {   delete theImage; 
	    theImage = NULL; }
   
    if(w != NULL)
    {  delete w;
	   w = NULL; }

#if SAMPLE_HACK>0
	if(!theWorlds.empty())
	{	for (vector<World*>::iterator iter = theWorlds.begin(); iter < theWorlds.end(); ++iter)
		if((*iter) != NULL)
		{	delete (*iter);
		}
		theWorlds.clear();
	}
#endif    
   
    if(timer != NULL)
    {   delete timer;
		timer = NULL; }
}

void RenderCanvas::SetImage(wxImage& image)
{
   if(theImage != NULL)
   {   delete theImage;
	   theImage = NULL; }
   
   theImage = new wxBitmap(image);
   
   SetScrollbars(10, 10, (int)(theImage->GetWidth()  / 10.0f),
                         (int)(theImage->GetHeight() / 10.0f), 0, 0, false);
   Refresh();
}

wxImage RenderCanvas::GetImage(void)
{
   if(theImage != NULL)
      return theImage->ConvertToImage();
   
   return wxImage();
}

void RenderCanvas::OnDraw(wxDC& dc)
{
   if(theImage != NULL && theImage->IsOk())
      wxBufferedDC bdc(&dc, *theImage);
}

void RenderCanvas::OnRenderCompleted( wxCommandEvent& event )
{	   
	  wxGetApp().SetStatusText( wxT( "Render Completed: Cleaning up memory" ) );
	  OnQuit();
   
	  if(manager != NULL)
	   {	manager->Delete();
			delete manager;
			manager = NULL; }

	   if(w != NULL)
	   {  delete w;
		  w = NULL; }
		  
#if SAMPLE_HACK>0
	if(!theWorlds.empty())
	{	for (vector<World*>::iterator iter = theWorlds.begin(); iter < theWorlds.end(); ++iter)
		if((*iter) != NULL)
		{	delete (*iter);
		}
		theWorlds.clear();
	}
#endif
   
	   if(timer != NULL)
	   {  long interval = timer->Time();
   
		  wxTimeSpan timeElapsed(0, 0, 0, interval);
		  wxString timeString = timeElapsed.Format(wxT("Elapsed Time: %H:%M:%S"));
		  wxGetApp().SetStatusText( timeString, 1);

		  delete timer;
		  timer = NULL;
	   }

	    wxGetApp().SetStatusText( wxT( "Render Completed" ) );
	   	
}

void RenderCanvas::OnNewPixel( wxCommandEvent& event )
{
   //set up double buffered device context
   wxClientDC cdc(this);
   DoPrepareDC(cdc);
   wxBufferedDC bufferedDC(&cdc, *theImage);
   
   //iterate over all pixels in the event
   vector<RenderPixel*> *pixelsUpdate =
                        (vector<RenderPixel*> *)event.GetClientData();
   
   for(vector<RenderPixel*>::iterator itr = pixelsUpdate->begin();
                        itr != pixelsUpdate->end(); itr++)
   {
      RenderPixel* pixel = *itr;
      
      wxPen pen(wxColour(pixel->red, pixel->green, pixel->blue));
      bufferedDC.SetPen(pen);
      bufferedDC.DrawPoint(pixel->x, pixel->y);
     
      pixelsRendered++;
      delete pixel;
	  pixel = NULL;
   }
   
   pixelsUpdate->clear();
   delete pixelsUpdate;
   pixelsUpdate = NULL;
}

void RenderCanvas::renderPause(void)
{
	if(!theThreads.empty())
	{	for (iter = theThreads.begin(); iter < theThreads.end(); ++iter)
		if((*iter) != NULL)
		{	if ((*iter)->Pause() != wxTHREAD_NO_ERROR )
                wxLogError("Can't pause the worker thread!");
		}
    
	}

	if(manager != NULL)
	{	if (manager->Pause() != wxTHREAD_NO_ERROR )
                wxLogError("Can't pause the manager thread!");
	}

    updateTimer.Stop();
   
    if(timer != NULL)
       timer->Pause();
}

void RenderCanvas::renderResume(void)
{     
	if(!theThreads.empty())
	{	for (iter = theThreads.begin(); iter < theThreads.end(); ++iter)
		if((*iter) != NULL)
		{	if ((*iter)->Resume() != wxTHREAD_NO_ERROR )
                wxLogError("Can't resume the worker thread!");
		}
    
	}

	if(manager != NULL)
	{	if (manager->Resume() != wxTHREAD_NO_ERROR )
                wxLogError("Can't resume the manager thread!");
	}

    updateTimer.Start();
   
    if(timer != NULL)
       timer->Resume();
}

void RenderCanvas::OnTimerUpdate( wxTimerEvent& event )
{
   if(timer == NULL)
      return;
   
   //percent
   float completed = (float)pixelsRendered / (float)pixelsToRender;
      
   wxString progressString = wxString::Format(wxT("Rendering...%d%%"),
                                             (int)(completed*100));
   wxGetApp().SetStatusText( progressString , 0);
   
   //time elapsed
   long interval = timer->Time();
   
   wxTimeSpan timeElapsed(0, 0, 0, interval);
   
   //time remaining
   float remaining = 1.0f - completed;
   long msecRemain = (long)
                       (((double)interval / (completed*100)) * 100 * remaining);
   
   wxTimeSpan timeRemaining(0, 0, 0, msecRemain);
   
   wxString timeRemainString = timeRemaining.Format(wxT(" / ETA: %H:%M:%S"));
   wxString timeString = timeElapsed.Format(wxT("Elapsed Time: %H:%M:%S"));
   
   //only display ETA if something has been completed
   if(msecRemain >= 0)
      wxGetApp().SetStatusText( timeString + timeRemainString, 1);
   else
      wxGetApp().SetStatusText( timeString, 1);

   if(completed >= 1)  // If we have reached 1 (100%) render call completed
   {
	   wxCommandEvent event(wxEVT_RENDER, ID_RENDER_COMPLETED);   
	   this->GetEventHandler()->AddPendingEvent(event);   
	   this->GetParent()->GetEventHandler()->AddPendingEvent(event);
   }

}

void RenderCanvas::renderStart(void)
{
    w = new World();
   
    wxGetApp().SetStatusText( wxT( "Building world..." ) );
   
    w->build();
    wxGetApp().SetStatusText( wxT( "Rendering..." ) );
   
  	int hres = w->vp.hres;
	int vres = w->vp.vres;
	int offset = 0;
	if(w->camera_ptr->get_stereo() == true)
	{	hres = hres * 2;
		offset = w->camera_ptr->get_offset();
		hres += offset;
	}

	pixelsRendered = 0;
    pixelsToRender = hres * vres;
   
    //set the background
    wxBitmap bitmap(hres, vres, -1);
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetBackground(*wxGREY_BRUSH);
    dc.Clear();
   
    wxBitmap tile(bg_xpm);
   
    for(int x = 0; x < hres; x += 16)
    {
       for(int y = 0; y < vres; y += 16)
          dc.DrawBitmap(tile, x, y, FALSE);
    }
   
	dc.SelectObject(wxNullBitmap);
   
    wxImage temp = bitmap.ConvertToImage();
    SetImage(temp);	

    updateTimer.Start(250);
 
    timer = new wxStopWatch();  //start timer 

    totalThreads = wxThread::GetCPUCount(); // get the total amout of cores for the system
    if(threadNumber == 0)
		threadNumber = totalThreads;        			

    manager = new RenderThread(this);	// RenderThread communicates with the canvas rendered pixels
	manager->Create();
	manager->Run();
	manager->SetPriority(30);
	w->paintArea = manager;				// Threads communicate with the manager via the World

	if(divisionsNumber == 0)
		divisions = 8;  // 8 is a nice multiple of 2
	else
		divisions = divisionsNumber;
	// else division will be what is set.

	int jobWidth = w->vp.hres / divisions;
	int jobHeight =  w->vp.vres / divisions;
	float xRemainder = w->vp.hres % divisions;
	float yRemainder = w->vp.vres % divisions;

	int currentX = 0;
	int currentY = 0;	
	for(int y=0; y < divisions; y++)	
	{	currentY = y * jobHeight;
		for(int x=0; x < divisions; x++)
		{	
			currentX = x * jobWidth;
			int iJob = rand();
			PixelPoints current;
			current.origin.x =currentX; current.origin.y = currentY;
			current.end.x =currentX + jobWidth; current.end.y = currentY + jobHeight;
			queue->AddJob(tJOB(tJOB::eID_THREAD_JOB, wxString::Format(wxT("%u"), iJob), current));
		}

	}	
	

	if(yRemainder != 0)
	{		int iJob = rand();
			PixelPoints current;
			current.origin.x = 0; current.origin.y = w->vp.vres - yRemainder;
			current.end.x = w->vp.hres; current.end.y = w->vp.vres;
			queue->AddJob(tJOB(tJOB::eID_THREAD_JOB, wxString::Format(wxT("%u"), iJob), current));
	}
	if(xRemainder != 0)
	{		int iJob = rand();
			PixelPoints current;
			current.origin.x = w->vp.hres - xRemainder; current.origin.y = 0;
			current.end.x = w->vp.hres; current.end.y = w->vp.vres;
			queue->AddJob(tJOB(tJOB::eID_THREAD_JOB, wxString::Format(wxT("%u"), iJob), current)); 
	}
	

	//threadNumber = divisions*divisions;		// make the number of threads the number of jobs
	//threadNumber = 4;							// simulate quad core cpu
	//threadNumber = 2;							// simulate dual core cpu
	//threadNumber = 1;							// simulate singe core cpu
    for(int i=0; i< threadNumber; i++)
	{	
#if SAMPLE_HACK>0
		World* wo = new World();
		wo->build();
		wo->paintArea = manager;
		theWorlds.push_back(wo);
#endif
		OnStart();		
	}
}

void RenderCanvas::OnStart(wxCommandEvent& WXUNUSED(event)) // start one worker thread
{
    int id = threads.empty()?1:threads.back()+1;
    threads.push_back(id);
	WorkerThread* thread=new WorkerThread(queue, this, w, id); // create a new worker thread, increment thread counter (this implies, thread will start OK)
    theThreads.push_back(thread);
	thread->SetPriority(20);
	if (thread->Run() != wxTHREAD_NO_ERROR )
    {	wxLogError("Can't create the worker thread!");
		delete theThreads.back();
		threads.back() = NULL;
		threads.pop_back(); }
}

void RenderCanvas::OnStart() // start one worker thread
{
    int id = threads.empty()?1:threads.back()+1;
    threads.push_back(id);	

#if SAMPLE_HACK>0
   WorkerThread* thread=new WorkerThread(queue, this, theWorlds.back(), id); // create a new worker thread, increment thread counter (this implies, thread will start OK)
#else
	WorkerThread* thread=new WorkerThread(queue, this, w, id);
#endif
	theThreads.push_back(thread);
	thread->SetPriority(20);
	if (thread->Run() != wxTHREAD_NO_ERROR )
    {	wxLogError("Can't create the thread!");
		delete theThreads.back();
		threads.back() = NULL;
		threads.pop_back(); }
}

void RenderCanvas::OnStop(wxCommandEvent& WXUNUSED(event)) // stop one worker thread
{
      if(threads.empty()) { Destroy(); return; } // no thread(s) running: frame can be destroyed right away
      queue->AddJob(tJOB(tJOB::eID_THREAD_EXIT, wxEmptyString), Queue::eHIGHEST); // add eID_THREAD_EXIT notification with highest priority to bypass other running jobs
      //SetStatusText(_T("Stopping thread..."));
}

void RenderCanvas::OnJob(wxCommandEvent& event) // handler for launching a job for worker thread(s)
{
      int iJob=rand();
      queue->AddJob(tJOB((tJOB::tCOMMANDS)event.GetId(), wxString::Format(wxT("%u"), iJob)));
      //SetStatusText(wxString::Format(wxT("Job #%i started."), iJob)); // just set the status text
}

void RenderCanvas::OnThread(wxCommandEvent& event) // handler for thread notifications
{
	switch(event.GetId())
    {
      case tJOB::eID_THREAD_JOB:
        //SetStatusText(wxString::Format(wxT("[%i]: %s"), event.GetInt(), event.GetString().c_str())); // progress display
		  break;
      case tJOB::eID_THREAD_EXIT:
        //SetStatusText(wxString::Format(wxT("[%i]: Stopped."), event.GetInt()));
        threads.remove(event.GetInt()); // thread has exited: remove thread ID from list
        break;
      case tJOB::eID_THREAD_STARTED:
        //SetStatusText(wxString::Format(wxT("[%i]: Ready."), event.GetInt()));
        break; 
      default: event.Skip();
     }
}

void RenderCanvas::OnQuit()
{	if(!threads.empty())
	{  for(size_t t=0; t<threads.size(); ++t) 
			queue->AddJob(tJOB(tJOB::eID_THREAD_EXIT, wxEmptyString), Queue::eHIGHEST); // send all running threads the "EXIT" signal
	} 
	if(!theThreads.empty())
		{	if(theThreads.size() == 1)
			{	theThreads.pop_back();	
				theThreads.clear();
	}
	else
		{
			for (iter = theThreads.begin(); iter < theThreads.end(); iter++)
			{
				if((*iter) != NULL)
				{		
					theThreads.pop_back();				
				}
			}
			theThreads.clear();
		}
	}
	
}
   



/******************************************************************************/
/********************* RenderPixel ********************************************/
/******************************************************************************/

RenderPixel::RenderPixel(int _x, int _y, int _red, int _green, int _blue)
 : x(_x), y(_y), red(_red), green(_green), blue(_blue)
{}




/******************************************************************************/
/********************* RenderThread *******************************************/
/******************************************************************************/

DEFINE_EVENT_TYPE(wxEVT_RENDER)

BEGIN_EVENT_TABLE( RenderCanvas, wxScrolledWindow )
	EVT_COMMAND(wxID_ANY, wxEVT_THREAD, RenderCanvas::OnThread)
	EVT_COMMAND(ID_RENDER_NEWPIXEL, wxEVT_RENDER, RenderCanvas::OnNewPixel)
    EVT_COMMAND(ID_RENDER_COMPLETED, wxEVT_RENDER, RenderCanvas::OnRenderCompleted)
    EVT_TIMER(ID_RENDER_UPDATE, RenderCanvas::OnTimerUpdate)
END_EVENT_TABLE()

void RenderThread::setPixel(int x, int y, int red, int green, int blue)
{  mutex.Lock();
   pixels.push_back(new RenderPixel(x, y, red, green, blue));
   mutex.Unlock();   
}

void RenderThread::setPixel(const list<RenderedInt>& re)
{  
   mutex.Lock();
   list<RenderedInt>::const_iterator it;
   for(it=re.begin(); it != re.end(); it++)
   {   pixels.push_back(new RenderPixel(it->x, it->y, it->r, it->g, it->b));
   }
   mutex.Unlock();   
}

void RenderThread::NotifyCanvas()
{
   mutex.Lock();
   lastUpdateTime = timer->Time();
   
   //copy rendered pixels into a new vector and reset
   vector<RenderPixel*> *pixelsUpdate = new vector<RenderPixel*>(pixels);
   pixels.clear();
   
   wxCommandEvent event(wxEVT_RENDER, ID_RENDER_NEWPIXEL);
   event.SetClientData(pixelsUpdate);
   canvas->GetEventHandler()->AddPendingEvent(event);
   mutex.Unlock();
}

void RenderThread::OnExit()
{
   NotifyCanvas(); // Make sure we have sent all our pixels
   
   if(timer != NULL)
   {   delete timer;
	   timer = NULL;
   }
}

void *RenderThread::Entry()
{
	lastUpdateTime = 0;
	timer = new wxStopWatch();

	while(!TestDestroy())
	{   wxThread::Sleep(50); // Sleep for 50 milliseconds
		NotifyCanvas();	     // Send pixels to canvas
	}
	return NULL;
}
