
#ifndef __mixr_base_util_constants_HPP__
#define __mixr_base_util_constants_HPP__

#include <limits>

namespace mixr {
namespace base {

// ---
// Make sure PI is undefined
// ---
#ifdef PI
#undef PI
#endif
constexpr double PI{3.1415926535897932384626433832795};
constexpr double TWO_PI{PI + PI};

// ---
// Standard constants ----
//  Source: National Institute of Science & Technology (NIST) web site
//    Speed of light:       299,792,458 m/s
//    Boltzmann constant:   1.3806503E-23 (Joules/Kelvin)
//                          0.0000024E-23 (uncertainty)
// ---
constexpr double LIGHTSPEED{299792458.0};
constexpr double BOLTZMANN{1.3806503e-23};

// One "G" to feet per second per second
//  [This is an estimated average.
//  [One actual "G" is a function of latitude, longitude, and other exotic parameters.]
constexpr double ETHG{32.16};
constexpr double ETHGM{ETHG * 0.3048};    // meters

// Convert Barometric Pressure (Inches Mercury to/from Millibars)
// This is an estimate.
constexpr double INCH2MBAR{1013.2 / 29.92};      // LDB -  Inches -> Mbars
constexpr double MBAR2INCH{1.0 / INCH2MBAR};     // LDB -  Mbars -> Inches

// Earth gravitational parameter.
// Units: meters cubed per seconds squared.
// 3986004.418E8 meters^3/sec^2.
constexpr double MuM3perS2{3986004.418E8};

// ---
// default initial values for various readouts
// ---
constexpr double UNDEFINED_VALUE{std::numeric_limits<float>::min()};
constexpr double INIT_VALUE{};
constexpr int    INIT_INT_VALUE{};
constexpr int    INIT_SELECT_VALUE{1};
constexpr double INIT_TIME_VALUE{};
constexpr double INIT_DIRECTION_VALUE{};
constexpr double INIT_LATITUDE_VALUE{};
constexpr double INIT_LONGITUDE_VALUE{};

}
}

#endif
