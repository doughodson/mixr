#
include ../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_ig.a

OBJS =  \
	Otm.o \
	Otw.o \
	OtwModel.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
