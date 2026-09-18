# Building wxRaytracer (VS2019 / VS2022 / VSCode)

## wxWidgets version audit

| | Old (bundled) | New (required) |
|---|---|---|
| Version | **2.8.0** (`wxRaytracer/wxwidgets/include/wx/version.h`: `wxMAJOR_VERSION 2`, `wxMINOR_VERSION 8`, `wxRELEASE_NUMBER 0`, string `"wxWidgets 2.8.0"`, ~2006) | **3.2.x** (CI uses 3.2.10; a newer 3.3.x line also exists) |
| Char set | ANSI (`<CharacterSet>NotSet</CharacterSet>`) | Unicode |
| Arch | Win32 only, prebuilt `wxmsw28_core.lib` / `wxbase28.lib` / `wxtiff` / `wxjpeg` / `wxpng` / `wxzlib` | Win32 + x64, built from source or vcpkg |
| VS support | Up to VS2012 (v110); the 2.8 headers/libs do **not** work on VS2019 (v142) / VS2022 (v143) | v142 / v143 / latest |

The bundled `wxRaytracer/wxwidgets/` tree is kept for the legacy solutions only.
The new projects ignore it: they consume a system wxWidgets 3.2 via the
**`WXWIN`** environment variable (e.g. `C:\wxWidgets-3.2.10`).

Get wxWidgets 3.2: build from source (`wx/build/msw/makefile.vc`,
`BUILD=release|debug SHARED=1 TARGET_CPU=X64`) or `vcpkg install wxwidgets`.
Bump `WX_VERSION` in `.github/workflows/build-windows.yml` to move versions.

## Project files

| File | Toolchain |
|---|---|
| `wxRaytracerVS2019.sln` + `wxRaytracer/wxRaytracer_vs2019.vcxproj` (+`.filters`) | VS2019, v142, Win32 + x64, Debug + Release, C++17, Unicode |
| `wxRaytracerVS2022.sln` + `wxRaytracer/wxRaytracer_vs2022.vcxproj` (+`.filters`) | VS2022, v143, Win32 + x64, Debug + Release, C++17, Unicode |
| `wxRaytracerVS2026.sln` + `wxRaytracer/wxRaytracer_vs2026.vcxproj` (+`.filters`) | VS2026, v145, Win32 + x64, Debug + Release, C++17, Unicode |
| `CMakeLists.txt` | VSCode (CMake Tools or `Tasks: Run Build Task`), VS2019/2022/2026 via CMake, Linux/macOS |
| `.vscode/{tasks,launch,c_cpp_properties,extensions}.json` | VSCode build/debug/IntelliSense |
| `.github/workflows/build-windows.yml` | CI: VS2022 job and VS2026 job each build their `.sln` (Debug+Release|x64) and CMake |

Unlike the 2010–2012 projects, the new `.vcxproj` files compile **all**
sources on disk, including `raytracer/build/BuildShadedObjects.cpp`
(defines `World::build()`, called from the UI — the old projects omitted it
and could not link) and the `Whitted` / `AreaLighting` tracers.

## Build steps

1. Install wxWidgets 3.2.x and set `WXWIN` to its root.
2. VS2019: open `wxRaytracerVS2019.sln`. VS2022: open `wxRaytracerVS2022.sln`.
   VS2026: open `wxRaytracerVS2026.sln` (v145; accepts wx lib dirs `vc145_*`,
   falling back to `vc143_*` for wx releases predating VS2026).
   Prefer **x64** (Win32 works but is legacy).
3. VSCode: install the recommended extensions, set `WXWIN`, then run the
   default build task (`CMake: build`). Or manually:
   `cmake -S . -B build -DwxWidgets_ROOT_DIR=%WXWIN%` then
   `cmake --build build --config Release`.
4. Static (non-DLL) wx builds: remove `WXUSINGDLL` from the project's
   preprocessor definitions (or pass `-DWXRAYTRACER_STATIC_WX=ON` to CMake).

## Source changes made for wx 3.2 / C++17

All in `wxRaytracer/raytracer/` (legacy wx 2.8 path preserved behind
`WXWIDGETS292`, now auto-detected via `wxCHECK_VERSION`):

- `UserInterface/wxraytracer.h/.cpp`: `wxDECLARE_EVENT` / `wxDEFINE_EVENT`
  for `wxEVT_THREAD` and `wxEVT_RENDER` on wx ≥ 2.9 (the old
  `DECLARE_EVENT_TYPE` macros are gone in wx 3.x).
- `UserInterface/wxraytracer.cpp`: 138 `menu->Enable/Check(menu->FindItem(label))`
  calls now use the `Menu_*` enum IDs directly — wx 3.x `FindItem(wxString)`
  returns `wxMenuItem*`, not `int`, so the old form no longer compiles.
  Seven `(wxChar*)"..."` casts became `wxT("...")` (garbage in Unicode builds).
- `UserInterface/wxraytracer.cpp`, `Samplers/Sampler.cpp`:
  `random_shuffle` (removed in C++17) became `std::shuffle` with `std::mt19937`.
- `UserInterface/wxraytracer.h/.cpp`: `wxEVT_RENDER` is now a `wxThreadEvent`
  and the pixel vector travels via `SetPayload`/`GetPayload` —
  `wxCommandEvent::SetClientData`/`GetClientData` were removed in wx 3.x.

## Related projects

- [danoli3/ofxRayTracer](https://github.com/danoli3/ofxRayTracer) —
  openFrameworks port of Kevin Suffern's *Ray Tracing from the Ground Up*
  core (originally wxRayTracer), with a progressive multi-threaded CPU renderer.
- Textbook: *Ray Tracing from the Ground Up* by Kevin Suffern —
  [Amazon](https://www.amazon.com/Ray-Tracing-Ground-Kevin-Suffern/dp/1568812728).
  The author's site (raytracegroundup.com) is offline, but a full snapshot —
  including the Downloads page with the skeleton code — is on the Wayback
  Machine: [archived site](https://web.archive.org/web/20140314141933/http://www.raytracegroundup.com/).

Legacy solutions (`wxRaytracer.NET2003.sln`, `wxRaytracerVCPP2005EE.sln`,
`...2008EE`, `...2010EE`, `...2012EE`) and the bundled wxWidgets 2.8 tree are
left untouched for reference.
