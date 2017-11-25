
include makedefs

# Library         : Requires
# ------------------------------------------------------------------------
# base            : -
# dafif           : -
# graphics        : OpenGL, FTGL, freetype
# gui_glut        : freeglut
# instruments     : -
# interop         : -
# interop_dis     : -
# interop_hla     : HLA RTI
# interop_rprfom  : HLA RTI
# iolinkage       : -
# linearsystem    : -
# map_rpf         : -
# models          : JSBSim
# ig              : -
# ig_cigi         : CIGICL 3.x
# ig_pov          : -
# recorder        : Google protocol buffers
# simulation      : -
# terrain         : -
#
PROJECTS = base
PROJECTS += dafif
PROJECTS += iolinkage
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
PROJECTS += ig/common
PROJECTS += ig/cigi
PROJECTS += ig/viewpoint

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

install: uninstall
	-cp -r ../include/mixr /usr/local/include/mixr
	-cp -r ../lib /usr/local/lib/mixr

uninstall:
	-rm -rf /usr/local/include/mixr
	-rm -rf /usr/local/lib/mixr

clean:
	-for d in $(PROJECTS); do (cd $$d; $(MAKE) clean ); done

