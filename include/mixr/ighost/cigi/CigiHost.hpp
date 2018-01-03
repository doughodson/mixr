
#ifndef __mixr_ighost_cigi_CigiHost_H__
#define __mixr_ighost_cigi_CigiHost_H__

#include "mixr/ighost/cigi/IgHost.hpp"

#include <array>

class CigiIGCtrlV3;
class CigiLosVectReqV3;   // CGBCGB CIGI_LOS_RANGE_REQUEST* los;          // LOS request packet
class CigiViewCtrlV3;
class CigiSensorCtrlV3;
class CigiViewDefV3;
class CigiCompCtrlV3;
class CigiEntityCtrlV3;
class CigiSOFV3;
class CigiHatHotRespV3;
class CigiLosRespV3;
class CigiCollDetSegRespV3;
class CigiSensorRespV3;
class CigiCollDetVolRespV3;
class CigiIGMsgV3;       // CGBCGB CIGI_IG_RESPONSE_MESSAGE

//class CigiArtPartCtrlV3;
//class CigiHatHotReqV3;
//class CigiHostSession;
//class CigiIncomingMsg;
//class CigiOutgoingMsg;

namespace mixr {
namespace base { class Boolean; class Number; class NetHandler; }
namespace models {
class AirVehicle; class Building; class Effect; class GroundVehicle; class LifeForm;
class Missile; class Player; class Ship; class SpaceVehicle; class AbstractWeapon;
}
namespace cigi {
class HostSession;
class CigiModel;
class IgThread;

//------------------------------------------------------------------------------
// Class: CigiHost
//
// Description: Host side for the Common Image Generator Interface (CIGI)
//
// Factory name: CigiHost
// Slots:
//    session              (HostSession)     CIGI host session
//    async                (Number)          True (non-zero) to run in CIGI async mode (default: false - CIGI sync)
//    hideOwnshipModel     (Number)          True to hide the ownship's model (default: true - ownship's model is not seen)
//    ownshipModel         (Number)          Ownship's model ID
//    mslTrailModel        (Number)          "Missile Trail" effect model ID
//    smokePlumeModel      (Number)          "Smoke Plume" effect model ID
//    airExplosionModel    (Number)          "Air Explosion" effect model ID
//    groundExplosionModel (Number)          "Ground Explosion" effect model ID
//    shipWakeModel        (Number)          "Ship Wake" effect model ID
//
// Note: In the async mode, the sendCigiData() function, which sends the CIGI
// packets to the session, is called by our frameSync() function in the
// R/T thread.  In the sync mode, the sendCigiData() function is called by the
// startOfFrame() callback (i.e., sync'd with the IG).
//
//------------------------------------------------------------------------------
class CigiHost final: public IgHost
{
   DECLARE_SUBCLASS(CigiHost, IgHost)

public:
   static const int NUM_BUFFERS{2};

public:
   CigiHost();

   // is a LOS request pending?
   bool isLosRequestPending() const                 { return (losReqId != losRespId); }

   // is an elevation request pending?
   bool isElevationRequestPending() const           { return elevReqFlg; }

   bool isASyncMode() const                         { return asyncMode;  }          // True if running in CIGI async mode
   bool isSyncMode() const                          { return !asyncMode; }          // True if running in CIGI sync mode
   void setASyncMode(const bool x)                  { asyncMode = x;     }          // Sets the CIGI async mode flag

   bool isOwnshipModelHidden() const                { return hideOwn;    }          // True if the ownship's model is hidden
   void setHideOwnshipModel(const bool x)           { hideOwn = x;       }          // Sets the hide ownship flag

   int getOwnshipModelId() const                    { return cmtOwnship; }          // Ownship's model ID
   void setOwnshipModelId(const int id)             { cmtOwnship = id;   }          // Ownship's model ID

   int getMslTrailModelId() const                   { return cmtMslTrail; }         // "Missile Trail" effect model ID
   void setMslTrailModelId(const int id)            { cmtMslTrail = id;   }         // "Missile Trail" effect model ID

   int getSmokePlumeModelId() const                 { return cmtSmokePlume; }       // "Smoke Plume" effect model ID
   void setSmokePlumeModelId(const int id)          { cmtSmokePlume = id;   }       // "Smoke Plume" effect model ID

   int getAirExplosionModelId() const               { return cmtAirExplosion; }     // "Air Explosion" effect model ID
   void setAirExplosionModelId(const int id)        { cmtAirExplosion = id;   }     // "Air Explosion" effect model ID

   int getGroundExplosionModelId() const            { return cmtGroundExplosion; }  // "Ground Explosion" effect model ID
   void setGroundExplosionModelId(const int id)     { cmtGroundExplosion = id;   }  // "Ground Explosion" effect model ID

   int getShipWakeModelId() const                   { return cmtShipWake; }         // "Ship Wake" effect model ID
   void setShipWakeModelId(const int id)            { cmtShipWake = id;   }         // "Ship Wake" effect model ID

   // IG callbacks
   void startOfFrame(const CigiSOFV3* const);
   void hatHotResp(const CigiHatHotRespV3* const);
   void losResp(const CigiLosRespV3* const);
   void collisionSegmentResp(const CigiCollDetSegRespV3* const);
   void sensorResp(const CigiSensorRespV3* const);
   void collisionVolumeResp(const CigiCollDetVolRespV3* const);
   void igResponse(const CigiIGMsgV3* const);

   // send data to the cigi handler
   bool sendCigiData();

   HostSession* getHostSession()                         { return session; }
   const HostSession* getHostSession() const             { return session; }

   void updateData(const double dt = 0.0) final;
   void reset() final;

private:
   CigiModel* modelFactory() final;         // Create objects unique to interface
   CigiModel* hotFactory() final;           // Create objects unique to interface
   void sendOwnshipAndModels() final;       // Send state data for ownship and models
   void sendElevationRequests() final;      // Sends terrain height requests
   void recvElevations() final;             // Receives terrain height data
   void frameSync() final;                  // Send frame sync

   enum LIFE_FORM_STATE { DEAD = 0, STANDING = 1, WALK = 2, RUN = 3 };

   bool updateOwnshipModel();          // update the ownship model; returns true if ok
   int updateModels();                 // update the other models; returns number of active models

   // access functions
   CigiIGCtrlV3* getIgControlPacket()                                          { return igc; }
   CigiLosVectReqV3* getLosRangeRequestPacket()                                { return los; }
   CigiViewCtrlV3* getViewControlPacket()                                      { return view; }
   CigiSensorCtrlV3* getSensorControlPacket()                                  { return sensor; }
   CigiViewDefV3* getViewDefinitionPacket()                                    { return fov; }
   CigiCompCtrlV3* getOwnshipComponentControlPacket(const int buffer)          {
      return (buffer < NUM_BUFFERS ? ownshipCC[buffer] : 0);
   }
   CigiEntityCtrlV3* getOwnshipEntityControlPacket(const int buffer)           {
      return (buffer < NUM_BUFFERS ? ownshipEC[buffer] : 0);
   }

   int getWriteBuffer() const                  { return iw;  }                       // Write buffer index
   int getLastWriteBuffer() const              { return iw0; }                       // Last write buffer index
   int getReadBuffer() const                   { return ir;  }                       // Read index {returns index or NUM_BUFFERS if not valid)
   void swapReadBuffer()                       { if (iw0 < NUM_BUFFERS) ir = iw0; }  // Swap the read buffer

   bool isIgResetRequested() const             { return resetRequest;  }
   void clearIgResetRequest()                  { resetRequest = false; }

   int getNexLosId()                           { return ++losReqId; }
   bool isNewLosequested() const               { return newLosReq;  }
   void losRequestSend();           // LOS request has been sent to the IG

   void elevationRequestSend();     // Elevation request has been sent to the IG

   // get Line of sight data from previous request
   bool getLineOfSightData(
      double* const lat,            // Point latitude         (deg)
      double* const lon,            // Point longitude        (deg)
      double* const alt,            // Point altitude         (m)
      double* const rng,            // Range to point         (m)
      int* const material           // Material code - not used in Cigi V3, returns 0
      );

   // issue a new light of sight request
   bool lineOfSightRequest(
      const double lat,             // Source latitude         (deg)
      const double lon,             // Source longitude        (deg)
      const double alt,             // Source altitude         (m)
      const double hdg,             // Source heading          (deg)
      const double pitch,           // Source pitch            (deg)
      const double minRange,        // Request minimum range   (m)
      const double maxRange         // Request maximum range   (m)
      );

   // set functions
   void setViewControlPacket(CigiViewCtrlV3* const p)            { view = p;   }
   void setViewDefinitionPacket(CigiViewDefV3* const p)          { fov = p;    }
   void setSensorControlPacket(CigiSensorCtrlV3* const p)        { sensor = p; }

   bool setCommonModelData(CigiEntityCtrlV3* const ec, const int entity, const models::Player* const);

   bool setAirVehicleData(CigiModel* const, const int entity, const models::AirVehicle* const);
   bool setBuildingData(CigiModel* const, const int entity, const models::Building* const);
   bool setEffectData(CigiModel* const, const int entity, const models::Effect* const);
   bool setGndVehicleData(CigiModel* const, const int entity, const models::GroundVehicle* const);
   bool setLifeFormData(CigiModel* const, const int entity, const models::LifeForm* const);
   bool setMissileData(CigiModel* const, const int entity, const models::Missile* const);
   bool setShipData(CigiModel* const, const int entity, const models::Ship* const);
   bool setSpaceVehicleData(CigiModel* const, const int entity, const models::SpaceVehicle* const);
   bool setWeaponData(CigiModel* const, const int entity, const models::AbstractWeapon* const);

   // creates a one shot task thread to process IG packets
   bool createProcessingThread();
   base::safe_ptr<IgThread> igThread;

   base::safe_ptr<HostSession> session;   // networked host session

   // CIGI init support
   bool cigiInitialized{};                // CIGI has been initialized
   bool cigiInitFailed{};                 // CIGI initialization has failed

   bool asyncMode{};                      // Running in ASYNC mode if true
   bool hideOwn{true};                    // Hide ownship model flag
   bool resetRequest{true};               // IG reset request

   int entityIdCount{};                   // Entity ID count
   int elevReqIdCount{};                  // Elevation request ID count

   // Terrain elevation request data
   bool elevReqFlg{};                     // Elevation request flag
   double elevReqTimer{};                 // Elevation request timer

   // Line of sight (LOS) data
   double losRespLat{};                   // LOS Response latitude intersection point (deg)
   double losRespLon{};                   // LOS Response longitude intersection point (deg)
   double losRespAlt{};                   // LOS Response altitude intersection point (m)
   double losRespRange{};                 // LOS response range (m)
   int losRespId{};                       // LOS Response ID
   bool losRespDataValid{true};           // LOS response data is valid flag
   int losReqId{};                        // LOS Request ID
   bool newLosReq{true};                  // New LOS request flag
   double losReqTimer{};                  // LOS request timer

   // CIGI entity data buffers
   int iw{NUM_BUFFERS};                   // Write buffer index
   int iw0{NUM_BUFFERS};                  // Last write buffer index
   int ir{NUM_BUFFERS};                   // Read index

   // messages
   std::array<CigiEntityCtrlV3*, NUM_BUFFERS> ownshipEC{}; // Ownship entity control
   std::array<CigiCompCtrlV3*, NUM_BUFFERS> ownshipCC{};   // ownship component control
   CigiIGCtrlV3*     igc{};               // IG control
   CigiLosVectReqV3* los{};               // LOS request
   CigiViewCtrlV3*   view{};              // View control (optional, set by derived classes)
   CigiViewDefV3*    fov{};               // FOV definition (optional, set by derived classes
   CigiSensorCtrlV3* sensor{};            // Sensor control

   // special model IDs
   int cmtOwnship{302};                   // Ownship's model ID
   int cmtMslTrail{1100};                 // "Missile Trail" effect model ID
   int cmtSmokePlume{1101};               // "Smoke Plume" effect model ID
   int cmtAirExplosion{1102};             // "Air Explosion" effect model ID
   int cmtGroundExplosion{1103};          // "Ground Explosion" effect model ID
   int cmtShipWake{1104};                 // "Ship Wake" effect model ID

private:
   // slot table helper methods
   bool setSlotHostSession(HostSession* const);
   bool setSlotASyncMode(const base::Boolean* const);
   bool setSlotHideOwnshipModel(const base::Number* const);
   bool setSlotOwnshipModelId(const base::Number* const);
   bool setSlotMslTrailModelId(const base::Number* const);
   bool setSlotSmokePlumeModelId(const base::Number* const);
   bool setSlotAirExplosionModelId(const base::Number* const);
   bool setSlotGroundExplosionModelId(const base::Number* const);
   bool setSlotShipWakeModelId(const base::Number* const);
};

}
}

#endif
