
#include "mixr/models/dynamics/IDynamicsSpace.hpp"
#include <iostream>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IDynamicsSpace, "IDynamicsSpace")
EMPTY_SLOTTABLE(IDynamicsSpace)
EMPTY_COPYDATA(IDynamicsSpace)
EMPTY_DELETEDATA(IDynamicsSpace)

IDynamicsSpace::IDynamicsSpace()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// setControlStickYawInput(yaw) --  Control inputs: normalized
//   yaw:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool IDynamicsSpace::setControlStickYawInput(const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setTranslateXStickInput(transx) --  Control inputs: normalized
//   transx:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool IDynamicsSpace::setTranslateXInput(const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setTranslateYStickInput(transy) --  Control inputs: normalized
//   transy:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool IDynamicsSpace::setTranslateYInput(const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setTranslateZStickInput(transz) --  Control inputs: normalized
//   transz:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool IDynamicsSpace::setTranslateZInput(const double)
{
   return false;
}

}
}
