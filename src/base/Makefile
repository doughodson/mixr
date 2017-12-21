
#----------------------------------------------------------------------------------------
# To build parser and scanner:
#
# cd edl_parser
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
	colors/Color.o \
	colors/Hls.o \
	colors/Hsva.o \
	colors/Hsv.o \
	colors/Rgb.o \
	colors/Rgba.o \
	colors/Yiq.o \
	concepts/linkage/AbstractIoData.o \
	concepts/linkage/AbstractIoDevice.o \
	concepts/linkage/AbstractIoHandler.o \
	edl_parser/EdlParser.o \
	edl_parser/EdlScanner.o \
	functors/FStorage.o \
	functors/Func1.o \
	functors/Func2.o \
	functors/Func3.o \
	functors/Func4.o \
	functors/Func5.o \
	functors/Function.o \
	functors/Polynomial.o \
	functors/Table.o \
	functors/Table1.o \
	functors/Table2.o \
	functors/Table3.o \
	functors/Table4.o \
	functors/Table5.o \
	functors/TableStorage.o \
	network/NetHandler.o \
	network/PosixHandler.o \
	network/TcpClient.o \
	network/TcpHandler.o \
	network/TcpServerMultiple.o \
	network/TcpServerSingle.o \
	network/UdpBroadcastHandler.o \
	network/UdpMulticastHandler.o \
	network/UdpUnicastHandler.o \
	numeric/Boolean.o \
	numeric/Complex.o \
	numeric/Float.o \
	numeric/Integer.o \
	numeric/Number.o \
	numeric/Operators.o \
	osg/math.o \
	osg/Matrixd.o \
	osg/MatrixDecomposition.o \
	osg/Matrixf.o \
	osg/Quat.o \
	threads/platform/AbstractThread_linux.o \
	threads/platform/PeriodicThread_linux.o \
	threads/platform/SyncThread_linux.o \
	threads/AbstractThread.o \
	threads/OneShotThread.o \
	threads/PeriodicThread.o \
	threads/SyncThread.o \
	ubf/AbstractAction.o \
	ubf/AbstractBehavior.o \
	ubf/AbstractState.o \
	ubf/Agent.o \
	ubf/Arbiter.o \
	units/Angles.o \
	units/AngularVelocity.o \
	units/Areas.o \
	units/Decibel.o \
	units/Density.o \
	units/Distances.o \
	units/Energies.o \
	units/FlowRate.o \
	units/Forces.o \
	units/Frequencies.o \
	units/LinearVelocity.o \
	units/Masses.o \
	units/Powers.o \
	units/Times.o \
	units/Volumes.o \
	util/platform/system_linux.o \
	util/lfi.o \
	util/math_utils.o \
	util/nav_utils.o \
	util/navDR_utils.o \
	util/osg_utils.o \
	util/str_utils.o \
	util/system_utils.o \
	Component.o \
	EarthModel.o \
	factory.o \
	FileReader.o \
	Identifier.o \
	LatLon.o \
	List.o \
	Locus.o \
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
	Transforms.o \
	Vectors.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f colors/*.o
	-rm -f concepts/linkage/*.o
	-rm -f threads/platform/*.o
	-rm -f threads/*.o
	-rm -f edl_parser/*.o
	-rm -f functors/*.o
	-rm -f io/*.o
	-rm -f network/*.o
	-rm -f numeric/*.o
	-rm -f osg/*.o
	-rm -f ubf/*.o
	-rm -f units/*.o
	-rm -f util/platform/*.o
	-rm -f util/*.o
	-rm -f *.o
	-rm -f $(LIB)
