#
include ../makedefs

LIB = $(DEPS_LIB_DIR)/libdeps.a

OBJS =  \
	loguru.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)
