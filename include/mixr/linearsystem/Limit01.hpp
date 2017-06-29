
#ifndef __mixr_linearsystem_Limit01_H__
#define __mixr_linearsystem_Limit01_H__

#include "mixr/linearsystem/LimitFunc.hpp"

namespace mixr {
namespace linearsystem {

//------------------------------------------------------------------------------
// Class: Limit01
// Description: Limits between 0 and 1, inclusive
//
//------------------------------------------------------------------------------
class Limit01 : public LimitFunc
{
   DECLARE_SUBCLASS(Limit01, LimitFunc)

public:
   Limit01();
};

}
}

#endif
