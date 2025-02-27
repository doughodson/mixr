
#ifndef __mixr_interop_dis_NetIO_HPP__
#define __mixr_interop_dis_NetIO_HPP__

#include "mixr/interop/INetIO.hpp"

#include <array>

namespace mixr {
namespace base { class IAngle; class Integer; class ILength; class INetHandler; class Identifier; }
namespace models { class Iff; class RfSensor; }
namespace interop { class INib; }
namespace dis {
class Nib;
class Ntm;
class EmissionPduHandler;

struct EeFundamentalParameterData;
struct EmitterBeamData;
struct EmitterSystem;
struct EmissionSystem;
struct FundamentalOpData;
struct PDUHeader;
struct TrackJamTargets;

struct DetonationPDU;
struct ElectromagneticEmissionPDU;
struct EntityStatePDU;
struct FirePDU;
struct SignalPDU;
struct TransmitterPDU;
struct DataQueryPDU;
struct DataPDU;
struct CommentPDU;
struct StartPDU;
struct StopPDU;
struct AcknowledgePDU;
struct ActionRequestPDU;
struct ActionRequestPDU_R;
struct ActionResponsePDU_R;

//------------------------------------------------------------------------------
// Class: dis::NetIO
// Description: Distributed-Interactive-Simulation (DIS) protocol manager.
//
// Slots:
//    netInput    <base::INetHandler>        ! Network input handler
//    netOutput   <base::INetHandler>        ! Network output handler
//
//    version     <base::Integer>            ! DIS version number [ 0 .. 6 ] (IST-CF-03-01, May 5, 2003)
//                                           !   0 => Other
//                                           !   1 => DIS PDU version 1.0 (May 92)
//                                           !   2 => IEEE 1278-1993
//                                           !   3 => DIS PDU version 2.0 - third draft (May 93)
//                                           !   4 => DIS PDU version 2.0 - fourth draft (revised) March 16, 1994
//                                           !   5 => IEEE 1278.1-1995
//                                           !   6 => IEEE 1278.1A-1998
//                                           !   7 => IEEE 1278.1 -- draft 15
//
//    siteID         <base::Integer>         ! Site Identification (default: 1)
//    applicationID  <base::Integer>         ! Application Identification (default: 1)
//    exerciseID     <base::Integer>         ! Exercise Identification (default: 1)
//
//    maxTimeDR   <base::ITime>              ! Max DR time (default: 5 seconds)
//                <base::PairStream>         ! List of max DR times by kinds and domains (see note #4)
//
//    maxPositionError <base::ILength>       ! Max DR position error (default: 3 meters)
//                     <base::PairStream>    ! List of max DR position errors by kinds and domains (see note #4)
//
//    maxOrientationError <base::IAngle>     ! Max DR angular error (default: 3 degrees)
//                        <base::PairStream> ! List of max DR angular errors by kinds and domains (see note #4)
//
//    maxAge         <base::ITime>           ! Max age (without update) (default: 12.5 seconds)
//                   <base::PairStream>      ! List of max ages (without update) by kinds and domains (see note #4)
//
//    maxEntityRange <base::ILength>         ! Max entity range, or zero for no max range (default: 0 -- no range filtering)
//                   <base::PairStream>      ! List of max entity ranges by kinds and domains (see note #4)
//
//    EmissionPduHandlers <base::PairStream> ! List of Electromagnetic-Emission PDU handlers
//
//
// Notes:
//    1) NetIO creates its own federate name based on the site and application numbers
//       using makeFederateName().  (e.g., site = 10 and  app = 143 gives the federate name "S10A143")
//
//    2) NetIO creates its own federation name based on the exercise number
//       using makeFederationName().  (e.g., exercise = 13 gives the federation name "E13")
//
//    3) findDisNib() searches the same input and output lists that are maintained by
//       NetIO, which are in order of player ID and the federate name.  Since our DIS
//       federate names are generated by site and app IDs, the lists are seen by DIS as
//       being order by player ID, site ID and app ID.
//
//    4) For the slots maxTimeDR, maxPositionError, maxOrientationError, maxAge and
//       maxEntityRange, if the slot type is base::Time, base::Angle or base::Length then that
//       parameter is set for entity types of all kinds and domains.  If a pair stream
//       is given then individual entity kind/domain parameters can be set.  To set the
//       parameters for individual entity kind/domain types, the slot name must have
//       the format:  Kn or KnDm where n and m are the Kind and Domain numbers.
//       Examples --
//          maxTimeDR: { K5: ( Seconds 10.0 )  K1D11: ( Seconds 5.0 ) }
//             K5 will set the parameter for all domains of kinds #5
//             K1D11 will set the parameter for kind #1, domain #11
//
//    5) Setting the 'maxEntityRange' slot to zero(0) for an entity kind/domain
//       will filter out all entities of that kind/domain type.
//
//    6) For outgoing emission PDUs, the list of EmissionPduHandlers are matched
//       with RfSensors using the RfSensor::getTypeId().  That is, the type id
//       of the sensor is matched with the type id of the EmissionPduHandler's
//       type id.  For incoming emission PDUs, the "emitter name" from the PDU
//       is matched with the EmissionPduHandler's "emitterName" value.
//
//------------------------------------------------------------------------------
class NetIO : public interop::INetIO
{
    DECLARE_SUBCLASS(NetIO, interop::INetIO)

public:
   // Max PDU buffer size
   enum { MAX_PDU_SIZE = 1536 };

   // Standard (IST-CF-03-01, May 5, 2003) entity type "kind" codes [ 0 .. 9 ]
   enum EntityTypeKindEnum {
      KIND_OTHER, KIND_PLATFORM, KIND_MUNITION, KIND_LIFEFORM,
      KIND_ENVIRONMENTAL, KIND_CULTURAL_FEATURE, KIND_SUPPLY, KIND_RADIO,
      KIND_EXPENDABLE, KIND_SENSOR_EMITTER, NUM_ENTITY_KINDS
   };

   // Standard (IST-CF-03-01, May 5, 2003) "platform domain" codes [ 0 .. 5 ]
   enum PlatformDomainEnum {
      PLATFORM_DOMAIN_OTHER, PLATFORM_DOMAIN_LAND, PLATFORM_DOMAIN_AIR, PLATFORM_DOMAIN_SURFACE,
      PLATFORM_DOMAIN_SUBSURFACE, PLATFORM_DOMAIN_SPACE
   };

   // Standard (IST-CF-03-01, May 5, 2003) "munition domain" codes [ 0 .. 11 ]
   enum MunitionDomainEnum {
      MUNITION_DOMAIN_OTHER, MUNITION_DOMAIN_ANTI_AIR, MUNITION_DOMAIN_ANTI_ARMOR, MUNITION_DOMAIN_ANTI_GUIDED_MUNITION,
      MUNITION_DOMAIN_ANTIRADAR, MUNITION_DOMAIN_ANTISATELLITE, MUNITION_DOMAIN_ANTISHIP, MUNITION_DOMAIN_ANTISUBMARINE,
      MUNITION_DOMAIN_ANTIPERSONNEL, MUNITION_DOMAIN_BATTLEFIELD_SUPPORT, MUNITION_DOMAIN_STRATEGIC, MUNITION_DOMAIN_TACTICAL
   };

   // Larges number of domains in any kind (IST-CF-03-01, May 5, 2003)
   enum { MAX_ENTITY_DOMAINS = MUNITION_DOMAIN_TACTICAL };

   // Standard (IST-CF-03-01, May 5, 2003) "country" codes
   enum EntityTypeCountryEnum {
      COUNTRY_OTHER = 0, COUNTRY_FRANCE = 71, COUNTRY_CIS = 222, COUNTRY_UK = 224, COUNTRY_USA = 225,
   };

    // Standard (IST-CF-03-01, May 5, 2003) "force" codes [ 0 .. 3 ]
    enum ForceEnum {
        OTHER_FORCE,   FRIENDLY_FORCE,   OPPOSING_FORCE,   NEUTRAL_FORCE
    };

   // Standard (IST-CF-03-01, May 5, 2003) "DIS Protocol Version" codes [ 0 .. 6 ]
   enum {
         VERSION_OTHER,    // Other
         VERSION_100,      // DIS PDU version 1.0 (May 92)
         VERSION_1278,     // IEEE 1278-1993
         VERSION_203,      // DIS PDU version 2.0 - third draft (May 93)
         VERSION_204,      // DIS PDU version 2.0 - fourth draft (revised) March 16, 1994
         VERSION_1278_1,   // IEEE 1278.1-1995
         VERSION_1278_1A,  // IEEE 1278.1A-1998
         VERSION_7,        // IEEE P1278.1/D15
         VERSION_MAX       // Max version numbers
    };

   // SISO-REF-010-2006; 12th May, 2006; Section 3.2 PDU Type
   enum {
      PDU_OTHER=0,                  PDU_ENTITY_STATE=1,           PDU_FIRE=2,
      PDU_DETONATION=3,             PDU_COLLISION=4,              PDU_SERVICE_REQUEST=5,
      PDU_RESUPPLY_OFFER=6,         PDU_RESUPPLY_RECEIVED=7,      PDU_RESUPPLY_CANCEL=8,
      PDU_REPAIR_COMPLETE=9,        PDU_REPAIR_RESPONSE=10,       PDU_CREATE_ENTITY=11,
      PDU_REMOVE_ENTITY=12,         PDU_START_RESUME=13,          PDU_STOP_FREEZE=14,
      PDU_ACKNOWLEDGE=15,           PDU_ACTION_REQUEST=16,        PDU_ACTION_RESPONSE=17,
      PDU_DATA_QUERY=18,            PDU_SET_DATA=19,              PDU_DATA=20,
      PDU_EVENT_REPORT=21,          PDU_COMMENT=22,               PDU_ELECTROMAGNETIC_EMISSION=23,
      PDU_DESIGNATOR=24,            PDU_TRANSMITTER=25,           PDU_SIGNAL=26,
      PDU_RECEIVER=27,              PDU_IFF_ATC_NAVAIDS=28,       PDU_UNDERWATER_ACOUSTIC=29,
      PDU_SUPPLEMENTAL_EMISSION=30, PDU_INTERCOM_SIGNAL=31,       PDU_INTERCOM_CONTROL=32,
      PDU_AGGREGATE_STATE=33,       PDU_IS_GROUP_OF=34,           PDU_TRANSFER_CONTROL=35,
      PDU_IS_PART_OF=36,            PDU_MINEFIELD_STATE=37,       PDU_MINEFIELD_QUERY=38,
      PDU_MINEFIELD_DATA=39,        PDU_MINEFIELD_RESPONSE_NAK=40, PDU_ENVIRONMENTAL_PROCESS=41,
      PDU_GRIDDED_DATA=42,          PDU_POINT_OBJECT_STATE=43,    PDU_LINEAR_OBJECT_STATE=44,
      PDU_AREAL_OBJECT_STATE=45,    PDU_TSPI=46,                  PDU_APPEARANCE=47,
      PDU_ARTICULATED_PARTS=48,     PDU_LE_FIRE=49,               PDU_LE_DETONATION=50,
      PDU_CREATE_ENTITY_R=51,       PDU_REMOVE_ENTITY_R=52,       PDU_START_RESUME_R=53,
      PDU_STOP_FREEZE_R=54,         PDU_ACKNOWLEDGE_R=55,         PDU_ACTION_REQUEST_R=56,
      PDU_ACTION_RESPONSE_R=57,     PDU_DATA_QUERY_R=58,          PDU_SET_DATA_R=59,
      PDU_DATA_R=60,                PDU_EVENT_REPORT_R=61,        PDU_COMMENT_R=62,
      PDU_RECORD_R=63,              PDU_SET_RECORD_R=64,          PDU_RECORD_QUERY_R=65,
      PDU_COLLISION_ELASTIC=66,     PDU_ENTITY_STATE_UPDATE=67,

      PDU_ANNOUNCE_OBJECT=129,      PDU_DELETE_OBJECT = 130,
      PDU_DESCRIBE_APPLICATION=131, PDU_DESCRIBE_EVENT = 132,
      PDU_DESCRIBE_OBJECT=133,      PDU_REQUEST_EVENT = 134,
      PDU_REQUEST_OBJECT=135
   };

   // Standard (IST-CF-03-01, May 5, 2003) PDU Family
   enum {
      PDU_FAMILY_OTHER,                // other
      PDU_FAMILY_ENTITY_INFO,          // Entity Information/Interaction
      PDU_FAMILY_WARFARE,              // Warfare
      PDU_FAMILY_LOGISTICS,            // Logistics
      PDU_FAMILY_RADIO_COMM,           // Radio Communication
      PDU_FAMILY_SIMULATION_MAN,       // Simulation Management
      PDU_FAMILY_DIS_EMISSION_REG,     // Distributed Emission Regeneration
      PDU_FAMILY_ENTITY_MAN,           // Entity Management
      PDU_FAMILY_MINEFIELD,            // Minefield
      PDU_FAMILY_SYNTHETIC_ENV,        // Synthetic Environment
      PDU_FAMILY_SIMULATION_MAN_REL,   // Simulation Management with Reliability
      PDU_FAMILY_LIVE_ENTITY,          // Live Entity
      PDU_FAMILY_NON_REAL_TIME,        // Non-Real Time
      PDU_FAMILY_EXPERIMENTAL = 129    // Experimental - Computer Generated Forces
   };


public:
   NetIO();

   // Network Identifications
   unsigned short getSiteID() const                        { return siteID;     }
   unsigned short getApplicationID() const                 { return appID;      }
   unsigned char getExerciseID() const                     { return exerciseID; }

   // Sends a packet (PDU) to the network
   bool sendData(const char* const packet, const int size);

   // Receives a packet (PDU) from the network
   int recvData(char* const packet, const int maxSize);

   unsigned int timeStamp();                                                  // Gets the current timestamp
   unsigned int makeTimeStamp(const double ctime, const bool absolute);       // Make a PDU time stamp

   bool isVersion(const unsigned char v) const    { return (v == version); }  // True if versions match
   unsigned char getVersion() const               { return version; }         // Returns the current version number
   virtual bool setVersion(const unsigned char v);                            // Sets the operating version number

   // Emission PDU handler
   const EmissionPduHandler* findEmissionPduHandler(const models::RfSensor* const);
   const EmissionPduHandler* findEmissionPduHandler(const EmissionSystem* const);

   // Finds the Nib for 'ioType' by player, site and app IDs
   virtual Nib* findDisNib(const unsigned short playerId, const unsigned short siteId, const unsigned short appId, const IoType ioType);

   // Finds the Ntm by DIS entity type codes
   virtual const Ntm* findNtmByTypeCodes(
         const unsigned char  kind,
         const unsigned char  domain,
         const unsigned short countryCode,
         const unsigned char  category,
         const unsigned char  subcategory = 0,
         const unsigned char  specific = 0,
         const unsigned char  extra = 0
      ) const;

   double getMaxEntityRange(const interop::INib* const nib) const final;
   double getMaxEntityRangeSquared(const interop::INib* const nib) const final;
   double getMaxTimeDR(const interop::INib* const nib) const final;
   double getMaxPositionErr(const interop::INib* const nib) const final;
   double getMaxOrientationErr(const interop::INib* const nib) const final;
   double getMaxAge(const interop::INib* const nib) const final;
   interop::INib* createNewOutputNib(models::Player* const player) final;

   // DIS v7 additions
   virtual double getHbtPduEe() const;
   virtual double getHbtTimeoutMplier() const;
   virtual double getEeAzThrsh() const;
   virtual double getEeElThrsh() const;

   virtual double getEeErpThrsh() const;
   virtual double getEeFreqThrsh() const;
   virtual double getEeFrngThrsh() const;
   virtual double getEePrfThrsh() const;
   virtual double getEePwThrsh() const;

protected:
   virtual void processEntityStatePDU(const EntityStatePDU* const);
   virtual void processFirePDU(const FirePDU* const);
   virtual void processDetonationPDU(const DetonationPDU* const);
   virtual void processElectromagneticEmissionPDU(const ElectromagneticEmissionPDU* const);
   virtual bool processSignalPDU(const SignalPDU* const);
   virtual bool processTransmitterPDU(const TransmitterPDU* const);
   virtual bool processDataQueryPDU(const DataQueryPDU* const);
   virtual bool processDataPDU(const DataPDU* const);
   virtual bool processCommentPDU(const CommentPDU* const);
   virtual bool processStartPDU(const StartPDU* const);
   virtual bool processStopPDU(const StopPDU* const);
   virtual bool processAcknowledgePDU(const AcknowledgePDU* const);
   virtual bool processActionRequestPDU(const ActionRequestPDU* const);
   virtual bool processActionRequestPDU_R(const ActionRequestPDU_R* const);
   virtual bool processActionResponsePDU_R(const ActionResponsePDU_R* const);

   // User defined function to process unknown PDUs (PDU bytes are still in network order)
   virtual bool processUserPDU(const PDUHeader* const);

   virtual void clearEmissionPduHandlers();
   virtual void addEmissionPduHandler(const EmissionPduHandler* const);
   virtual void defineFederateName();
   virtual void defineFederationName();

   // Set functions
   virtual bool setSiteID(const unsigned short);          // Sets the network's site ID
   virtual bool setApplicationID(const unsigned short);   // Sets the network's application ID
   virtual bool setExerciseID(const unsigned char);       // Sets the network's exercise ID

   virtual bool slot2KD(const char* const slotname, unsigned char* const k, unsigned char* const d);
   virtual bool setMaxTimeDR(const double v, const unsigned char kind, const unsigned char domain);
   virtual bool setMaxTimeDR(const base::ITime* const p, const unsigned char kind, const unsigned char domain);
   virtual bool setMaxPositionErr(const double v, const unsigned char kind, const unsigned char domain);
   virtual bool setMaxPositionErr(const base::ILength* const p, const unsigned char kind, const unsigned char domain);
   virtual bool setMaxOrientationErr(const double v, const unsigned char kind, const unsigned char domain);
   virtual bool setMaxOrientationErr(const base::IAngle* const p, const unsigned char kind, const unsigned char domain);
   virtual bool setMaxAge(const double v, const unsigned char kind, const unsigned char domain);
   virtual bool setMaxAge(const base::ITime* const p, const unsigned char kind, const unsigned char domain);
   virtual bool setMaxEntityRange(const double v, const unsigned char kind, const unsigned char domain);
   virtual bool setMaxEntityRange(const base::ILength* const p, const unsigned char kind, const unsigned char domain);

   // NetIO Interface
   bool initNetwork() override;                                                   // Initialize the network
   void netInputHander() override;                                                // Network input handler
   void processInputList() override;                                              // Update players/systems from the Input-list
   interop::INib* nibFactory(const interop::INetIO::IoType ioType) override;      // Create a new Nib
   interop::INtmInputNode* rootNtmInputNodeFactory() const override;
   void testOutputEntityTypes(const int) final;                                   // Test quick lookup of outgoing entity types
   void testInputEntityTypes(const int) final;                                    // Test quick lookup of incoming entity types

private:
    void initData();

    base::safe_ptr<base::INetHandler> netInput;    // Input network handler
    base::safe_ptr<base::INetHandler> netOutput;   // Output network handler
    unsigned char version{VERSION_1278_1A};       // Version number [ 0 .. 6 ]

   // Network Model IDs
   unsigned short siteID{1};                      // Site ID
   unsigned short appID{1};                       // Application ID
   unsigned char exerciseID{1};                   // Exercise ID

   static const unsigned int MAX_PDUs{500};               // Max PDUs in input buffer
   unsigned int inputBuffer[MAX_PDUs][MAX_PDU_SIZE/4]{};  // Input buffer

   // Distance filter by entity kind/domain
   double  maxEntityRange[NUM_ENTITY_KINDS][MAX_ENTITY_DOMAINS]{};     // Max range from ownship           (meters)
   double  maxEntityRange2[NUM_ENTITY_KINDS][MAX_ENTITY_DOMAINS]{};    // Max range squared from ownship   (meters^2)

   // Dead Reckoning (DR) parameters by entity kind/domain
   double  maxTimeDR[NUM_ENTITY_KINDS][MAX_ENTITY_DOMAINS]{};          // Maximum DR time                  (seconds)
   double  maxPositionErr[NUM_ENTITY_KINDS][MAX_ENTITY_DOMAINS]{};     // Maximum position error           (meters)
   double  maxOrientationErr[NUM_ENTITY_KINDS][MAX_ENTITY_DOMAINS]{};  // Maximum orientation error        (radians)
   double  maxAge[NUM_ENTITY_KINDS][MAX_ENTITY_DOMAINS]{};             // Maximum age of networked players (seconds)

   static const unsigned int MAX_EMISSION_HANDLERS{500};               // Max table size

   // Table of pointers to emission PDU handlers; EmissionPduHandler objects
   std::array<const EmissionPduHandler*, MAX_EMISSION_HANDLERS> emissionHandlers{};

   // Number of emission PDU handlers in the table, 'emissionHandlers'
   unsigned int nEmissionHandlers{};

private:
   // slot table helper methods
   bool setSlotFederateName(const base::Identifier* const) final;     // Sets our federate name
   bool setSlotFederationName(const base::Identifier* const) final;   // Sets our federation name
   bool setSlotMaxTimeDR(const base::ITime* const) final;             // Sets the max DR time(s) for all entity types
   bool setSlotMaxPositionErr(const base::ILength* const) final;      // Sets the max positional error(s) for all entity types
   bool setSlotMaxOrientationErr(const base::IAngle* const) final;    // Sets the max orientation error(s) for all entity types
   bool setSlotMaxEntityRange(const base::ILength* const) final;      // Sets the max entity range(s) for all entity types
   bool setSlotMaxAge(const base::ITime* const) final;                // Sets the max age(s) for all entity types

   bool setSlotNetInput(base::INetHandler* const);                    // Network input handler
   bool setSlotNetOutput(base::INetHandler* const);                   // Network output handler
   bool setSlotVersion(const base::Integer* const);                   // DIS version
   bool setSlotMaxTimeDR(const base::PairStream* const);              // Sets the max DR time(s) for selected entity types
   bool setSlotMaxPositionErr(const base::PairStream* const);         // Sets the max positional error(s) for selected entity types
   bool setSlotMaxOrientationErr(const base::PairStream* const);      // Sets the max orientation error(s) for selected entity types
   bool setSlotMaxAge(const base::PairStream* const);                 // Sets the max age(s) for selected entity types
   bool setSlotMaxEntityRange(const base::PairStream* const);         // Sets the max entity range(s) for selected entity types
   bool setSlotEmissionPduHandlers(base::PairStream* const);          // Sets the list of Electromagnetic Emission PDU handlers
   bool setSlotSiteID(const base::Integer* const);                    // Sets Site ID
   bool setSlotApplicationID(const base::Integer* const);             // Sets Application ID
   bool setSlotExerciseID(const base::Integer* const);                // Sets Exercise ID
};

}
}

#endif
