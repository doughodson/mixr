
include ../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_map_vpf.a

OBJS =  \
	VMap0FeatureClass.o \
	VMap0LibDirectory.o \
	VMap0MainDirectory.o \
	VMap0RefCoverageDirectory.o \
	VMap0ReferenceDirectory.o \
	VpfDataType.o \
	VpfDirectory.o \
	VpfIndexTable.o \
	VpfRecord.o \
	VpfSpatialIndexTable.o \
	VpfTable.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
