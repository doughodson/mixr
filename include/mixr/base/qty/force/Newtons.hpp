
#ifndef __mixr_base_Newtons_HPP__
#define __mixr_base_Newtons_HPP__

#include "mixr/base/qty/force/IForce.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Newtons
// Description: Concrete class to specify a force in Newtons
//------------------------------------------------------------------------------
class Newtons final: public IForce
{
   DECLARE_SUBCLASS(Newtons, IForce)

public:
   explicit Newtons();

private:
   // convert Newtons to Newtons (our base unit)
   double convertToBaseUnit(const double x) final     { return x; }
};

}
}

#endif
