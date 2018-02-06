
#ifndef __mixr_base_Newtons_H__
#define __mixr_base_Newtons_H__

#include "mixr/base/units/force/Force.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Newtons
// Description: Concrete class to specify a force in Newtons
//------------------------------------------------------------------------------
class Newtons final: public Force
{
   DECLARE_SUBCLASS(Newtons, Force)

public:
   explicit Newtons();

private:
   // convert Newtons to Newtons (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
