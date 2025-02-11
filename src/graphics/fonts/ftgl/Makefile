#
include ../../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_graphics_ftgl.a

CPPFLAGS += -I$(FREETYPE2_INC_DIR)

OBJS =  \
	factory.o \
	IFtglFont.o \
	FtglBitmapFont.o \
	FtglExtrdFont.o \
	FtglHaloFont.o \
	FtglOutlineFont.o \
	FtglPixmapFont.o \
	FtglPolygonFont.o \
	FtglTextureFont.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)

