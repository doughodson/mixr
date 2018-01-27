
#ifndef __mixr_base_Area_H__
#define __mixr_base_Area_H__

#include "mixr/base/units/Unit.hpp"

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
    Area(const double);

    void set(const double v)          { setValue(v); }
    void set(const Area& n)           { setValue(fromArea(n.toArea())); }

    double convert(const Area& n)     { return fromArea(n.toArea()); }

    virtual double toArea() const = 0;
    virtual double fromArea(const double) const = 0;
};


inline std::ostream& operator<<(std::ostream& sout, const Area& n)
{
   sout << "( " << n.getFactoryName() << " " << n.getValue() << " )";
   return sout;
}

}
}

#endif
