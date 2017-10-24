
#ifndef __mixr_linearsystem_Limit_H__
#define __mixr_linearsystem_Limit_H__

#include "mixr/linearsystem/LimitFunc.hpp"

namespace mixr {
namespace linearsystem {

//------------------------------------------------------------------------------
// Class: Limit
//
// Description: Limits function: limits X to the upper and lower boundaries.
//
// Factory name: Limit
// Slots:
//    lower  <Number>   Lower boundary (default: -1) (must be less than the upper)
//    upper  <Number>   Upper boundary (default:  1) (must be greater than the lower)
//
//------------------------------------------------------------------------------
class Limit : public LimitFunc
{
   DECLARE_SUBCLASS(Limit, LimitFunc)

public:
   Limit();
   Limit(const double lower, const double upper);

private:
   // slot table helper methods
   bool setSlotLowerLimit(const base::Number* const);
   bool setSlotUpperLimit(const base::Number* const);
};

}
}

#endif
