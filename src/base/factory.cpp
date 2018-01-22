
#include "mixr/base/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/base/FileReader.hpp"
#include "mixr/base/Statistic.hpp"
#include "mixr/base/Transforms.hpp"
#include "mixr/base/Timers.hpp"

#include "mixr/base/Latitude.hpp"
#include "mixr/base/Longitude.hpp"

// relation classes (functions and tables)
#include "mixr/base/relations/Func1.hpp"
#include "mixr/base/relations/Func2.hpp"
#include "mixr/base/relations/Func3.hpp"
#include "mixr/base/relations/Func4.hpp"
#include "mixr/base/relations/Func5.hpp"
#include "mixr/base/relations/Polynomial.hpp"
#include "mixr/base/relations/Table1.hpp"
#include "mixr/base/relations/Table2.hpp"
#include "mixr/base/relations/Table3.hpp"
#include "mixr/base/relations/Table4.hpp"
#include "mixr/base/relations/Table5.hpp"

// Numbers
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Complex.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Float.hpp"
#include "mixr/base/numeric/Operators.hpp"

// Network handlers
#include "mixr/base/network/TcpHandler.hpp"
#include "mixr/base/network/TcpClient.hpp"
#include "mixr/base/network/TcpServerMultiple.hpp"
#include "mixr/base/network/TcpServerSingle.hpp"
#include "mixr/base/network/UdpBroadcastHandler.hpp"
#include "mixr/base/network/UdpMulticastHandler.hpp"
#include "mixr/base/network/UdpUnicastHandler.hpp"

// Colors
#include "mixr/base/colors/Cie.hpp"
#include "mixr/base/colors/Cmy.hpp"
#include "mixr/base/colors/Hls.hpp"
#include "mixr/base/colors/Hsv.hpp"
#include "mixr/base/colors/Hsva.hpp"
#include "mixr/base/colors/Rgb.hpp"
#include "mixr/base/colors/Rgba.hpp"
#include "mixr/base/colors/Yiq.hpp"

// Units
#include "mixr/base/units/AngularVelocity.hpp"
#include "mixr/base/units/Angles.hpp"
#include "mixr/base/units/Areas.hpp"
#include "mixr/base/units/Decibel.hpp"
#include "mixr/base/units/Distances.hpp"
#include "mixr/base/units/Energies.hpp"
#include "mixr/base/units/Forces.hpp"
#include "mixr/base/units/Frequencies.hpp"
#include "mixr/base/units/LinearVelocity.hpp"
#include "mixr/base/units/Masses.hpp"
#include "mixr/base/units/Powers.hpp"
#include "mixr/base/units/Times.hpp"

// Others
#include "mixr/base/EarthModel.hpp"

// ubf
#include "mixr/base/ubf/Agent.hpp"
#include "mixr/base/ubf/Arbiter.hpp"

#include <string>

namespace mixr {
namespace base {

Object* factory(const std::string& name)
{
    Object* obj {};

    // numbers
    if ( name == Number::getFactoryName() ) {
        obj = new Number();
    }
    else if ( name == Complex::getFactoryName() ) {
        obj = new Complex();
    }
    else if ( name == Integer::getFactoryName() ) {
        obj = new Integer();
    }
    else if ( name == Float::getFactoryName() ) {
        obj = new Float();
    }

    // operators
    else if ( name == Add::getFactoryName() ) {
        obj = new Add();
    }
    else if ( name == Subtract::getFactoryName() ) {
        obj = new Subtract();
    }
    else if ( name == Multiply::getFactoryName() ) {
        obj = new Multiply();
    }
    else if ( name == Divide::getFactoryName() ) {
        obj = new Divide();
    }

    //
    else if ( name == Boolean::getFactoryName() ) {
        obj = new Boolean();
    }

    // position
    else if ( name == Latitude::getFactoryName() ) {
        obj = new Latitude();
    }
    else if ( name == Longitude::getFactoryName() ) {
        obj = new Longitude();
    }

    // components
    else if ( name == FileReader::getFactoryName() ) {
        obj = new FileReader();
    }
    else if ( name == Statistic::getFactoryName() ) {
        obj = new Statistic();
    }

    // transformations
    else if ( name == Translation::getFactoryName() ) {
        obj = new Translation();
    }
    else if ( name == Rotation::getFactoryName() ) {
        obj = new Rotation();
    }
    else if ( name == Scale::getFactoryName() ) {
        obj = new Scale();
    }

    // relations
    else if ( name == Func1::getFactoryName() ) {
        obj = new Func1();
    }
    else if ( name == Func2::getFactoryName() ) {
        obj = new Func2();
    }
    else if ( name == Func3::getFactoryName() ) {
        obj = new Func3();
    }
    else if ( name == Func4::getFactoryName() ) {
        obj = new Func4();
    }
    else if ( name == Func5::getFactoryName() ) {
        obj = new Func5();
    }
    else if ( name == Polynomial::getFactoryName() ) {
        obj = new Polynomial();
    }
    else if ( name == Table1::getFactoryName() ) {
        obj = new Table1();
    }
    else if ( name == Table2::getFactoryName() ) {
        obj = new Table2();
    }
    else if ( name == Table3::getFactoryName() ) {
        obj = new Table3();
    }
    else if ( name == Table4::getFactoryName() ) {
        obj = new Table4();
    }
    else if ( name == Table5::getFactoryName() ) {
        obj = new Table5();
    }

    // timers
    else if ( name == UpTimer::getFactoryName() ) {
        obj = new UpTimer();
    }
    else if ( name == DownTimer::getFactoryName() ) {
        obj = new DownTimer();
    }

    // units: angles
    else if ( name == Degrees::getFactoryName() ) {
        obj = new Degrees();
    }
    else if ( name == Radians::getFactoryName() ) {
        obj = new Radians();
    }
    else if ( name == Semicircles::getFactoryName() ) {
        obj = new Semicircles();
    }

    // units
    else if ( name == Decibel::getFactoryName() ) {
        obj = new Decibel();
    }

    // units: areas
    else if ( name == SquareMeters::getFactoryName() ) {
        obj = new SquareMeters();
    }
    else if ( name == SquareFeet::getFactoryName() ) {
        obj = new SquareFeet();
    }
    else if ( name == SquareInches::getFactoryName() ) {
        obj = new SquareInches();
    }
    else if ( name == SquareYards::getFactoryName() ) {
        obj = new SquareYards();
    }
    else if ( name == SquareMiles::getFactoryName() ) {
        obj = new SquareMiles();
    }
    else if ( name == SquareCentiMeters::getFactoryName() ) {
        obj = new SquareCentiMeters();
    }
    else if ( name == SquareMilliMeters::getFactoryName() ) {
        obj = new SquareMilliMeters();
    }
    else if ( name == SquareKiloMeters::getFactoryName() ) {
        obj = new SquareKiloMeters();
    }
    else if ( name == DecibelSquareMeters::getFactoryName() ) {
        obj = new DecibelSquareMeters();
    }

    // units: distances
    else if ( name == Meters::getFactoryName() ) {
        obj = new Meters();
    }
    else if ( name == CentiMeters::getFactoryName() ) {
        obj = new CentiMeters();
    }
    else if ( name == MicroMeters::getFactoryName() ) {
        obj = new MicroMeters();
    }
    else if ( name == Microns::getFactoryName() ) {
        obj = new Microns();
    }
    else if ( name == KiloMeters::getFactoryName() ) {
        obj = new KiloMeters();
    }
    else if ( name == Inches::getFactoryName() ) {
        obj = new Inches();
    }
    else if ( name == Feet::getFactoryName() ) {
        obj = new Feet();
    }
    else if ( name == NauticalMiles::getFactoryName() ) {
        obj = new NauticalMiles();
    }
    else if ( name == StatuteMiles::getFactoryName() ) {
        obj = new StatuteMiles();
    }

    // units: energies
    else if ( name == KiloWattHours::getFactoryName() ) {
        obj = new KiloWattHours();
    }
    else if ( name == BTUs::getFactoryName() ) {
        obj = new BTUs();
    }
    else if ( name == Calories::getFactoryName() ) {
        obj = new Calories();
    }
    else if ( name == FootPounds::getFactoryName() ) {
        obj = new FootPounds();
    }
    else if ( name == Joules::getFactoryName() ) {
        obj = new Joules();
    }

    // units: forces
    else if ( name == Newtons::getFactoryName() ) {
        obj = new Newtons();
    }
    else if ( name == KiloNewtons::getFactoryName() ) {
        obj = new KiloNewtons();
    }
    else if ( name == Poundals::getFactoryName() ) {
        obj = new Poundals();
    }
    else if ( name == PoundForces::getFactoryName() ) {
        obj = new PoundForces();
    }

    // units: frequencies
    else if ( name == Hertz::getFactoryName() ) {
        obj = new Hertz();
    }
    else if ( name == KiloHertz::getFactoryName() ) {
        obj = new KiloHertz();
    }
    else if ( name == MegaHertz::getFactoryName() ) {
        obj = new MegaHertz();
    }
    else if ( name == GigaHertz::getFactoryName() ) {
        obj = new GigaHertz();
    }
    else if ( name == TeraHertz::getFactoryName() ) {
        obj = new TeraHertz();
    }

    // units: masses
    else if ( name == Grams::getFactoryName() ) {
        obj = new Grams();
    }
    else if ( name == KiloGrams::getFactoryName() ) {
        obj = new KiloGrams();
    }
    else if ( name == Slugs::getFactoryName() ) {
        obj = new Slugs();
    }

    // units: powers
    else if ( name == KiloWatts::getFactoryName() ) {
        obj = new KiloWatts();
    }
    else if ( name == Watts::getFactoryName() ) {
        obj = new Watts();
    }
    else if ( name == MilliWatts::getFactoryName() ) {
        obj = new MilliWatts();
    }
    else if ( name == Horsepower::getFactoryName() ) {
        obj = new Horsepower();
    }
    else if ( name == DecibelWatts::getFactoryName() ) {
        obj = new DecibelWatts();
    }
    else if ( name == DecibelMilliWatts::getFactoryName() ) {
        obj = new DecibelMilliWatts();
    }

    // units: time
    else if ( name == Seconds::getFactoryName() ) {
        obj = new Seconds();
    }
    else if ( name == MilliSeconds::getFactoryName() ) {
        obj = new MilliSeconds();
    }
    else if ( name == MicroSeconds::getFactoryName() ) {
        obj = new MicroSeconds();
    }
    else if ( name == NanoSeconds::getFactoryName() ) {
        obj = new NanoSeconds();
    }
    else if ( name == Minutes::getFactoryName() ) {
        obj = new Minutes();
    }
    else if ( name == Hours::getFactoryName() ) {
        obj = new Hours();
    }
    else if ( name == Days::getFactoryName() ) {
        obj = new Days();
    }

    // units: velocities
    else if ( name == AngularVelocity::getFactoryName() ) {
        obj = new AngularVelocity();
    }
    else if ( name == LinearVelocity::getFactoryName() ) {
        obj = new LinearVelocity();
    }

    // colors
    else if ( name == Color::getFactoryName() ) {
        obj = new Color();
    }
    else if ( name == Cie::getFactoryName() ) {
        obj = new Cie();
    }
    else if ( name == Cmy::getFactoryName() ) {
        obj = new Cmy();
    }
    else if ( name == Hls::getFactoryName() ) {
        obj = new Hls();
    }
    else if ( name == Hsv::getFactoryName() ) {
        obj = new Hsv();
    }
    else if ( name == Hsva::getFactoryName() ) {
        obj = new Hsva();
    }
    else if ( name == Rgb::getFactoryName() ) {
        obj = new Rgb();
    }
    else if ( name == Rgba::getFactoryName() ) {
        obj = new Rgba();
    }
    else if ( name == Yiq::getFactoryName() ) {
        obj = new Yiq();
    }

    // network handlers
    else if ( name == TcpClient::getFactoryName() ) {
        obj = new TcpClient();
    }
    else if ( name == TcpServerSingle::getFactoryName() ) {
        obj = new TcpServerSingle();
    }
    else if ( name == TcpServerMultiple::getFactoryName() ) {
        obj = new TcpServerMultiple();
    }
    else if ( name == UdpBroadcastHandler::getFactoryName() ) {
        obj = new UdpBroadcastHandler();
    }
    else if ( name == UdpMulticastHandler::getFactoryName() ) {
        obj = new UdpMulticastHandler();
    }
    else if ( name == UdpUnicastHandler::getFactoryName() ) {
        obj = new UdpUnicastHandler();
    }

    // earth models
    else if ( name == EarthModel::getFactoryName() ) {
        obj = new EarthModel();
    }

    // ubf
    else if ( name == ubf::Agent::getFactoryName() ) {
        obj = new ubf::Agent();
    }
    else if ( name == ubf::Arbiter::getFactoryName() ) {
        obj = new ubf::Arbiter();
    }

    return obj;
}

}
}

