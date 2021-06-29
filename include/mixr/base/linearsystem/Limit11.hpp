
#ifndef __mixr_linearsystem_Limit11_HPP__
#define __mixr_linearsystem_Limit11_HPP__

#include "mixr/base/linearsystem/LimitFunc.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Limit11
// Base class:  Object -> ScalerFunc -> LimitFunc -> Limit11
//
// Description: Limits between -1 and 1, inclusive
//
//------------------------------------------------------------------------------
class Limit11 : public LimitFunc
{
   DECLARE_SUBCLASS(Limit11, LimitFunc)

public:
   Limit11();
};

}
}

#endif
