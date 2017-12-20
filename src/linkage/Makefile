
include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_linkage.a

OBJS =  \
	adapters/AbstractAdapter.o \
	adapters/Ai2DiSwitch.o \
	adapters/AnalogInput.o \
	adapters/AnalogOutput.o \
	adapters/DiscreteInput.o \
	adapters/DiscreteOutput.o \
	generators/AbstractGenerator.o \
	generators/AnalogInputFixed.o \
	generators/AnalogSignalGen.o \
	generators/DiscreteInputFixed.o \
	platform/UsbJoystick_linux.o \
	factory.o \
	IoData.o \
	IoDevice.o \
	IoHandler.o \
	IoPeriodicThread.o \
	MockDevice.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f adapters/*.o
	-rm -f platform/*.o
	-rm -f *.o
	-rm -f $(LIB)
