#
include ../../../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_models_jsbsim.a

CPPFLAGS += -I$(JSBSIM_INC_DIR)

OBJS =  \
	factory.o \
	JSBSimDynamics.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f *.o
	-rm -f $(LIB)

