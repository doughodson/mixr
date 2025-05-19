
#ifndef __mixr_interop_dis_EmissionPduHandler_HPP__
#define __mixr_interop_dis_EmissionPduHandler_HPP__

#include "mixr/interop/dis/NetIO.hpp"
#include "mixr/interop/dis/structs.hpp"

namespace mixr {
namespace models { class Antenna; class Boolean; class Integer; class Number; class IRfSensor; }
namespace dis {

//------------------------------------------------------------------------------
// Class: EmissionPduHandler
//
// Description: Handles the DIS input/output from an emitter system
//              (e.g., IRfSensor class)
//
// Factory name: EmissionPduHandler
// Slots:
//     emitterName      <Integer>   ! DIS emitter name (see DIS enums)
//     emitterFunction  <Integer>   ! DIS emitter function code (see enums)
//     sensor           <IRfSensor> ! Template sensor model
//     antenna          <Antenna>   ! Template antenna model
//     defaultIn        <Boolean>   ! This is the default handler for incoming PDUs
//     defaultOut       <Boolean>   ! This is the default handler for outgoing PDUs
//
//
// Example:
//    ( DisEmissionPduHandler
//       emitterName: 1970
//       emitterFunction: 1
//       sensor: ( Radar typeId: "APG-78" )
//       antenna: ( Antenna )
//    )
//
//------------------------------------------------------------------------------
class EmissionPduHandler : public base::IObject
{
    DECLARE_SUBCLASS(EmissionPduHandler, base::IObject)

public:
   enum { MAX_EM_BEAMS   = 6 };
   enum { MAX_TARGETS_IN_TJ_FIELD = 10 };

   // Emitter System Function (ESF) codes -- (partial list) Section 8.1.2, SISO-REF-010-2005
   enum EmitterSystemFunctionCode {
      ESF_OTHER                = 0,
      ESF_MULTI_FUNCTION       = 1,
      ESF_EW                   = 2,
      ESF_HEIGHT_FINDING       = 3,
      ESF_FIRE_CONTROL         = 4,
      ESF_ACQUISITION          = 5,
      ESF_TRACKING             = 6,
      ESF_GUIDANCE             = 7,
      ESF_FIRE_POINT_LOCATION  = 8,
      ESF_RANGING              = 9,
      ESF_RADAR_ALTIMETER      = 10,
      ESF_IMAGING              = 11,
      ESF_MOTION_DETECTION     = 12,
      ESF_NAVIGATION           = 13,
      ESF_AAA_FIRE_CONTROL     = 17,
      ESF_DATA_TRANSMISSION    = 29,
      ESF_JAMMING              = 42,
   };

   // Beam Function (BF) codes -- (partial list) Section 8.1.4, SISO-REF-010-2005
   enum BeamFunctionCode {
      BF_OTHER                         =  0,
      BF_SEARCH                        =  1,
      BF_HEIGHT_FINDER                 =  2,
      BF_ACQUISITION                   =  3,
      BF_TRACKING                      =  4,
      BF_ACQUISITION_AND_TRACKING      =  5,
      BF_COMMAND_GUIDANCE              =  6,
      BF_ILLUMINATION                  =  7,
      BF_RANGE_ONLY_RADAR              =  8,
      BF_MISSILE_BEACON                =  9,
      BF_MISSILE_FUSE                  = 10,
      BF_ACTIVE_RADAR_MISSILE_SEEKER   = 11,
      BF_JAMMER                        = 12,
      BF_IFF                           = 13,
      BF_NAV_WEATHER                   = 14,
      BF_METEOROLOGICAL                = 15,
      BF_DATA_TRANSMISSION             = 16,
      BF_NAV_DIRECTIONAL_BEACON        = 17
   };

   enum BeamStatusCode {
      BS_ACTIVE                        = 0,
      BS_INACTIVE                      = 1
   };

   enum JammingTechniqueKind {
      JT_OTHER                         = 0,
      JT_NOISE                         = 1,
      JT_DECEPTION                     = 2,
      JT_SPECIAL                       = 3
   };

public:
   EmissionPduHandler();

   unsigned char getEmitterIdNumber() const;                   // Unique emitter ID number
   virtual bool setEmitterIdNumber(const unsigned char num);   // Unique emitter ID number

   unsigned short getEmitterName() const;                      // Gets our DIS Emitter Name
   virtual bool setEmitterName(const unsigned short newEmitterName); // Sets our DIS Emitter Name

   unsigned char getEmitterFunction() const;                   // Emitter function code
   virtual bool setEmitterFunction(const unsigned char num);   // Emitter function code

   models::IRfSensor* getSensor();                         // Our R/F emitter System
   const models::IRfSensor* getSensor() const;             // Our R/F emitter System (const version)
   virtual bool setSensor(models::IRfSensor* const);       // Sets our R/F emitter system

   models::IRfSensor* getSensorModel();                    // Our template sensor model
   const models::IRfSensor* getSensorModel() const;        // Our template sensor model
   virtual bool setSensorModel(models::IRfSensor* const);  // Sets our template sensor model

   models::Antenna* getAntennaModel();                     // Our template antenna model
   const models::Antenna* getAntennaModel() const;         // Our template antenna model
   virtual bool setAntennaModel(models::Antenna* const);   // Sets our template antenna model

   double getEmPduExecTime() const { return emPduExecTime; }
   virtual void setEmPduExecTime(const double);

   bool isDefaultIncomingHandler() const;    // Is this the default incoming emission PDU handler?
   bool setDefaultIn(const bool);

   bool isDefaultOutgoingHandler() const;    // Is this the default outgoing emission PDU handler?
   bool setDefaultOut(const bool);

   // True if IRfSensor data matches our parameters
   // (does not check the default handler flags)
   virtual bool isMatchingRfSystemType(const models::IRfSensor* const) const;

   // True if EmissionSystem PDU data matches our parameters
   // (does not check the default handler flags)
   virtual bool isMatchingRfSystemType(const EmissionSystem* const) const;

   // Process an emission system from incoming PDUs
   virtual bool updateIncoming(const ElectromagneticEmissionPDU* const pdu, const EmissionSystem* const es, Nib* const nib);

   // tell input handler to disable its sensors due to timeout
   virtual void setTimedOut();

   // Check to see if a PDU needs to be sent; returns true if a PDU was sent
   virtual bool updateOutgoing(const double curExecTime, Nib* const nib);

protected:
    const EmissionSystem* getSavedEmissionSystemData() const;
    bool setSavedEmissionSystemData(const EmissionSystem& newES);

    const EmitterBeamData* getSavedEmitterBeamData(const unsigned int idx) const;
    bool setSavedEmitterBeamData(const unsigned int idx, const EmitterBeamData& newEBD);

    const TrackJamTargets* getSavedTrackJamTargetData(const unsigned int ibeam, const unsigned int ifield) const;
    bool setSavedTrackJamTargetData(const unsigned int ibeam, const unsigned int ifield, const TrackJamTargets& newTJT);

    virtual unsigned short emissionSystemData2PDU(EmissionSystem* const es);
    virtual bool isUpdateRequired(const double curExecTime, bool* const stateChg, Nib* const nib);

    bool getTemplatesFound() const;
    bool setTemplatesFound(const bool flg);

private:
   EmissionSystem  emissionSystemN1;                                    // Saved (n-1) Description of the Emitter System
   EmitterBeamData emitterBeamDataN1[MAX_EM_BEAMS];                     // Saved (n-1) EmitterBeamData
   TrackJamTargets tjTargetsN1[MAX_EM_BEAMS][MAX_TARGETS_IN_TJ_FIELD];  // Saved (n-1) TrackJamTargets

   bool defaultIn {};                  // This is the default handler for incoming PDUs
   bool defaultOut {};                 // This is the default handler for outgoing PDUs
   bool noTemplatesFound {};           // No template sensor and/or antenna found

   unsigned short emitterName {};                      // DIS emitter number -- see DIS enums (e.g., 1805 )
   unsigned char emitterIdNumber {};                   // Unique ID number for each emitter system
   unsigned char emitterFunction {ESF_FIRE_CONTROL};   // Emitter function code

   base::safe_ptr<models::IRfSensor> sensor;           // The R/F sensor (radar, jammers, etc)
   base::safe_ptr<models::IRfSensor> sensorModel;      // Our template sensor model
   base::safe_ptr<models::Antenna>  antennaModel;      // Our template antenna model

   double emPduExecTime {};                            // Exec time of last Emission PDU output (seconds)

private:
   // slot table helper methods
   bool setSlotEmitterName(const base::Integer* const);      // Sets our DIS Emitter Name
   bool setSlotEmitterFunction(const base::Integer* const);  // Sets our DIS Emitter Function
   bool setSlotSensorTemplate(models::IRfSensor* const);     // Sets our template sensor model
   bool setSlotAntennaTemplate(models::Antenna* const);      // Sets our template antenna model
   bool setSlotDefaultIn(const base::Boolean* const);
   bool setSlotDefaultOut(const base::Boolean* const);
};

inline unsigned char EmissionPduHandler::getEmitterIdNumber() const        { return emitterIdNumber; }
inline unsigned short EmissionPduHandler::getEmitterName() const           { return emitterName; }
inline unsigned char EmissionPduHandler::getEmitterFunction() const        { return emitterFunction; }

inline models::IRfSensor* EmissionPduHandler::getSensor()                  { return sensor; }
inline const models::IRfSensor* EmissionPduHandler::getSensor() const      { return sensor; }

inline models::IRfSensor* EmissionPduHandler::getSensorModel()             { return sensorModel; }
inline const models::IRfSensor* EmissionPduHandler::getSensorModel() const { return sensorModel; }

inline models::Antenna* EmissionPduHandler::getAntennaModel()              { return antennaModel; }
inline const models::Antenna* EmissionPduHandler::getAntennaModel() const  { return antennaModel; }

inline bool EmissionPduHandler::isDefaultIncomingHandler() const           { return defaultIn; }
inline bool EmissionPduHandler::isDefaultOutgoingHandler() const           { return defaultOut; }

inline bool EmissionPduHandler::getTemplatesFound() const                  { return !noTemplatesFound; }

inline const EmissionSystem* EmissionPduHandler::getSavedEmissionSystemData() const  { return &emissionSystemN1; }

inline const EmitterBeamData* EmissionPduHandler::getSavedEmitterBeamData(const unsigned int idx) const
{
   return (idx < MAX_EM_BEAMS ? &emitterBeamDataN1[idx] : 0);
}

inline const TrackJamTargets* EmissionPduHandler::getSavedTrackJamTargetData(const unsigned int ibeam, const unsigned int ifield) const
{
   return (ibeam < MAX_EM_BEAMS && ifield < MAX_TARGETS_IN_TJ_FIELD ? &tjTargetsN1[ibeam][ifield] : 0);
}

}
}

#endif
