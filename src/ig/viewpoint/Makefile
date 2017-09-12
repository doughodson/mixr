#
include ../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_ig_viewpoint.a

OBJS =  \
	OtwPC.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
