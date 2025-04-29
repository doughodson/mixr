
--
-- Image Generator (IG) Host libraries
--

   -- CIGI interface library
   project "ighost_cigi"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/ighost/cigi/**.h*",
         "../../src/ighost/cigi/**.c*"
      }
      includedirs { MIXR_IncPath, MIXR_3rdPartyIncPath }
      targetname "ighost_cigi"

   -- FlightGear IG interface library
   project "ighost_flightgear"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/ighost/flightgear/*.h*",
         "../../src/ighost/flightgear/*.c*"
      }
      includedirs { MIXR_IncPath }
      targetname "ighost_flightgear"

   -- Pov IG interface library
   project "ighost_pov"
      location ("../" .. _ACTION .. "/projects/%{prj.name}")
      files {
         "../../include/mixr/ighost/pov/*.h*",
         "../../src/ighost/pov/*.c*"
      }
      includedirs { MIXR_IncPath }
      targetname "ighost_pov"
