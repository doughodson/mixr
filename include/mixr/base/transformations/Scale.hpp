
#ifndef __mixr_base_Scale_H__
#define __mixr_base_Scale_H__

#include "mixr/base/transformations/Transform.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Scale
// Description: Coordinate Transformations (e.g., scale)
//------------------------------------------------------------------------------
// Factory name: Scale
//------------------------------------------------------------------------------
// Slots:
//    1. x  (Number)  x scaling factor
//    2. y  (Number)  y scaling factor (optional)
//    3. z  (Number)  z scaling factor (optional)
//------------------------------------------------------------------------------
// Notes:
//    One parameter: Scale both X and Y by a factor of "x".
//    Two parameters: Scale X by a factor of "x" and Y by a factor of "y".
//    Three Parameters: Scale X, Y and Z by factors of "x", "y" and "z" respectively.
//------------------------------------------------------------------------------
class Scale final: public Transform
{
   DECLARE_SUBCLASS(Scale, Transform)
public:
   Scale();
private:
   void computeMatrix() final;
};

}
}

#endif
