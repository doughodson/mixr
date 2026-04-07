
#ifndef __mixr_linearsystem_SecondOrderTf_HPP__
#define __mixr_linearsystem_SecondOrderTf_HPP__

#include "mixr/base/linearsystem/ISecondOrderTf.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SecondOrderTf
// ##### IN-WORK #####
//
// Description: Models a second order continuous transfer function in the discrete
//              z-domain using a difference equation with Tustin coefficients.
//
// Transfer function:
//       N1*s*s + N2*s + N3
//       ------------------
//       D1*s*s + D2*s + D3
//
//------------------------------------------------------------------------------
class SecondOrderTf final: public ISecondOrderTf
{
   DECLARE_SUBCLASS(SecondOrderTf, ISecondOrderTf)

public:
   SecondOrderTf();
};

}
}

#endif
