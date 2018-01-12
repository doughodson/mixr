
#ifndef __mixr_base_util_enum_utils_H__
#define __mixr_base_util_enum_utils_H__

// enum utilities

namespace mixr {
namespace base {

//
// template converts enum class to underlying integer value representation
// see Antenna class for an example of using an enum's underlying value
//
template <typename Enumeration>
auto as_integer(Enumeration const value)
    -> typename std::underlying_type<Enumeration>::type
{
    return static_cast<typename std::underlying_type<Enumeration>::type>(value);
}

}
}

#endif
