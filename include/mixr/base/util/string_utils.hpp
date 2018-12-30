
#ifndef __mixr_base_util_string_utils_HPP__
#define __mixr_base_util_string_utils_HPP__

//------------------------------------------------------------------------------
// General purpose functions that support C++ (i.e., std::string) strings
//------------------------------------------------------------------------------

#include <string>

namespace mixr {
namespace base {

//
// justification
//

enum class Justify { None, Left, Right, Center };
// return a standard string aligned within a given width and justification
// Set to the first 'w' characters of 'str'.
// If the length of 'str' is less than 'w', than justify as 'j'.
std::string align(const std::string&, const std::size_t, const Justify j = Justify::None);

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

