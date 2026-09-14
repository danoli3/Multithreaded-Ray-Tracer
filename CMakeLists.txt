# wxRaytracer — CMake build (VSCode, VS2019/2022, Linux, macOS)
#
#   cmake -S . -B build -DwxWidgets_ROOT_DIR=$env:WXWIN   # Windows PowerShell
#   cmake --build build --config Release
#
# Requires wxWidgets 3.2.x. Point wxWidgets_ROOT_DIR (or WXWIN) at it.
cmake_minimum_required(VERSION 3.20)

project(wxRaytracer VERSION 1.3.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(WXRAYTRACER_STATIC_WX "Link wxWidgets statically instead of shared/DLL" OFF)

if(WXRAYTRACER_STATIC_WX)
  set(wxWidgets_USE_STATIC ON)
endif()
set(wxWidgets_USE_UNICODE ON)

if(DEFINED ENV{WXWIN} AND NOT DEFINED wxWidgets_ROOT_DIR)
  set(wxWidgets_ROOT_DIR $ENV{WXWIN})
endif()

find_package(wxWidgets 3.2 REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

set(RT wxRaytracer/raytracer)

set(WXRAYTRACER_SOURCES
  ${RT}/BRDFs/BRDF.cpp
  ${RT}/BRDFs/Lambertian.cpp
  ${RT}/build/BuildShadedObjects.cpp
  ${RT}/Cameras/Camera.cpp
  ${RT}/Cameras/Pinhole.cpp
  ${RT}/GeometricObjects/GeometricObject.cpp
  ${RT}/GeometricObjects/Plane.cpp
  ${RT}/GeometricObjects/Sphere.cpp
  ${RT}/Lights/Ambient.cpp
  ${RT}/Lights/Directional.cpp
  ${RT}/Lights/Light.cpp
  ${RT}/Materials/Material.cpp
  ${RT}/Materials/Matte.cpp
  ${RT}/Samplers/Hammersley.cpp
  ${RT}/Samplers/Jittered.cpp
  ${RT}/Samplers/MultiJittered.cpp
  ${RT}/Samplers/NRooks.cpp
  ${RT}/Samplers/PureRandom.cpp
  ${RT}/Samplers/Regular.cpp
  ${RT}/Samplers/Sampler.cpp
  ${RT}/Tracers/AreaLighting.cpp
  ${RT}/Tracers/MultipleObjects.cpp
  ${RT}/Tracers/RayCast.cpp
  ${RT}/Tracers/SingleSphere.cpp
  ${RT}/Tracers/Tracer.cpp
  ${RT}/Tracers/Whitted.cpp
  ${RT}/UserInterface/wxraytracer.cpp
  ${RT}/Utilities/Maths.cpp
  ${RT}/Utilities/Matrix.cpp
  ${RT}/Utilities/MTRand.cpp
  ${RT}/Utilities/Normal.cpp
  ${RT}/Utilities/Point2D.cpp
  ${RT}/Utilities/Point3D.cpp
  ${RT}/Utilities/Ray.cpp
  ${RT}/Utilities/RGBColor.cpp
  ${RT}/Utilities/ShadeRec.cpp
  ${RT}/Utilities/Vector3D.cpp
  ${RT}/World/ViewPlane.cpp
  ${RT}/World/World.cpp
)

if(WIN32)
  add_executable(wxRaytracer WIN32 ${WXRAYTRACER_SOURCES})
else()
  add_executable(wxRaytracer ${WXRAYTRACER_SOURCES})
endif()

target_include_directories(wxRaytracer PRIVATE
  ${RT}/BRDFs
  ${RT}/build
  ${RT}/Cameras
  ${RT}/GeometricObjects
  ${RT}/Lights
  ${RT}/Materials
  ${RT}/Samplers
  ${RT}/Tracers
  ${RT}/UserInterface
  ${RT}/Utilities
  ${RT}/World
)

target_link_libraries(wxRaytracer PRIVATE ${wxWidgets_LIBRARIES})
target_compile_definitions(wxRaytracer PRIVATE ${wxWidgets_DEFINITIONS})
target_compile_options(wxRaytracer PRIVATE ${wxWidgets_CXX_FLAGS})

if(MSVC)
  target_compile_options(wxRaytracer PRIVATE /W3 /MP)
endif()
