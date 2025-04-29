
--
-- Map libraries
--

   -- raster product format map library
   project "map_rpf"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/map/rpf/*.h*",
         "../../src/map/rpf/*.c*"
      }
      includedirs { MIXR_IncPath }
      targetname "map_rpf"

   -- vector product format map library
   project "map_vpf"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/map/vpf/*.h*",
         "../../src/map/vpf/*.c*"
      }
      includedirs { MIXR_IncPath }
      targetname "map_vpf"
