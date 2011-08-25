Multithreaded Ray Tracing from the Ground Up Skeleton Framework by Daniel Rosser
----------------------------------------------

Based on the Textbook "Ray Tracing from the Ground Up" by Kevin Suffern
http://www.raytracegroundup.com/

Original Skeleton made by Sverre Kvaale can be found on the downloads page of the textbook's website
http://www.raytracegroundup.com/downloads.html

Project Files
----------------------------------------------
wxRaytracer.NET2003.sln   - Microsoft Visual Studio .NET 2003
wxRaytracerVCPP2005EE.sln - Microsoft Visual C++ 2005 Express Edition
wxRaytracerVCPP2008EE.sln - Microsoft Visual C++ 2008 Express Edition
wxRaytracerVCPP2010EE.sln - Microsoft Visual C++ 2010 Express Edition

Other Enhancements in the Multithreaded Skeleton:
----------------------------------------------
-Save as JPEG default at 100 quality
-Pausing/Resuming the render works
-Queue system to split up screen into Job's
-Enhanced WxWidgets settings


How to Extend a project built with the original Skeleton?
----------------------------------------------
1. ) Download and extract the Multithreaded Skeleton
2. ) Use the Multithreaded Solution as the main project and start adding your custom made classes and files to the project directory/solution.
3. ) You will need to Merge and Update the Pinhole Camera class using the example function Pinhole::render_scene(const World& w, const PixelPoints& grid)
3.a) Note the implementation of this function with the skeleton is the basic type, you need to update the Sampler and other variables.
4. ) Make sure if you are using any compound objects to read the Notes below:

Notes on implementation
----------------------------------------------
1.a)Implementing Grids or Compound objects you are going to want to be careful with the following function.
 "material_ptr = objects[j]->get_material();" 
 Materials can not be returned the way described in the textbook for grids/compound objects (using a temporary local variable bound to the upper abstract object as this object is being used by multiple threads)
 In the Multithreaded version you store the returned material inside the ShadeRec. "sr.material_ptr" instead of in the compound object material_ptr.

----------------------------------------------
Q: ) What's changed from the Original Skeleton?
A: ) The following files have the main changes from the Skeleton:
		User Interface/
		  -wxraytracer.h/cpp
		  -ShadeRec.h/cpp
		World/
		  -World.h/cpp
		Utilities/
		  -ShadeRec.h/cpp
		  +MultiThread.h
		Samplers/
		  -Sampler.h/cpp
		Cameras/
		  -Camera.h/cpp
		  -Pinhole.h/cpp
		Tracers/
		  -Tracer.h/cpp
		  -Whitted.h/cpp
		  -AreaLighting.h/cpp
		  -RayCast.h/cpp


