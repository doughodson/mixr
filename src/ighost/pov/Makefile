#
include ../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_ighost_pov.a

OBJS =  \
	PovHost.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
