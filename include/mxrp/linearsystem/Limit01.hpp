
#ifndef __mxrp_linearsystem_Limit01_H__
#define __mxrp_linearsystem_Limit01_H__

#include "mxrp/linearsystem/LimitFunc.hpp"

namespace oe {
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
