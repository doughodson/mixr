
--
-- If premake command is not supplied an action (target compiler), exit!
--
-- Targets of interest:
--     vs2017     (Visual Studio 2017)
--     vs2019     (Visual Studio 2019)
--
if (_ACTION == nil) then
    return
end

--
-- set include and library paths
--
DEPS_IncPath         = "../../deps/include"

workspace "deps"

   -- destination directory for generated solution/project files
   location ("../" .. _ACTION)

   -- destination directory for compiled binary target
   targetdir ("../../lib/")

   -- creating static libraries
   kind "StaticLib"

   -- C++ code in all projects
   language "C++"
   cppdialect "C++14"

   --
   -- Build (solution) configuration options:
   --     Release        (Runtime library is Multi-threaded DLL)
   --     Debug          (Runtime library is Multi-threaded Debug DLL)
   --
   configurations { "Release", "Debug" }
   platforms { "Win32", "Win64" }

   -- visual studio options and warnings
   -- /wd4351 (C4351 warning) - disable warning associated with array brace initialization
   -- /wd4996 (C4996 warning) - disable deprecated declarations
   -- /wd4005 (C4005 warning) - disable macro redefinition
   -- /wd4100 (C4100 warning) - disable unreferenced formal parameter
   -- /Oi - generate intrinsic functions
   buildoptions( { "/wd4351", "/wd4996", "/wd4005", "/wd4100", "/Oi" } )

   -- common release configuration flags and symbols
   filter { "configurations:Release" }
      optimize "On"
      -- favor speed over size
      buildoptions { "/Ot" }
      defines { "WIN32", "_LIB", "NDEBUG" }

   -- common debug configuration flags and symbols
   filter { "configurations:Debug" }
      targetsuffix "_d"
      symbols "On"
      defines { "WIN32", "_LIB", "_DEBUG" }

   filter { "platforms:Win32" }
      system "Windows"
      architecture "x32"

   filter { "platforms:Win64" }
      system "Windows"
      architecture "x64"

   -- deps library
   project "deps"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../deps/include/loguru/**.h*",
         "../../deps/include/osg/**",
         "../../deps/src/loguru/**.c**",
         "../../deps/src/osg/**.c**"
      }
      excludes {
         "../../deps/src/osg/Matrix_implementation.cpp"
      }
      includedirs { DEPS_IncPath }
      targetname "deps"

   -- cigicl library
   project "ccl_lib"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../deps/include/cigicl/**.h*",
         "../../deps/src/cigicl/**.c**"
      }
      includedirs { DEPS_IncPath.."/cigicl" }
      defines { "CIGI_LITTLE_ENDIAN" }
      targetname "ccl_lib"

