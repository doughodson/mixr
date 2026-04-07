
#ifndef __mixr_linearsystem_FirstOrderTf_HPP__
#define __mixr_linearsystem_FirstOrderTf_HPP__

#include "mixr/base/linearsystem/IFirstOrderTf.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class:  FirstOrderTf
//
// Base class:  base::IObject -> IScalerFunc-> IDiffEquation -> IFirstOrderTf -> FirstOrderTf
//
// Description: Models a first order continuous transfer function in the discrete
//              z-domain using a difference equation with Tustin coefficients.
//
// Transfer function:
//       N1*s + N2
//       ---------
//       D1*s + D2
//
//------------------------------------------------------------------------------
class FirstOrderTf final: public IFirstOrderTf
{
   DECLARE_SUBCLASS(FirstOrderTf, IFirstOrderTf)

public:
   FirstOrderTf();
};

}
}

#endif
