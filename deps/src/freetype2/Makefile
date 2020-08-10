#
include ../makedefs

LIB = $(DEPS_LIB_DIR)/libfreetype2.a

CPPFLAGS += -I$(MIXR_ROOT)/deps/include/freetype2
CPPFLAGS += -I$(MIXR_ROOT)/deps/include/freetype2/freetype
CPPFLAGS += -I$(MIXR_ROOT)/deps/include/freetype2/freetype/config

CPPFLAGS  += -DFT_CONFIG_OPTION_SYS
CPPFLAGS  += -DFT_CONFIG_OPTION_SYSTEM_ZLIB
CPPFLAGS  += -DFT_CONFIG_OPTION_USE_PNG
CPPFLAGS  += -DFT_CONFIG_CONFIG_H="<ftconfig.h>"
CPPFLAGS  += -DFT2_BUILD_LIBRARY
CPPFLAGS  += -DFT_CONFIG_MODULES_H="<ftmodule.h>"

OBJS =  \
	builds/unix/ftsystem.o\
	base/ftdebug.o\
	base/ftinit.o\
	base/ftbase.o\
	base/ftbbox.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f base/*.o
	-rm -f builds/unix/*.o
	-rm -f $(LIB)

