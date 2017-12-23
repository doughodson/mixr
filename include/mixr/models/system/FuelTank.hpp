
#ifndef __mixr_models_FuelTank_H__
#define __mixr_models_FuelTank_H__

#include "mixr/models/system/ExternalStore.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: FuelTank
//
// Description: Generic fuel tank
//
// Factory name: FuelTank
// Slots:
//    fuelWt      <Number>  ! Fuel wt (lb) (default: -1)
//    capacity    <Number>  ! Tank capacity (lb) (default: -1)
//
//------------------------------------------------------------------------------
class FuelTank : public ExternalStore
{
   DECLARE_SUBCLASS(FuelTank, ExternalStore)

public:
   FuelTank();

   // Tank capacity (lb)
   virtual double getCapacity() const;
   virtual bool setCapacity(const double);
   virtual bool isCapacityValid() const;

   // Fuel weight (lb)
   virtual double getFuelWt() const;
   virtual bool setFuelWt(const double);
   virtual bool isFuelWtValid() const;

   void reset() override;

private:
   double capacity {-1.0};      // Fuel capacity (lb)
   double fuelWt {-1.0};        // Fuel contents (lb)
   double initFuelWt {-1.0};    // Fuel contents at reset (lb)

private:
   // slot table helper methods
   bool setSlotFuelWt(const base::Number* const);
   bool setSlotCapacity(const base::Number* const);
};

}
}

#endif
