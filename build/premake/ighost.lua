
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

   -- Simple IG interface library
   project "ighost_pov"
      files {
         "../../include/mixr/ighost/pov/**.h*",
         "../../src/ighost/pov/**.h**",
         "../../src/ighost/pov/**.cpp"
      }
      targetname "ighost_pov"
