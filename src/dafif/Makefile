#
include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_dafif.a

OBJS =  \
	loaders/AirportLoader.o \
	loaders/Database.o \
	loaders/NavaidLoader.o \
	loaders/WaypointLoader.o \
	records/Airport.o \
	records/Record.o \
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
	-rm -f $(LIB)
