
#ifndef __mixr_ighost_cigi_Cigi_H__
#define __mixr_ighost_cigi_Cigi_H__

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
namespace base { class Number; class NetHandler; class Thread; }
namespace models {
class AirVehicle; class Building; class Effect; class GroundVehicle; class LifeForm;
class Missile; class Player; class Ship; class SpaceVehicle; class AbstractWeapon;
}
namespace cigi {
class HostSession;
class OtwModelCigiCl;

//------------------------------------------------------------------------------
// Class: Cigi
//
// Description: Host side for the Common Image Generator Interface (CIGI)
//
// Factory name: Cigi
// Slots:
//    session              (HostSession)     CIGI host session
//    async                (Number)          True (non-zero) to run in CIGI async mode (default: false - CIGI sync)
//    hideOwnshipModel     (Number)          True to hide the ownship's model (default: true - ownship's model is not seen)
//    ownshipModel         (Number)          Ownship's model ID
//    mslTrailModel        (Number)          Missile Trail" effect model ID
//    smokePlumeModel      (Number)          Smoke Plume" effect model ID
//    airExplosionModel    (Number)          Air Explosion" effect model ID
//    groundExplosionModel (Number)          Ground Explosion" effect model ID
//    shipWakeModel        (Number)          Ship Wake" effect model ID
//
// Note: In the async mode, the sendCigiData() function, which sends the CIGI
// packets to the CigiCl handler, is called by our frameSync() function in the
// R/T thread.  In the sync mode, the sendCigiData() function is called by the
// startOfFrame() callback (i.e., sync'd with the IG).
//
//------------------------------------------------------------------------------
class Cigi : public IgHost
{
   DECLARE_SUBCLASS(Cigi, IgHost)

public:
   static const int NUM_BUFFERS{2};

public:
   Cigi();

   // is a LOS request pending?
   virtual bool isLosRequestPending() const         { return (losReqId != losRespId); }

   // is an elevation request pending?
   virtual bool isElevationRequestPending() const   { return elevReqFlg; }

   bool isASyncMode() const                         { return asyncMode; }            // True if running in CIGI async mode
   bool isSyncMode() const                          { return !asyncMode; }           // True if running in CIGI sync mode
   virtual bool setASyncMode(const bool f);                                         // Sets the CIGI async mode flag

   bool isOwnshipModelHidden() const                { return hideOwn; }              // True if the ownship's model is hidden
   virtual bool setHideOwnshipModel(const bool f);                                  // Sets the hide ownship flag

   unsigned short getOwnshipModelID() const         { return cmtOwnship; }          // Ownship's model ID
   virtual bool setOwnshipModelId(const unsigned short);                            // Ownship's model ID

   unsigned short getMslTrailModelID() const        { return cmtMslTrail; }         // "Missile Trail" effect model ID
   virtual bool setMslTrailModelId(const unsigned short);                           // "Missile Trail" effect model ID

   unsigned short getSmokePlumeModelID() const      { return cmtSmokePlume; }       // "Smoke Plume" effect model ID
   virtual bool setSmokePlumeModelId(const unsigned short);                         // "Smoke Plume" effect model ID

   unsigned short getAirExplosionModelID() const    { return cmtAirExplosion; }     // "Air Explosion" effect model ID
   virtual bool setAirExplosionModelId(const unsigned short);                       // "Air Explosion" effect model ID

   unsigned short getGroundExplosionModelID() const { return cmtGroundExplosion; }  // "Ground Explosion" effect model ID
   virtual bool setGroundExplosionModelId(const unsigned short);                    // "Ground Explosion" effect model ID

   unsigned short getShipWakeModelID() const        { return cmtShipWake; }         // "Ship Wake" effect model ID
   virtual bool setShipWakeModelId(const unsigned short);                           // "Ship Wake" effect model ID

   // IG callbacks
   virtual void startOfFrame(const CigiSOFV3* const);
   virtual void hatHotResp(const CigiHatHotRespV3* const);
   virtual void losResp(const CigiLosRespV3* const);
   virtual void collisionSegmentResp(const CigiCollDetSegRespV3* const);
   virtual void sensorResp(const CigiSensorRespV3* const);
   virtual void collisionVolumeResp(const CigiCollDetVolRespV3* const);
   virtual void igResponse(const CigiIGMsgV3* const);

   // send data to the cigi handler
   virtual bool sendCigiData();

   HostSession* getHostSession()                         { return session; }
   const HostSession* getHostSession() const             { return session; }

   void updateData(const double dt = 0.0) override;
   void reset() override;

protected:
   enum LIFE_FORM_STATE { DEAD = 0, STANDING = 1, WALK = 2, RUN = 3 };

   virtual bool updateOwnshipModel();          // update the ownship model; returns true if ok
   virtual int updateModels();                 // update the other models; returns number of active models

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

   int getWriteBuffer() const                  { return iw; }                        // Write buffer index
   int getLastWriteBuffer() const              { return iw0; }                       // Last write buffer index
   int getReadBuffer() const                   { return ir; }                        // Read index {returns index or NUM_BUFFERS if not valid)
   void swapReadBuffer()                       { if (iw0 < NUM_BUFFERS) ir = iw0; }  // Swap the read buffer

   bool isIgResetRequested() const             { return resetRequest; }
   void clearIgResetRequest()                  { resetRequest = false; }

   unsigned short getNexLosId()                { return ++losReqId; }
   bool isNewLosequested() const               { return newLosReq; }
   void losRequestSend();           // LOS request has been sent to the IG

   void elevationRequestSend();     // Elevation request has been sent to the IG

   // get Line of sight data from previous request
   virtual bool getLineOfSightData(
      double* const lat,            // Point latitude         (deg)
      double* const lon,            // Point longitude        (deg)
      double* const alt,            // Point altitude         (m)
      double* const rng,            // Range to point         (m)
      int* const material           // Material code - not used in Cigi V3, returns 0
      );

   // issue a new light of sight request
   virtual bool lineOfSightRequest(
      const double lat,             // Source latitude         (deg)
      const double lon,             // Source longitude        (deg)
      const double alt,             // Source altitude         (m)
      const double hdg,             // Source heading          (deg)
      const double pitch,           // Source pitch            (deg)
      const double minRange,        // Request minimum range   (m)
      const double maxRange         // Request maximum range   (m)
      );

   // set functions
   virtual bool setViewControlPacket(CigiViewCtrlV3* const);
   virtual bool setViewDefinitionPacket(CigiViewDefV3* const);
   virtual bool setSensorControlPacket(CigiSensorCtrlV3* const);

   void sendOwnshipAndModels() override;     // Send state data for ownship and models
   void sendElevationRequests() override;    // Sends terrain height requests
   void recvElevations() override;           // Receives terrain height data
   void frameSync() override;                // Send frame sync (if any)
   OtwModel* modelFactory() override;        // Create OtwModel objects unique to interface
   OtwModel* hotFactory() override;          // Create OtwHot objects unique to interface

   virtual bool setAirVehicleData(OtwModelCigiCl* const m, const unsigned short entity, const models::AirVehicle* const p);
   virtual bool setBuildingData(OtwModelCigiCl* const m, const unsigned short entity, const models::Building* const p);
   virtual bool setEffectData(OtwModelCigiCl* const m, const unsigned short entity, const models::Effect* const p);
   virtual bool setGndVehicleData(OtwModelCigiCl* const m, const unsigned short entity, const models::GroundVehicle* const p);
   virtual bool setLifeFormData(OtwModelCigiCl* const m, const unsigned short entity, const models::LifeForm* const p);
   virtual bool setMissileData(OtwModelCigiCl* const m, const unsigned short entity, const models::Missile* const p);
   virtual bool setShipData(OtwModelCigiCl* const m, const unsigned short entity, const models::Ship* const p);
   virtual bool setSpaceVehicleData(OtwModelCigiCl* const m, const unsigned short entity, const models::SpaceVehicle* const p);
   virtual bool setWeaponData(OtwModelCigiCl* const m, const unsigned short entity, const models::AbstractWeapon* const p);
   virtual bool setCommonModelData(CigiEntityCtrlV3* const ec, const unsigned short entity, const models::Player* const p);

private:
   // creates a thread to process IG packets
   bool createProcessingThread();
   base::safe_ptr<base::Thread> thread;

   base::safe_ptr<HostSession> session;         // networked host session

   // CIGI init support
   bool cigiInitialized{};                      // CIGI has been initialized
   bool cigiInitFailed{};                       // CIGI initialization has failed

   bool asyncMode{};                            // Running in ASYNC mode if true
   bool hideOwn{true};                          // Hide ownship model flag
   bool resetRequest{true};                     // IG reset request

   unsigned short entityIdCount{};              // Entity ID count
   unsigned short elevReqIdCount{};             // Elevation request ID count

   // Terrain elevation request data
   bool elevReqFlg {};                          // Elevation request flag
   double elevReqTimer {};                      // Elevation request timer

   // Line of sight (LOS) data
   double losRespLat{};                         // LOS Response latitude intersection point (deg)
   double losRespLon{};                         // LOS Response longitude intersection point (deg)
   double losRespAlt{};                         // LOS Response altitude intersection point (m)
   double losRespRange{};                       // LOS response range (m)
   unsigned short losRespId{};                  // LOS Response ID
   bool losRespDataValid{true};                 // LOS response data is valid flag
   unsigned short losReqId{};                   // LOS Request ID
   bool newLosReq{true};                        // New LOS request flag
   double losReqTimer{};                        // LOS request timer

   // CIGI entity data buffers
   int iw{NUM_BUFFERS};                         // Write buffer index
   int iw0{NUM_BUFFERS};                        // Last write buffer index
   int ir{NUM_BUFFERS};                         // Read index

   // messages
   std::array<CigiEntityCtrlV3*, NUM_BUFFERS> ownshipEC{}; // Ownship entity control
   std::array<CigiCompCtrlV3*, NUM_BUFFERS> ownshipCC{};   // ownship component control
   CigiIGCtrlV3*     igc{};                    // IG control
   CigiLosVectReqV3* los{};                    // LOS request
   CigiViewCtrlV3*   view{};                   // View control (optional, set by derived classes)
   CigiViewDefV3*    fov{};                    // FOV definition (optional, set by derived classes
   CigiSensorCtrlV3* sensor{};                 // Sensor control

   // special model IDs
   unsigned short cmtOwnship{302};             // Ownship's model ID
   unsigned short cmtMslTrail{1100};           // "Missile Trail" effect model ID
   unsigned short cmtSmokePlume{1101};         // "Smoke Plume" effect model ID
   unsigned short cmtAirExplosion{1102};       // "Air Explosion" effect model ID
   unsigned short cmtGroundExplosion{1103};    // "Ground Explosion" effect model ID
   unsigned short cmtShipWake{1104};           // "Ship Wake" effect model ID

private:
   // slot table helper methods
   bool setSlotHostSession(HostSession* const);
   bool setSlotASyncMode(const base::Number* const);
   bool setSlotHideOwnshipModel(const base::Number* const);
   bool setSlotOwnshipModel(const base::Number* const);
   bool setSlotMslTrailModel(const base::Number* const);
   bool setSlotSmokePlumeModel(const base::Number* const);
   bool setSlotAirExplosionModel(const base::Number* const);
   bool setSlotGroundExplosionModel(const base::Number* const);
   bool setSlotShipWakeModel(const base::Number* const);
};

}
}

#endif
