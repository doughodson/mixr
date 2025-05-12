
--
-- core libraries
--

   -- base library
   project "base"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/base/**.*",
         "../../src/base/**.*"
      }
      removefiles {
         "../../src/base/osg/Matrix_implementation.cpp",
         "../../src/base/util/platform/system_linux.cpp",
         "../../src/base/util/platform/system_mingw.cpp",
         "../../src/base/threads/platform/*_linux.cpp"
      }
      includedirs { MIXR_IncPath }
      targetname "base"

   -- OpenGL-based graphics library
   project "graphics"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/graphics/**.*",
         "../../src/graphics/**.*"
      }
      removefiles {
         "../../include/mixr/graphics/fonts/ftgl/**.*",
         "../../src/graphics/fonts/ftgl/**.*"
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      targetname "graphics"

   -- OpenGL-based FTGL Fonts
   project "graphics_ftgl"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/graphics/fonts/ftgl/**.*",
         "../../src/graphics/fonts/ftgl/**.*",
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      includedirs { MIXR_3rdPartyIncPath.."/freetype2" }
      defines { "FTGL_LIBRARY_STATIC" }
      targetname "graphics_ftgl"

   -- DAFIF airport loader library
   project "dafif"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/dafif/**.*",
         "../../src/dafif/**.*"
      }
      includedirs { MIXR_IncPath }
      targetname "dafif"

   -- graphical instruments library
   project "instruments"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/instruments/**.*",
         "../../src/instruments/**.*"
      }
      includedirs { MIXR_IncPath }
      targetname "instruments"

   -- linkage library
   project "linkage"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/linkage/**.*",
         "../../src/linkage/**.*"
      }
      removefiles { "../../src/linkage/platform/UsbJoystick_linux.*" }
      includedirs { MIXR_IncPath }
      targetname "linkage"

   -- models library
   project "models"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/models/**.*",
         "../../src/models/**.*"
      }
      removefiles {
                 "../../include/mixr/models/dynamics/jsbsim/**.*",
                 "../../src/models/dynamics/jsbsim/**.*"
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      targetname "models"

   -- models_jsbsim library
   project "models_jsbsim"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/models/dynamics/jsbsim/**.*",
         "../../src/models/dynamics/jsbsim/**.*"
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      includedirs { MIXR_3rdPartyIncPath.."/JSBSim" }
      targetname "models_jsbsim"

   -- data recorder library
   project "recorder_protobuf_v2"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/recorder/protobuf_v2/**.*",
         "../../src/recorder/protobuf_v2/**.*"
      }
      defines { "_SCL_SECURE_NO_WARNINGS" } -- suppress protocol buffer warning
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      targetname "recorder_protobuf_v2"

   -- simulation library
   project "simulation"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/simulation/**.*",
         "../../src/simulation/**.*"
      }
      includedirs { MIXR_IncPath }
      targetname "simulation"

   -- terrain library
   project "terrain"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/terrain/**.*",
         "../../src/terrain/**.*"
      }
      includedirs { MIXR_IncPath }
      targetname "terrain"

   -- OpenGL GLUT interface library
   project "ui_glut"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/ui/glut/**.*",
         "../../src/ui/glut/**.*"
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      targetname "ui_glut"
