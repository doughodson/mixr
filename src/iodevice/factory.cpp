
#include "mxrp/iodevice/factory.hpp"

#include "mxrp/base/Object.hpp"

#include "mxrp/iodevice/Ai2DiSwitch.hpp"
#include "mxrp/iodevice/AnalogInput.hpp"
#include "mxrp/iodevice/AnalogOutput.hpp"
#include "mxrp/iodevice/DiscreteInput.hpp"
#include "mxrp/iodevice/DiscreteOutput.hpp"
#include "mxrp/iodevice/IoData.hpp"
#include "mxrp/iodevice/SignalGen.hpp"

#include <string>

#if defined(WIN32)
   #include "./platform/UsbJoystick_msvc.hpp"
#else
   #include "./platform/UsbJoystick_linux.hpp"
#endif

namespace oe {
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
