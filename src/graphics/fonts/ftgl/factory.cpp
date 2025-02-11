
#include "mixr/base/Object.hpp"
#include "mixr/graphics/fonts/ftgl/factory.hpp"

// ftgl fonts
#include "mixr/graphics/fonts/ftgl/FtglBitmapFont.hpp"

#include <string>

namespace mixr {
namespace graphics {
namespace ftgl {

base::Object* factory(const std::string& name)
{
   base::Object* obj {};

   //
   if ( name == FtglBitmapFont::getFactoryName() ) {
      obj = new FtglBitmapFont();
   }

   return obj;
}

}
}
}
