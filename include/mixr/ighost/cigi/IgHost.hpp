
#ifndef __mixr_ighost_cigi_IgHost_H__
#define __mixr_ighost_cigi_IgHost_H__

#include "mixr/simulation/AbstractIgHost.hpp"
#include <array>

namespace mixr {
namespace base { class Distance; class Identifier; class Number; class PairStream; class String; }
namespace models { class Player; }
namespace cigi {
class CigiModel;
class TypeMapper;

//------------------------------------------------------------------------------
// Class: IgHost
// Description: Abstract base image generator host interface class
//
// Notes:
//    Derived versions of this class are used to interface with various
//    visual systems.
//
// Factory name: BaseIgHost
// Slots:
//    maxRange       <Distance>     ! Max range of visual system (default: 20000.0f)
//    maxRange       <Number>       ! Max range of visual system (meters)
//
//    maxModels      <Number>       ! Max number of active, in-range player/models (default: 0)
//
//    maxElevations  <Number>       ! Max number of player terrain elevation requests (default: 0)
//
//    latitude       <Number>       ! Visual database reference latitude (deg) (default: 0.0)
//
//    longitude      <Number>       ! Visual database reference longitude (deg) (default: 0.0)
//
//    typeMap        <PairStream>   ! IG's system model type IDs (list of TypeMapper objects) (default: 0)
//
//------------------------------------------------------------------------------
class IgHost : public simulation::AbstractIgHost
{
    DECLARE_SUBCLASS(IgHost, simulation::AbstractIgHost)

public:
    // Model table types
    enum TableType {
       MODEL_TABLE,     // Model table -- maps players to visual system models
       HOT_TABLE        // Height-Of-Terrain request table
    };

public:
    IgHost();

    const models::Player* getOwnship() const              { return ownship; }        // Our ownship -- the player that we're following
    int getMaxModels() const                              { return maxModels; }      // Max number of active, in-range player/models
    int getMaxElevations() const                          { return maxElevations; }  // Max number of terrain elevation requests
    double getMaxRange() const                            { return maxRange; }       // Max range of active player/models
    double getRefLatitude() const                         { return refLat; }         // Visual database reference latitude  (degs)
    double getRefLongitude() const                        { return refLon; }         // Visual database reference longitude (degs)
    virtual bool isResetInProgress() const;                                          // True if visual system is resetting

    virtual bool setRefLatitude(const double);              // Sets the visual database reference latitude  (degs)
    virtual bool setRefLongitude(const double);             // Sets the visual database reference longitude (degs)
    void setPlayerList(base::PairStream* const) override;   // Sets the player list that we're to use to generate player/models

    bool setMaxRange(const double);                         // Sets the max range (meters)
    bool setMaxModels(const int);                           // Sets the max number of active, in-range player/models
    bool setMaxElevations(const int);                       // Sets the max number of player terrain elevation requests

    // Sets our ownship pointer; public version, which is usually called by the Station class.  Derived classes
    // can override this function and control the switching of the ownship using setOwnship0()
    void setOwnship(simulation::AbstractPlayer* const) override;

    void reset() override;

protected:
    virtual void setOwnship0(models::Player* const);           // Sets our ownship player

    // Computers the range (meters) from our ownship to this player.
    virtual double computeRangeToPlayer(const models::Player* const) const;

    // Find a player's model object in table 'type' by the player IDs
    virtual CigiModel* findModel(const unsigned short playerID, const base::String* const federateName, const TableType type);

    // Find a player's model object in table 'type' using a pointer to the player
    virtual CigiModel* findModel(const simulation::AbstractPlayer* const player, const TableType type);

    // Add a player's model object to table 'type'
    virtual bool addModelToList(CigiModel* const model, const TableType type);

    // Remove a player's model object from table 'type'
    virtual void removeModelFromList(CigiModel* const model, const TableType type);

    // Remove model object at index, 'idx', from table 'type'
    virtual void removeModelFromList(const int idx, const TableType type);

    // Derived class' access to the tables
    int getModelTableSize() const               { return nModels; }
    int getElevationTableSize() const           { return nHots; }
    CigiModel** getModelTable()                 { return modelTbl.data(); }
    CigiModel** getElevationTable()             { return hotTbl.data(); }

    // Create Cigi model objects to manage player/models
    virtual CigiModel* modelFactory() =0;

    // Create Cigi model objects to manage player terrain elevation requests
    virtual CigiModel* hotFactory() =0;

    // Manages the sending of ownship and player/model state data
    virtual void sendOwnshipAndModels() =0;

    // Manages the sending of player terrain elevation requests
    virtual void sendElevationRequests() =0;

    // Handles received player terrain elevation data
    virtual void recvElevations() =0;

private:
    void updateIg(const double dt = 0.0) override;

    static const int MAX_MODELS{400};                    // Max model table size
    static const int MAX_MODELS_TYPES{400};              // Max IG model type table size

    void processesModels();                              // Process ownship & player models
    void processesElevations();                          // Process terrain elevation requests

    // Send frame sync (if any)
    virtual void frameSync() =0;

    void resetTables();                                  // Resets the tables
    void clearModelTypes();                              // Clear the IG model types table
    void mapPlayerList2ModelTable();                     // Map the player list to the model table
    void mapPlayers2ElevTable();                         // Map player list to terrain elevation table
    CigiModel* newModelEntry(models::Player* const ip);  // Create a new model entry for this player & return the table index
    CigiModel* newElevEntry(models::Player* const ip);   // Create a new elevation entry for this player & return the table index

    // Parameters
    double maxRange{20000.0};               // Max range of visual system  (meters) (default: 20km)
    int maxModels{};                        // Max number of models (must be <= MAX_MODELS)
    int maxElevations{};                    // Max number of terrain elevation requests (default: no requests)

    // Ref position
    double refLat{};                        // Visual database reference latitude (deg)
    double refLon{};                        // Visual database reference longitude (deg)

    // Simulation inputs
    models::Player* ownship{};              // Current ownship
    base::PairStream* playerList{};         // Current player list
    bool rstFlg{};                          // Reset in progress
    bool rstReq{};                          // Reset request flag

    // Model table
    std::array<CigiModel*, MAX_MODELS> modelTbl{};    // The table of models
    int nModels{};                                    // Number of models

    // Height-Of-Terrain request table
    std::array<CigiModel*, MAX_MODELS> hotTbl{};      // Height-Of-Terrain request table
    int nHots{};                                      // Number of HOTs requests

    // Model quick lookup key
    struct ModelKey {
        ModelKey(const unsigned short pid, const base::String* const federateName);
        // IgModel IDs  -- Comparisons in this order --
        unsigned short playerID;                    // Player ID
        base::safe_ptr<const base::String> fName;   // Federate name
    };

    // IG model type table
    std::array<const TypeMapper*, MAX_MODELS_TYPES> otwModelTypes{}; // Table of pointers to IG type mappers
    int nOtwModelTypes{};                                            // Number of type mappers in the table, 'otwModelTable'

    // bsearch callbacks: object name compare function --
    //   True types are (const IgModelKey* key, const IgModel** model)
    static int compareKey2Model(const void* key, const void* nib);

private:
    // slot table helper methods
    bool setSlotMaxRange(const base::Distance* const);     // Sets the max range (Distance) slot
    bool setSlotMaxRange(const base::Number* const);       // Sets the max range (meters) slot
    bool setSlotMaxModels(const base::Number* const);      // Sets the max number of active, in-range player/models slot
    bool setSlotMaxElevations(const base::Number* const);  // Sets the max number of player terrain elevation requests slot
    bool setSlotRefLatitude(const base::Number* const);    // Sets the visual database reference latitude  (degs) slot
    bool setSlotRefLongitude(const base::Number* const);   // Sets the visual database reference longitude (degs) slot
    bool setSlotTypeMap(const base::PairStream* const);    // Sets the list of IG model type IDs (TypeMapper objects)
};

}
}

#endif
