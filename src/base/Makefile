
#----------------------------------------------------------------------------------------
# To build parser and scanner:
#
# cd edl
# dos2unix edl_parser.y
# dos2unix edl_scanner.l
# bison -d edl_parser.y -o EdlParser.cpp  // creates EdlParser.cpp and EdlParser.hpp
# flex -f -oEdlScanner.cpp edl_scanner.l  // creates EdlScanner.cpp (EdlScanner.hpp already exists)
#
# search for "FlexLexer.h" in EdlScanner.cpp - replace:
#   #include <FlexLexer.h>
# with
#   #include "mixr/base/util/FlexLexer.h"
#----------------------------------------------------------------------------------------

include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_base.a

OBJS =  \
	colors/Cie.o \
	colors/Cmy.o \
	colors/IColor.o \
	colors/Hls.o \
	colors/Hsva.o \
	colors/Hsv.o \
	colors/Rgb.o \
	colors/Rgba.o \
	colors/Yiq.o \
	concepts/linkage/IIoData.o \
	concepts/linkage/IIoDevice.o \
	concepts/linkage/IIoHandler.o \
	edl/EdlParser.o \
	edl/EdlScanner.o \
    linearsystem/DiffEquation.o \
    linearsystem/FirstOrderTf.o \
    linearsystem/LagFilter.o \
    linearsystem/Limit.o \
    linearsystem/Limit01.o \
    linearsystem/Limit11.o \
    linearsystem/LimitFunc.o \
    linearsystem/LowpassFilter.o \
    linearsystem/SaH.o \
    linearsystem/ScalerFunc.o \
    linearsystem/SecondOrderTf.o \
    linearsystem/Sz1.o \
    linearsystem/Sz2.o \
	network/NetHandler.o \
	network/PosixHandler.o \
	network/TcpClient.o \
	network/TcpHandler.o \
	network/TcpServerMultiple.o \
	network/TcpServerSingle.o \
	network/UdpBroadcastHandler.o \
	network/UdpMulticastHandler.o \
	network/UdpUnicastHandler.o \
	numeric/operators/Add.o \
	numeric/operators/Divide.o \
	numeric/operators/Multiply.o \
	numeric/operators/Operator.o \
	numeric/operators/Subtract.o \
	numeric/Boolean.o \
	numeric/Complex.o \
	numeric/Decibel.o \
	numeric/Float.o \
	numeric/Integer.o \
	numeric/Number.o \
	osg/math.o \
	osg/Matrixd.o \
	osg/MatrixDecomposition.o \
	osg/Matrixf.o \
	osg/Quat.o \
	relations/FStorage.o \
	relations/Func1.o \
	relations/Func2.o \
	relations/Func3.o \
	relations/Func4.o \
	relations/Func5.o \
	relations/Function.o \
	relations/Polynomial.o \
	relations/Table.o \
	relations/Table1.o \
	relations/Table2.o \
	relations/Table3.o \
	relations/Table4.o \
	relations/Table5.o \
	relations/TableStorage.o \
	threads/platform/IThread_linux.o \
	threads/platform/PeriodicThread_linux.o \
	threads/platform/SyncThread_linux.o \
	threads/IThread.o \
	threads/OneShotThread.o \
	threads/PeriodicThread.o \
	threads/SyncThread.o \
	transformations/Transform.o \
	transformations/Translation.o \
	transformations/Rotation.o \
	transformations/Scale.o \
	ubf/IAction.o \
	ubf/IBehavior.o \
	ubf/IState.o \
	ubf/Agent.o \
	ubf/Arbiter.o \
	units/angle/IAngle.o \
	units/angle/Degrees.o \
	units/angle/Radians.o \
	units/angle/Semicircles.o \
	units/area/IArea.o \
	units/area/DecibelSquareMeters.o \
	units/area/SquareCentiMeters.o \
	units/area/SquareFeet.o \
	units/area/SquareInches.o \
	units/area/SquareKiloMeters.o \
	units/area/SquareMeters.o \
	units/area/SquareMiles.o \
	units/area/SquareMilliMeters.o \
	units/area/SquareYards.o \
	units/energy/BTUs.o \
	units/energy/Calories.o \
	units/energy/IEnergy.o \
	units/energy/FootPounds.o \
	units/energy/Joules.o \
	units/energy/KiloWattHours.o \
	units/force/IForce.o \
	units/force/KiloNewtons.o \
	units/force/Newtons.o \
	units/force/Poundals.o \
	units/force/PoundForces.o \
	units/frequency/IFrequency.o \
	units/frequency/GigaHertz.o \
	units/frequency/Hertz.o \
	units/frequency/KiloHertz.o \
	units/frequency/MegaHertz.o \
	units/frequency/TeraHertz.o \
	units/length/CentiMeters.o \
	units/length/Feet.o \
	units/length/Inches.o \
	units/length/KiloMeters.o \
	units/length/ILength.o \
	units/length/Meters.o \
	units/length/MicroMeters.o \
	units/length/Microns.o \
	units/length/NauticalMiles.o \
	units/length/StatuteMiles.o \
	units/mass/Grams.o \
	units/mass/KiloGrams.o \
	units/mass/IMass.o \
	units/mass/Slugs.o \
	units/power/DecibelMilliWatts.o \
	units/power/DecibelWatts.o \
	units/power/Horsepower.o \
	units/power/KiloWatts.o \
	units/power/MilliWatts.o \
	units/power/IPower.o \
	units/power/Watts.o \
	units/time/Days.o \
	units/time/Hours.o \
	units/time/MicroSeconds.o \
	units/time/MilliSeconds.o \
	units/time/Minutes.o \
	units/time/NanoSeconds.o \
	units/time/Seconds.o \
	units/time/ITime.o \
	units/volume/CubicFeet.o \
	units/volume/CubicInches.o \
	units/volume/CubicMeters.o \
	units/volume/Liters.o \
	units/volume/IVolume.o \
	units/IQuantity.o \
	util/platform/system_linux.o \
	util/endian_utils.o \
	util/filesystem_utils.o \
	util/lfi.o \
	util/math_utils.o \
	util/nav_utils.o \
	util/navDR_utils.o \
	util/osg_utils.o \
	util/str_utils.o \
	util/string_utils.o \
	util/system_utils.o \
	Component.o \
	EarthModel.o \
	factory.o \
	FileReader.o \
	Identifier.o \
	Latitude.o \
	List.o \
	Locus.o \
	Longitude.o \
	Matrix.o \
	MetaObject.o \
	MonitorMetrics.o \
	Object.o \
	Pair.o \
	PairStream.o \
	SlotTable.o \
	Stack.o \
	Statistic.o \
	StateMachine.o \
	String.o \
	Timers.o \
	Vectors.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f colors/*.o
	-rm -f concepts/linkage/*.o
	-rm -f edl/*.o
	-rm -f linearsystem/*.o
	-rm -f network/*.o
	-rm -f numeric/operators/*.o
	-rm -f numeric/*.o
	-rm -f osg/*.o
	-rm -f relations/*.o
	-rm -f threads/platform/*.o
	-rm -f threads/*.o
	-rm -f transformations/*.o
	-rm -f ubf/*.o
	-rm -f units/area/*.o
	-rm -f units/angle/*.o
	-rm -f units/distance/*.o
	-rm -f units/energy/*.o
	-rm -f units/force/*.o
	-rm -f units/frequency/*.o
	-rm -f units/mass/*.o
	-rm -f units/power/*.o
	-rm -f units/rate/*.o
	-rm -f units/time/*.o
	-rm -f units/volumne/*.o
	-rm -f units/*.o
	-rm -f util/platform/*.o
	-rm -f util/*.o
	-rm -f *.o
	-rm -f $(LIB)
