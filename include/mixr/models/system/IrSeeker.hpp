
#ifndef __mixr_models_common_IrSeeker_HPP__
#define __mixr_models_common_IrSeeker_HPP__

#include "mixr/models/system/ScanGimbal.hpp"

#include "mixr/models/Tdb.hpp"

#include "mixr/base/safe_queue.hpp"
#include "mixr/base/safe_stack.hpp"

//#define USE_TDBIR

namespace mixr {
namespace base { class PairStream; }
namespace models {
class Player;
class IrSystem;
class IrSensor;
class IrQueryMsg;

//------------------------------------------------------------------------------
// Class: IrSeeker
//
// Description: Simple IR seeker model
//
// Factory name: IrSeeker
//------------------------------------------------------------------------------
class IrSeeker : public ScanGimbal
{
   DECLARE_SUBCLASS(IrSeeker, ScanGimbal)

public:
   IrSeeker();

   virtual void irRequestSignature(IrQueryMsg* const);
   virtual bool irQueryReturnEvent(IrQueryMsg* const);

   // System limits
   int getMaxQueries() const                 { return MAX_QUERIES; }

#ifdef USE_TDBIR
   // FAB - was missing, but needed, since IrSeeker uses TdbIr; copied in from v2009_0204
   // Gimbal Interface
   virtual unsigned int processPlayersOfInterest(base::PairStream* const poi);
#endif

   bool event(const int event, base::Object* const obj = nullptr) override;
   void reset() override;

protected:
   void clearQueues();

   void process(const double dt) override;

   bool shutdownNotification() override;

   base::safe_stack<IrQueryMsg*> freeQueryStack {MAX_QUERIES};   // stack of free queries of target IR signatures
   mutable long freeQueryLock {};                                // Semaphore to protect 'freeQueryStack'

   base::safe_queue<IrQueryMsg*> inUseQueryQueue {MAX_QUERIES};  // Queue of in use queries of target IR signatures
   mutable long inUseQueryLock {};                               // Semaphore to protect 'inUseQueryQueue'

private:
   static const int MAX_QUERIES{10000};                          // Max size of queues and arrays
};

#ifdef USE_TDBIR
//------------------------------------------------------------------------------
// Class: TdbIr
//
// Description: Target(s) Data Block for an IrSeeker (see Tdb in Gimbal.hpp)
//------------------------------------------------------------------------------
class TdbIr : public Tdb
{
   DECLARE_SUBCLASS(TdbIr, Tdb)

public:
   TdbIr() = delete;
   TdbIr(const unsigned int maxTargets, const Gimbal* const gimbal);

   //------------------------------------------------------------------------------
   // Process players-of-interest --- Scan the provided player list and compute range,
   // range rate, normalized Line-Of-Sight (LOS) vectors for each target player.
   // (Background task)
   //------------------------------------------------------------------------------
   virtual unsigned int processPlayers(base::PairStream* const players);

   static bool horizonCheck(const base::Vec3d& position1, const base::Vec3d& position2);
};
#endif

}
}

#endif
