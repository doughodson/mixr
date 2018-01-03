
#ifndef __mixr_ighost_cigi_IgHost_H__
#define __mixr_ighost_cigi_IgHost_H__

#include "mixr/simulation/AbstractIgHost.hpp"
#include <array>

namespace mixr {
namespace base { class Distance; class Identifier; class Number; class PairStream; class String; }
namespace simulation { class AbstractPlayer; }
namespace models { class Player; }
namespace cigi {
class CigiModel;
class Player2CigiMap;

//------------------------------------------------------------------------------
// Class: IgHost
// Description: Abstract base image generator host interface class
//
// Factory name: BaseIgHost
// Slots:
//    maxRange       <Distance>     ! Max range of visual system (default: 20000.0)
//    maxRange       <Number>       ! Max range of visual system (meters)
//
//    maxModels      <Number>       ! Max number of active, in-range player/models (default: 0)
//
//    maxElevations  <Number>       ! Max number of player terrain elevation requests (default: 0)
//
//    typeMap        <PairStream>   ! IG's system model type IDs (list of TypeMapper objects) (default: 0)
//
//------------------------------------------------------------------------------
class IgHost : public simulation::AbstractIgHost
{
   DECLARE_SUBCLASS(IgHost, simulation::AbstractIgHost)

public:
   IgHost();

   // sets our ownship and player list pointers, used by Station class
   void setOwnship(simulation::AbstractPlayer* const) final;
   void setPlayerList(base::PairStream* const) final;      // Sets the player list that we're to use to generate player/models

   void reset() override;

protected:
   // model table types
   enum class TableType {
      MODEL,               // Model table -- maps players to visual system models
      HOT                  // Height-Of-Terrain (HOT) request table
   };

   int getMaxModels() const                              { return maxModels; }      // Max number of active, in-range player/models
   int getMaxElevations() const                          { return maxElevations; }  // Max number of terrain elevation requests

   const models::Player* getOwnship() const              { return ownship; }        // Our ownship -- the player that we're following

   // computes the range (meters) from our ownship to this player.
   double computeRangeToPlayer(const models::Player* const) const;

   // find a player's model object in table 'type' by the player IDs
   CigiModel* findModel(const int playerID, const base::String* const federateName, const TableType type);

   // find a player's model object in table 'type' using a pointer to the player
   CigiModel* findModel(const simulation::AbstractPlayer* const player, const TableType type);

   // add a player's model object to table 'type'
   bool addModelToList(CigiModel* const model, const TableType type);

   // remove a player's model object from table 'type'
   void removeModelFromList(CigiModel* const model, const TableType type);

   // remove model object at index, 'idx', from table 'type'
   void removeModelFromList(const int idx, const TableType type);

   // access to the tables
   int getModelTableSize() const               { return nModels; }
   int getElevationTableSize() const           { return nHots; }
   CigiModel** getModelTable()                 { return modelTbl.data(); }
   CigiModel** getElevationTable()             { return hotTbl.data(); }

private:
   void updateIg(const double dt = 0.0) final;

   bool setMaxRange(const double);                         // Sets the max range (meters)
   bool setMaxModels(const int);                           // Sets the max number of active, in-range player/models
   bool setMaxElevations(const int);                       // Sets the max number of player terrain elevation requests

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
   // Send frame sync (if any)
   virtual void frameSync() =0;

   static const int MAX_MODELS{400};                    // Max model table size
   static const int MAX_MODELS_TYPES{400};              // Max IG model type table size

   void processesModels();                              // Process ownship & player models
   void processesElevations();                          // Process terrain elevation requests

   void resetTables();                                  // Resets the tables
   void clearIgModelTypes();                            // Clear the IG model types table
   void mapPlayerList2ModelTable();                     // Map the player list to the model table
   void mapPlayers2ElevTable();                         // Map player list to terrain elevation table
   CigiModel* newModelEntry(models::Player* const ip);  // Create a new model entry for this player & return the table index
   CigiModel* newElevEntry(models::Player* const ip);   // Create a new elevation entry for this player & return the table index

   // Parameters
   double maxRange{20000.0};                            // Max range of visual system  (meters) (default: 20km)
   int maxModels{};                                     // Max number of models (must be <= MAX_MODELS)
   int maxElevations{};                                 // Max number of terrain elevation requests (default: no requests)

   // Simulation inputs
   models::Player* ownship{};                           // Current ownship
   base::PairStream* playerList{};                      // Current player list
   bool rstReq{};                                       // Reset request flag

   // Model table
   std::array<CigiModel*, MAX_MODELS> modelTbl{};       // The table of models
   int nModels{};                                       // Number of models

   // Height-Of-Terrain request table
   std::array<CigiModel*, MAX_MODELS> hotTbl{};         // Height-Of-Terrain request table
   int nHots{};                                         // Number of HOTs requests

   // Model quick lookup key
   struct ModelKey {
      ModelKey(const int pid, const base::String* const federateName);
      // IgModel IDs  -- Comparisons in this order --
         int playerID;                                    // Player ID
         base::safe_ptr<const base::String> fName;        // Federate name
   };

   // IG model type table
   std::array<const Player2CigiMap*, MAX_MODELS_TYPES> igModelTypes{};   // Table of pointers to IG type mappers
   int nIgModelTypes{};                                                  // Number of type mappers in the table, 'igModelTable'

   // bsearch callbacks: object name compare function --
   //   True types are (const IgModelKey* key, const IgModel** model)
   static int compareKey2Model(const void* key, const void* nib);

private:
   // slot table helper methods
   bool setSlotMaxRange(const base::Distance* const);     // Sets the max range (Distance) slot
   bool setSlotMaxRange(const base::Number* const);       // Sets the max range (meters) slot
   bool setSlotMaxModels(const base::Number* const);      // Sets the max number of active, in-range player/models slot
   bool setSlotMaxElevations(const base::Number* const);  // Sets the max number of player terrain elevation requests slot
   bool setSlotTypeMap(const base::PairStream* const);    // Sets the list of IG model type IDs (TypeMapper objects)
};

}
}

#endif
