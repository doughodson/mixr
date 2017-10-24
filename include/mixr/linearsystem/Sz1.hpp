
#ifndef __mixr_linearsystem_Sz1_H__
#define __mixr_linearsystem_Sz1_H__

#include "mixr/linearsystem/FirstOrderTf.hpp"

namespace mixr {
namespace linearsystem {

//------------------------------------------------------------------------------
// Class:  Sz1
//
// Description: Models a first order continuous transfer function (see FirstOrderTf)
//
// Transfer function:
//       N1*s + N2
//       ---------
//       D1*s + D2
//
// Factory name: Sz1
// Slots:
//    N1    <Number>     Coefficient N1 (default: 0)
//    n1    <Number>     Coefficient N1 (default: 0)
//
//    N2    <Number>     Coefficient N2 (default: 0)
//    n2    <Number>     Coefficient N2 (default: 0)
//
//    D1    <Number>     Coefficient D1 (default: 0)
//    d1    <Number>     Coefficient D1 (default: 0)
//
//    D2    <Number>     Coefficient D2 (default: 0)
//    d1    <Number>     Coefficient D2 (default: 0)
//
//------------------------------------------------------------------------------
class Sz1 : public FirstOrderTf
{
    DECLARE_SUBCLASS(Sz1, FirstOrderTf)

public:
   Sz1();

private:
   // slot table helper methods
   bool setSlotN1(const base::Number* const);
   bool setSlotN2(const base::Number* const);
   bool setSlotD1(const base::Number* const);
   bool setSlotD2(const base::Number* const);
};

}
}

#endif
