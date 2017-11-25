
#include "mixr/iolinkage/factory.hpp"

#include "mixr/base/Object.hpp"

// adapters
#include "mixr/iolinkage/adapters/Ai2DiSwitch.hpp"
#include "mixr/iolinkage/adapters/AnalogInput.hpp"
#include "mixr/iolinkage/adapters/AnalogOutput.hpp"
#include "mixr/iolinkage/adapters/DiscreteInput.hpp"
#include "mixr/iolinkage/adapters/DiscreteOutput.hpp"

// signal generators
#include "mixr/iolinkage/generators/AnalogInputFixed.hpp"
#include "mixr/iolinkage/generators/AnalogSignalGen.hpp"
#include "mixr/iolinkage/generators/DiscreteInputFixed.hpp"

// data
#include "mixr/iolinkage/IoData.hpp"

#include <string>

// devices
#include "mixr/iolinkage/MockDevice.hpp"

#if defined(WIN32)
   #include "./platform/UsbJoystick_msvc.hpp"
#else
   #include "./platform/UsbJoystick_linux.hpp"
#endif

namespace mixr {
namespace iolinkage {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

    // data buffers
    if ( name == IoData::getFactoryName() )                  { obj = new IoData(); }

    // adapters
    else if ( name == DiscreteInput::getFactoryName() )      { obj = new DiscreteInput();  }
    else if ( name == DiscreteOutput::getFactoryName() )     { obj = new DiscreteOutput(); }
    else if ( name == AnalogInput::getFactoryName() )        { obj = new AnalogInput();    }
    else if ( name == AnalogOutput::getFactoryName() )       { obj = new AnalogOutput();   }
    else if ( name == Ai2DiSwitch::getFactoryName() )        { obj = new Ai2DiSwitch();    }

    // signal generators
    else if ( name == AnalogInputFixed::getFactoryName() )   { obj = new AnalogInputFixed();   }
    else if ( name == AnalogSignalGen::getFactoryName() )    { obj = new AnalogSignalGen();    }
    else if ( name == DiscreteInputFixed::getFactoryName() ) { obj = new DiscreteInputFixed(); }

    // device interfaces
    else if ( name == MockDevice::getFactoryName() )         { obj = new MockDevice();  }
    else if ( name == UsbJoystick::getFactoryName() )        { obj = new UsbJoystick(); }

    return obj;
}

}
}
