
#include "mixr/instruments/factory.hpp"

#include "mixr/base/IObject.hpp"

// Instrument component
#include "mixr/instruments/Instrument.hpp"

// Analog Dial components
#include "mixr/instruments/dials/AnalogDial.hpp"
#include "mixr/instruments/dials/DialArcSegment.hpp"
#include "mixr/instruments/dials/DialTickMarks.hpp"
#include "mixr/instruments/dials/DialPointer.hpp"
#include "mixr/instruments/dials/AltitudeDial.hpp"
#include "mixr/instruments/dials/GMeterDial.hpp"

// Analog Gauge components
#include "mixr/instruments/gauges/AnalogGauge.hpp"
#include "mixr/instruments/gauges/GaugeSlider.hpp"
#include "mixr/instruments/gauges/AoaIndexer.hpp"
#include "mixr/instruments/gauges/TickMarks.hpp"
#include "mixr/instruments/gauges/Tape.hpp"

// Generic compass rose and bearing pointer, and dynamic maps
#include "mixr/instruments/maps/CompassRose.hpp"
#include "mixr/instruments/maps/BearingPointer.hpp"

// Generic Landing gear
#include "mixr/instruments/landinggear/LandingGear.hpp"
#include "mixr/instruments/landinggear/LandingLight.hpp"

// Generic engine page
#include "mixr/instruments/engines/EngPage.hpp"

// Buttons
#include "mixr/instruments/buttons/Button.hpp"
#include "mixr/instruments/buttons/PushButton.hpp"
#include "mixr/instruments/buttons/RotarySwitch.hpp"
#include "mixr/instruments/buttons/Knob.hpp"
#include "mixr/instruments/buttons/Switch.hpp"
#include "mixr/instruments/buttons/SolenoidButton.hpp"
#include "mixr/instruments/buttons/SolenoidSwitch.hpp"

// Adi
#include "mixr/instruments/adi/Adi.hpp"

// Ghost Horizon
#include "mixr/instruments/adi/GhostHorizon.hpp"

// Eadi3D
#include "mixr/instruments/eadi3d/Eadi3DPage.hpp"

#include <string>

namespace mixr {
namespace instruments {

base::IObject* factory(const std::string& name)
{
    base::IObject* obj {};

    // Instrument
    if (name == Instrument::getFactoryName()) {
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

