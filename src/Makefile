
include makedefs

# Platform library  : Dependency libraries
# ------------------------------------------------------------------------
# base              : deps
# dafif             : deps
# graphics          : deps, OpenGL, FTGL, freetype
# gui_glut          : deps, freeglut
# instruments       : deps
# interop           : deps
# interop_dis       : deps
# interop_hla       : deps, HLA RTI
# interop_rprfom    : deps, HLA RTI
# linkage           : deps
# linearsystem      : deps
# map_rpf           : deps
# models            : deps, JSBSim
# ighost_cigi       : deps, CIGICL 3.x
# ighost_flightgear : deps
# recorder          : deps, Google protocol buffers
# simulation        : -
# terrain           : -
#
PROJECTS = base
PROJECTS += dafif
PROJECTS += linkage
PROJECTS += linearsystem 
PROJECTS += simulation terrain
PROJECTS += models
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

