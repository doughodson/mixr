
#include "mixr/base/IObject.hpp"
#include "mixr/models/dynamics/jsbsim/factory.hpp"
#include "mixr/models/dynamics/jsbsim/JSBSimModel.hpp"

#include <string>

namespace mixr {
namespace models {
namespace jsbsim {

base::IObject* factory(const std::string& name)
{
   base::IObject* obj {};

   //
   if ( name == JSBSimModel::getFactoryName() ) {      // JSBSim
      obj = new JSBSimModel();
   }

   return obj;
}

}
}
}
