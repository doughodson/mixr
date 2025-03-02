
#include "mixr/base/IObject.hpp"
#include "mixr/graphics/fonts/ftgl/factory.hpp"

// ftgl fonts
#include "mixr/graphics/fonts/ftgl/FtglBitmapFont.hpp"
#include "mixr/graphics/fonts/ftgl/FtglExtrdFont.hpp"
#include "mixr/graphics/fonts/ftgl/FtglHaloFont.hpp"
#include "mixr/graphics/fonts/ftgl/FtglOutlineFont.hpp"
#include "mixr/graphics/fonts/ftgl/FtglPixmapFont.hpp"
#include "mixr/graphics/fonts/ftgl/FtglPolygonFont.hpp"
#include "mixr/graphics/fonts/ftgl/FtglTextureFont.hpp"

#include <string>

namespace mixr {
namespace graphics {
namespace ftgl {

base::IObject* factory(const std::string& name)
{
   base::IObject* obj {};

   //
   if ( name == FtglBitmapFont::getFactoryName() ) {
      obj = new FtglBitmapFont();
   }
   else if ( name == FtglExtrdFont::getFactoryName() ) {
      obj = new FtglExtrdFont();
   }
   else if ( name == FtglHaloFont::getFactoryName() ) {
      obj = new FtglHaloFont();
   }
   else if ( name == FtglOutlineFont::getFactoryName() ) {
      obj = new FtglOutlineFont();
   }
   else if ( name == FtglPixmapFont::getFactoryName() ) {
      obj = new FtglPixmapFont();
   }
   else if ( name == FtglPolygonFont::getFactoryName() ) {
      obj = new FtglPolygonFont();
   }
   else if ( name == FtglTextureFont::getFactoryName() ) {
      obj = new FtglTextureFont();
   }




   return obj;
}

}
}
}
