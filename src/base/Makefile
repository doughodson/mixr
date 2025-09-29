
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
	linearsystem/IDiffEquation.o \
	linearsystem/FirstOrderTf.o \
	linearsystem/LagFilter.o \
	linearsystem/Limit.o \
	linearsystem/Limit01.o \
	linearsystem/Limit11.o \
	linearsystem/LimitFunc.o \
	linearsystem/LowpassFilter.o \
	linearsystem/SaH.o \
	linearsystem/IScalerFunc.o \
	linearsystem/SecondOrderTf.o \
	linearsystem/Sz1.o \
	linearsystem/Sz2.o \
	network/INetHandler.o \
	network/IPosixHandler.o \
	network/TcpClient.o \
	network/ITcpHandler.o \
	network/TcpServerMultiple.o \
	network/TcpServerSingle.o \
	network/UdpBroadcastHandler.o \
	network/UdpMulticastHandler.o \
	network/UdpUnicastHandler.o \
	numeric/operators/Add.o \
	numeric/operators/Divide.o \
	numeric/operators/Multiply.o \
	numeric/operators/IOperator.o \
	numeric/operators/Subtract.o \
	numeric/Boolean.o \
	numeric/Complex.o \
	numeric/Decibel.o \
	numeric/Float.o \
	numeric/Integer.o \
	numeric/INumber.o \
	osg/math.o \
	osg/Matrixd.o \
	osg/MatrixDecomposition.o \
	osg/Matrixf.o \
	osg/Quat.o \
	qty/angle/IAngle.o \
	qty/angle/Degrees.o \
	qty/angle/Radians.o \
	qty/angle/Semicircles.o \
	qty/area/IArea.o \
	qty/area/DecibelSquareMeters.o \
	qty/area/SquareCentiMeters.o \
	qty/area/SquareFeet.o \
	qty/area/SquareInches.o \
	qty/area/SquareKiloMeters.o \
	qty/area/SquareMeters.o \
	qty/area/SquareMiles.o \
	qty/area/SquareMilliMeters.o \
	qty/area/SquareYards.o \
	qty/energy/BTUs.o \
	qty/energy/Calories.o \
	qty/energy/IEnergy.o \
	qty/energy/FootPounds.o \
	qty/energy/Joules.o \
	qty/energy/KiloWattHours.o \
	qty/force/IForce.o \
	qty/force/KiloNewtons.o \
	qty/force/Newtons.o \
	qty/force/Poundals.o \
	qty/force/PoundForces.o \
	qty/frequency/IFrequency.o \
	qty/frequency/GigaHertz.o \
	qty/frequency/Hertz.o \
	qty/frequency/KiloHertz.o \
	qty/frequency/MegaHertz.o \
	qty/frequency/TeraHertz.o \
	qty/length/CentiMeters.o \
	qty/length/Feet.o \
	qty/length/Inches.o \
	qty/length/KiloMeters.o \
	qty/length/ILength.o \
	qty/length/Meters.o \
	qty/length/MicroMeters.o \
	qty/length/Microns.o \
	qty/length/NauticalMiles.o \
	qty/length/StatuteMiles.o \
	qty/mass/Grams.o \
	qty/mass/KiloGrams.o \
	qty/mass/IMass.o \
	qty/mass/Slugs.o \
	qty/power/DecibelMilliWatts.o \
	qty/power/DecibelWatts.o \
	qty/power/Horsepower.o \
	qty/power/KiloWatts.o \
	qty/power/MilliWatts.o \
	qty/power/IPower.o \
	qty/power/Watts.o \
	qty/time/Days.o \
	qty/time/Hours.o \
	qty/time/MicroSeconds.o \
	qty/time/MilliSeconds.o \
	qty/time/Minutes.o \
	qty/time/NanoSeconds.o \
	qty/time/Seconds.o \
	qty/time/ITime.o \
	qty/volume/CubicFeet.o \
	qty/volume/CubicInches.o \
	qty/volume/CubicMeters.o \
	qty/volume/Liters.o \
	qty/volume/IVolume.o \
	qty/IQuantity.o \
	relations/IFStorage.o \
	relations/Func1.o \
	relations/Func2.o \
	relations/Func3.o \
	relations/Func4.o \
	relations/Func5.o \
	relations/IFunction.o \
	relations/Polynomial.o \
	relations/ITable.o \
	relations/Table1.o \
	relations/Table2.o \
	relations/Table3.o \
	relations/Table4.o \
	relations/Table5.o \
	relations/TableStorage.o \
	threads/platform/IThread_linux.o \
	threads/platform/IPeriodicThread_linux.o \
	threads/platform/ISyncThread_linux.o \
	threads/IThread.o \
	threads/IOneShotThread.o \
	threads/IPeriodicThread.o \
	threads/ISyncThread.o \
	timers/ITimer.o \
	timers/UpTimer.o \
	timers/DownTimer.o \
	transformations/ITransform.o \
	transformations/Translation.o \
	transformations/Rotation.o \
	transformations/Scale.o \
	ubf/IAction.o \
	ubf/IBehavior.o \
	ubf/IState.o \
	ubf/Agent.o \
	ubf/Arbiter.o \
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
	IComponent.o \
	EarthModel.o \
	factory.o \
	FileReader.o \
	Identifier.o \
	Latitude.o \
	IList.o \
	List.o \
	Locus.o \
	Longitude.o \
	Matrix.o \
	MetaObject.o \
	MonitorMetrics.o \
	IObject.o \
	Pair.o \
	PairStream.o \
	SlotTable.o \
	Stack.o \
	Statistic.o \
	IStateMachine.o \
	String.o \
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
	-rm -f qty/area/*.o
	-rm -f qty/angle/*.o
	-rm -f qty/length/*.o
	-rm -f qty/energy/*.o
	-rm -f qty/force/*.o
	-rm -f qty/frequency/*.o
	-rm -f qty/mass/*.o
	-rm -f qty/power/*.o
	-rm -f qty/rate/*.o
	-rm -f qty/time/*.o
	-rm -f qty/volume/*.o
	-rm -f qty/*.o
	-rm -f relations/*.o
	-rm -f threads/platform/*.o
	-rm -f threads/*.o
	-rm -f timers/*.o
	-rm -f transformations/*.o
	-rm -f ubf/*.o
	-rm -f util/platform/*.o
	-rm -f util/*.o
	-rm -f *.o
	-rm -f $(LIB)
