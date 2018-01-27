
#ifndef __mixr_base_Unit_H__
#define __mixr_base_Unit_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base {
class Number;

//------------------------------------------------------------------------------
// Class: Unit
// Description: Abstract base class for all units.  Provides storage for a
//              double precision value and a slot method to set it.
//
// Factory name: AbstractUnit
//------------------------------------------------------------------------------
class Unit : public Object
{
    DECLARE_SUBCLASS(Unit, Object)

public:
    Unit();
    Unit(const double);

    void setValue(const double x)     { val = x;    }
    double getValue() const           { return val; }

private:
    double val{};

private:
    // slot table helper methods
    bool setSlotValue(const Number* const);
};

}
}

#endif

