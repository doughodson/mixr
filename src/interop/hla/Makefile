#
include ../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_interop_hla.a

CPPFLAGS += -I$(HLA_INC_DIR)
CPPFLAGS += -DRTI_USES_STD_FSTREAM
CPPFLAGS += -Wno-deprecated

OBJS =  \
	Ambassador.o \
	NetIO.o \
	Nib.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
