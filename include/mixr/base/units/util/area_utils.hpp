
#ifndef __mixr_base_util_area_H__
#define __mixr_base_util_area_H__

#include <cmath>

namespace mixr {
namespace base {
namespace area {

// conversion constants
constexpr double SM2SFT{10.76391};          // Sq. Meters      => Sq. Feet
constexpr double SFT2SM{1.0 / SM2SFT};      // Sq. Feet        => Sq. Meters
constexpr double SM2SYD{1.19599};           // Sq. Meters      => Sq. Yards
constexpr double SYD2SM{1.0 / SM2SYD};      // Sq. Yards       => Sq. Meters
constexpr double SM2SMI{0.00000038610216};  // Sq. Meters      => Sq. Miles
constexpr double SMI2SM{1.0 / SM2SMI};      // Sq. Miles       => Sq. Meters
constexpr double SM2SIN{1550.0030399};      // Sq. Meters      => Sq. Inches
constexpr double SIN2SM{1.0 / SM2SIN};      // Sq. Inches      => Sq. Meters
constexpr double SM2SCM{10000.0};           // Sq. Meters      => Sq. CentiMeters
constexpr double SCM2SM{1.0 / SM2SCM};      // Sq. CentiMeters => Sq. Meters
constexpr double SM2SMM{1000000.0};         // Sq. Meters      => Sq. MilliMeters
constexpr double SMM2SM{1.0 / SM2SMM};      // Sq. MilliMeters => Sq. Meters
constexpr double SM2SKM{0.000001};          // Sq. Meters      => Sq. KiloMeters
constexpr double SKM2SM{1.0 / SM2SKM};      // Sq. KiloMeters  => Sq. Meters

// conversion functions
inline double squareFeetToInches(const double v)    { return (v * SFT2SM) * SM2SIN; }
inline double squareFeetToYards(const double v)     { return (v * SFT2SM) * SM2SYD; }
inline double squareFeetToMeters(const double v)    { return v * SFT2SM; }
inline double squareFeetToMiles(const double v)     { return (v * SFT2SM) * SM2SMI; }
inline double squareInchesToFeet(const double v)    { return (v * SIN2SM) * SM2SFT; }
inline double squareInchesToYards(const double v)   { return (v * SIN2SM) * SM2SYD; }
inline double squareInchesToMeters(const double v)  { return v * SIN2SM; }
inline double squareInchesToMiles(const double v)   { return (v * SIN2SM) * SM2SMI; }
inline double squareYardsToFeet(const double v)     { return (v * SYD2SM) * SM2SFT; }
inline double squareYardsToInches(const double v)   { return (v * SYD2SM) * SM2SIN; }
inline double squareYardsToMeters(const double v)   { return v * SYD2SM; }
inline double squareYardsToMiles(const double v)    { return (v * SYD2SM) * SM2SMI; }
inline double squareMilesToFeet(const double v)     { return (v * SMI2SM) * SM2SFT; }
inline double squareMilesToInches(const double v)   { return (v * SMI2SM) * SM2SIN; }
inline double squareMilesToYards(const double v)    { return (v * SMI2SM) * SM2SYD; }
inline double squareMilesToMeters(const double v)   { return v * SMI2SM; }
inline double squareMetersToFeet(const double v)    { return v * SM2SFT; }
inline double squareMetersToInches(const double v)  { return v * SM2SIN; }
inline double squareMetersToYards(const double v)   { return v * SM2SYD; }
inline double squareMetersToMiles(const double v)   { return v * SM2SMI; }
inline double squareMetersToDecibelSquareMeters(const double v) { return 10.0f * std::log10(v); }
inline double decibelSquareMetersToSquareMeters(const double v) { return std::pow(static_cast<double>(10.0), static_cast<double>(v/10.0)); }

}
}
}

#endif
