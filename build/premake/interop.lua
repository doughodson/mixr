
--
-- Interoperability libraries
--

   -- Common interoperability code
   project "interop"
      files {
         "../../include/mxrp/interop/common/**.h*",
         "../../src/interop/common/**.cpp"
      }
      targetname "interop"

   -- IEEE DIS interface library
   project "interop_dis"
      files {
         "../../include/mxrp/interop/dis/**.h*",
         "../../src/interop/dis/**.cpp"
      }
      targetname "interop_dis"

--[[
   -- IEEE HLA interface library (common code)
   project "interop_hla"
      files {
         "../../include/mxrp/interop/hla/**.h*",
         "../../src/interop/hla/**.cpp"
      }
      includedirs { HLAIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "interop_hla"

   -- IEEE HLA interface library for RPR FOM
   project "interop_rprfom"
      files {
         "../../include/mxrp/interop/rprfom/**.h*",
         "../../src/interop/rprfom/**.cpp"
      }
      includedirs { HLAIncPath }
      defines { "RTI_USES_STD_FSTREAM" }
      targetname "interop_rprfom"
--]]
