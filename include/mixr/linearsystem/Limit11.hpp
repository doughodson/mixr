
#ifndef __mixr_linearsystem_Limit11_H__
#define __mixr_linearsystem_Limit11_H__

#include "mixr/linearsystem/LimitFunc.hpp"

namespace mixr {
namespace linearsystem {

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
