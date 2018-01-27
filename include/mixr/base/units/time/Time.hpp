
#ifndef __mixr_base_Time_H__
#define __mixr_base_Time_H__

#include "mixr/base/units/Unit.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Time
// Description:  Base class for times.  Defined as a second which is equivalent
// to an instance of Seconds with its value equal to 1.0.
//
// Public methods (Defined in Time, and inherited by all derived classes):
//
//     set(const double v)
//        Sets an Time derived instance with an double.
//
//     set(const Time& n)
//        Sets, and converts if necessary, a Time derived instance with
//          another Time derived instance.
//
//     double convert(const Time& n)
//        Converts the value of an Time derived instance into
//        the units of another Time derived instance.
//
//      Output stream operator:  >>
//        ostream& operator<<(ostream& sout, const Time& n)
//        Sends "( <the Time derived instance class name and value> )"
//        to the output stream.
//
//
// Public methods (For classes:  Seconds, MilliSeconds, Minutes, Hours, Days):
//
//     double convertStatic(const Time& n)
//        Static function to convert the given Time derived
//        instance into the units of another Time derived instance.
//
//------------------------------------------------------------------------------
class Time : public Unit
{
    DECLARE_SUBCLASS(Time, Unit)

public:
    Time();
    Time(const double);

    void set(const double v)                      { setValue(v); }
    void set(const Time& n)                       { setValue(fromTime(n.toTime())); }

    double convert(const Time& n)                 { return fromTime(n.toTime()); }

    virtual double toTime() const =0;
    virtual double fromTime(const double a) const =0;
};

inline std::ostream& operator<<(std::ostream& sout, const Time& n)
{
   sout << "( " << n.getFactoryName() << " " << n.getValue() << " )";
   return sout;
}

}
}

#endif
