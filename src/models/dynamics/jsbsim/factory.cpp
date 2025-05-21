
#include "mixr/base/IObject.hpp"
#include "mixr/models/dynamics/jsbsim/factory.hpp"
#include "mixr/models/dynamics/jsbsim/DynamicsJSBSim.hpp"

#include <string>

namespace mixr {
namespace models {
namespace jsbsim {

base::IObject* factory(const std::string& name)
{
   base::IObject* obj {};

   //
   if ( name == DynamicsJSBSim::getFactoryName() ) {      // JSBSim dynamics
      obj = new DynamicsJSBSim();
   }

   return obj;
}

}
}
}
