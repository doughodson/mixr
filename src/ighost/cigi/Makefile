#
include ../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_ighost_cigi.a

OBJS =  \
	session/HostSession.o \
	session/SignalProcessor.o \
	Cigi.o \
	IgHost.o \
	IgThread.o \
	Otm.o \
	OtwModel.o \
	OtwModelCigiCl.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f session/*.o
	-rm -f *.o
	-rm -f $(LIB)
