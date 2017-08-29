
#ifndef __mixr_base_Density_H__
#define __mixr_base_Density_H__

#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/safe_ptr.hpp"
#include "mixr/base/units/Masses.hpp"
#include "mixr/base/units/Volumes.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Classes: Density
//
// Description:  Density as a function of mass and volume.
//
// Factory name: Density
// Slots:
//    mass    <Mass>    ! Mass (default: 1 kg)
//    volume  <Volume>  ! Volume (default: 1 m^3)
//
// getMassUnits() -- get units of this density object
//     const Mass* Density::getMass() const
//
// getVolumeUnits() -- get units of this density object
//     const Volume* Density::getVolume() const
//
// setSlotMass() -- sets our mass object.
//     bool Density::setSlotMass(const Mass* newMass)
//
// setSlotVolume() -- sets our volume object.
//     bool Density::setSlotVolume(const Volume* newVolume)
//
// set() -- sets our density from some other density
//     void Density::set(const Density& n)
//
// convert() -- converts from one mass/volume ratio to another
//     double Density::convert(const Density& n) const
//
// NOTE: The = operator and copy constructor WILL change the units
// of the target density object.  To perform a conversion, use set().
//------------------------------------------------------------------------------
class Density : public Number
{
    DECLARE_SUBCLASS(Density, Number)

public:
    Density(double newDensity, const Mass* newMass, const Volume* newVolume);
    Density();

    double convert(const Density& n) const;
    void set(const Density& n);
    const Mass* getMass() const;
    const Volume* getVolume() const;

private:
    safe_ptr<const Mass>   myMass;
    safe_ptr<const Volume> myVolume;

private:
    // slot table helper methods
    bool setSlotMass(const Mass*);
    bool setSlotVolume(const Volume*);
};

inline std::ostream& operator<<(std::ostream& sout, const Density& n)
{
    const Mass* m {n.getMass()};
    const Volume* v {n.getVolume()};

    sout << "( " << n.getFactoryName() << " " << n.getReal();
    if ( m != nullptr ) sout << " " << *m;
    if ( v != nullptr ) sout << " " << *v;
    sout << " )";
    return sout;
}

}
}

#endif
