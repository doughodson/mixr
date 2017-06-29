#
include ../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_interop.a

OBJS =  \
	NetIO.o \
	Nib.o \
	Ntm.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
