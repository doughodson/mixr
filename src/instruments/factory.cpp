
#include "mxrp/instruments/factory.hpp"

#include "mxrp/base/Object.hpp"

// Top Level objects
#include "mxrp/instruments/Instrument.hpp"

// Analog Dial components
#include "mxrp/instruments/dials/AnalogDial.hpp"
#include "mxrp/instruments/dials/DialArcSegment.hpp"
#include "mxrp/instruments/dials/DialTickMarks.hpp"
#include "mxrp/instruments/dials/DialPointer.hpp"
#include "mxrp/instruments/dials/AltitudeDial.hpp"
#include "mxrp/instruments/dials/GMeterDial.hpp"

// Analog Gauge components
#include "mxrp/instruments/gauges/AnalogGauge.hpp"
#include "mxrp/instruments/gauges/GaugeSlider.hpp"
#include "mxrp/instruments/gauges/AoaIndexer.hpp"
#include "mxrp/instruments/gauges/TickMarks.hpp"
#include "mxrp/instruments/gauges/Tape.hpp"

// Generic compass rose and bearing pointer, and dynamic maps
#include "mxrp/instruments/maps/CompassRose.hpp"
#include "mxrp/instruments/maps/BearingPointer.hpp"

// Generic Landing gear
#include "mxrp/instruments/landinggear/LandingGear.hpp"
#include "mxrp/instruments/landinggear/LandingLight.hpp"

// Generic engine page
#include "mxrp/instruments/engines/EngPage.hpp"

// Buttons
#include "mxrp/instruments/buttons/Button.hpp"
#include "mxrp/instruments/buttons/PushButton.hpp"
#include "mxrp/instruments/buttons/RotarySwitch.hpp"
#include "mxrp/instruments/buttons/Knob.hpp"
#include "mxrp/instruments/buttons/Switch.hpp"
#include "mxrp/instruments/buttons/SolenoidSwitch.hpp"

// Adi
#include "mxrp/instruments/adi/Adi.hpp"
 
// Ghost Horizon
#include "mxrp/instruments/adi/GhostHorizon.hpp"

// Eadi3D
#include "mxrp/instruments/eadi3d/Eadi3DPage.hpp"

#include <string>

namespace mxrp {
namespace instruments {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    // Instrument
    if ( name == Instrument::getFactoryName() ) {
        obj = new Instrument;
    }
    // Analog Dial
    else if ( name == AnalogDial::getFactoryName() ) {
        obj = new AnalogDial;
    }
    // Tick Marks for the analog dial
    else if ( name == DialTickMarks::getFactoryName() ) {
        obj = new DialTickMarks;
    }
    // Arc Segments for the analog dial
    else if ( name == DialArcSegment::getFactoryName() ) {
        obj = new DialArcSegment;
    }
    // Dial Pointer
    else if ( name == DialPointer::getFactoryName() ) {
        obj = new DialPointer;
    }
    // CompassRose
    else if ( name == CompassRose::getFactoryName() ) {
        obj = new CompassRose;
    }
    // Bearing Pointer
    else if ( name == BearingPointer::getFactoryName() ) {
        obj = new BearingPointer;
    }
    // AltitudeDial
    else if ( name == AltitudeDial::getFactoryName() ) {
        obj = new AltitudeDial;
    }
    // GMeterDial
    else if ( name == GMeterDial::getFactoryName() ) {
        obj = new GMeterDial;
    }    
    // Here is the analog gauge and its pieces
    // AnalogGauge
    else if ( name == AnalogGauge::getFactoryName() ) {
        obj = new AnalogGauge;
    }
    else if ( name == GaugeSlider::getFactoryName() ) {
        obj = new GaugeSlider;
    }
    // Tape
    else if ( name == Tape::getFactoryName() ) {
        obj = new Tape;
    }
    // digital AOA gauge
    else if ( name == AoAIndexer::getFactoryName() ) {
        obj = new AoAIndexer;
    }
    // Tick Marks (horizontal and vertical)
    else if ( name == TickMarks::getFactoryName() ) {
        obj = new TickMarks;
    }
    // Landing Gear
    else if ( name == LandingGear::getFactoryName() ) {
        obj = new LandingGear;
    }
    // Landing Lights
    else if ( name == LandingLight::getFactoryName() ) {
        obj = new LandingLight;
    }
    // EngPage
    else if ( name == EngPage::getFactoryName() ) {
        obj = new EngPage;
    }
    // Button
    else if (name == Button::getFactoryName() ) {
        obj = new Button;
    }
    // Push Button
    else if (name == PushButton::getFactoryName() ) {
        obj = new PushButton;
    }
    // Rotary Switch
    else if (name == RotarySwitch::getFactoryName() ) {
        obj = new RotarySwitch;
    }
    // Knob
    else if (name == Knob::getFactoryName() ) {
        obj = new Knob;
    }
    // Switch
    else if (name == Switch::getFactoryName() ) {
        obj = new Switch;
    }
    // Hold Switch
    else if (name == SolenoidSwitch::getFactoryName() ) {
        obj = new SolenoidSwitch;
    }
    // Hold Button
    else if (name == SolenoidButton::getFactoryName() ) {
        obj = new SolenoidButton;
    }
    // Adi
    else if (name == Adi::getFactoryName() ) {
        obj = new Adi;
    }
    // Ghost Horizon
    else if (name == GhostHorizon::getFactoryName() ) {
        obj = new GhostHorizon;
    }
    // Eadi3D
    else if (name == Eadi3DPage::getFactoryName() ) {
        obj = new Eadi3DPage;
    }

    return obj;
}

}
}

