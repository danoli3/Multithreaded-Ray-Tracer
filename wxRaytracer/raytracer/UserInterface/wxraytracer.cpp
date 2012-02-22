

#include <wx/wx.h>
#include <wx/dcbuffer.h>
#include "wxraytracer.h"
#include "main.xpm"
#include "bg.xpm"
#include "black.xpm"

#if WXWIDGETS292>0
	wxDEFINE_EVENT(THE_THREADS, wxThreadEvent);
#else
	DEFINE_EVENT_TYPE(wxEVT_THREAD);
#endif


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
   EVT_MENU( Menu_Thread_Job, wxraytracerFrame::OnThreadJob )
   EVT_MENU( Menu_Render_Mode_Grid, wxraytracerFrame::OnRenderModeGrid )
   EVT_MENU( Menu_Render_Mode_Random, wxraytracerFrame::OnRenderModeRandom )
   EVT_MENU( Menu_Render_Mode_Spiral_Out, wxraytracerFrame::OnRenderModeSpiralOut )
   EVT_MENU( Menu_Render_Mode_Spiral_In, wxraytracerFrame::OnRenderModeSpiralIn )
   EVT_MENU( Menu_Render_Mode_Spiral_In_And_Out, wxraytracerFrame::OnRenderModeSpiralInAndOut )
   EVT_MENU( Menu_Render_Mode_Spiral_In_And_Out2, wxraytracerFrame::OnRenderModeSpiralInAndOut2 )
   EVT_MENU( Menu_Render_Mode_Sequence, wxraytracerFrame::OnRenderModeSequence )
   EVT_MENU( Menu_Render_Display_Pixel, wxraytracerFrame::OnRenderDisplayPixel )
   EVT_MENU( Menu_Render_Display_Row, wxraytracerFrame::OnRenderDisplayRow )
   EVT_MENU( Menu_Render_Display_Job, wxraytracerFrame::OnRenderDisplayJob )
   EVT_MENU( Menu_Division_Default, wxraytracerFrame::OnDivisionDefault )
   EVT_MENU( Menu_Division_Single, wxraytracerFrame::OnDivisionSingle )
   EVT_MENU( Menu_Division_Dual, wxraytracerFrame::OnDivisionDual )
   EVT_MENU( Menu_Division_Quad, wxraytracerFrame::OnDivisionQuad )
   EVT_MENU( Menu_Division_16, wxraytracerFrame::OnDivision16 )
   EVT_MENU( Menu_Division_32, wxraytracerFrame::OnDivision32 )
   EVT_MENU( Menu_Division_64, wxraytracerFrame::OnDivision64 )
   EVT_MENU( Menu_Samples_Build, wxraytracerFrame::OnSamples_Build )
   EVT_MENU( Menu_Samples_1, wxraytracerFrame::OnSamples_1 )
   EVT_MENU( Menu_Samples_2, wxraytracerFrame::OnSamples_2 )
   EVT_MENU( Menu_Samples_4, wxraytracerFrame::OnSamples_4 )
   EVT_MENU( Menu_Samples_8, wxraytracerFrame::OnSamples_8 )
   EVT_MENU( Menu_Samples_16, wxraytracerFrame::OnSamples_16 )
   EVT_MENU( Menu_Samples_24, wxraytracerFrame::OnSamples_24 )
   EVT_MENU( Menu_Samples_32, wxraytracerFrame::OnSamples_32 )
   EVT_MENU( Menu_Samples_64, wxraytracerFrame::OnSamples_64 )
   EVT_MENU( Menu_Samples_128, wxraytracerFrame::OnSamples_128 )
   EVT_MENU( Menu_Samples_256, wxraytracerFrame::OnSamples_256 )
   EVT_COMMAND(ID_RENDER_COMPLETED, wxEVT_RENDER, wxraytracerFrame::OnRenderCompleted)
END_EVENT_TABLE()

wxraytracerFrame::wxraytracerFrame(const wxPoint& pos, const wxSize& size)
   : wxFrame((wxFrame *)NULL, -1, wxT( "Ray Tracer" ), pos, size)
{
   wxMenu *menuFile = new wxMenu;
   
   menuFile->Append(Menu_File_Open, wxT("&Open..."   ));
   menuFile->Append(Menu_File_Save, wxT("&Save As..."));
   menuFile->AppendSeparator();
   menuFile->Append(Menu_File_Quit, wxT("&Exit"));
   
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
   menuMultithread->AppendRadioItem(Menu_Thread_Job, wxT("&Thread per Job"));

   menuMultithread->Check(menuMultithread->FindItem(wxT("&Thread per system core" )), TRUE );   
 
   //---------------------------------------- RenderMode Menu

   wxMenu *menuRenderMode = new wxMenu;
   menuRenderMode->AppendRadioItem(Menu_Render_Mode_Spiral_In_And_Out2, wxT("&Spiral In and Out v2"));
   menuRenderMode->AppendRadioItem(Menu_Render_Mode_Spiral_In_And_Out, wxT("&Spiral In and Out"));
   menuRenderMode->AppendRadioItem(Menu_Render_Mode_Spiral_Out, wxT("&Spiral Out"));  
   menuRenderMode->AppendRadioItem(Menu_Render_Mode_Spiral_In, wxT("&Spiral In"));
   menuRenderMode->AppendRadioItem(Menu_Render_Mode_Sequence, wxT("&Sequence"));  
   menuRenderMode->AppendRadioItem(Menu_Render_Mode_Random, wxT("&Random"));  
   menuRenderMode->AppendRadioItem(Menu_Render_Mode_Grid , wxT("&Grid" ));
  
   menuRenderMode->Check(menuRenderMode->FindItem(wxT("&Spiral In and Out v2" )), TRUE );  

   //---------------------------------------- Divisions Menu

   wxMenu *menuDivisions = new wxMenu;

   menuDivisions->AppendRadioItem(Menu_Division_Default , wxT("&64 Jobs (8 x 8)" ));
   menuDivisions->AppendRadioItem(Menu_Division_Single , wxT("&1 Job (1 x 1)" ));
   menuDivisions->AppendRadioItem(Menu_Division_Dual, wxT("&4 Jobs (2 x 2)"));
   menuDivisions->AppendRadioItem(Menu_Division_Quad, wxT("&16 Jobs (4 x 4)"));
   menuDivisions->AppendRadioItem(Menu_Division_16, wxT("&256 Jobs (16 x 16)"));
   menuDivisions->AppendRadioItem(Menu_Division_32, wxT("&1024 Jobs (32 x 32)"));
   menuDivisions->AppendRadioItem(Menu_Division_64, wxT("&4096 Jobs (64 x 64)"));

   menuDivisions->Check(menuDivisions->FindItem(wxT("&64 Jobs (8 x 8)")), TRUE );   

  //---------------------------------------- RenderDisplay Menu

   wxMenu *menuRenderDisplay = new wxMenu;

   menuRenderDisplay->AppendRadioItem(Menu_Render_Display_Pixel , wxT("&Every Pixel" ));
   menuRenderDisplay->AppendRadioItem(Menu_Render_Display_Row, wxT("&Every Row"));
   menuRenderDisplay->AppendRadioItem(Menu_Render_Display_Job, wxT("&End of Job"));
   menuRenderDisplay->Check(menuRenderDisplay->FindItem(wxT("&Every Pixel" )), TRUE );  

   wxMenu *menuSamples = new wxMenu;

   menuSamples->AppendRadioItem(Menu_Samples_Build, wxT("&Set by Build" ));
   menuSamples->AppendRadioItem(Menu_Samples_1, wxT("&1" ));
   menuSamples->AppendRadioItem(Menu_Samples_2, wxT("&2" ));
   menuSamples->AppendRadioItem(Menu_Samples_4, wxT("&4" ));
   menuSamples->AppendRadioItem(Menu_Samples_8, wxT("&8" ));
   menuSamples->AppendRadioItem(Menu_Samples_16, wxT("&16" ));
   menuSamples->AppendRadioItem(Menu_Samples_24, wxT("&24" ));
   menuSamples->AppendRadioItem(Menu_Samples_32, wxT("&32" ));
   menuSamples->AppendRadioItem(Menu_Samples_64, wxT("&64" ));
   menuSamples->AppendRadioItem(Menu_Samples_128, wxT("&128" ));
   menuSamples->AppendRadioItem(Menu_Samples_256, wxT("&256" ));
   menuSamples->Check(menuSamples->FindItem(wxT("&Set by Build" )), TRUE );  


   //-------------------------------------- Create the MenuBar

   wxMenuBar *menuBar = new wxMenuBar;
   menuBar->Append(menuFile  , wxT("&File"  ));
   menuBar->Append(menuRender, wxT("&Render"));
   menuBar->Append(menuMultithread, wxT("&Multi-Threading"));
   menuBar->Append(menuRenderMode, wxT("&RenderMode"));
   menuBar->Append(menuDivisions, wxT("&Divisions"));
   menuBar->Append(menuRenderDisplay, wxT("&Display"));
   menuBar->Append(menuSamples, wxT("&Anti-Aliasing"));

   
   SetMenuBar( menuBar );

   canvas = new RenderCanvas(this);
   
   CreateStatusBar();
   SetStatusText(wxT("Ready"));
   
   wxIcon icon(main_xpm);
   SetIcon(icon);   
   
   wxStatusBar* statusBar = GetStatusBar();
   int widths[] = {160,320};
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

void wxraytracerFrame::OnThreadJob( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(2);
   menu->Check(menu->FindItem(wxT("&Thread per Job")), TRUE);
   
  /* int divisions = canvas->divisionsNumber * canvas->divisionsNumber;
   if(divisions > 4096)  
		canvas->threadNumber = 4096;
   else
	   canvas->threadNumber = divisions;*/
   canvas->threadNumber = 9000;
}


void wxraytracerFrame::OnRenderModeGrid( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&Grid")), TRUE);
   
   canvas->renderMode = canvas->GRID;
}

void wxraytracerFrame::OnRenderModeRandom( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&Random")), TRUE);
   
   canvas->renderMode = canvas->RANDOM;
}

void wxraytracerFrame::OnRenderModeSpiralOut( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&Spiral Out")), TRUE);
   
   canvas->renderMode = canvas->SPIRAL_OUT;
}

void wxraytracerFrame::OnRenderModeSpiralIn( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&Spiral In")), TRUE);
   
   canvas->renderMode = canvas->SPIRAL_IN;
}

void wxraytracerFrame::OnRenderModeSpiralInAndOut( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&Spiral In and Out")), TRUE);
   
   canvas->renderMode = canvas->SPIRAL_IN_AND_OUT;
}

void wxraytracerFrame::OnRenderModeSpiralInAndOut2( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&Spiral In and Out v2")), TRUE);
   
   canvas->renderMode = canvas->SPIRAL_IN_AND_OUT2;
}

void wxraytracerFrame::OnRenderModeSequence( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(3);
   menu->Check(menu->FindItem(wxT("&Sequence")), TRUE);
   
   canvas->renderMode = canvas->SEQUENCE;
}


//------------------------------- Divisions

void wxraytracerFrame::OnDivisionDefault( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(4);
   menu->Check(menu->FindItem(wxT("&64 Jobs (8 x 8)")), TRUE);
   
   canvas->divisionsNumber = 0;
}

void wxraytracerFrame::OnDivisionSingle( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(4);
   menu->Check(menu->FindItem(wxT("&1 Job (1 x 1)")), TRUE);
   
   canvas->divisionsNumber = 1;
}

void wxraytracerFrame::OnDivisionDual( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(4);
   menu->Check(menu->FindItem(wxT("&4 Jobs (2 x 2)")), TRUE);
   
   canvas->divisionsNumber = 2;
}


void wxraytracerFrame::OnDivisionQuad( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(4);
   menu->Check(menu->FindItem(wxT("&16 Jobs (4 x 4)")), TRUE);
   
   canvas->divisionsNumber = 4;
}

void wxraytracerFrame::OnDivision16( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(4);
   menu->Check(menu->FindItem(wxT("&256 Jobs (16 x 16)")), TRUE);
   
   canvas->divisionsNumber = 16;
}

void wxraytracerFrame::OnDivision32( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(4);
   menu->Check(menu->FindItem(wxT("&1024 Jobs (32 x 32)")), TRUE);
   
   canvas->divisionsNumber = 32;
}

void wxraytracerFrame::OnDivision64( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(4);
   menu->Check(menu->FindItem(wxT("&64 Jobs (8 x 8)")), TRUE);
   
   canvas->divisionsNumber = 64;
}


void wxraytracerFrame::OnRenderDisplayPixel( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(5);
   menu->Check(menu->FindItem(wxT("&Every Pixel")), TRUE);
   
   canvas->renderDisplay = EVERY_PIXEL;
   if(canvas->manager != NULL)
	   canvas->manager->SetRenderDisplay(EVERY_PIXEL);
}

void wxraytracerFrame::OnRenderDisplayRow( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(5);
   menu->Check(menu->FindItem(wxT("&Every Row")), TRUE);
   
   canvas->renderDisplay = EVERY_ROW;
   if(canvas->manager != NULL)
	   canvas->manager->SetRenderDisplay(EVERY_ROW);
}

void wxraytracerFrame::OnRenderDisplayJob( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(5);
   menu->Check(menu->FindItem(wxT("&End of Job")), TRUE);
   
   canvas->renderDisplay = EVERY_JOB;
   if(canvas->manager != NULL)
	   canvas->manager->SetRenderDisplay(EVERY_JOB);
}

void wxraytracerFrame::OnSamples_Build( wxCommandEvent& WXUNUSED( event ) )
{  wxMenu* menu = GetMenuBar()->GetMenu(6);
   menu->Check(menu->FindItem(wxT("&Set by Build")), TRUE);   
   if(canvas != NULL)
	   canvas->samples = 0;  }
void wxraytracerFrame::OnSamples_1( wxCommandEvent& WXUNUSED( event ) )
{  wxMenu* menu = GetMenuBar()->GetMenu(6);
   menu->Check(menu->FindItem(wxT("&1")), TRUE);   
   if(canvas != NULL)
	   canvas->samples = 1;  }
void wxraytracerFrame::OnSamples_2( wxCommandEvent& WXUNUSED( event ) )
{  wxMenu* menu = GetMenuBar()->GetMenu(6);
   menu->Check(menu->FindItem(wxT("&2")), TRUE);   
   if(canvas != NULL)
	   canvas->samples = 2;  }
void wxraytracerFrame::OnSamples_4( wxCommandEvent& WXUNUSED( event ) )
{  wxMenu* menu = GetMenuBar()->GetMenu(6);
   menu->Check(menu->FindItem(wxT("&4")), TRUE);   
   if(canvas != NULL)
	   canvas->samples = 4;  }
void wxraytracerFrame::OnSamples_8( wxCommandEvent& WXUNUSED( event ) )
{  wxMenu* menu = GetMenuBar()->GetMenu(6);
   menu->Check(menu->FindItem(wxT("&8")), TRUE);   
   if(canvas != NULL)
	   canvas->samples = 8;  }
void wxraytracerFrame::OnSamples_16( wxCommandEvent& WXUNUSED( event ) )
{  wxMenu* menu = GetMenuBar()->GetMenu(6);
   menu->Check(menu->FindItem(wxT("&16")), TRUE);   
   if(canvas != NULL)
	   canvas->samples = 16;  }
void wxraytracerFrame::OnSamples_24( wxCommandEvent& WXUNUSED( event ) )
{  wxMenu* menu = GetMenuBar()->GetMenu(6);
   menu->Check(menu->FindItem(wxT("&24")), TRUE);   
   if(canvas != NULL)
	   canvas->samples = 24;  }
void wxraytracerFrame::OnSamples_32( wxCommandEvent& WXUNUSED( event ) )
{  wxMenu* menu = GetMenuBar()->GetMenu(6);
   menu->Check(menu->FindItem(wxT("&32")), TRUE);   
   if(canvas != NULL)
	   canvas->samples = 32;  }
void wxraytracerFrame::OnSamples_64( wxCommandEvent& WXUNUSED( event ) )
{  wxMenu* menu = GetMenuBar()->GetMenu(6);
   menu->Check(menu->FindItem(wxT("&64")), TRUE);   
   if(canvas != NULL)
	   canvas->samples = 64;  }
void wxraytracerFrame::OnSamples_128( wxCommandEvent& WXUNUSED( event ) )
{  wxMenu* menu = GetMenuBar()->GetMenu(6);
   menu->Check(menu->FindItem(wxT("&128")), TRUE);   
   if(canvas != NULL)
	   canvas->samples = 128;  }
void wxraytracerFrame::OnSamples_256( wxCommandEvent& WXUNUSED( event ) )
{  wxMenu* menu = GetMenuBar()->GetMenu(6);
   menu->Check(menu->FindItem(wxT("&256")), TRUE);   
   if(canvas != NULL)
	   canvas->samples = 256;  }


void wxraytracerFrame::OnRenderStart( wxCommandEvent& WXUNUSED( event ) )
{
   wxMenu* menu = GetMenuBar()->GetMenu(1);
   menu->Enable(menu->FindItem(wxT("&Start" )), FALSE);
   menu->Enable(menu->FindItem(wxT("&Pause" )), TRUE );
   menu->Enable(menu->FindItem(wxT("&Resume")), FALSE);   
   
   canvas->renderStart();
   
   wxMenu* menuFile = GetMenuBar()->GetMenu(0);
   menuFile->Enable(menuFile->FindItem(wxT( "&Open..."   )), FALSE);
   menuFile->Enable(menuFile->FindItem(wxT( "&Save As...")), TRUE );

   wxMenu* menuThread = GetMenuBar()->GetMenu(2);
   menuThread->Enable(menuThread->FindItem(wxT("&Thread per system core" )), FALSE );
   menuThread->Enable(menuThread->FindItem(wxT("&Single Thread" )), FALSE);
   menuThread->Enable(menuThread->FindItem(wxT("&Dual Threads")), FALSE);
   menuThread->Enable(menuThread->FindItem(wxT("&Quad Threads")), FALSE);
   menuThread->Enable(menuThread->FindItem(wxT("&Thread per Job")), FALSE);

   wxMenu* menuRenderMode = GetMenuBar()->GetMenu(3);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Grid")), FALSE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Random")), FALSE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Spiral Out")), FALSE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Spiral In and Out")), FALSE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Spiral In and Out v2")), FALSE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Spiral In")), FALSE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Sequence")), FALSE);

   wxMenu* menuDivision = GetMenuBar()->GetMenu(4);
   menuDivision->Enable(menuDivision->FindItem(wxT("&8x8 Grid" )), FALSE );
   menuDivision->Enable(menuDivision->FindItem(wxT("&1x1 Grid" )), FALSE);
   menuDivision->Enable(menuDivision->FindItem(wxT("&2x2 Grid")), FALSE);
   menuDivision->Enable(menuDivision->FindItem(wxT("&4x4 Grid")), FALSE);
   menuDivision->Enable(menuDivision->FindItem(wxT("&64x64 Grid")), FALSE);

   wxMenu* menuRenderDisplay = GetMenuBar()->GetMenu(5);
   menuRenderDisplay->Enable(menuRenderDisplay->FindItem(wxT("&Every Pixel" )), TRUE );
   menuRenderDisplay->Enable(menuRenderDisplay->FindItem(wxT("&Every Row" )), TRUE);
   menuRenderDisplay->Enable(menuRenderDisplay->FindItem(wxT("&End of Job")), TRUE);

   wxMenu* menuSamples = GetMenuBar()->GetMenu(6);
   if(canvas->samples != canvas->w->vp.num_samples)
   {

	   int currentSamples = canvas->w->vp.num_samples;
	   if(currentSamples == 1)
			menuSamples->Check(menuSamples->FindItem(wxT("&1" )), TRUE );  
	   else if(currentSamples == 2)
			menuSamples->Check(menuSamples->FindItem(wxT("&2" )), TRUE );  
	   else if(currentSamples == 4)
			menuSamples->Check(menuSamples->FindItem(wxT("&4" )), TRUE );  
	   else if(currentSamples == 8)
			menuSamples->Check(menuSamples->FindItem(wxT("&8" )), TRUE );  
	   else if(currentSamples == 16)
			menuSamples->Check(menuSamples->FindItem(wxT("&16" )), TRUE );  
	   else if(currentSamples == 24)
			menuSamples->Check(menuSamples->FindItem(wxT("&24" )), TRUE );  
	   else if(currentSamples == 32)
			menuSamples->Check(menuSamples->FindItem(wxT("&32" )), TRUE );  
	   else if(currentSamples == 64)
			menuSamples->Check(menuSamples->FindItem(wxT("&64" )), TRUE );  
	   else if(currentSamples == 128)
			menuSamples->Check(menuSamples->FindItem(wxT("&128" )), TRUE );  
	   else if(currentSamples == 256)
			menuSamples->Check(menuSamples->FindItem(wxT("&256" )), TRUE );  
	   else
			menuSamples->Check(menuSamples->FindItem(wxT("&Set by Build" )), TRUE );  
   }
   menuSamples->Enable(menuSamples->FindItem(wxT("&Set by Build" )), FALSE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&1" )), FALSE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&2" )), FALSE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&4" )), FALSE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&8" )), FALSE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&16" )), FALSE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&24" )), FALSE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&32" )), FALSE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&64" )), FALSE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&128" )), FALSE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&256" )), FALSE );
   

}

void wxraytracerFrame::OnRenderCompleted( wxCommandEvent& event )
{
   wxMenu* menu = GetMenuBar()->GetMenu(1);
   menu->Enable(menu->FindItem(wxT("&Start" )), TRUE );
   menu->Enable(menu->FindItem(wxT("&Pause" )), FALSE);
   menu->Enable(menu->FindItem(wxT("&Resume")), FALSE); 

   wxMenu* menuThread = GetMenuBar()->GetMenu(2);
   menuThread->Enable(menuThread->FindItem(wxT("&Thread per system core" )), TRUE );
   menuThread->Enable(menuThread->FindItem(wxT("&Single Thread" )), TRUE);
   menuThread->Enable(menuThread->FindItem(wxT("&Dual Threads")), TRUE);
   menuThread->Enable(menuThread->FindItem(wxT("&Quad Threads")), TRUE);
   menuThread->Enable(menuThread->FindItem(wxT("&Thread per Job")), TRUE);

   wxMenu* menuRenderMode = GetMenuBar()->GetMenu(3);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Grid")), TRUE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Random")), TRUE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Spiral Out")), TRUE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Spiral In and Out")), TRUE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Spiral In and Out v2")), TRUE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Spiral In")), TRUE);
   menuRenderMode->Enable(menuRenderMode->FindItem(wxT("&Sequence")), TRUE);

   wxMenu* menuDivision = GetMenuBar()->GetMenu(4);
   menuDivision->Enable(menuDivision->FindItem(wxT("&8x8 Grid" )), TRUE );
   menuDivision->Enable(menuDivision->FindItem(wxT("&1x1 Grid" )), TRUE );
   menuDivision->Enable(menuDivision->FindItem(wxT("&2x2 Grid")), TRUE );
   menuDivision->Enable(menuDivision->FindItem(wxT("&4x4 Grid")), TRUE );
   menuDivision->Enable(menuDivision->FindItem(wxT("&64x64 Grid")), TRUE );

   wxMenu* menuRenderDisplay = GetMenuBar()->GetMenu(5);
   menuRenderDisplay->Enable(menuRenderDisplay->FindItem(wxT("&Every Pixel" )), TRUE );
   menuRenderDisplay->Enable(menuRenderDisplay->FindItem(wxT("&Every Row" )), TRUE);
   menuRenderDisplay->Enable(menuRenderDisplay->FindItem(wxT("&End of Job")), TRUE);

   wxMenu* menuSamples = GetMenuBar()->GetMenu(6);
   menuSamples->Enable(menuSamples->FindItem(wxT("&Set by Build" )), TRUE);
   menuSamples->Enable(menuSamples->FindItem(wxT("&1" )), TRUE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&2" )), TRUE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&4" )), TRUE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&8" )), TRUE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&16" )), TRUE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&24" )), TRUE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&32" )), TRUE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&64" )), TRUE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&128" )), TRUE );
   menuSamples->Enable(menuSamples->FindItem(wxT("&256" )), TRUE );
   
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
	  while(!TestDestroy()) //while this thread is not asked to be destroyed
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
				world->camera_ptr->render_scene(*world, job.theJobs);
			else
				world->render_scene(job.theJobs);					 // Orthographic render:
			queue->Report(tJOB::eID_THREAD_JOB, wxString::Format(wxT("Job #%s done."), job.arg.c_str()), id); // report successful completion
			break;
		  case tJOB::eID_THREAD_JOBERR: // process a job that terminates with an error
			queue->Report(tJOB::eID_THREAD_JOB, wxString::Format(wxT("Job #%s errorneous."), job.arg.c_str()), id);
			//Sleep(1000);
			throw tJOB::eID_THREAD_EXIT; // report exit of worker thread
			break;
		  case tJOB::eID_THREAD_NULL: // dummy command
		  default: break; // default
		  } // switch(job.cmd)
	  }
    } // virtual void OnJob()

/******************************************************************************/
/********************* RenderCanvas *******************************************/
/******************************************************************************/

RenderCanvas::RenderCanvas(wxWindow *parent)
   : wxScrolledWindow(parent), theImage(NULL), w(NULL), timer(NULL), updateTimer(this, ID_RENDER_UPDATE), totalThreads(1),
   threads(NULL), queue(NULL), threadNumber(0), divisions(0), divisionsNumber(0), manager(NULL), theThreads(NULL), renderMode(SPIRAL_IN_AND_OUT2), renderDisplay(EVERY_PIXEL), samples(0)
{
   SetOwnBackgroundColour(wxColour(143,144,150));
   queue=new Queue(this);
}

RenderCanvas::~RenderCanvas(void)
{	// Double check we are not leaving anything behind in memory

	if(!threads.empty())
	{  for(size_t t=0; t<threads.size(); ++t) 
			queue->AddJob(tJOB(tJOB::eID_THREAD_EXIT, wxEmptyString), Queue::eHIGHEST); // send all running threads the "EXIT" signal
	} 
	if(manager != NULL)
		manager->StopRendering();


	if(queue != NULL)
	{   if(threads.size() != 0)
			OnQuit();
		wxThread::Sleep(500);
		delete queue;
		queue = NULL; }	
		
	if(manager != NULL)
    {	manager->Delete();
		delete manager;
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
    wxGetApp().SetStatusText( wxT( "Preparing Environment..." ) );

  	int hres = w->vp.hres;
	int vres = w->vp.vres;
	int offset = 0;
	if(w->camera_ptr->get_stereo() == true)
	{	hres = hres * 2;
		offset = w->camera_ptr->get_offset();
		hres += offset;
	}


	// Check samples	
	int currentSamples = w->vp.num_samples;
	if(samples != currentSamples && samples != 0)
	{
		w->vp.set_samples(samples);
	}


	pixelsRendered = 0;
    pixelsToRender = hres * vres;
   
    //set the background
    wxBitmap bitmap(hres, vres, -1);
    wxMemoryDC dc;
    dc.SelectObject(bitmap);
    dc.SetBackground(*wxGREY_BRUSH);
    dc.Clear();
   
	if(renderMode == this->GRID || renderMode == this->SPIRAL_IN ||  renderMode == this->SPIRAL_OUT || renderMode == this->SPIRAL_IN_AND_OUT )
	{
		wxBitmap tile(bg_xpm);
   
		for(int x = 0; x < hres; x += 16)
		{
		   for(int y = 0; y < vres; y += 16)
			  dc.DrawBitmap(tile, x, y, FALSE);
		}	
	}
	
	else if(renderMode == this->RANDOM || renderMode == this->SEQUENCE || renderMode == this->SPIRAL_IN_AND_OUT2)
	{
		wxBitmap tile(black_xpm);
   
		for(int x = 0; x < hres; x += 16)
		{
		   for(int y = 0; y < vres; y += 16)
			  dc.DrawBitmap(tile, x, y, FALSE);
		}	
	}

   
	dc.SelectObject(wxNullBitmap);
   
    wxImage temp = bitmap.ConvertToImage();
    SetImage(temp);	

    updateTimer.Start(250);
 
    timer = new wxStopWatch();  //start timer 
    

    manager = new RenderThread(this);	// RenderThread communicates with the canvas rendered pixels
	manager->Create();
	manager->Run();
	manager->SetPriority(40);
	manager->SetRenderDisplay(this->renderDisplay);
	w->paintArea = manager;				// Threads communicate with the manager via the World

	if(divisionsNumber == 0)
		divisions = 8;  // 8 is a nice multiple of 2
	else
		divisions = divisionsNumber;
	// else division will be what is set.

	totalThreads = wxThread::GetCPUCount(); // get the total amout of cores for the system
    if(threadNumber == 0)
		threadNumber = totalThreads;    
	else if(threadNumber == 9000)
		threadNumber = divisions*divisions;

	
	wxGetApp().SetStatusText( wxT( "Setting up Rendering Queue..." ) );
	// Assign job division sizes
	int jobWidth = 0;
	int jobHeight = 0;

	if(w->vp.hres < divisions)	
		jobWidth = w->vp.hres;
	else
		jobWidth = w->vp.hres / divisions;
	if(w->vp.vres < divisions)	
		jobHeight = w->vp.vres;
	else 
		jobHeight = w->vp.vres / divisions;

	float xRemainder = w->vp.hres % divisions;
	float yRemainder = w->vp.vres % divisions;	
	
	if(renderMode == this->GRID ||
	   renderMode == this->RANDOM ||
	   renderMode == this->SEQUENCE ||
	   renderMode == this->SPIRAL_IN || 
	   renderMode == this->SPIRAL_OUT || 
	   renderMode == this->SPIRAL_IN_AND_OUT ||
	   renderMode == this->SPIRAL_IN_AND_OUT2 )
	{
		vector<Pixel> toRender;
		toRender.reserve(pixelsToRender);			

		// Generate Points for Spiral
		if(renderMode == this->SPIRAL_IN || renderMode == this->SPIRAL_OUT || renderMode == SPIRAL_IN_AND_OUT || renderMode == SPIRAL_IN_AND_OUT2)
		{	int x = 0;
			int y = 0;
			int hres = w->vp.hres-1;
			int vres = w->vp.vres-1;
			toRender = spiral(x, y, hres, vres, RIGHT);
			if(renderMode == SPIRAL_OUT)
			{	
				std::reverse(toRender.begin(), toRender.end()); // Reverse if spiral going out from center			
			}	
			else if(renderMode == SPIRAL_IN_AND_OUT)
			{
				std::vector<Pixel> center;
				std::vector<Pixel> bounds;
				center.reserve(toRender.size()/2);
				bounds.reserve(toRender.size()/2);
				int toSizeD2 = toRender.size()/2;
				int counter = 0;
				while (counter<toSizeD2)
				{
					bounds.push_back(toRender[counter++]);			
				}		

				std::reverse(toRender.begin(), toRender.end());
				counter = 0;
				while (counter<toSizeD2)
				{
					center.push_back(toRender[counter++]);					
				}		
				
				toRender.clear();
				int centerSize = center.size()-1;
				int boundsSize = bounds.size()-1;
				int boundsCounter = 0;		
				int centerCounter = 0;	
				for(counter = 0; counter<toSizeD2; counter++)
				{	if(centerCounter <= centerSize)
						toRender.push_back(center[centerCounter++]);					
					if(boundsCounter <= boundsSize )
						toRender.push_back(bounds[boundsCounter++]);					
				}
			}
			else if(renderMode == SPIRAL_IN_AND_OUT2)
			{
				// Spiral Two Ways
				std::vector<Pixel> center;
				std::vector<Pixel> bounds;
				center.reserve(toRender.size()/2);
				bounds.reserve(toRender.size()/2);
				int toSize = toRender.size();
				int counter = 0;
				while (counter<toSize)
				{	
					center.push_back(toRender[counter++]);
					if(counter<toSize)
						bounds.push_back(toRender[counter++]);
				}				
				toRender.clear();
				int centerSize = center.size()-1;
				int boundsSize = bounds.size()-1;
				int boundsCounter = 0;
				int toSizeD2 = toSize/2;
				for(counter = 0; counter<toSizeD2; counter++)
				{	if(centerSize >= 0)
						toRender.push_back(center[centerSize--]);
					else
						int kkkk = 0;
					if(boundsCounter <= boundsSize )
						toRender.push_back(bounds[boundsCounter++]);				
					else
						int kkkk = 0;
				}
			}
		}
		else if(renderMode == this->GRID)  //  Grid
		{
			int currentX = 0;
			int currentY = 0;	
			for(int y=0; y < divisions; y++)	
			{	currentY = y * jobHeight;
				for(int x=0; x < divisions; x++)
				{	
					currentX = x * jobWidth;				
					int xEnd = currentX + jobWidth;
					int yEnd = currentY + jobHeight;	
					for(int iy = currentY; iy<yEnd; iy++)					
					{
						for(int ix = currentX; ix<xEnd; ix++)
						{
							toRender.push_back(Pixel(ix, iy));
						}
					}
				}

			}
			if(yRemainder != 0)
			{		int yStart = w->vp.vres - yRemainder;
					int xStart = 0;
					int yEnd = w->vp.vres;
					int xEnd = w->vp.hres;
					for(int iY = yStart; iY < yEnd; iY++)
					{
						for(int iX = xStart; iX < xEnd; iX++)
						{
							toRender.push_back(Pixel(iX, iY));
						}
					}					
			}
			if(xRemainder != 0)
			{		int yStart = 0;
					int xStart = w->vp.hres - xRemainder;
					int yEnd = w->vp.vres;
					int xEnd = w->vp.hres;
					for(int iX = xStart; iX < xEnd; iX++)					
					{
						for(int iY = yStart; iY < yEnd; iY++)
						{
							toRender.push_back(Pixel(iX, iY));
						}
					}
			}
		}
		else // if not spiral (Random, Sequence)
		{
			for(int y=0; y < w->vp.vres; y++)	
			{
				for(int x=0; x < w->vp.hres; x++)
				{	
					toRender.push_back(Pixel(x, y));
				}
			}	
		}
				
		if(renderMode == this->RANDOM)
		{	
			std::random_shuffle(toRender.begin(), toRender.end());		
		}
		
		int totalQ = jobWidth * jobHeight;

		int id = 0;	

		vector<Pixel> current;
		current.reserve(totalQ);
		for(int y=0; y < divisions; y++)	
		{
			for(int x=0; x < divisions; x++)
			{		
				if(id < toRender.size())
				{
					for(int i = 0; i<totalQ; i++)
					{	
						if(id >= toRender.size())
							break;
						current.push_back(toRender[id]);					
						id++;
					}

					int iJob = rand();
					queue->AddJob(tJOB(tJOB::eID_THREAD_JOB, wxString::Format(wxT("%u"), iJob), current));
					current.clear();
				}
			}		

		}			
		if(id < pixelsToRender)
		{
			while(id < pixelsToRender)
			{
				for(int i = 0; i < totalQ; i++)
				{
					current.push_back(toRender[id]);
					id++;
					if(id == pixelsToRender)
						i = totalQ;
				}
				int iJob = rand();
				queue->AddJob(tJOB(tJOB::eID_THREAD_JOB, wxString::Format(wxT("%u"), iJob), current));
				current.clear();
			}
		}
		toRender.clear();			
	}

	
	/*if(w->photontracer_ptr != NULL)
	{	wxGetApp().SetStatusText( wxT( "Building PhotonMap..." ) );
		w->generate_photonmap();
	}*/
		

	//threadNumber = divisions*divisions;		// make the number of threads the number of jobs
	//threadNumber = 4;							// simulate quad core cpu
	//threadNumber = 2;							// simulate dual core cpu
	//threadNumber = 1;							// simulate singe core cpu
	if(threadNumber >= 1000)
		threadNumber = 1000;
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
	wxGetApp().SetStatusText( wxT( "Rendering..." ) );

}

std::vector<Pixel> RenderCanvas::spiral(int &x, int &y, int &width, int &height, Direction direction)
{
	vector<Pixel> current;
	Direction newDirection = RIGHT;
	int value = 0;
	/*bool end = false;
	if(height != width)
	{
		if(height <= 1 || width <= 1)
		{
			if(height > width)
			{
				width = 0;
				height = 0;
				return current;
			}
			else if(width > height)
			{
				width = 0;
				height = 0;
				return current;
			}
		}
	}*/

	switch(direction)
	{
		case RIGHT:
			
			value = width;
			for(; x<=value; x++)
			{
				current.push_back(Pixel(x,y));
			}			
			x -= 1;
			y += 1;
			newDirection = UP;
			break;
		case UP:			
			value = height;
			for(; y<=value; y++)
			{
				current.push_back(Pixel(x,y));
			}			
			y -= 1;
			newDirection = LEFT;
			break;
		case LEFT:
			value = w->vp.hres - (width + 1);
			while(x>value)
			{	--x;
				current.push_back(Pixel(x,y));				
			}
			if(width >= 1)
				width -= 1;
			else
				int i = 0;
			newDirection = DOWN;
			break;
		case DOWN:
			if(height >= 1)
			{	
				height -= 1;
			}
			else
				int i = 0;
			value = w->vp.vres - (height + 1);
			while(y>value)
			{	--y;
				current.push_back(Pixel(x,y));
			}			
			newDirection = RIGHT;
			x += 1;
			break;
	}	
	if(!(width <= 0 && height <= 0) && current.size() != 0)
	{
		vector<Pixel> addVector;
		addVector = spiral(x,y, width, height, newDirection);
		current.insert(current.end(), addVector.begin(), addVector.end());
	}
	else
	{
     int temp = 0;
	}
	return current;
}

void RenderCanvas::OnStart(wxCommandEvent& WXUNUSED(event)) // start one worker thread
{
    int id = threads.empty()?1:threads.back()+1;
    threads.push_back(id);
	WorkerThread* thread=new WorkerThread(queue, this, w, id); // create a new worker thread, increment thread counter (this implies, thread will start OK)
    theThreads.push_back(thread);	
	if (thread->Run() != wxTHREAD_NO_ERROR )
    {	wxLogError("Can't create the worker thread!");
		delete theThreads.back();
		threads.back() = NULL;
		threads.pop_back(); }
	else
		thread->SetPriority(20);
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
	thread->SetPriority(40);
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
{		
	
	if(!threads.empty())
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
					//(*iter)->Delete();
					//delete (*iter);
					//(*iter) = NULL;
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
   pixels.clear();   

   if(timer != NULL)
   {   delete timer;
	   timer = NULL;
   }
}

bool RenderThread::Stop() const
{
   return stop;
}

RenderDisplay RenderThread::Display() const
{
   return renderDisplay;
}

void RenderThread::StopRendering()
{
	stop = true;
}

void RenderThread::SetRenderDisplay(RenderDisplay display)
{
	renderDisplay = display;
}

void *RenderThread::Entry()
{
	lastUpdateTime = 0;
	timer = new wxStopWatch();

	while(!TestDestroy())
	{   NotifyCanvas();	     // Send pixels to canvas
		wxThread::Sleep(50); // Sleep for 50 milliseconds		
	}
	return NULL;
}
