
--
-- Interoperability libraries
--

   -- Common interoperability code
   project "interop"
      files {
         "../../include/mixr/interop/common/**.h*",
         "../../src/interop/common/**.cpp"
      }
      targetname "interop"

   -- IEEE DIS interface library
   project "interop_dis"
      files {
         "../../include/mixr/interop/dis/**.h*",
         "../../src/interop/dis/**.cpp"
      }
      targetname "interop_dis"

--[[
   -- IEEE HLA interface library (common code)
   project "interop_hla"
      files {
         "../../include/mixr/interop/hla/**.h*",
         "../../src/interop/hla/**.cpp"
      }
      includedirs { HLAIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "interop_hla"

   -- IEEE HLA interface library for RPR FOM
   project "interop_rprfom"
      files {
         "../../include/mixr/interop/rprfom/**.h*",
         "../../src/interop/rprfom/**.cpp"
      }
      includedirs { HLAIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "interop_rprfom"
--]]
