#
include ../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_gui_glut.a

OBJS =  \
	factory.o \
	GlutDisplay.o \
	Shapes3D.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
