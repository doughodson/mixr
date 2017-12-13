
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
   project "ighost_viewpoint"
      files {
         "../../include/mixr/ighost/viewpoint/**.h*",
         "../../src/ighost/viewpoint/**.h**",
         "../../src/ighost/viewpoint/**.cpp"
      }
      targetname "ighost_viewpoint"
