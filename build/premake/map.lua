
--
-- Map libraries
--

   -- raster product format map library
   project "map_rpf"
      files {
         "../../include/mixr/map/rpf/**.h*",
         "../../src/map/rpf/**.cpp"
      }
      targetname "map_rpf"

   -- vector product format map library
   project "map_vpf"
      files {
         "../../include/mixr/map/vpf/**.h*",
         "../../src/map/vpf/**.cpp"
      }
      targetname "map_vpf"
