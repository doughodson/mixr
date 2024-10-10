
#include "mixr/models/dynamics/jsbsim/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/models/dynamics/jsbsim/JSBSimModel.hpp"

#include <string>

namespace mixr {

namespace jsbsim {

base::Object* factory(const std::string& name)
{
   base::Object* obj {};

   //
   if ( name == JSBSimModel::getFactoryName() ) {      // JSBSim
      obj = new JSBSimModel();
   }

   return obj;
}

}

}
