
#include "mixr/models/system/FuelTank.hpp"

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(FuelTank, "FuelTank")

BEGIN_SLOTTABLE(FuelTank)
    "fuelWt",     // 1: Fuel wt (lb)
    "capacity",   // 2: Tank capacity (lb)
END_SLOTTABLE(FuelTank)

BEGIN_SLOT_MAP(FuelTank)
    ON_SLOT(1, setSlotFuelWt,   base::Number)
    ON_SLOT(2, setSlotCapacity, base::Number)
END_SLOT_MAP()

FuelTank::FuelTank()
{
    STANDARD_CONSTRUCTOR()
}

void FuelTank::copyData(const FuelTank& org, const bool)
{
    BaseClass::copyData(org);

    capacity   = org.capacity;
    fuelWt     = org.fuelWt;
    initFuelWt = org.initFuelWt;
}

EMPTY_DELETEDATA(FuelTank)

//------------------------------------------------------------------------------
// revert to our initial fuel weight
//------------------------------------------------------------------------------
void FuelTank::reset()
{
    BaseClass::reset();

    fuelWt = initFuelWt;
}

//------------------------------------------------------------------------------
// Tank capacity (lb) functions
//------------------------------------------------------------------------------
double FuelTank::getCapacity() const            { return capacity; }
bool   FuelTank::setCapacity(const double cap)  { capacity = cap; return true; }
bool   FuelTank::isCapacityValid() const        { return (capacity >= 0); }

//------------------------------------------------------------------------------
// Fuel weight (lb) functions
//------------------------------------------------------------------------------
double FuelTank::getFuelWt() const              { return fuelWt;        }
bool   FuelTank::setFuelWt(const double wt)     { fuelWt = wt; return true; }
bool   FuelTank::isFuelWtValid() const          { return (fuelWt >= 0 && fuelWt <= capacity); }

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool FuelTank::setSlotFuelWt(const base::Number* const msg)
{
    if (msg == nullptr) return false;
    bool ok{setFuelWt(msg->getReal())};
    if (ok) initFuelWt = getFuelWt();
    return ok;
}

bool FuelTank::setSlotCapacity(const base::Number* const msg)
{
    if (msg == nullptr) return false;
    return setCapacity( msg->getReal() );
}

}
}

