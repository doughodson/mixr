
#----------------------------------------------------------------------------------------
# To build scanner:
#
# cd readouts
# dos2unix reformat_scanner.l
# flex -f -oReformatScanner.cpp reformat_scanner.l   // creates ReformatScanner.cpp
#   (ReformatScanner.hpp already exists)
#
# search for "FlexLexer.h" in ReformatScanner.cpp - replace:
#   #include <FlexLexer.h>
# with
#   #include "mixr/base/util/FlexLexer.h"
#----------------------------------------------------------------------------------------

include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_graphics.a

OBJS =  \
	fonts/IFont.o \
	fonts/BitmapFont.o \
	fonts/StrokeFont.o \
	readouts/IReadout.o \
	readouts/AsciiText.o \
	readouts/BooleanText.o \
	readouts/Cursor.o \
	readouts/DirectionReadout.o \
	readouts/HexReadout.o \
	readouts/LatitudeReadout.o \
	readouts/LongitudeReadout.o \
	readouts/NumericReadout.o \
	readouts/OctalReadout.o \
	readouts/readout_utils.o \
	readouts/ReformatScanner.o \
	readouts/Rotary.o \
	readouts/Rotary2.o \
	readouts/TimeReadout.o \
	BmpTexture.o \
	Clip3D.o \
	ColorGradient.o \
	ColorRotary.o \
	Display.o \
	factory.o \
	Graphic.o \
	Image.o \
	Material.o \
	Page.o \
	Polygon.o \
	Rotators.o \
	Scanline.o \
	Shapes.o \
	SymbolLoader.o \
	Texture.o \
	Translator.o \
	MfdPage.o \
	MapPage.o

CPPFLAGS += -I$(FREETYPE2_INC_DIR)

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f fonts/*.o
	-rm -f readouts/*.o
	-rm -f *.o
	-rm -f $(LIB)
