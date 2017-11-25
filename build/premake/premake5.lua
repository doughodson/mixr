
--
-- If premake command is not supplied an action (target compiler), exit!
--
-- Targets of interest:
--     vs2013     (Visual Studio 2013)
--     vs2015     (Visual Studio 2015)
--     vs2017     (Visual Studio 2017)
--
if (_ACTION == nil) then
    return
end

--
-- directory location for 3rd party dependencies
--
MIXR_3RD_PARTY_ROOT = "../../../mixr-3rdparty"

--
-- set include and library paths
--
MIXR_IncPath         = "../../include"
MIXR_3rdPartyIncPath = MIXR_3RD_PARTY_ROOT.."/include"

--
-- directory location for HLA include and library paths
--
HLA_ROOT = "../../../openrti"
HLAIncPath = HLA_ROOT.."/include/RTI13"
HLALibPath = HLA_ROOT.."/lib"
print ("HLA Paths:")
print ("  Include   : "..HLALibPath)
--print ("  Libraries : "..MIXR_LibPath)

workspace "mixr"

   -- destination directory for generated solution/project files
   location ("../" .. _ACTION)

   -- destination directory for compiled binary target
   targetdir ("../../lib/")

   -- creating static libraries
   kind "StaticLib"

   -- C++ code in all projects
   language "C++"

   -- common include directories (all configurations/all projects)
   includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }

   -- target suffix (all configurations/all projects)
   targetprefix "mixr_"

   --
   -- Build (solution) configuration options:
   --     Release        (Runtime library is Multi-threaded DLL)
   --     Debug          (Runtime library is Multi-threaded Debug DLL)
   --
   configurations { "Release", "Debug" }

   -- visual studio options and warnings
   -- /wd4351 (C4351 warning) - disable warning associated with array brace initialization
   -- /wd4996 (C4996 warning) - disable deprecated declarations
   -- /wd4005 (C4005 warning) - disable macro redefinition
   -- /wd4100 (C4100 warning) - disable unreferenced formal parameter
   -- /Oi - generate intrinsic functions
   buildoptions( { "/wd4351", "/wd4996", "/wd4005", "/wd4100", "/Oi" } )

   -- common release configuration flags and symbols
   filter { "Release" }
      optimize "On"
      -- favor speed over size
      buildoptions { "/Ot" }
      defines { "WIN32", "_LIB", "NDEBUG" }

   -- common debug configuration flags and symbols
   filter { "Debug" }
      targetsuffix "_d"
      symbols "On"
      -- enable compiler intrinsics
      defines { "WIN32", "_LIB", "_DEBUG" }

   --
   -- libraries
   --

   -- base library
   project "base"
      files {
         "../../include/mixr/base/**.h*",
         "../../include/mixr/base/**.inl",
         "../../include/mixr/base/**.epp",
         "../../include/mixr/base/osg/*",
         "../../src/base/**.h*",
         "../../src/base/**.cpp",
         "../../src/base/**.y",
         "../../src/base/**.l"
      }
      excludes {
         "../../src/base/osg/Matrix_implementation.cpp",
         "../../src/base/util/platform/system_linux.cpp",
         "../../src/base/util/platform/system_mingw.cpp",
         "../../src/base/concurrent/platform/*_linux.cpp"
      }
      targetname "base"

   -- OpenGL-based graphics library
   project "graphics"
      files {
         "../../include/mixr/graphics/**.h*",
         "../../src/graphics/**.h*",
         "../../src/graphics/**.cpp",
         "../../src/graphics/**.l"
      }
      includedirs { MIXR_3rdPartyIncPath.."/freetype2" }
      defines { "FTGL_LIBRARY_STATIC" }
      targetname "graphics"

   -- DAFIF airport loader library
   project "dafif"
      files {
         "../../include/mixr/dafif/**.h*",
         "../../src/dafif/**.cpp"
      }
      targetname "dafif"
	  
   -- graphical instruments library
   project "instruments"
      files {
         "../../include/mixr/instruments/**.h*",
         "../../include/mixr/instruments/**.epp",
         "../../src/instruments/**.cpp"
      }
      targetname "instruments"

   -- i/o linkage library
   project "iolinkage"
      files {
         "../../include/mixr/iolinkage/**.h*",
         "../../src/iolinkage/**.*",
         "../../src/iolinkage/**.h*"
      }
      excludes { "../../src/iolinkage/platform/UsbJoystick_linux.*"   }
      targetname "iolinkage"

   -- linear systems library
   project "linearsystem"
      files {
         "../../include/mixr/linearsystem/**.h*",
         "../../src/linearsystem/**.cpp"
      }
      targetname "linearsystem"

   -- models library
   project "models"
      files {
         "../../include/mixr/models/**.h*",
         "../../include/mixr/models/**.inl",
         "../../src/models/**.cpp"
      }
      includedirs { MIXR_3rdPartyIncPath.."/JSBSim" }
      targetname "models"

   -- data recorder library
   project "recorder"
      files {
         "../../include/mixr/recorder/**.h*",
         "../../include/mixr/recorder/*.inl",
         "../../include/mixr/recorder/**.proto",
         "../../src/recorder/**.cpp",
         "../../src/recorder/**.cc"
      }
      defines { "_SCL_SECURE_NO_WARNINGS" } -- suppress protocol buffer warning
      targetname "recorder"

   -- simulation library
   project "simulation"
      files {
         "../../include/mixr/simulation/**.h*",
         "../../include/mixr/simulation/**.inl",
         "../../src/simulation/**.cpp"
      }
      targetname "simulation"

   -- terrain library
   project "terrain"
      files {
         "../../include/mixr/terrain/**.h*",
         "../../src/terrain/**.cpp"
      }
      targetname "terrain"

   -- OpenGL GLUT interface library
   project "ui_glut"
      files {
         "../../include/mixr/ui/glut/**.h*",
         "../../src/ui/glut/**.cpp"
      }
      targetname "ui_glut"

   -- interoperability libraries
   dofile "interop.lua"

   -- image generator libraries
   dofile "ig.lua"
	
	-- map libraries
	dofile "map.lua"
	