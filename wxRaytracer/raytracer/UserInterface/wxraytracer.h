#ifndef _WXRAYTRACER_H_
#define _WXRAYTRACER_H_

/**
 * Ray Tracer skeleton
 *
 * Author : Sverre Kvaale <sverre@kvaale.com>
 * Multithreading Author: Daniel Rosser <danoli3@gmail.com>
 * Version: 1.3.1
 */
#include <wx/setup.h>
#include <wx/wx.h>
#include <wx/frame.h>
#include <wx/thread.h>
#include <wx/menu.h>
#include <wx/app.h>
#include <wx/rawbmp.h>
#include <wx/event.h>

#include <stdlib.h>
#include <assert.h>
#include <map>
#include <list>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <iterator>

#include "World.h"
#include <vector>
#include "MultiThread.h"

#define SAMPLE_HACK 0 // 0 - (One shared world object) = Ideal
					   // 1 - Creates multiple instances of World (1 per thread) 
#define WXWIDGETS292 0 // 0 is standard WxWidgets 2.6->2.8
						// 1 is WxWidgets 2.9.2 (Due to changes in some declarations)
				      

class wxraytracerFrame;
class RenderCanvas;
class RenderThread;
class WorkerThread;
class RenderPixel;

#if WXWIDGETS292>0
	wxDECLARE_EVENT(wxEVT_THREAD, wxThreadEvent); // 2.9.2 way of declaring events
#else
	DECLARE_EVENT_TYPE(wxEVT_THREAD, -1)
#endif


class tJOB
{
  public:
    enum tCOMMANDS                    // list of commands that are currently implemented
    {
	  eID_THREAD_EXIT_SUCESS = 0,
      eID_THREAD_EXIT=wxID_EXIT,	  // thread should exit or wants to exit
      eID_THREAD_NULL=wxID_HIGHEST+1, // dummy command
      eID_THREAD_STARTED,		      // worker thread has started OK
      eID_THREAD_JOB,				  // process normal job
      eID_THREAD_JOBERR				  // process errorneous job after which thread likes to exit
    }; // enum tCOMMANDS
  
    tJOB() : cmd(eID_THREAD_NULL) {}
	tJOB(tCOMMANDS cmd, const wxString& arg) : cmd(cmd), arg(arg){}	
	tJOB(tCOMMANDS cmd, const wxString& arg, vector<Pixel>& jobID) : cmd(cmd), arg(arg)
	{ theJobs = jobID; }
    tCOMMANDS cmd; wxString arg;	
	vector<Pixel> theJobs;
}; // class tJOB
  
  class Queue
  {
  public:
    enum tPRIORITY { eHIGHEST, eHIGHER, eNORMAL, eBELOW_NORMAL, eLOW, eIDLE }; // priority classes

    Queue(wxEvtHandler* parent) : parent(parent) {}

    void AddJob(const tJOB& job, const tPRIORITY& priority=eLOW);			   // push a job with given priority class onto the FIFO
    
    tJOB Pop();

    void Report(const tJOB::tCOMMANDS& cmd, const wxString& sArg=wxEmptyString, int iArg=0); // report back to parent  

    size_t Stacksize();

	std::multimap<tPRIORITY, tJOB> jobs; // multimap to reflect prioritization: values with lower keys come first, newer values with same key are appended
    
  
  private:
    wxEvtHandler* parent;
    wxMutex mutexQueue;					 // protects queue access
    wxSemaphore queueCount;				 // semaphore count reflects number of queued jobs
};


  
class WorkerThread : public wxThread
{
  public:
    WorkerThread(Queue* pQueue, RenderCanvas* canvas, World* world, int id=0) : wxThread(wxTHREAD_DETACHED), queue(pQueue), canvas(canvas), world(world), id(id) { assert(pQueue); wxThread::Create(); }
    ~WorkerThread();
	virtual tJOB::tCOMMANDS OnJob();
	unsigned int getID() const;

  private:
	virtual wxThread::ExitCode Entry();

    Queue* queue;
    unsigned int id;
	RenderCanvas* canvas;
	World* world;
  
       
    

}; // class WorkerThread : public wxThread



class wxraytracerapp : public wxApp
{
public:
   virtual bool OnInit();
   virtual int OnExit();
   virtual void SetStatusText(const wxString&  text, int number = 0);

private:
   wxraytracerFrame *frame;
   DECLARE_EVENT_TABLE()
};

class wxraytracerFrame : public wxFrame
{
public:
   wxraytracerFrame(const wxPoint& pos, const wxSize& size);
   
   //event handlers
   void OnQuit( wxCommandEvent& event );
   void OnClose( wxCommandEvent& event );
   void OnOpenFile( wxCommandEvent& event );
   void OnSaveFile( wxCommandEvent& event );
   void OnRenderStart( wxCommandEvent& event );
   void OnRenderCompleted( wxCommandEvent& event );
   void OnRenderPause( wxCommandEvent& event );
   void OnRenderResume( wxCommandEvent& event );
   void OnThreadDefault( wxCommandEvent& event );
   void OnThreadSingle( wxCommandEvent& event );
   void OnThreadDual( wxCommandEvent& event );
   void OnThreadQuad( wxCommandEvent& event );
   void OnThreadJob( wxCommandEvent& event );
   void OnRenderModeGrid ( wxCommandEvent& event );
   void OnRenderModeRandom ( wxCommandEvent& event );
   void OnRenderModeSpiralOut ( wxCommandEvent& event );
   void OnRenderModeSpiralIn ( wxCommandEvent& event );
   void OnRenderModeSpiralInAndOut ( wxCommandEvent& event );
   void OnRenderModeSpiralInAndOut2 ( wxCommandEvent& event );
   void OnRenderModeSequence ( wxCommandEvent& event );
   void OnRenderModeSequence2 ( wxCommandEvent& event );
   void OnRenderDisplayPixel ( wxCommandEvent& event );
   void OnRenderDisplayRow ( wxCommandEvent& event );
   void OnRenderDisplayJob ( wxCommandEvent& event );
   void OnDivisionDefault ( wxCommandEvent& event );
   void OnDivisionSingle ( wxCommandEvent& event );
   void OnDivisionDual ( wxCommandEvent& event );
   void OnDivisionQuad ( wxCommandEvent& event );
   void OnDivision16 ( wxCommandEvent& event );
   void OnDivision32 ( wxCommandEvent& event );
   void OnDivision64 ( wxCommandEvent& event );
   void OnSamples_Build( wxCommandEvent& event );
   void OnSamples_1( wxCommandEvent& event );
   void OnSamples_2( wxCommandEvent& event );
   void OnSamples_4( wxCommandEvent& event );
   void OnSamples_8( wxCommandEvent& event );
   void OnSamples_16( wxCommandEvent& event );
   void OnSamples_24( wxCommandEvent& event );
   void OnSamples_32( wxCommandEvent& event );
   void OnSamples_64( wxCommandEvent& event );
   void OnSamples_128( wxCommandEvent& event );
   void OnSamples_256( wxCommandEvent& event );

private:
   RenderCanvas *canvas; //where the rendering takes place
   wxString currentPath; //for file dialogues  
   DECLARE_EVENT_TABLE()
};

//IDs for menu items
enum
{
   Menu_File_Quit = 100,
   Menu_File_Open,
   Menu_File_Save,
   Menu_Render_Start,
   Menu_Render_Pause,
   Menu_Render_Resume,
   Menu_Thread_Default,
   Menu_Thread_Single,
   Menu_Thread_Dual,
   Menu_Thread_Quad,
   Menu_Thread_Job,
   Menu_Render_Mode_Grid,
   Menu_Render_Mode_Random,
   Menu_Render_Mode_Spiral_Out,
   Menu_Render_Mode_Spiral_In,
   Menu_Render_Mode_Spiral_In_And_Out,
   Menu_Render_Mode_Spiral_In_And_Out2,
   Menu_Render_Mode_Sequence,
   Menu_Render_Mode_Sequence2,
   Menu_Render_Display_Pixel,
   Menu_Render_Display_Row,
   Menu_Render_Display_Job,
   Menu_Division_Default,
   Menu_Division_Single,
   Menu_Division_Dual,
   Menu_Division_Quad,
   Menu_Division_16,
   Menu_Division_32,
   Menu_Division_64,
   Menu_Samples_Build,
   Menu_Samples_1,
   Menu_Samples_2,
   Menu_Samples_4,
   Menu_Samples_8,
   Menu_Samples_16,
   Menu_Samples_24,
   Menu_Samples_32,
   Menu_Samples_64,
   Menu_Samples_128,
   Menu_Samples_256
};



class RenderCanvas: public wxScrolledWindow
{	
	enum { eQUIT=wxID_CLOSE, eSTART_THREAD=wxID_HIGHEST+100 };
   

public:

   enum RenderMode { GRID, RANDOM, SPIRAL_IN, SPIRAL_OUT, SPIRAL_IN_AND_OUT, SPIRAL_IN_AND_OUT2, SEQUENCE, SEQUENCE2 };
   enum Direction { UP, DOWN, LEFT, RIGHT, END };

   RenderCanvas(wxWindow *parent);
   virtual ~RenderCanvas(void);
    
   void SetImage(wxImage& image);
   wxImage GetImage(void);
   
   virtual void OnDraw(wxDC& dc);
   void renderStart(void);
   void renderPause(void);
   void renderResume(void);
   void OnRenderCompleted( wxCommandEvent& event );
   void OnTimerUpdate( wxTimerEvent& event );
   void OnNewPixel( wxCommandEvent& event );

   void OnStart(wxCommandEvent& WXUNUSED(event)); // start one worker thread
   void OnStop(wxCommandEvent& WXUNUSED(event));   // stop one worker thread
   void OnJob(wxCommandEvent& event); // handler for launching a job for worker thread(s)
   void addJob();
   void OnThread(wxCommandEvent& event); // handler for thread notifications
   void addThread();
   void OnQuit();
   void OnStart();

   // recursive function to calculate spiral
   void spiral(unsigned int &x, unsigned int &y, unsigned int &width, unsigned int &height, Direction direction, std::vector<Pixel> &pixels);
   // changed spiral function to be loop based
   void spiral(std::vector<Pixel> &pixels);

   unsigned int totalThreads; 
   unsigned int threadNumber;
   unsigned int divisions;
   unsigned int divisionsNumber;
   RenderMode renderMode;
   RenderDisplay renderDisplay;
   World* w;
   RenderThread* manager;
   unsigned int samples;
   wxCriticalSection managerCS;
   vector<wxCriticalSection*> threadsCS;    // protects the thread pointers
   vector<WorkerThread*> theThreads;
protected:
   wxBitmap *theImage;
   wxStopWatch* timer; 
   Queue* queue;   
   std::list<unsigned int> threads; 
   
   
#if SAMPLE_HACK>0
   vector<World*> theWorlds;
#endif
   vector<WorkerThread*>::iterator iter;

private:    
   unsigned long pixelsRendered;
   unsigned long pixelsToRender;
   wxTimer updateTimer;
   
   DECLARE_EVENT_TABLE()
};



class RenderPixel
{
public:
   RenderPixel()
	   : x(0), y(0), red(0), green(0), blue(0)
   {}

   RenderPixel(unsigned int x, unsigned int y, unsigned int red, unsigned int green, unsigned int blue);

public:
   unsigned int x, y;
   unsigned int red, green, blue;
};


DECLARE_EVENT_TYPE(wxEVT_RENDER, -1)
#define ID_RENDER_COMPLETED 100
#define ID_RENDER_NEWPIXEL  101
#define ID_RENDER_UPDATE    102

class RenderThread : public wxThread
{
public:
   RenderThread(RenderCanvas* c) : wxThread(wxTHREAD_DETACHED), canvas(c), timer(NULL), stop(false) {}
   virtual void *Entry();
   virtual void OnExit();
   ~RenderThread();
   virtual void setPixel(int x, int y, int red, int green, int blue);
   virtual void setPixel(const list<RenderedInt>& rendered);
   virtual void setPixel(const vector<RenderedInt>& rendered);
   virtual void setPixel(const vector<RenderPixel*>& rendered);
   virtual bool Stop() const;
   virtual void StopRendering();
   virtual void SetRenderDisplay(RenderDisplay display);
   virtual RenderDisplay Display() const;
private:
   void NotifyCanvas();
   RenderCanvas* canvas;   
   vector<RenderPixel*>* pixels;
   long lastUpdateTime; 
   wxStopWatch* timer;
   bool stop;
   RenderDisplay renderDisplay;

  // wxMutex mutex;
   wxCriticalSection critical;
};


#endif
