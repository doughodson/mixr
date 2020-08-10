#
include ../makedefs

LIB = $(DEPS_LIB_DIR)/libdeps.a

OBJS =  \
	Math.o \
	Matrixd.o \
	MatrixDecomposition.o \
	Matrixf.o \
	Quat.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)

