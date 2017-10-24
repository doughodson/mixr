
#include "mixr/models/player/space/SpaceVehicle.hpp"
#include "mixr/models/dynamics/SpaceDynamicsModel.hpp"

#include "mixr/base/units/Distances.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(SpaceVehicle, "SpaceVehicle")
EMPTY_SLOTTABLE(SpaceVehicle)
EMPTY_COPYDATA(SpaceVehicle)
EMPTY_DELETEDATA(SpaceVehicle)

SpaceVehicle::SpaceVehicle()
{
   STANDARD_CONSTRUCTOR()

   static base::String generic("GenericSpaceVehicle");
   setType(&generic);
}

//-----------------------------------------------------------------------------
// getMajorType() -- Returns the player's major type
//-----------------------------------------------------------------------------
unsigned int SpaceVehicle::getMajorType() const
{
   return SPACE_VEHICLE;
}

//------------------------------------------------------------------------------
// Get Vehicle data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------

double SpaceVehicle::getFuelWt() const
{
   double value{};
   if (getDynamicsModel() != nullptr) {
      value = getDynamicsModel()->getFuelWt();
   }
   return value;
}

double SpaceVehicle::getFuelWtMax() const
{
   double value{};
   if (getDynamicsModel() != nullptr) {
      value = getDynamicsModel()->getFuelWtMax();
   }
   return value;
}

double SpaceVehicle::getGrossWeight() const
{
   double value{};
   if (getDynamicsModel() != nullptr) {
      value = getDynamicsModel()->getGrossWeight();
   }
   return value;
}

//------------------------------------------------------------------------------
// setControlStickYawInput(yaw) -- Yaw inputs: normalized
//          yaw:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool SpaceVehicle::setControlStickYawInput(const double yaw)
{
   bool ok{};
   const auto model = dynamic_cast<SpaceDynamicsModel*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setControlStickYawInput(yaw);
   }
   return ok;
}

//------------------------------------------------------------------------------
// setTranslateXInput(transx) -- Translate X inputs: normalized
//          transx:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool SpaceVehicle::setTranslateXInput(const double transx)
{
   bool ok{};
   const auto model = dynamic_cast<SpaceDynamicsModel*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setTranslateXInput(transx);
   }
   return ok;
}

//------------------------------------------------------------------------------
// setTranslateYInput(transy) -- Translate Y inputs: normalized
//          transy:  -1.0 -> max left;  0.0 -> center;  1.0 -> max right
//------------------------------------------------------------------------------
bool SpaceVehicle::setTranslateYInput(const double transy)
{
   bool ok{};
   const auto model = dynamic_cast<SpaceDynamicsModel*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setTranslateYInput(transy);
   }
   return ok;
}

//------------------------------------------------------------------------------
// setTranslateZInput(transz) -- Translate Z inputs: normalized
//          transz:  -1.0 -> maz left;  0.0 -> center;  1.0 -> maz right
//------------------------------------------------------------------------------
bool SpaceVehicle::setTranslateZInput(const double transz)
{
   bool ok{};
   const auto model = dynamic_cast<SpaceDynamicsModel*>(getDynamicsModel());
   if (model != nullptr) {
      ok = model->setTranslateZInput(transz);
   }
   return ok;
}

//------------------------------------------------------------------------------
// Get engine data: num engines, thrust, rpm, pla and fuel flow
//------------------------------------------------------------------------------
int SpaceVehicle::getNumberOfEngines() const
{
   int n{};
   if (getDynamicsModel() != nullptr) {
      n = getDynamicsModel()->getNumberOfEngines();
   }
   return n;
}

int SpaceVehicle::getEngThrust(double* const data, const int max) const
{
   int n{};
   if (getDynamicsModel() != nullptr) {
      n = getDynamicsModel()->getEngThrust(data, max);
   }
   return n;
}

}
}
