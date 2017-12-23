
#ifndef __mixr_models_SpaceVehicle_H__
#define __mixr_models_SpaceVehicle_H__

#include "mixr/models/player/Player.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: SpaceVehicle
// Description: Generic Space Vehicles
// Factory name: SpaceVehicle
//------------------------------------------------------------------------------
class SpaceVehicle : public Player
{
   DECLARE_SUBCLASS(SpaceVehicle, Player)

public:
   SpaceVehicle();

   // Vehicle data
   virtual double getFuelWt() const;     // wt: lbs
   virtual double getFuelWtMax() const;  // wt: lbs

   // Engine data
   virtual int getNumberOfEngines() const;
   virtual int getEngThrust(double* const fn, const int max) const; // Thrust: lbs

   // setControlStickYawInput(const double yaw)
   //    yaw inputs: normalized
   //    yaw:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setControlStickYawInput(const double);

   // setTranslateXInput(const double transx)
   //    transx inputs: normalized
   //    transx:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setTranslateXInput(const double);

   // setTranslateYInput(const double transy)
   //    transy inputs: normalized
   //    transy:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setTranslateYInput(const double);

   // setTranslateZInput(const double transz)
   //    transz inputs: normalized
   //    transz:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
   virtual bool setTranslateZInput(const double);

   double getGrossWeight() const override;
   unsigned int getMajorType() const override;
};

}
}

#endif
