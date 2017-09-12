
--
-- Image Generator (IG) libraries
--

   -- common otw code
   project "ig"
      files {
         "../../include/mixr/ig/common/**.h*",
         "../../src/ig/common/**.cpp"
      }
      targetname "ig"

   -- CIGI interface library
   project "ig_cigi"
      files {
         "../../include/mixr/ig/cigi/**.h*",
         "../../src/ig/cigi/**.cpp"
      }
      targetname "ig_cigi"

   -- Simple IG interface library
   project "ig_viewpoint"
      files {
         "../../include/mixr/ig/viewpoint/**.h*",
         "../../src/ig/viewpoint/**.h**",
         "../../src/ig/viewpoint/**.cpp"
      }
      targetname "ig_viewpoint"
