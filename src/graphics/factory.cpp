
#include "mixr/graphics/factory.hpp"

#include "mixr/base/IObject.hpp"

#include "mixr/graphics/Graphic.hpp"
#include "mixr/graphics/Display.hpp"
#include "mixr/graphics/Shapes.hpp"
#include "mixr/graphics/Polygon.hpp"
#include "mixr/graphics/ColorRotary.hpp"
#include "mixr/graphics/ColorGradient.hpp"
#include "mixr/graphics/BmpTexture.hpp"
#include "mixr/graphics/Rotators.hpp"
#include "mixr/graphics/Translator.hpp"
#include "mixr/graphics/Material.hpp"

// fonts
#include "mixr/graphics/fonts/BitmapFont.hpp"
#include "mixr/graphics/fonts/StrokeFont.hpp"

// fields
#include "mixr/graphics/readouts/AsciiText.hpp"
#include "mixr/graphics/readouts/BooleanText.hpp"
#include "mixr/graphics/readouts/Cursor.hpp"
#include "mixr/graphics/readouts/Rotary.hpp"
#include "mixr/graphics/readouts/Rotary2.hpp"

// readouts
#include "mixr/graphics/readouts/DirectionReadout.hpp"
#include "mixr/graphics/readouts/HexReadout.hpp"
#include "mixr/graphics/readouts/LatitudeReadout.hpp"
#include "mixr/graphics/readouts/LongitudeReadout.hpp"
#include "mixr/graphics/readouts/NumericReadout.hpp"
#include "mixr/graphics/readouts/OctalReadout.hpp"
#include "mixr/graphics/readouts/TimeReadout.hpp"

// pages and maps
#include "mixr/graphics/MfdPage.hpp"
#include "mixr/graphics/MapPage.hpp"
#include "mixr/graphics/SymbolLoader.hpp"

#include <string>

namespace mixr {
namespace graphics {

base::IObject* factory(const std::string& name)
{
    base::IObject* obj{};

    // general graphic classes
    if ( name == Graphic::getFactoryName() ) {
        obj = new Graphic();
    }
    else if ( name == Page::getFactoryName() ) {
        obj = new Page();
    }
    else if ( name == Display::getFactoryName() ) {
        obj = new Display();
    }
    else if ( name == Translator::getFactoryName() ) {
        obj = new Translator();
    }
    else if ( name == Rotators::getFactoryName() ) {
        obj = new Rotators();
    }
    else if ( name == ColorRotary::getFactoryName() ) {
        obj = new ColorRotary();
    }
    else if ( name == ColorGradient::getFactoryName() ) {
        obj = new ColorGradient();
    }

    // shapes
    else if ( name == Circle::getFactoryName() ) {
        obj = new Circle();
    }
    else if ( name == Point::getFactoryName() ) {
        obj = new Point();
    }
    else if ( name == Polygon::getFactoryName() ) {
        obj = new Polygon();
    }
    else if ( name == LineLoop::getFactoryName() ) {
        obj = new LineLoop();
    }
    else if ( name == Line::getFactoryName() ) {
        obj = new Line();
    }
    else if ( name == Arc::getFactoryName() ) {
        obj = new Arc();
    }
    else if ( name == OcclusionCircle::getFactoryName() ) {
        obj = new OcclusionCircle();
    }
    else if ( name == OcclusionArc::getFactoryName() ) {
        obj = new OcclusionArc();
    }
    else if ( name == Quad::getFactoryName() ) {
        obj = new Quad();
    }
    else if ( name == Triangle::getFactoryName() ) {
        obj = new Triangle();
    }

    // fields
    else if ( name == AsciiText::getFactoryName() ) {
        obj = new AsciiText();
    }
    else if ( name == BooleanText::getFactoryName() ) {
        obj = new BooleanText();
    }
    else if ( name == Cursor::getFactoryName() ) {
        obj = new Cursor();
    }

    // readouts
    else if ( name == NumericReadout::getFactoryName() ) {
        obj = new NumericReadout();
    }
    else if ( name == HexReadout::getFactoryName() ) {
        obj = new HexReadout();
    }
    else if ( name == OctalReadout::getFactoryName() ) {
        obj = new OctalReadout();
    }
    else if ( name == TimeReadout::getFactoryName() ) {
        obj = new TimeReadout();
    }
    else if ( name == DirectionReadout::getFactoryName() ) {
        obj = new DirectionReadout();
    }
    else if ( name == LatitudeReadout::getFactoryName() ) {
        obj = new LatitudeReadout();
    }
    else if ( name == LongitudeReadout::getFactoryName() ) {
        obj = new LongitudeReadout();
    }
    else if ( name == Rotary::getFactoryName() ) {
        obj = new Rotary();
    }
    else if ( name == Rotary2::getFactoryName() ) {
        obj = new Rotary2();
    }

    // stroke font
    else if ( name == StrokeFont::getFactoryName() ) {
        obj = new StrokeFont();
    }

    // bitmap font
    else if ( name == BitmapFont::getFactoryName() ) {
        obj = new BitmapFont();
    }

    // bitmap textures
    else if ( name == BmpTexture::getFactoryName() ) {
        obj = new BmpTexture();
    }
    // material
    else if ( name == Material::getFactoryName() ) {
        obj = new Material();
    }
    // pages
    else if ( name == MfdPage::getFactoryName() ) {
        obj = new MfdPage();
    }
    else if ( name == MapPage::getFactoryName() ) {
        obj = new MapPage();
    }
    // symbol loader
    else if ( name == SymbolLoader::getFactoryName() ) {
        obj = new SymbolLoader();
    }

    return obj;
}

}
}

