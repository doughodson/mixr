
include makedefs

# Platform library  : Dependency libraries
# ------------------------------------------------------------------------
# base              : -
# dafif             : -
# graphics          : OpenGL, FTGL, freetype
# gui_glut          : freeglut
# instruments       : -
# interop           : -
# interop_dis       : -
# interop_hla       : HLA RTI
# interop_rprfom    : HLA RTI
# linkage           : -
# map_rpf           : -
# models            : JSBSim
# ighost_cigi       : CIGICL 3.x
# ighost_flightgear : -
# ighost_pov        : -
# recorder          : Google protocol buffers
# simulation        : -
# terrain           : -
#
PROJECTS = base
PROJECTS += dafif
PROJECTS += linkage
PROJECTS += simulation terrain
#
# Models
#
PROJECTS += models/common
PROJECTS += models/jsbsim

#
# Data recorder
#
PROJECTS += recorder

#
# OpenGL-based graphics libraries
#
PROJECTS += graphics
PROJECTS += instruments

#
# User interface libraries
#
PROJECTS += ui/glut

#
# Interoperability interfaces
#
PROJECTS += interop/common
PROJECTS += interop/dis
PROJECTS += interop/hla
PROJECTS += interop/rprfom

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

