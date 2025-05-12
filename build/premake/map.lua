
--
-- Map libraries
--

   -- raster product format map library
   project "map_rpf"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/map/rpf/**.*",
         "../../src/map/rpf/**.*"
      }
      includedirs { MIXR_IncPath }
      targetname "map_rpf"

   -- vector product format map library
   project "map_vpf"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/map/vpf/**.*",
         "../../src/map/vpf/**.*"
      }
      includedirs { MIXR_IncPath }
      targetname "map_vpf"
