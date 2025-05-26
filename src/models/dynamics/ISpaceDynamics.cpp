
#include "mixr/models/dynamics/ISpaceDynamics.hpp"
#include <iostream>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(ISpaceDynamics, "ISpaceDynamics")
EMPTY_SLOTTABLE(ISpaceDynamics)
EMPTY_COPYDATA(ISpaceDynamics)
EMPTY_DELETEDATA(ISpaceDynamics)

ISpaceDynamics::ISpaceDynamics()
{
   STANDARD_CONSTRUCTOR()
}

//------------------------------------------------------------------------------
// setControlStickYawInput(yaw) --  Control inputs: normalized
//   yaw:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool ISpaceDynamics::setControlStickYawInput(const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setTranslateXStickInput(transx) --  Control inputs: normalized
//   transx:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool ISpaceDynamics::setTranslateXInput(const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setTranslateYStickInput(transy) --  Control inputs: normalized
//   transy:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool ISpaceDynamics::setTranslateYInput(const double)
{
   return false;
}

//------------------------------------------------------------------------------
// setTranslateZStickInput(transz) --  Control inputs: normalized
//   transz:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool ISpaceDynamics::setTranslateZInput(const double)
{
   return false;
}

}
}
