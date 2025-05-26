#
include ../makedefs

LIB = $(MIXR_LIB_DIR)/libmixr_models.a

OBJS =  \
	action/IAction.o \
	action/ActionCamuflageType.o \
	action/ActionDecoyRelease.o \
	action/ActionImagingSar.o \
	action/ActionWeaponRelease.o \
	dynamics/IDynamics.o \
	dynamics/IAeroDynamics.o \
	dynamics/ISpaceDynamics.o \
	dynamics/LaeroDynamics.o  \
	dynamics/RacDynamics.o \
	environment/IAtmosphere.o \
	environment/IrAtmosphere.o \
	environment/IrAtmosphere1.o \
	navigation/Bullseye.o \
	navigation/Gps.o \
	navigation/Ins.o \
	navigation/INavigation.o \
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
	player/weapon/IWeapon.o \
	player/weapon/Agm.o \
	player/weapon/Bomb.o \
	player/weapon/Bullet.o \
	player/weapon/Missile.o \
	player/weapon/Sam.o \
	player/Building.o \
	player/LifeForm.o \
	player/IPlayer.o \
	player/Ship.o \
	sensor/Gmti.o \
	sensor/Stt.o \
	sensor/Tws.o \
	signature/IIrShapeSignature.o \
	signature/IIrAircraftSignature.o \
	signature/IIrSignature.o \
	signature/IrSignature.o \
	signature/IrSphereSignature.o \
	signature/IrBoxSignature.o \
	signature/IRfSignature.o \
	signature/IRfDihedralCRSignature.o \
	signature/IRfPlateSignature.o \
	signature/RfAzElSignature.o \
	signature/RfConstantSignature.o \
	signature/RfSphereSignature.o \
	signature/RfSwitchSignature.o \
	signature/RfTrihedralCRSignature.o \
	system/trackmanager/AirAngleOnlyTrkMgr.o \
	system/trackmanager/AirAngleOnlyTrkMgrPT.o \
	system/trackmanager/AirTrkMgr.o \
	system/trackmanager/AngleOnlyTrackManager.o \
	system/trackmanager/GmtiTrkMgr.o \
	system/trackmanager/RwrTrkMgr.o \
	system/trackmanager/ITrackMgr.o \
	system/Antenna.o \
	system/Autopilot.o \
	system/AvionicsPod.o \
	system/CollisionDetect.o \
	system/CommRadio.o \
	system/Datalink.o \
	system/IExternalStore.o \
	system/FuelTank.o \
	system/IGimbal.o \
	system/Gimbal.o \
	system/Gun.o \
	system/Iff.o \
	system/IrSeeker.o \
	system/IrSensor.o \
	system/IrSystem.o \
	system/Jammer.o \
	system/MergingIrSensor.o \
	system/OnboardComputer.o \
	system/IPilot.o \
	system/IRadar.o \
	system/IRadio.o \
	system/IRfSensor.o \
	system/IRfSystem.o \
	system/Rwr.o \
	system/Sar.o \
	system/IScanGimbal.o \
	system/SensorMgr.o \
	system/SimpleStoresMgr.o \
	system/StabilizingGimbal.o \
	system/IStores.o \
	system/IStoresMgr.o \
	system/ISystem.o \
	system/System.o \
	track/ITrack.o \
	track/RfTrack.o \
	track/IrTrack.o \
	Designator.o \
	Image.o \
	IrQueryMsg.o \
	ISensorMsg.o \
	Message.o \
	MultiActorAgent.o \
	RfEmission.o \
	SimAgent.o \
	SynchronizedState.o \
	TargetData.o \
	Tdb.o \
	WorldModel.o \
	factory.o

.PHONY: all clean

all: $(LIB)

$(LIB) : $(OBJS)
	ar rs $@ $(OBJS)

clean:
	-rm -f action/*.o
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
	-rm -f signature/*.o
	-rm -f system/trackmanager/*.o
	-rm -f system/*.o
	-rm -f *.o
	-rm -f $(LIB)

