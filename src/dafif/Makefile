#
include ../makedefs

LIB = $(MXRP_LIB_DIR)/libmxrp_dafif.a

OBJS =  \
	Airport.o \
	AirportLoader.o \
	Record.o \
	Database.o \
	factory.o \
	Ils.o \
	Navaid.o \
	NavaidLoader.o \
	Runway.o \
	Waypoint.o \
	WaypointLoader.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
