
--
-- Interoperability libraries
--

   -- Common interoperability code
   project "interop"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/*.h*",
         "../../src/interop/*.c*"
      }
      includedirs { MIXR_IncPath }
      targetname "interop"

   -- IEEE DIS interface library
   project "interop_dis"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/dis/*.h*",
         "../../src/interop/dis/*.c*"
      }
      includedirs { MIXR_IncPath }
      targetname "interop_dis"

   -- IEEE HLA interface library
   project "interop_hla"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/hla/*.h*",
         "../../src/interop/hla/*.c*"
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "interop_hla"

   -- IEEE HLA interface library for RPR FOM
   project "interop_hla_rprfom"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/hla/rprfom/*.h*",
         "../../src/interop/hla/rprfom/*.c*"
      }
      includedirs { HLAIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "interop_hla_rprfom"

