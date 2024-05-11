
#ifndef __mixr_models_common_IrSystem_HPP__
#define __mixr_models_common_IrSystem_HPP__

#include "mixr/models/common/system/System.hpp"

#include <string>

namespace mixr {
namespace base { class Boolean; class Identifier; }
namespace models {
class IrSeeker;
class IrQueryMsg;
class OnboardComputer;

//------------------------------------------------------------------------------
// Class: IrSystem
//
// Description: Generic class for IR type systems.  Provides a common
//              interface, and some default behavior, for IR systems.
//
// Notes
//    1) By convention, IR systems are responsible for filtering their own players
//    of interest, so there is the processPlayersOfInterest() function, which is
//    called by our updateData(), that can be used by our derived classes to filter
//    players, as needed.  The Gimbal class, which is used to create antennas and
//    seekers, has its own member function processPlayersOfInterest() that filters
//    by gimbal field of view and player type.  It's the responsibility of the
//    various IR systems to use or not use this gimbal function -- our default
//    member function processPlayersOfInterest() does use the gimbal function.
//
// Factory name: IrSystem
// Slots:
//    seekerName     <Identifier>  ! Name of the requested Seeker
//    disableQueries <Boolean>     ! Disable sending queries packets flag (default: false)
//
//------------------------------------------------------------------------------
class IrSystem : public System
{
   DECLARE_SUBCLASS(IrSystem, System)

public:
   IrSystem();

   virtual bool isQuerying() const;

   virtual bool areQueriesDisabled() const;              // Returns true if sending query packets has been disabled
   virtual bool setDisableQueriesFlag(const bool);       // Disables/enables sending the I/R queries packets

   virtual IrSeeker* getSeeker();                        // Get the seeker model, or zero (0) if none
   virtual const IrSeeker* getSeeker() const;            // Get the seeker model (const version)
   virtual bool setSeeker(IrSeeker* const);              // Sets the IR's seeker model

   virtual const std::string& getSeekerName() const;     // name of our seeker

   void updateData(const double dt = 0.0) override;
   void reset() override;

protected:

   // Process players of interest -- Called by our updateData() -- the background thread --
   // This function will create a filtered list of players that IR systems will interact with.
   virtual void processPlayersOfInterest();

   bool shutdownNotification() override;

private:
   IrSeeker* seeker{};        // Our seeker
   std::string seekerName;    // Name of our seeker

   bool disableQueries{};     // Disable sending queries flag

private:
   // slot table helper methods
   bool setSlotSeekerName(base::Identifier* const);
   bool setSlotDisableQueries(base::Boolean* const);
};

}
}

#endif
