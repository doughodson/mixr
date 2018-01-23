//------------------------------------------------------------------------------
// Classes:  Area, SquareFeet, SquareInches, SquareYards, SquareMiles,
//           SquareCentiMeters, SquareMeters, SquareMilliMeters,
//           SquareKiloMeters, DecibelSquareMeters
//------------------------------------------------------------------------------
#ifndef __mixr_base_Areas_H__
#define __mixr_base_Areas_H__

#include "mixr/base/units/Unit.hpp"

#include "mixr/base/units/area_utils.hpp"

#include <cmath>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Area
// Description:  Base class for area.  Defined as a square meter which is
//               equivalent to an instance of SquareMeters with its value equal
//               to 1.0.
//
// Public methods (Defined in Area, and inherited by all derived classes):
//
//     set(const double v)
//        Sets an Area derived instance with an double.
//
//     set(const Area& n)
//        Sets, and converts if necessary, an Area derived instance with
//        another Area derived instance.
//
//     double convert(const Area& n)
//        Converts the value of an Area derived instance into
//        the units of another Area derived instance.
//
//     Output stream operator: >>
//        ostream& operator<<(ostream& sout, const Area& n)
//        Sends "( <the Area derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes: SquareFeet, SquareYards, SquareMeters, SquareMiles, SquareInches,
//                  SquareCentiMeters, SquareMilliMeters, SquareKiloMeters, DecibelSquareMeters):
//
//     double convertStatic(const Area& n)
//        Static function to convert the given Area derived instance
//        into the units of a specific Area derived class.
//
//------------------------------------------------------------------------------
class Area : public Unit
{
    DECLARE_SUBCLASS(Area, Unit)

public:
    Area();
    Area(const double value);

    void set(const double v) { val = v; }
    void set(const Area& n)  { val = fromArea(n.toArea()); }

    double convert(const Area& n) { return fromArea(n.toArea()); }

    virtual double toArea() const = 0;
    virtual double fromArea(const double) const = 0;
};


inline std::ostream& operator<<(std::ostream& sout, const Area& n)
    { sout << "( " << n.getFactoryName() << " " << n.getReal() << " )"; return sout; }

//------------------------------------------------------------------------------
// Class: SquareMeters
// Description: An instance of SquareMeters with its value equal to 1.0 is one
//              base unit for area.
//------------------------------------------------------------------------------
class SquareMeters final: public Area
{
    DECLARE_SUBCLASS(SquareMeters, Area)

public:
    SquareMeters();
    SquareMeters(const double value);
    SquareMeters(const Area& value);

    static double convertStatic(const Area& n)              { return n.toArea(); }

    double toArea() const final                             { return static_cast<double>(val); }
    double fromArea(const double a) const final             { return a; }
};

//------------------------------------------------------------------------------
// Class: SquareFeet
// Description: Square Meters * 10.76391
//------------------------------------------------------------------------------
class SquareFeet final: public Area
{
    DECLARE_SUBCLASS(SquareFeet, Area)

public:
    SquareFeet();
    SquareFeet(const double value);
    SquareFeet(const Area& value);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SFT; }

    double toArea() const final                     { return static_cast<double>(val * area::SFT2SM); }
    double fromArea(const double a) const final     { return a * area::SM2SFT; }
};

//------------------------------------------------------------------------------
// Class: SquareInches
// Description: Square Meters * 1550.0030399
//------------------------------------------------------------------------------
class SquareInches final: public Area
{
    DECLARE_SUBCLASS(SquareInches, Area)

public:
    SquareInches();
    SquareInches(const double value);
    SquareInches(const Area& value);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SIN; }

    double toArea() const final                     { return static_cast<double>(val * area::SIN2SM); }
    double fromArea(const double a) const final     { return a * area::SM2SIN; }
};

//------------------------------------------------------------------------------
// Class: SquareYards
// Description: Square Meters * 1.19599
//------------------------------------------------------------------------------
class SquareYards final: public Area
{
    DECLARE_SUBCLASS(SquareYards, Area)

public:
    SquareYards();
    SquareYards(const double value);
    SquareYards(const Area& value);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SYD; }

    double toArea() const final                     { return static_cast<double>(val * area::SYD2SM); }
    double fromArea(const double a) const final     { return a * area::SM2SYD; }
};

//------------------------------------------------------------------------------
// Class: SquareMiles
// Description: Square Meters * 0.00000038610216
//------------------------------------------------------------------------------
class SquareMiles final: public Area
{
    DECLARE_SUBCLASS(SquareMiles, Area)

public:
    SquareMiles();
    SquareMiles(const double value);
    SquareMiles(const Area& value);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SMI; }

    double toArea() const final                     { return static_cast<double>(val * area::SMI2SM); }
    double fromArea(const double a) const final     { return a * area::SM2SMI; }
};

//------------------------------------------------------------------------------
// Class: SquareCentiMeters
// Description: Square Meters * 10000.0
//------------------------------------------------------------------------------
class SquareCentiMeters final: public Area
{
    DECLARE_SUBCLASS(SquareCentiMeters, Area)

public:
    SquareCentiMeters();
    SquareCentiMeters(const double value);
    SquareCentiMeters(const Area& value);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SCM; }

    double toArea() const final                     { return static_cast<double>(val * area::SCM2SM); }
    double fromArea(const double a) const final     { return a * area::SM2SCM; }
};

//------------------------------------------------------------------------------
// Class: SquareMilliMeters
// Description: Square Meters * 1000000.0
//------------------------------------------------------------------------------
class SquareMilliMeters final: public Area
{
    DECLARE_SUBCLASS(SquareMilliMeters, Area)

public:
    SquareMilliMeters();
    SquareMilliMeters(const double value);
    SquareMilliMeters(const Area& value);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SMM; }

    double toArea() const final                     { return static_cast<double>(val * area::SMM2SM); }
    double fromArea(const double a) const final     { return a * area::SM2SMM; }
};

//------------------------------------------------------------------------------
// Class: SquareKiloMeters
// Description: Square Meters * 0.000001
//------------------------------------------------------------------------------
class SquareKiloMeters final: public Area
{
    DECLARE_SUBCLASS(SquareKiloMeters, Area)

public:
    SquareKiloMeters();
    SquareKiloMeters(const double value);
    SquareKiloMeters(const Area& value);

    static double convertStatic(const Area& n)      { return n.toArea() * area::SM2SKM; }

    double toArea() const final                     { return static_cast<double>(val * area::SKM2SM); }
    double fromArea(const double a) const final     { return a * area::SM2SKM; }
};

//------------------------------------------------------------------------------
// Class: DecibelSquareMeters
// Description: 10 * Log(Square Meters)
//------------------------------------------------------------------------------
class DecibelSquareMeters final: public Area
{
    DECLARE_SUBCLASS(DecibelSquareMeters, Area)

public:
    DecibelSquareMeters();
    DecibelSquareMeters(const double value);
    DecibelSquareMeters(const Area& value);

    static double convertStatic(const Area& n)     { return 10.0f * std::log10( n.toArea() ); }

    double toArea() const final                    { return std::pow(10.0, val/10.0); }
    double fromArea(const double a) const final    { return 10.0f * std::log10(a); }
};

}
}

#endif
