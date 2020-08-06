
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
-- directory location for 3rd party dependencies
--
MIXR_3RD_PARTY_ROOT = "../../../mixr-3rdparty"

--
-- set include and library paths
--
MIXR_IncPath         = "../../include"
MIXR_3rdPartyIncPath = MIXR_3RD_PARTY_ROOT.."/include"
DEPS_IncPath         = "../../deps/include"

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
   cppdialect "C++14"

   -- target suffix (all configurations/all projects)
   targetprefix "mixr_"

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

   -- core libraries
   dofile "core-libs.lua"

   -- interoperability libraries
   dofile "interop.lua"

   -- image generator host libraries
   dofile "ighost.lua"

   -- map libraries
   dofile "map.lua"
