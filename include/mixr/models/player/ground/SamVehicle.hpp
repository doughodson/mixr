
#ifndef __mixr_models_SamVehicle_HPP__
#define __mixr_models_SamVehicle_HPP__

#include "mixr/models/player/ground/GroundVehicle.hpp"

namespace mixr {
namespace base { class ILength; }
namespace models {

//------------------------------------------------------------------------------
// Class: SamVehicle
// Description: Sam vehicle (TEL, TELAR, etc)
//
// Factory name: SamVehicle
// Slots:
//    minLaunchRange    <base::Length>     ! Min launch range (base::Length)
//                                         ! (default: DEFAULT_MIN_LAUNCH_RANGE)
//    maxLaunchRange    <base::Length>     ! Max launch range (base::Length)
//                                         ! (default: DEFAULT_MAX_LAUNCH_RANGE)
//
//------------------------------------------------------------------------------
class SamVehicle : public GroundVehicle
{
   DECLARE_SUBCLASS(SamVehicle, GroundVehicle)

public:
   SamVehicle();

   // Access functions
   virtual unsigned int getNumberOfMissiles() const;  // Number of missiles

   virtual double getMaxLaunchRange() const;          // Max missile launch range (meters)
   virtual double getMinLaunchRange() const;          // Min missile launch range (meters)

   virtual bool isLauncherReady() const;              // Return true if launcher is ready to fire.

   // Set functions
   virtual bool setMaxLaunchRange(const double rng);
   virtual bool setMinLaunchRange(const double rng);

   void updateData(const double dt = 0.0) override;

private:
   double       maxMslRng {};        // Max Launch Range        (meters)
   double       minMslRng {};        // Min Launch Range        (meters)
   unsigned int numMsl {};           // Number of missile available

private:
   // slot table helper methods
   bool setSlotMinLaunchRange(const base::ILength* const);
   bool setSlotMaxLaunchRange(const base::ILength* const);
};

}
}

#endif
