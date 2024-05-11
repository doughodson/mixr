
#include "mixr/models/common/factory.hpp"

#include "mixr/base/Object.hpp"

// dynamics models
#include "mixr/models/common/dynamics/RacModel.hpp"
#include "mixr/models/common/dynamics/LaeroModel.hpp"

// environment models
#include "mixr/models/common/environment/IrAtmosphere.hpp"
#include "mixr/models/common/environment/IrAtmosphere1.hpp"

// navigation models
#include "mixr/models/common/navigation/Bullseye.hpp"
#include "mixr/models/common/navigation/Gps.hpp"
#include "mixr/models/common/navigation/Ins.hpp"
#include "mixr/models/common/navigation/Navigation.hpp"
#include "mixr/models/common/navigation/Route.hpp"
#include "mixr/models/common/navigation/Steerpoint.hpp"

// player models
#include "mixr/models/common/player/air/Aircraft.hpp"
#include "mixr/models/common/player/air/AirVehicle.hpp"
#include "mixr/models/common/player/air/Helicopter.hpp"
#include "mixr/models/common/player/air/UnmannedAirVehicle.hpp"
#include "mixr/models/common/player/effect/Chaff.hpp"
#include "mixr/models/common/player/effect/Decoy.hpp"
#include "mixr/models/common/player/effect/Flare.hpp"
#include "mixr/models/common/player/ground/ArmoredVehicle.hpp"
#include "mixr/models/common/player/ground/Artillery.hpp"
#include "mixr/models/common/player/ground/GroundStation.hpp"
#include "mixr/models/common/player/ground/GroundStationRadar.hpp"
#include "mixr/models/common/player/ground/GroundStationUav.hpp"
#include "mixr/models/common/player/ground/GroundVehicle.hpp"
#include "mixr/models/common/player/ground/SamVehicle.hpp"
#include "mixr/models/common/player/ground/Tank.hpp"
#include "mixr/models/common/player/ground/WheeledVehicle.hpp"
#include "mixr/models/common/player/space/BoosterSpaceVehicle.hpp"
#include "mixr/models/common/player/space/MannedSpaceVehicle.hpp"
#include "mixr/models/common/player/space/SpaceVehicle.hpp"
#include "mixr/models/common/player/space/UnmannedSpaceVehicle.hpp"
#include "mixr/models/common/player/weapon/Aam.hpp"
#include "mixr/models/common/player/weapon/Agm.hpp"
#include "mixr/models/common/player/weapon/Bomb.hpp"
#include "mixr/models/common/player/weapon/Bullet.hpp"
#include "mixr/models/common/player/weapon/Missile.hpp"
#include "mixr/models/common/player/weapon/Sam.hpp"
#include "mixr/models/common/player/Building.hpp"
#include "mixr/models/common/player/LifeForm.hpp"
#include "mixr/models/common/player/Player.hpp"
#include "mixr/models/common/player/Ship.hpp"

// sensor models
#include "mixr/models/common/sensor/Gmti.hpp"
#include "mixr/models/common/sensor/Tws.hpp"
#include "mixr/models/common/sensor/Stt.hpp"

// world models
#include "mixr/models/common/WorldModel.hpp"

// system models - track managers
#include "mixr/models/common/system/trackmanager/AirAngleOnlyTrkMgr.hpp"
#include "mixr/models/common/system/trackmanager/AirAngleOnlyTrkMgrPT.hpp"
#include "mixr/models/common/system/trackmanager/AirTrkMgr.hpp"
#include "mixr/models/common/system/trackmanager/AngleOnlyTrackManager.hpp"
#include "mixr/models/common/system/trackmanager/GmtiTrkMgr.hpp"
#include "mixr/models/common/system/trackmanager/RwrTrkMgr.hpp"
#include "mixr/models/common/system/trackmanager/TrackManager.hpp"

// system models
#include "mixr/models/common/system/Antenna.hpp"
#include "mixr/models/common/system/Autopilot.hpp"
#include "mixr/models/common/system/AvionicsPod.hpp"
#include "mixr/models/common/system/CollisionDetect.hpp"
#include "mixr/models/common/system/CommRadio.hpp"
#include "mixr/models/common/system/Datalink.hpp"
#include "mixr/models/common/system/ExternalStore.hpp"
#include "mixr/models/common/system/FuelTank.hpp"
#include "mixr/models/common/system/Gimbal.hpp"
#include "mixr/models/common/system/Gun.hpp"
#include "mixr/models/common/system/Iff.hpp"
#include "mixr/models/common/system/IrSeeker.hpp"
#include "mixr/models/common/system/IrSensor.hpp"
#include "mixr/models/common/system/Jammer.hpp"
#include "mixr/models/common/system/MergingIrSensor.hpp"
#include "mixr/models/common/system/OnboardComputer.hpp"
#include "mixr/models/common/system/Pilot.hpp"
#include "mixr/models/common/system/Radar.hpp"
#include "mixr/models/common/system/Radio.hpp"
#include "mixr/models/common/system/RfSensor.hpp"
#include "mixr/models/common/system/Rwr.hpp"
#include "mixr/models/common/system/Sar.hpp"
#include "mixr/models/common/system/ScanGimbal.hpp"
#include "mixr/models/common/system/SensorMgr.hpp"
#include "mixr/models/common/system/SimpleStoresMgr.hpp"
#include "mixr/models/common/system/StabilizingGimbal.hpp"
#include "mixr/models/common/system/Stores.hpp"
#include "mixr/models/common/system/StoresMgr.hpp"
#include "mixr/models/common/system/System.hpp"

// signatures
#include "mixr/models/common/signature/IrSignature.hpp"
#include "mixr/models/common/signature/AircraftIrSignature.hpp"
#include "mixr/models/common/signature/SigAzEl.hpp"
#include "mixr/models/common/signature/SigConstant.hpp"
#include "mixr/models/common/signature/SigDihedralCR.hpp"
#include "mixr/models/common/signature/SigPlate.hpp"
#include "mixr/models/common/signature/SigSphere.hpp"
#include "mixr/models/common/signature/SigSwitch.hpp"
#include "mixr/models/common/signature/SigTrihedralCR.hpp"

// misc
#include "mixr/models/common/Actions.hpp"
#include "mixr/models/common/IrShapes.hpp"

#include "mixr/models/common/SimAgent.hpp"
#include "mixr/models/common/MultiActorAgent.hpp"

#include "mixr/models/common/TargetData.hpp"
#include "mixr/models/common/Track.hpp"

#include <string>

namespace mixr {
namespace models {

base::Object* factory(const std::string& name)
{
   base::Object* obj {};

   // dynamics models
   if ( name == RacModel::getFactoryName() ) {              // RAC
      obj = new RacModel();
   }
   else if ( name == LaeroModel::getFactoryName() ) {       // Laero
      obj = new LaeroModel();
   }

   // environment
   else if ( name == IrAtmosphere::getFactoryName() ) {
      obj = new IrAtmosphere;
   }
   else if ( name == IrAtmosphere1::getFactoryName() ) {
      obj = new IrAtmosphere1;
   }

   // sensor models
   if ( name == Gmti::getFactoryName() ) {
      obj = new Gmti();
   }
   else if ( name == Stt::getFactoryName() ) {
      obj = new Stt();
   }
   else if ( name == Tws::getFactoryName() ) {
      obj = new Tws();
   }

   // world models
   else if ( name == WorldModel::getFactoryName() ) {
      obj = new WorldModel();
   }

   // Players
   else if ( name == Player::getFactoryName() ) {
      obj = new Player();
   }
   else if ( name == AirVehicle::getFactoryName() ) {
      obj = new AirVehicle();
   }
   else if ( name == Building::getFactoryName() ) {
      obj = new Building();
   }
   else if ( name == GroundVehicle::getFactoryName() ) {
      obj = new GroundVehicle();
   }
   else if ( name == LifeForm::getFactoryName() ) {
      obj = new LifeForm();
   }
   else if ( name == Ship::getFactoryName() ) {
      obj = new Ship();
   }
   else if ( name == SpaceVehicle::getFactoryName() ) {
      obj = new SpaceVehicle();
   }

   // Air Vehicles
   else if ( name == Aircraft::getFactoryName() ) {
      obj = new Aircraft();
   }
   else if ( name == Helicopter::getFactoryName() ) {
      obj = new Helicopter();
   }
   else if ( name == UnmannedAirVehicle::getFactoryName() ) {
      obj = new UnmannedAirVehicle();
   }

   // Ground Vehicles
   else if ( name == Tank::getFactoryName() ) {
      obj = new Tank();
   }
   else if ( name == ArmoredVehicle::getFactoryName() ) {
      obj = new ArmoredVehicle();
   }
   else if ( name == WheeledVehicle::getFactoryName() ) {
      obj = new WheeledVehicle();
   }
   else if ( name == Artillery::getFactoryName() ) {
      obj = new Artillery();
   }
   else if ( name == SamVehicle::getFactoryName() ) {
      obj = new SamVehicle();
   }
   else if ( name == GroundStation::getFactoryName() ) {
      obj = new GroundStation();
   }
   else if ( name == GroundStationRadar::getFactoryName() ) {
      obj = new GroundStationRadar();
   }
   else if ( name == GroundStationUav::getFactoryName() ) {
      obj = new GroundStationUav();
   }

   // Space Vehicles
   else if ( name == MannedSpaceVehicle::getFactoryName() ) {
      obj = new MannedSpaceVehicle();
   }
   else if ( name == UnmannedSpaceVehicle::getFactoryName() ) {
      obj = new UnmannedSpaceVehicle();
   }
   else if ( name == BoosterSpaceVehicle::getFactoryName() ) {
      obj = new BoosterSpaceVehicle();
   }

   // System
   else if ( name == System::getFactoryName() ) {
      obj = new System();
   }
   else if ( name == AvionicsPod::getFactoryName() ) {
      obj = new AvionicsPod();
   }

   // Basic Pilot types
   else if ( name == Pilot::getFactoryName() ) {
      obj = new Pilot();
   }
   else if ( name == Autopilot::getFactoryName() ) {
      obj = new Autopilot();
   }

   // Navigation types
   else if ( name == Navigation::getFactoryName() ) {
      obj = new Navigation();
   }
   else if ( name == Ins::getFactoryName() ) {
      obj = new Ins();
   }
   else if ( name == Gps::getFactoryName() ) {
      obj = new Gps();
   }
   else if ( name == Route::getFactoryName() ) {
      obj = new Route();
   }
   else if ( name == Steerpoint::getFactoryName() ) {
      obj = new Steerpoint();
   }

   // Target Data
   else if ( name == TargetData::getFactoryName() ) {
      obj = new TargetData();
   }

   // Bullseye
   else if ( name == Bullseye::getFactoryName() ) {
      obj = new Bullseye();
   }

   // Actions
   else if ( name == ActionImagingSar::getFactoryName() ) {
      obj = new ActionImagingSar();
   }
   else if ( name == ActionWeaponRelease::getFactoryName() ) {
      obj = new ActionWeaponRelease();
   }
   else if ( name == ActionDecoyRelease::getFactoryName() ) {
      obj = new ActionDecoyRelease();
   }
   else if ( name == ActionCamouflageType::getFactoryName() ) {
      obj = new ActionCamouflageType();
   }

   // Bombs and Missiles
   else if ( name == Bomb::getFactoryName() ) {
      obj = new Bomb();
   }
   else if ( name == Missile::getFactoryName() ) {
      obj = new Missile();
   }
   else if ( name == Aam::getFactoryName() ) {
      obj = new Aam();
   }
   else if ( name == Agm::getFactoryName() ) {
      obj = new Agm();
   }
   else if ( name == Sam::getFactoryName() ) {
      obj = new Sam();
   }

   // Effects
   else if ( name == Chaff::getFactoryName() ) {
      obj = new Chaff();
   }
   else if ( name == Decoy::getFactoryName() ) {
      obj = new Decoy();
   }
   else if ( name == Flare::getFactoryName() ) {
      obj = new Flare();
   }

   // Stores, stores manager and external stores (FuelTank, Gun & Bullets (used by the Gun))
   else if ( name == Stores::getFactoryName() ) {
      obj = new Stores();
   }
   else if ( name == SimpleStoresMgr::getFactoryName() ) {
      obj = new SimpleStoresMgr();
   }
   else if ( name == FuelTank::getFactoryName() ) {
      obj = new FuelTank();
   }
   else if ( name == Gun::getFactoryName() ) {
      obj = new Gun();
   }
   else if ( name == Bullet::getFactoryName() ) {
      obj = new Bullet();
   }

   // Data links
   else if ( name == Datalink::getFactoryName() ) {
      obj = new Datalink();
   }

   // Gimbals, Antennas and Optics
   else if ( name == Gimbal::getFactoryName() ) {
      obj = new Gimbal();
   }
   else if ( name == ScanGimbal::getFactoryName() ) {
      obj = new ScanGimbal();
   }
   else if ( name == StabilizingGimbal::getFactoryName() ) {
      obj = new StabilizingGimbal();
   }
   else if ( name == Antenna::getFactoryName() ) {
      obj = new Antenna();
   }
   else if ( name == IrSeeker::getFactoryName() ) {
      obj = new IrSeeker();
   }

   // R/F Signatures
   else if ( name == SigConstant::getFactoryName() ) {
      obj = new SigConstant();
   }
   else if ( name == SigSphere::getFactoryName() ) {
      obj = new SigSphere();
   }
   else if ( name == SigPlate::getFactoryName() ) {
      obj = new SigPlate();
   }
   else if ( name == SigDihedralCR::getFactoryName() ) {
      obj = new SigDihedralCR();
   }
   else if ( name == SigTrihedralCR::getFactoryName() ) {
      obj = new SigTrihedralCR();
   }
   else if ( name == SigSwitch::getFactoryName() ) {
      obj = new SigSwitch();
   }
   else if ( name == SigAzEl::getFactoryName() ) {
      obj = new SigAzEl();
   }
   // IR Signatures
   else if ( name == IrSignature::getFactoryName() ) {
      obj = new IrSignature();
   }
   else if ( name == AircraftIrSignature::getFactoryName() ) {
      obj = new AircraftIrSignature;
   }
   else if ( name == IrShape::getFactoryName() ) {
      obj = new IrShape();
   }
   else if ( name == IrSphere::getFactoryName() ) {
      obj = new IrSphere();
   }
   else if ( name == IrBox::getFactoryName() ) {
      obj = new IrBox();
   }
    // Onboard Computers
   else if ( name == OnboardComputer::getFactoryName() ) {
      obj = new OnboardComputer();
   }
   // Radios
   else if ( name == Radio::getFactoryName() ) {
      obj = new Radio();
   }
   else if ( name == CommRadio::getFactoryName() ) {
      obj = new CommRadio();
   }
   else if ( name == Iff::getFactoryName() ) {
      obj = new Iff();
   }
   // Sensors
   else if ( name == RfSensor::getFactoryName() ) {
      obj = new RfSensor();
   }
   else if ( name == SensorMgr::getFactoryName() ) {
      obj = new SensorMgr();
   }
   else if ( name == Radar::getFactoryName() ) {
      obj = new Radar();
   }
   else if ( name == Rwr::getFactoryName() ) {
      obj = new Rwr();
   }
   else if ( name == Sar::getFactoryName() ) {
      obj = new Sar();
   }
   else if ( name == Jammer::getFactoryName() ) {
      obj = new Jammer();
   }
   else if ( name == IrSensor::getFactoryName() ) {
      obj = new IrSensor();
   }
   else if ( name == MergingIrSensor::getFactoryName() ) {
      obj = new MergingIrSensor();
   }

   // Tracks
   else if ( name == Track::getFactoryName() ) {
      obj = new Track();
   }

   // Track Managers
   else if ( name == GmtiTrkMgr::getFactoryName() ) {
      obj = new GmtiTrkMgr();
   }
   else if ( name == AirTrkMgr::getFactoryName() ) {
      obj = new AirTrkMgr();
   }
   else if ( name == RwrTrkMgr::getFactoryName() ) {
      obj = new RwrTrkMgr();
   }
   else if ( name == AirAngleOnlyTrkMgr::getFactoryName() ) {
      obj = new AirAngleOnlyTrkMgr();
   }

   // UBF Agents
   else if ( name == SimAgent::getFactoryName() ) {
      obj = new SimAgent();
   }
   else if ( name == MultiActorAgent::getFactoryName() ) {
      obj = new MultiActorAgent();
   }

   // Collision detection component
   else if ( name == CollisionDetect::getFactoryName() ) {
      obj = new CollisionDetect();
   }

   return obj;
}

}
}
