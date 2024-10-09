
#ifndef __mixr_interop_common_NetIO_HPP__
#define __mixr_interop_common_NetIO_HPP__

#include "mixr/simulation/AbstractNetIO.hpp"

#include <array>
#include <string>

namespace mixr {
namespace base { class Angle; class Boolean; class Identifier; class Integer; class Length; class Time; }
namespace models { class Player; }
namespace simulation { class Simulation; class Station; }
namespace interop {
class Nib;
class Ntm;
class NtmInputNode;
class NtmOutputNode;

//------------------------------------------------------------------------------
// Class: NetIO
// Description: Abstract interface class for interoperability Network I/O.
//
//    Defines the framework for interoperability networks (e.g., DIS, HLA,
//    TENA, etc).  Derived classes will manage the network specific protocols.
//
// Factory name: NetIO
//
// Slots:
//    networkID            <base::Integer>     ! Network ID number: [ 1 .. MAX_NETWORK_ID ] (default: 1)
//    federationName       <base::Identifier>  ! Federation name (default: 0)
//    federateName         <base::Identifier>  ! Name of this federate (default: 0)
//
//    enableInput          <base::Boolean>     ! Input Enable: accept players from this network (default: true)
//    enableOutput         <base::Boolean>     ! Output enable: send players to this network (default: true)
//    enableRelay          <base::Boolean>     ! Relay enable: send other network players to this network (default: true)
//    timeline             <base::Identifier>  ! Source of the time line { UTC or EXEC } (default: UTC)
//
//    inputEntityTypes     <base::PairSteam>   ! Incoming entity type mappers (default: 0)
//                                             !   (Ntm objects that map incoming entity types to mixr player types)
//
//    outputEntityTypes    <base::PairSteam>   ! Outgoing entity type mappers (default: 0)
//                                             !   (Ntm objects that map mixr player types to outgoing entity types)
//
//    maxTimeDR            <base::Time>        ! Max DR time (default: 5 seconds)
//    maxPositionError     <base::Length>      ! Max DR position error (default: 3 meters)
//    maxOrientationError  <base::Angle>       ! Max DR angular error (default: 3 degrees)
//    maxAge               <base::Time>        ! Max age (without update) of networked players (default: 12.5 seconds)
//
//    maxEntityRange       <base::Length>      ! Max entity range of networked players,
//                                             !  or zero for no max range (default: 0 -- no range filtering)
//
//
// NetIO class objects:
//
//    An mixr application, which is typically controlled by a Station class
//    object, can have multiple (i.e., MAX_NETWORK_ID) NetIO objects.  Each NetIO
//    object handles a different interoperabiltiy network
//
//    Each NetIO object in the simulation has an unique, user defined network ID,
//    which is set using the 'networkID' slot.  The NetIO objects are also
//    identified by their federation and federate names, which are based on HLA.
//    (see "dis/NetIO.hpp" for functions that will map the DIS exercise number to
//    a federation name, and the DIS site and application numbers to the federate
//    name)
//
//
// Incoming entities:
//
//    Use the 'enableInput' slot to enable or disable the discovery of entities
//    on the interoperability network.
//
//    As incoming entities are discovered, their entity type codes are mapped
//    to mixr player types using Network Type Mapping (Ntm) objects.  These
//    Ntm objects are listed in the "incoming entity type mapper" list, which
//    is set using the 'inputEntityTypes' slot.  An entity is ignored if its
//    entity type does not match any of the Ntm objects on the incoming list.
//    (Note: without the incoming Ntm list, all network entities are ignored)
//
//    When an entity's type matches a Ntm object, the matching Ntm's "template
//    player" (see Ntm.hpp) is cloned to create the proxy player, which
//    is added to simulation's player list.
//
//    A network specific Network Interface Block (Nib) is created, using the
//    nibFactory() function, to manage the flow of data from the network entity
//    to the mixr player.  The incoming Nib objects are managed using the
//    "input Nib" list.
//
//    Incoming interoperability player data contains the ID of the NetIO object that
//    created it, and a pointer to the Nib object that is controlling its data
//    flow.
//
//    Incoming entities can be filtered by range using the 'maxEntityRange'
//    slot, which defaults to zero or no range filtering.  Currently, this
//    applies only to new entities; proxy players are not filtered.
//
// Outgoing entities:
//
//    Use the 'enableOutput' slot to enable or disable the sending of our
//    application's local players to this interoperability network.
//
//    As local players are discovered by this class, their player
//    types are mapped to network entity type codes using the Ntm obects that
//    are listed in the "outgoing entity type mapper" list, which is set using
//    the "outputEntityTypes" slot.  A local player is not sent to the network
//    if its type does not match any of the Ntm objects on the outgoing list.
//    (Note: without the outgoing Ntm list, no local players will be written
//    to the network)
//
//    A network specific Nib is created, using the nibFactory() function, to
//    manage the flow of data from the mixr player to the network entity.
//    The outgoing Nib objects are managed using the "output Nib" list.
//
//    The mixr player objects do not contain any state data related to the
//    interoperability networks that their being sent to.  As a result, an
//    mixr player can be sent to more than one interoperability network,
//    and the 'enableRelay' slot can be used to enable the relaying of mixr
//    players that were discovered from other interoperability networks.
//
//
// Input/Output frames:
//
//    The functions inputFrame() and outputFrame() need to be called by our
//    container object (e.g., Station class) to process the incoming and out-
//    going entities, respectively.  For DIS, these can be called by different
//    threads, and for HLA they need to be called from the same thread.
//
//
// Time line:
//
//    Data sent to the network can be marked with universal UTC time, or
//    with relative executive time.
//
//
// Event IDs
//
//    Some network specific outgoing event messages require an unique ID
//    to identify unique events.  The following functions are used to
//    generate event unique IDs.
//
//       unsigned short getNewIffEventID()         // New IFF message event
//       unsigned short getNewEmissionEventID()    // New emission event
//
//
// Note: public and protected sections for Nib and Ntm support are located
//       after the main public and protected sections.
//------------------------------------------------------------------------------
class NetIO : public simulation::AbstractNetIO
{
   DECLARE_SUBCLASS(NetIO, simulation::AbstractNetIO)

public:
    // Source of the time line
    enum TSource {
       EXEC,         // Using our executive time
       UTC           // Using Coordinated Universal Time (UTC) from the Operating System
    };

    //  Max number of new, outgoing players published per frame
    static const unsigned int MAX_NEW_OUTGOING{MIXR_CONFIG_MAX_NETIO_NEW_OUTGOING};

public:
   NetIO();

   // Updates the 'input' side of the network
   void inputFrame(const double dt) override;

   // Updates the 'output' side of the network
   void outputFrame(const double dt) override;

   // Network ID number
   unsigned short getNetworkID() const override { return netID; }

   // Federate name
   virtual const std::string& getFederateName() const;

   // Federation name
   virtual const std::string& getFederationName() const;

   // Current time (seconds: UTC or EXEC)
   double getCurrentTime();

   // Timeline (UTC or EXEC)
   TSource getTimeline() const { return timeline; }

   // True if inputs are enabled
   bool isInputEnabled() const  { return inputFlg; }

   // True if outputs are enabled
   bool isOutputEnabled() const { return outputFlg; }

   // True if relaying is enabled
   bool isRelayEnabled() const  { return (relayFlg && isInputEnabled() && isOutputEnabled()); }

   // Entity filter: Returns max entity ranged (meters)
   virtual double getMaxEntityRange(const Nib* const nib = nullptr) const;

   // Entity filter: Returns max entity ranged squared (meters^2)
   virtual double getMaxEntityRangeSquared(const Nib* const nib = nullptr) const;

   // Dead-Reckoning: Returns max DR time before next 'heart beat' (seconds)
   virtual double getMaxTimeDR(const Nib* const nib = nullptr) const;

   // Dead-Reckoning: Returns max DR position error (meters)
   virtual double getMaxPositionErr(const Nib* const nib = nullptr) const;

   // Dead-Reckoning: Returns max DR orientation error (radians)
   virtual double getMaxOrientationErr(const Nib* const nib = nullptr) const;

   // Dead-Reckoning: Returns max age before a networked player is removed (seconds)
   virtual double getMaxAge(const Nib* const nib = nullptr) const;

   // Network initialization
   bool isNetworkInitialized() const                      { return netInit; }
   bool didInitializationFail() const                     { return netInitFail; }
   virtual bool networkInitialization();

   // Other components
   simulation::Station* getStation()                      { return station; }
   const simulation::Station* getStation() const          { return station; }

   simulation::Simulation* getSimulation()                { return simulation; }
   const simulation::Simulation* getSimulation() const    { return simulation; }

   // Event IDs
   unsigned short getNewIffEventID()                      { return ++iffEventID; }
   unsigned short getNewEmissionEventID()                 { return ++emEventID; }

   // proxy player factory: creates a proxy player based on NIB data
   virtual models::Player* createProxyPlayer(Nib* const nib);

   void reset() override;

protected:
   // Derived class callbacks
   virtual bool initNetwork()=0;                              // Initialize the network (pure functions!)
   virtual void netInputHander()=0;                           // Network input handler (pure functions!)
   virtual void processInputList()=0;                         // Update players/systems from the Input-list (pure functions!)

   // Create output packets from Output-List
   virtual void processOutputList();

   // Set functions
   virtual bool setNetworkID(const unsigned short);           // Sets the Network's ID
   virtual bool setTimeline(const TSource);                   // Sets the timeline (UTC or EXEC)
   virtual bool setMaxTimeDR(const double);                   // Sets the max dead-rec time; forces next update (sec)
   virtual bool setMaxPositionErr(const double);              // Sets the max positional error (meters)
   virtual bool setMaxOrientationErr(const double);           // Sets the max orientation error (rad)
   virtual bool setMaxAge(const double);                      // Sets the max age; for removal (sec)
   virtual bool setMaxEntityRange(const double);              // Sets the max entity range (meters)
   virtual bool setFederateName(const std::string&);          // Sets our federate name
   virtual bool setFederationName(const std::string&);        // Sets our federation name

   bool shutdownNotification() override;

//------------------------------------------------------------------------------
// Network Interface Block (NIB) maintenance sections
//------------------------------------------------------------------------------
public:
    // NIB I/O type
    enum IoType {
       INPUT_NIB,    // NIB is used to map incoming entities to proxy players
       OUTPUT_NIB    // NIB is used to map outgoing players to entities.
    };

public:
   // NIB support
   virtual Nib* findNib(const unsigned short playerID, const std::string& federateName, const IoType);
   virtual Nib* findNib(const models::Player* const, const IoType);
   virtual bool addNibToList(Nib* const, const IoType);
   virtual void removeNibFromList(Nib* const, const IoType);

   // More NIB support
   virtual Nib* createNewInputNib();
   virtual Nib* createNewOutputNib(models::Player* const);
   virtual void destroyInputNib(Nib* const);
   virtual void destroyOutputNib(Nib* const);
   virtual bool addNib2InputList(Nib* const);

protected:
   // Maximum number of active objects
   static const int MAX_OBJECTS{MIXR_CONFIG_MAX_NETIO_ENTITIES};

   // Create NIB unique to protocol (pure functions!)
   virtual Nib* nibFactory(const NetIO::IoType ioType) =0;

   // Create a new Network Interface Block (NIB) for 'player' and insert it
   // in the output list.  Returns a pointer to the new NIB or 0.
   Nib* insertNewOutputNib(models::Player* const player);

   // Number of NIBs on the input list
   unsigned int getInputListSize() const {
      return nInNibs;
   }

   // Returns the idx'th NIB from the input list
   Nib* getInputNib(const int idx) {
      return (idx < nInNibs) ? inputList[idx] : 0;
   }

   // Returns the idx'th NIB from the input list (const version)
   const Nib* getInputNib(const int idx) const  {
      return (idx < nInNibs) ? inputList[idx] : 0;
   }

   // Returns the input list
   Nib** getInputList() {
      return inputList.data();
   }

   // Number of NIBs on the output list
   unsigned int getOutputListSize() const {
      return nOutNibs;
   }

   // Returns the input list
   Nib** getOutputList() {
      return outputList.data();
   }

   // Returns the idx'th NIB from the output list
   Nib* getOutputNib(const int idx) {
      return (idx < nOutNibs) ? outputList[idx] : 0;
   }

   // Returns the idx'th NIB from the output list (const version)
   const Nib* getOutputNib(const int idx) const {
      return (idx < nOutNibs) ? outputList[idx] : 0;
   }


//------------------------------------------------------------------------------
// Network Type Mapper (NTM) maintenance sections
//------------------------------------------------------------------------------
public:

   // Finds the network type mapper by NIB type codes
   virtual const Ntm* findNetworkTypeMapper(const Nib* const nib) const;

   // Finds the network type mapper by Player
   virtual const Ntm* findNetworkTypeMapper(const models::Player* const p) const;


protected:
   virtual bool addOutputEntityType(Ntm* const item);          // Adds an item to the output entity type table
   virtual bool addInputEntityType(Ntm* const item);           // Adds an item to the input entity type table
   virtual bool clearOutputEntityTypes();                      // Clears the output entity type table
   virtual bool clearInputEntityTypes();                       // Clears the input entity type table

   // ---
   // Quick look trees
   // ---
   const NtmOutputNode* getRootNtmOutputNode() const;          // Return the quick look root node for outgoing entity types
   virtual NtmOutputNode* rootNtmOutputNodeFactory() const;    // Outgoing quick look root node factory

   const NtmInputNode* getRootNtmInputNode() const;            // Return the quick look root node for incoming entity types
   virtual NtmInputNode* rootNtmInputNodeFactory() const =0;   // Incoming quick look root node factory

   // ---
   // Raw lists
   // ---
   const Ntm* getOutputEntityTypes(const int) const;           // Return a outgoing entity type by index
   const Ntm* getInputEntityType(const int) const;             // Return a incoming entity type by index
   int getNumOutputEntityTypes() const;                        // Number of input types
   int getNumInputEntityTypes() const;                         // Number of output types

   virtual void testOutputEntityTypes(const int) =0;           // Test rig for outgoing quick lookup
   virtual void testInputEntityTypes(const int) =0;            // Test rig for incoming quick lookup


//------------------------------------------------------------------------------
// Private data
//------------------------------------------------------------------------------
private:
   void updateOutputList();                             // Update the Output-List from the simulation player list (Background thread)
   void cleanupInputList();                             // Clean-up the Input-List (remove out of date items)

   // Network Model IDs
   unsigned short netID{1};                             // Network ID
   std::string federationName;                          // Federation name
   std::string federateName;                            // Federate name

   base::safe_ptr<simulation::Station> station;         // Our station class
   base::safe_ptr<simulation::Simulation> simulation;   // Our simulation class
   TSource timeline {UTC};                              // Source of our timeline
   unsigned short iffEventID{};                         // IFF event ID (as needed)
   unsigned short emEventID{};                          // Emission event ID (as needed)

   // Network Model mode flags
   bool inputFlg{true};      // Network input enabled
   bool outputFlg{true};     // Network output enabled
   bool relayFlg{true};      // Network relay enabled
   bool netInit{};           // Network has been initialized
   bool netInitFail{};       // Initialization attempt failed

   // Distance filter by entity kind/domain
   double maxEntityRange{};   // Max range from ownship           (meters)
   double maxEntityRange2{};  // Max range squared from ownship   (meters^2)

   // Dead Reckoning (DR) parameters by entity kind/domain
   double maxTimeDR{};          // Maximum DR time                  (seconds)
   double maxPositionErr{};     // Maximum position error           (meters)
   double maxOrientationErr{};  // Maximum orientation error        (radians)
   double maxAge{};             // Maximum age of networked players (seconds)

private: // Nib related private
   // input tables
   std::array<Nib*, MAX_OBJECTS> inputList{};  // Table of input objects in name order
   int nInNibs{};                              // Number of input objects in both tables

   // output tables
   std::array<Nib*, MAX_OBJECTS> outputList{}; // Table of output objects in name order
   int nOutNibs{};                             // Number of output objects in both tables

   // NIB quick lookup key
   struct NibKey {
      NibKey(const unsigned short playerId, const std::string& federateName): id(playerId), fName(federateName) {}
      // NIB IDs  -- Comparisons in this order --
      unsigned short id{};     // Player id
      std::string fName;       // Federate name
   };

   // Search callbacks: object name compare function --
   //   True types are (const NibKey* key, const Nib** nib)
   static int compareKey2Nib(const void* key, const void* nib);

private:  // Ntm related private
   static const int MAX_ENTITY_TYPES{MIXR_CONFIG_MAX_NETIO_ENTITY_TYPES};

   NtmInputNode* inputNtmTree{};   // Input NTM quick lookup tree
   NtmOutputNode* outputNtmTree{}; // Output NTM quick lookup tree

   // Input entity type table
   std::array<const Ntm*, MAX_ENTITY_TYPES> inputEntityTypes{};  // Table of pointers to input entity type mappers; Ntm objects
   int nInputEntityTypes {};                                     // Number of input entity mappers (Ntm objects) in the table, 'inputEntityTypes'

   // Output entity type table
   std::array<const Ntm*, MAX_ENTITY_TYPES> outputEntityTypes{};  // Table of pointers to output entity type mappers; Ntm objects
   int nOutputEntityTypes {};                                     // Number of output entity mappers (Ntm objects) in the table, 'outputEntityTypes'

private:
   // slot table helper methods
   virtual bool setSlotFederateName(const base::Identifier* const);    // Sets our federate name
   virtual bool setSlotFederationName(const base::Identifier* const);  // Sets our federation name
   virtual bool setSlotMaxTimeDR(const base::Time* const);             // Sets the mac DR time(s)
   virtual bool setSlotMaxPositionErr(const base::Length* const);      // Sets the max positional error(s)
   virtual bool setSlotMaxOrientationErr(const base::Angle* const);    // Sets the max orientation error(s)
   virtual bool setSlotMaxEntityRange(const base::Length* const);      // Sets the max entity range(s)
   virtual bool setSlotMaxAge(const base::Time* const);                // Sets the max age(s)

   bool setSlotNetworkID(const base::Integer* const);                  // Sets the network ID
   bool setSlotEnableInput(const base::Boolean* const);                // Sets input enabled flag
   bool setSlotEnableOutput(const base::Boolean* const);               // Sets output enabled flag
   bool setSlotEnableRelay(const base::Boolean* const);                // Sets relay enabled flag
   bool setSlotTimeline(const base::Identifier* const);                // Sets the source of the time ( UTC or EXEC )
   bool setSlotInputEntityTypes(base::PairStream* const);              // Sets the table of input entity to player mapper objects
   bool setSlotOutputEntityTypes(base::PairStream* const);             // Sets the table of output entity to player mapper objects
};

}
}

#endif

