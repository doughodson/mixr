
include makedefs

# Platform library  : Dependency libraries
# ------------------------------------------------------------------------
# base                 : -
# dafif                : -
# graphics             : OpenGL
# graphics_ftgl        : FTGL, freetype
# gui_glut             : freeglut
# instruments          : -
# interop              : -
# interop_dis          : -
# interop_hla          : HLA RTI
# interop_rprfom       : HLA RTI
# linkage              : -
# map                  : -
# models               : -
# models_jsbsim        : JSBSim
# ighost_cigi          : CIGICL 3.x
# ighost_flightgear    : -
# ighost_pov           : -
# recorder_protobuf_v2 : Google protocol buffers, version 2
# simulation           : -
# terrain              : -
#
PROJECTS = base
PROJECTS += dafif
PROJECTS += linkage
PROJECTS += simulation
PROJECTS += terrain
#
# Models
#
PROJECTS += models
PROJECTS += models/dynamics/jsbsim

#
# Data recorder
#
PROJECTS += recorder/protobuf_v2

#
# OpenGL-based graphics libraries
#
PROJECTS += graphics
PROJECTS += graphics/fonts/ftgl
PROJECTS += instruments

#
# User interface libraries
#
PROJECTS += ui/glut

#
# Interoperability interfaces
#
PROJECTS += interop
PROJECTS += interop/dis
PROJECTS += interop/hla
PROJECTS += interop/hla/rprfom

#
# Image generator host interfaces
#
PROJECTS += ighost/cigi
PROJECTS += ighost/flightgear
PROJECTS += ighost/pov

#
# Map format reader libraries
#
PROJECTS += map/rpf
PROJECTS += map/vpf

.PHONY: all clean $(PROJECTS)

#
# Rules
#
all: $(PROJECTS)

$(PROJECTS):
	$(MAKE) -C $@

clean:
	-for d in $(PROJECTS); do (cd $$d; $(MAKE) clean ); done

