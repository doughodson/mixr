#
include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_models.a

CPPFLAGS += -I$(JSBSIM_INC_DIR)

OBJS =  \
	dynamics/AerodynamicsModel.o \
	dynamics/DynamicsModel.o \
	dynamics/JSBSimModel.o \
	dynamics/LaeroModel.o  \
	dynamics/RacModel.o \
	dynamics/SpaceDynamicsModel.o \
	environment/AbstractAtmosphere.o \
	environment/IrAtmosphere.o \
	environment/IrAtmosphere1.o \
	navigation/Bullseye.o \
	navigation/Gps.o \
	navigation/Ins.o \
	navigation/Navigation.o \
	navigation/Route.o \
	navigation/Steerpoint.o \
	player/air/Aircraft.o \
	player/air/AirVehicle.o \
	player/air/Helicopter.o \
	player/air/UnmannedAirVehicle.o \
	player/effect/Chaff.o \
	player/effect/Decoy.o \
	player/effect/Effect.o \
	player/effect/Flare.o \
	player/ground/ArmoredVehicle.o \
	player/ground/Artillery.o \
	player/ground/GroundStation.o \
	player/ground/GroundStationRadar.o \
	player/ground/GroundStationUav.o \
	player/ground/GroundVehicle.o \
	player/ground/SamVehicle.o \
	player/ground/Tank.o \
	player/ground/WheeledVehicle.o \
	player/space/BoosterSpaceVehicle.o \
	player/space/MannedSpaceVehicle.o \
	player/space/SpaceVehicle.o \
	player/space/UnmannedSpaceVehicle.o \
	player/weapon/Aam.o \
	player/weapon/AbstractWeapon.o \
	player/weapon/Agm.o \
	player/weapon/Bomb.o \
	player/weapon/Bullet.o \
	player/weapon/Missile.o \
	player/weapon/Sam.o \
	player/Building.o \
	player/LifeForm.o \
	player/Player.o \
	player/Ship.o \
	sensor/Gmti.o \
	sensor/Stt.o \
	sensor/Tws.o \
	system/trackmanager/AirAngleOnlyTrkMgr.o \
	system/trackmanager/AirAngleOnlyTrkMgrPT.o \
	system/trackmanager/AirTrkMgr.o \
	system/trackmanager/AngleOnlyTrackManager.o \
	system/trackmanager/GmtiTrkMgr.o \
	system/trackmanager/RwrTrkMgr.o \
	system/trackmanager/TrackManager.o \
	system/Antenna.o \
	system/Autopilot.o \
	system/AvionicsPod.o \
	system/CollisionDetect.o \
	system/CommRadio.o \
	system/Datalink.o \
	system/ExternalStore.o \
	system/FuelTank.o \
	system/Gimbal.o \
	system/Guns.o \
	system/Iff.o \
	system/IrSeeker.o \
	system/IrSensor.o \
	system/IrSystem.o \
	system/Jammer.o \
	system/MergingIrSensor.o \
	system/OnboardComputer.o \
	system/Pilot.o \
	system/Radar.o \
	system/Radio.o \
	system/RfSensor.o \
	system/RfSystem.o \
	system/Rwr.o \
	system/Sar.o \
	system/ScanGimbal.o \
	system/SensorMgr.o \
	system/SimpleStoresMgr.o \
	system/StabilizingGimbal.o \
	system/Stores.o \
	system/StoresMgr.o \
	system/System.o \
	Actions.o \
	AircraftIrSignature.o \
	Designator.o \
	Emission.o \
	Image.o \
	IrQueryMsg.o \
	IrShapes.o \
	IrSignature.o \
	Message.o \
	MultiActorAgent.o \
	SensorMsg.o \
	Signatures.o \
	SimAgent.o \
	SimAgent.o \
	SynchronizedState.o \
	TargetData.o \
	Tdb.o \
	Track.o \
	WorldModel.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f dynamics/*.o
	-rm -f environment/*.o
	-rm -f navigation/*.o
	-rm -f player/air/*.o
	-rm -f player/effect/*.o
	-rm -f player/ground/*.o
	-rm -f player/space/*.o
	-rm -f player/weapon/*.o
	-rm -f player/*.o
	-rm -f sensor/*.o
	-rm -f system/trackmanager/*.o
	-rm -f system/*.o
	-rm -f *.o
	-rm -f $(LIB)

