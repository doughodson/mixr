
#include "mixr/iodevice/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/iodevice/Ai2DiSwitch.hpp"
#include "mixr/iodevice/AnalogInput.hpp"
#include "mixr/iodevice/AnalogOutput.hpp"
#include "mixr/iodevice/DiscreteInput.hpp"
#include "mixr/iodevice/DiscreteOutput.hpp"
#include "mixr/iodevice/IoData.hpp"
#include "mixr/iodevice/SignalGen.hpp"

#include <string>

#if defined(WIN32)
   #include "./platform/UsbJoystick_msvc.hpp"
#else
   #include "./platform/UsbJoystick_linux.hpp"
#endif

namespace mixr {
namespace iodevice {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    // Data buffers
    if ( name == IoData::getFactoryName() ) {
        obj = new IoData();
    }

    // Data Handlers
    else if ( name == DiscreteInput::getFactoryName() ) {
        obj = new DiscreteInput();
    }
    else if ( name == DiscreteOutput::getFactoryName() ) {
        obj = new DiscreteOutput();
    }
    else if ( name == AnalogInput::getFactoryName() ) {
        obj = new AnalogInput();
    }
    else if ( name == AnalogOutput::getFactoryName() ) {
        obj = new AnalogOutput();
    }

    // Signal converters and generators
    else if ( name == Ai2DiSwitch::getFactoryName() ) {
        obj = new Ai2DiSwitch();
    }
    else if ( name == SignalGen::getFactoryName() ) {
        obj = new SignalGen();
    }

    // ---
    // Device handler implementations (Linux and/or Windows)
    // ---
    else if ( name == UsbJoystick::getFactoryName() ) {
        obj = new UsbJoystick();
    }

    return obj;
}

}
}
