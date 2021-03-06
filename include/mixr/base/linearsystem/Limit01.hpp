
#ifndef __mixr_linearsystem_Limit01_HPP__
#define __mixr_linearsystem_Limit01_HPP__

#include "mixr/base/linearsystem/LimitFunc.hpp"

namespace mixr {
namespace base {

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
