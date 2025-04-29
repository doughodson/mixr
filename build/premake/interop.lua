
--
-- Interoperability libraries
--

   -- Common interoperability code
   project "interop"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/**.h*",
         "../../src/interop/**.hpp",
         "../../src/interop/**.cpp"
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      targetname "interop"

   -- IEEE DIS interface library
   project "interop_dis"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/dis/**.h*",
         "../../src/interop/dis/**.hpp",
         "../../src/interop/dis/**.cpp"
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      targetname "interop_dis"

--[[
   -- IEEE HLA interface library (common code)
   project "interop_hla"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/hla/**.h*",
         "../../src/interop/hla/**.cpp"
      }
      includedirs { HLAIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "interop_hla"

   -- IEEE HLA interface library for RPR FOM
   project "interop_rprfom"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/interop/rprfom/**.h*",
         "../../src/interop/rprfom/**.cpp"
      }
      includedirs { HLAIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "interop_rprfom"
--]]
