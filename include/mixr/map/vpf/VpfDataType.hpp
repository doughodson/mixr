
#ifndef __mixr_map_vpf_VpfDataType_H__
#define __mixr_map_vpf_VpfDataType_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base { class String; }
namespace vpf {

// -------------------------------------------------------------------------------
// Class: VpfDataType
// Description: Simple object which keeps track of data type, length, and value
// of our table.
// -------------------------------------------------------------------------------
class VpfDataType : public base::Object
{
   DECLARE_SUBCLASS(VpfDataType, base::Object)

public:
    VpfDataType();

    enum { LONG_INT, SHORT_INT, CHAR, FIXED_TEXT, VARIABLE_TEXT, FLOAT, DOUBLE, THREE_D_COORD, DATE };

    // Set functions 
    virtual void setLength(const int x = -1);
    // Value sets
    virtual void setValue(char* x);
    virtual void setType(const int x)       { type = x; }

    // Get functions
    int getType()   { return type; }
    int getLength() { return length; }
    const char* getValue();

    float getFloat();

private:
    int type {};               // Type of data we are using
    int length {};             // Length of the data we are using (number of values, not byte length)
                               // A length of 1 would be an integer value of one digit, not one sizeof(int);
    base::String* value {};    // This will hold the value type until we need it
};

}
}

#endif
