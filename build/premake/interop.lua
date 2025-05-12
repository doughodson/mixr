
--
-- Interoperability libraries
--

   -- Common interoperability code
   project "interop"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/**",
         "../../src/interop/**"
      }
      removefiles {
         "../../include/mixr/interop/dis/**",
         "../../src/interop/dis/**",
         "../../include/mixr/interop/hla/**",
         "../../src/interop/hla/**"
      }
      includedirs { MIXR_IncPath }
      targetname "interop"

   -- IEEE DIS interface library
   project "interop_dis"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/dis/**",
         "../../src/interop/dis/**"
      }
      includedirs { MIXR_IncPath }
      targetname "interop_dis"

   -- IEEE HLA interface library
   project "interop_hla"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/hla/**",
         "../../src/interop/hla/**"
      }
      removefiles {
         "../../include/mixr/interop/hla/rprfom/**",
         "../../src/interop/hla/rprfom/**"
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "interop_hla"

   -- IEEE HLA interface library for RPR FOM
   project "interop_hla_rprfom"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/hla/rprfom/**",
         "../../src/interop/hla/rprfom/**"
      }
      includedirs { HLAIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "interop_hla_rprfom"

