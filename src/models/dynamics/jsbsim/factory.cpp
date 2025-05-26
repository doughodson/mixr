
#include "mixr/base/IObject.hpp"
#include "mixr/models/dynamics/jsbsim/factory.hpp"
#include "mixr/models/dynamics/jsbsim/JSBSimDynamics.hpp"

#include <string>

namespace mixr {
namespace models {
namespace jsbsim {

base::IObject* factory(const std::string& name)
{
   base::IObject* obj {};

   //
   if ( name == JSBSimDynamics::getFactoryName() ) {      // JSBSim dynamics
      obj = new JSBSimDynamics();
   }

   return obj;
}

}
}
}
