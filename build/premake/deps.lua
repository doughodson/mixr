
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

   project "cigicl"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../deps/include/cigicl/**.h*",
         "../../deps/src/cigicl/**.c**"
      }
      includedirs { DEPS_IncPath.."/cigicl" }
      defines { "CIGI_LITTLE_ENDIAN" }
      targetname "cigicl"

   project "freetype2"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         -- FT_MODULES
         "../../deps/src/freetype2/base/ftbbox.c",
         "../../deps/src/freetype2/base/ftgxval.c",
         "../../deps/src/freetype2/base/ftlcdfil.c",
         "../../deps/src/freetype2/base/ftmm.c",
         "../../deps/src/freetype2/base/ftotval.c",
         "../../deps/src/freetype2/base/ftpatent.c",
         "../../deps/src/freetype2/base/ftpfr.c",
         "../../deps/src/freetype2/base/ftsynth.c",
         "../../deps/src/freetype2/base/fttype1.c",
         "../../deps/src/freetype2/base/ftwinfnt.c",
         "../../deps/src/freetype2/base/ftxf86.c",
         "../../deps/src/freetype2/pcf/pcf.c",
         "../../deps/src/freetype2/pfr/pfr.c",
         "../../deps/src/freetype2/psaux/psaux.c",
         "../../deps/src/freetype2/pshinter/pshinter.c",
         "../../deps/src/freetype2/psnames/psmodule.c",
         "../../deps/src/freetype2/raster/raster.c",
         "../../deps/src/freetype2/sfnt/sfnt.c",
         "../../deps/src/freetype2/truetype/truetype.c",
         "../../deps/src/freetype2/type1/type1.c",
         "../../deps/src/freetype2/cid/type1cid.c",
         "../../deps/src/freetype2/type42/type42.c",
         "../../deps/src/freetype2/winfonts/winfnt.c",
         -- other
         "../../deps/include/freetype2/f2build.h",
         "../../deps/include/freetype2/freetype/config/*.h",
         "../../deps/src/freetype2/autofit/autofit.c",
         "../../deps/src/freetype2/bdf/bdf.c",
         "../../deps/src/freetype2/cff/cff.c",
         "../../deps/src/freetype2/base/ftbase.c",
         "../../deps/src/freetype2/base/ftbitmap.c",
         "../../deps/src/freetype2/cache/ftcache.c",
         "../../deps/src/freetype2/builds/win32/ftdebug.c",
         "../../deps/src/freetype2/base/ftfstype.c",
         "../../deps/src/freetype2/base/ftgasp.c",
         "../../deps/src/freetype2/base/ftglyph.c",
         "../../deps/src/freetype2/gzip/ftgzip.c",
         "../../deps/src/freetype2/base/ftinit.c",
         "../../deps/src/freetype2/lzw/ftlzw.c",
         "../../deps/src/freetype2/base/ftstroke.c",
         "../../deps/src/freetype2/base/ftsystem.c",
         "../../deps/src/freetype2/smooth/smooth.c"
      }
      includedirs { DEPS_IncPath .. "/freetype2" }
      configuration "Release"
         defines { "WIN32", "_LIB", "NDEBUG" }
         defines { "_CRT_SECURE_NO_WARNINGS" }
         defines { "FT2_BUILD_LIBRARY" }
      configuration "Debug"
         defines { "WIN32", "_LIB", "_DEBUG" }
         defines { "_CRT_SECURE_NO_WARNINGS" }
         defines { "FT_DEBUG_LEVEL_ERROR", "FT_DEBUG_LEVEL_TRACE" }
         defines { "FT2_BUILD_LIBRARY", "_CRT_SECURE_NO_DEPRECATE" }
      targetname "freetype2"
