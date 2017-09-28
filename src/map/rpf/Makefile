
include ../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_map_rpf.a

OBJS = \
	CadrgClut.o \
	CadrgFile.o \
	CadrgFrame.o \
	CadrgFrameEntry.o \
	CadrgMap.o \
	CadrgTocEntry.o \
	MapDrawer.o \
	TexturePager.o \
	TextureTable.o \
	map_utils.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
