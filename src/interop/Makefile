#
include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_interop.a

OBJS =  \
	INetIO.o \
	INib.o \
	Ntm.o \
	NtmInputNode.o \
	NtmOutputNode.o \
	NtmOutputNodeStd.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
