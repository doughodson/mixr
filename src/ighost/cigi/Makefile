#
include ../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_ighost_cigi.a

OBJS =  \
	session/HostSession.o \
	session/SignalProcessor.o \
	CigiHost.o \
	CigiModel.o \
	IgHost.o \
	IgThread.o \
	Player2CigiMap.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f session/*.o
	-rm -f *.o
	-rm -f $(LIB)
