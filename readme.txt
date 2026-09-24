Multithreaded Ray Tracing from the Ground Up Skeleton Framework by Daniel Rosser
----------------------------------------------

Based on the Textbook "Ray Tracing from the Ground Up" by Kevin Suffern
https://www.amazon.com/Ray-Tracing-Ground-Kevin-Suffern/dp/1568812728

Original Skeleton made by Sverre Kvaale was hosted on the textbook's website
(raytracegroundup.com, now offline) - archived copy with the Downloads page:
https://web.archive.org/web/20140314141933/http://www.raytracegroundup.com/

Project Files
----------------------------------------------
wxRaytracer.NET2003.sln   - Microsoft Visual Studio .NET 2003
wxRaytracerVCPP2005EE.sln - Microsoft Visual C++ 2005 Express Edition (Also works with Professional, Ultimate)
wxRaytracerVCPP2008EE.sln - Microsoft Visual C++ 2008 Express Edition (Also works with Professional, Ultimate)
wxRaytracerVCPP2010EE.sln - Microsoft Visual C++ 2010 Express Edition (Also works with Professional, Ultimate)

Recommendations:
-Use Visual Studio 2010 for the project as I have optimised the Solution file for faster speeds in Release mode.
-When rendering an image that is going to take a long time, always use Release mode unless you are debugging a problem, speed increase of 2-4 times in Release.
-Start your project with this Skeleton as the base if you want Multithreading, else as stated below you can merge them later.

Other Enhancements in the Multithreaded Skeleton:
----------------------------------------------
-Save as JPEG default at 100 quality
-Pausing/Resuming the render works
-Queue system to split up screen into Job's
-Enhanced WxWidgets settings
-Mersenne Twister Random Number Generator
-SmartPointer and Reference Counting for Memory Management of some Pointers.


How to Extend a project built with the original Skeleton?
----------------------------------------------
1. ) Download and extract the Multithreaded Skeleton
2. ) Use the Multithreaded Solution as the main project and start adding your custom made classes and files to the project directory/solution.
3. ) You will need to Merge and Update the Pinhole Camera class using the example function Pinhole::render_scene(const World& w, const std::vector<Pixel>& pixels)
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
		  -wxraytracer.h/cpp (Interface changes, Queue System added, Multithreading added, fixed crashes, Pause / End now working) 
		World/
		  -World.h/cpp (Changes for Multithreading)
		Utilities/
		  -ShadeRec.h/cpp (Added variables: Sync Variable, Jump and Count used for Samplers over multiple threads)
		  +MultiThread.h (Multithreading Data structures)
		  +RandomNumber.h (Wrapper for RandomNumber Generator)
		  +MTRand.h/cpp (Mersenne twister Random Number Generator)
		  +SmartPointer.h (SmartPointer Template)
		  +ReferenceCount.h (ReferenceCount class to be used by the SmartPointer)
		Samplers/
		  -Sampler.h/cpp (Function changes for Multithreading and RandomNumber generator)
		Cameras/
		  -Camera.h/cpp (Added overloaded function for Multithreading)
		  -Pinhole.h/cpp (As above)
		Tracers/
		  -Tracer.h/cpp (Added overloaded function for Multithreading)
		  -Whitted.h/cpp (Added overloaded function for Multithreading)
		  -AreaLighting.h/cpp (Added overloaded function for Multithreading)
		  -RayCast.h/cpp (Added overloaded function for Multithreading)

----------------------------------------------
Modern builds (VS2019 / VS2022 / VSCode) - see BUILD.md
----------------------------------------------
New solutions wxRaytracerVS2019.sln (v142), wxRaytracerVS2022.sln (v143)
and wxRaytracerVS2026.sln (v145) target wxWidgets 3.2.x (Unicode, Win32 + x64,
C++17) via the WXWIN env var; CMakeLists.txt + .vscode/ cover VSCode, and
.github/workflows/build-windows.yml tests VS2022 and VS2026.
The bundled wxWidgets 2.8.0 tree is legacy-only and cannot link on VS2019+.

Related: openFrameworks port skeleton - danoli3/ofxRayTracer
https://github.com/danoli3/ofxRayTracer

----------------------------------------------
Build and run on macOS (Homebrew + CMake)
----------------------------------------------
1. ) Install the dependencies:
     brew install wxwidgets cmake
2. ) From the repository root, configure and build (Release):
     cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
     cmake --build build --parallel
3. ) Run it from the repository root (exact binary name):
     ./build/wxRaytracer
4. ) In the app: Render > Start to ray trace the image,
     File > Save As... to save it (JPEG default).

Notes:
- Requires wxWidgets 3.2 or newer (Homebrew supplies it).
- Use Release for rendering speed (2-4x faster than Debug).

----------------------------------------------
Build and run on Windows with Visual Studio 2026
----------------------------------------------
1. ) Install Visual Studio 2026 (v145 toolset) and CMake.
2. ) Install/build wxWidgets 3.2.x and set the WXWIN environment
     variable to its root, e.g. C:\wxWidgets-3.2.10
     (see BUILD.md for the from-source steps).
3. ) Open wxRaytracerVS2026.sln, select Release + x64,
     then Build > Build Solution (F5 to build and run).
4. ) The exe lands in:
     wxRaytracer\bin\vs2026\x64\Release\wxRaytracer.exe
     Run it from Explorer, the terminal, or with F5.
5. ) If Windows reports missing wx DLLs at startup, add the matching
     wx lib dir to PATH, e.g. %WXWIN%\lib\vc145_x64_dll
     (or copy those DLLs next to the exe).
6. ) In the app: Render > Start to ray trace the image,
     File > Save As... to save it (JPEG default).


