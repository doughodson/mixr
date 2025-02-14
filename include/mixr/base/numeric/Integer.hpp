
#ifndef __mixr_base_numeric_Integer_HPP__
#define __mixr_base_numeric_Integer_HPP__

#include "mixr/base/numeric/INumber.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: Integer
// Description: Class that stores an EDL defined (regex) integer value as a
//              C++ integer
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Integer
// Slots:
//     value  <Integer>  ! Sets the value of this number (default: 0)
//------------------------------------------------------------------------------
class Integer final: public INumber
{
    DECLARE_SUBCLASS(Integer, INumber)

public:
    explicit Integer()                           { STANDARD_CONSTRUCTOR() }
    explicit Integer(const int x) : value(x)     { STANDARD_CONSTRUCTOR() }

    void setValue(const int x)                   { value = x; }

private:
   double getValue() const final                 { return static_cast<double>(value); }
   int getIntValue() const final                 { return value; }
   int value{};

private:
   // slot table helper methods
   virtual bool setSlotValue(const Integer* const);
};

}
}

#endif
