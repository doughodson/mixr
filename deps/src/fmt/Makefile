#
include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_deps.a

OBJS =  \
	format.o \
	os.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
