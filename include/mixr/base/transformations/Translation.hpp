
#ifndef __mixr_base_Translation_H__
#define __mixr_base_Translation_H__

#include "mixr/base/transformations/Transform.hpp"

namespace mixr {
namespace base {
class Angle;
class Number;

//------------------------------------------------------------------------------
// Class: Translation
// Description: Coordinate Transformations (e.g., translate)
//------------------------------------------------------------------------------
// Factory name: Translation
//------------------------------------------------------------------------------
// Notes:
//    Two Parameters: Translate X by a factor of "x" and Y by a factor of "y".
//    Three Parameters: Translate X, Y and Z by factors of "x", "y" and "z" respectively.
//
//    1. x  (Number)  x translation.
//    2. y  (Number)  y translation.
//    3. z  (Number)  z translation (optional).
//------------------------------------------------------------------------------
class Translation final: public Transform
{
   DECLARE_SUBCLASS(Translation, Transform)
public:
   Translation();

private:
   void computeMatrix() final;
};

}
}

#endif
