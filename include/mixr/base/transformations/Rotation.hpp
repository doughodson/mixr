
#ifndef __mixr_base_Rotation_HPP__
#define __mixr_base_Rotation_HPP__

#include "mixr/base/transformations/Transform.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Rotation
// Description: Coordinate Transformations (e.g., rotate)
//------------------------------------------------------------------------------
// Factory name: Rotation
//------------------------------------------------------------------------------
// Slots:
//    1. x  (Number)  x component of vector (optional)
//    2. y  (Number)  y component of vector (optional)
//    3. z  (Number)  z component of vector (optional)
//    4. w  (Angle)   radians to rotate about vector
//------------------------------------------------------------------------------
// Notes:
//    One parameter: Rotate "w" radians about the Z axis.
//    Four parameters: Rotate "w" radians about the vector (x,y,z).
//------------------------------------------------------------------------------
class Rotation final: public Transform
{
   DECLARE_SUBCLASS(Rotation, Transform)
public:
   Rotation();
private:
   void computeMatrix() final;
};

}
}

#endif
