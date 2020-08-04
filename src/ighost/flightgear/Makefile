#
include ../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_ighost_flightgear.a

OBJS =  \
	FlightGearHost.o \
	swap_endian.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
