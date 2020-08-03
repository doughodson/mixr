
--
-- Image Generator (IG) Host libraries
--

   -- CIGI interface library
   project "ighost_cigi"
      files {
         "../../include/mixr/ighost/cigi/**.h*",
         "../../src/ighost/cigi/**.h**",
         "../../src/ighost/cigi/**.cpp"
      }
      targetname "ighost_cigi"

   -- FlightGear IG interface library
   project "ighost_flightgear"
      files {
         "../../include/mixr/ighost/flightgear/**.h*",
         "../../src/ighost/flightgear/**.h**",
         "../../src/ighost/flightgear/**.cpp"
      }
      targetname "ighost_flightgear"
