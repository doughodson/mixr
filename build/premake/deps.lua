
--
-- Dependency library
--

   -- dependency library
   project "deps"
      files {
         "../../deps/include/**.h*",
         "../../deps/src/**.c**"
      }
      includedirs { DEPS_IncPath,
                    DEPS_IncPath.."/cigicl",
                    DEPS_IncPath.."/loguru" }
      defines { "CIGI_LITTLE_ENDIAN" }
      targetname "deps"
