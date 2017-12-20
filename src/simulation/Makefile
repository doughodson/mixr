#
include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_simulation.a

OBJS =  \
	AbstractDataRecorder.o \
	AbstractIgHost.o \
	AbstractNetIO.o \
	AbstractNib.o \
	AbstractPlayer.o \
	AbstractRecorderComponent.o \
	Simulation.o \
	SimulationBgSyncThread.o \
	SimulationTcSyncThread.o \
	Station.o \
	StationBgPeriodicThread.o \
	StationNetPeriodicThread.o \
	StationTcPeriodicThread.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f environment/*.o
	-rm -f *.o
	-rm -f $(LIB)

