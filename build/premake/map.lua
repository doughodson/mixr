
--
-- Map libraries
--

   -- raster product format map library
   project "map_rpf"
      files {
         "../../include/mixr/map/rpf/**.h*",
         "../../src/map/rpf/**.cpp"
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      targetname "map_rpf"

   -- vector product format map library
   project "map_vpf"
      files {
         "../../include/mixr/map/vpf/**.h*",
         "../../src/map/vpf/**.cpp"
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      targetname "map_vpf"
