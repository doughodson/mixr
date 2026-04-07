
#ifndef __mixr_linearsystem_Limit01_HPP__
#define __mixr_linearsystem_Limit01_HPP__

#include "mixr/base/linearsystem/ILimitFunc.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Limit01
// Description: Limits between 0 and 1, inclusive
//
//------------------------------------------------------------------------------
class Limit01 final: public ILimitFunc
{
   DECLARE_SUBCLASS(Limit01, ILimitFunc)

public:
   Limit01();
};

}
}

#endif
