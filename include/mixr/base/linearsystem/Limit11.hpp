
#ifndef __mixr_linearsystem_Limit11_HPP__
#define __mixr_linearsystem_Limit11_HPP__

#include "mixr/base/linearsystem/ILimitFunc.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Limit11
// Base class:  Object -> ScalerFunc -> ILimitFunc -> Limit11
//
// Description: Limits between -1 and 1, inclusive
//
//------------------------------------------------------------------------------
class Limit11 : public ILimitFunc
{
   DECLARE_SUBCLASS(Limit11, ILimitFunc)

public:
   Limit11();
};

}
}

#endif
