
#include "mixr/base/units/Density.hpp"
#include "mixr/base/SlotTable.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Density, "Density")
EMPTY_DELETEDATA(Density)

BEGIN_SLOTTABLE(Density)
    "mass",    // 1: Mass
    "volume",  // 2: Volume
END_SLOTTABLE(Density)

BEGIN_SLOT_MAP(Density)
    ON_SLOT(1, setSlotMass, Mass)
    ON_SLOT(2, setSlotVolume, Volume)
END_SLOT_MAP()

Density::Density(double value, const Mass* newMass, const Volume* newVolume) : Number()
{
    STANDARD_CONSTRUCTOR()

    if (newMass != nullptr && newVolume != nullptr) {
        myMass = newMass;
        myVolume = newVolume;
        val = value;
    }
    else {
        std::cerr << "Density::Density() - missing a mass or volume object,"
                  << " density is default to 1.0 kilograms per cubic meter"
                  << std::endl;

        // default mass, volume and density (1 Kilogram per Cubic Meter)
        // calling default constructor here doesn't seem to work
        myMass = new KiloGrams(1);
        myVolume = new CubicMeters(1);
        val = 1;
    }
}

Density::Density() : Number()
{
    STANDARD_CONSTRUCTOR()

    // default mass, volume and density (1 Kilogram per Cubic Meter)
    myMass = new KiloGrams(1);
    myVolume = new CubicMeters(1);
    val = 1;
}

void Density::copyData(const Density& org, const bool)
{
    BaseClass::copyData(org);

    val = org.getReal();
    myMass = org.getMass();
    myVolume = org.getVolume();
}

//------------------------------------------------------------------------------
// convert() -- converts from one mass/volume ratio to another
//------------------------------------------------------------------------------
double Density::convert(const Density& n) const
{
    return myMass->convert(*n.getMass()) / myVolume->convert(*n.getVolume());
}

//------------------------------------------------------------------------------
// set() -- sets our density from some other density
//------------------------------------------------------------------------------
void Density::set(const Density& n)
{
    val = convert(n);
}

//------------------------------------------------------------------------------
// getMassUnits() and getVolumeUnits() -- get units of this density object
//------------------------------------------------------------------------------
const Mass* Density::getMass() const
{
    return myMass;
}

const Volume* Density::getVolume() const
{
    return myVolume;
}

//------------------------------------------------------------------------------
// setSlotMass() -- sets our mass object.
//------------------------------------------------------------------------------
bool Density::setSlotMass(const Mass* newMass)
{
    if (newMass != nullptr) {
        myMass = newMass;
        return true;
    }
    else return false;
}

//------------------------------------------------------------------------------
// setSlotVolume() -- sets our volume object.
//------------------------------------------------------------------------------
bool Density::setSlotVolume(const Volume* newVolume)
{
    if (newVolume != nullptr) {
        myVolume = newVolume;
        return true;
    }
    else return false;
}

}
}

