#
include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_dafif.a

OBJS =  \
	loaders/AirportLoader.o \
	loaders/IDatabase.o \
	loaders/NavaidLoader.o \
	loaders/WaypointLoader.o \
	records/Airport.o \
	records/IRecord.o \
	records/Ils.o \
	records/Navaid.o \
	records/Runway.o \
	records/Waypoint.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f loaders/*.o
	-rm -f records/*.o
	-rm -f $(LIB)
