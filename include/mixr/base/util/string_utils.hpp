
#ifndef __mixr_base_util_string_utils_H__
#define __mixr_base_util_string_utils_H__

//------------------------------------------------------------------------------
// General purpose functions that support C++ (i.e., std::string) strings
//------------------------------------------------------------------------------

#include <string>

namespace mixr {
namespace base {

// returns true if our string is the ascii form of a number
bool isNumber(const std::string&);

// returns the value converted to a number or zero if isNumber() is false
double getNumber(const std::string&);

// returns true if our string is the ascii form of an integer number
bool isInteger(const std::string&);

// returns the value of this string converted to an integer value or zero if isInteger() is false
int getInteger(const std::string&);

}
}

#endif

