
#ifndef __mixr_base_util_volume_HPP__
#define __mixr_base_util_volume_HPP__

namespace mixr {
namespace base {
namespace volume {

// conversion constants
constexpr double CM2CFT{35.3146667215};   // CubicMeters => CubicFeet
constexpr double CFT2CM{1.0 / CM2CFT};    // CubicFeet => CubicMeters
constexpr double CM2CIN{61023.7440947};   // CubicMeters => CubicInches
constexpr double CIN2CM{1.0 / CM2CIN};    // CubicInches => CubicMeters
constexpr double CM2L{1000.0};            // CubicMeters => Liters
constexpr double L2CM{1.0 / CM2L};        // Liters => CubicMeters

// conversion functions
inline double cubicMetersToFeet(const double v)   { return v * CM2CFT; }
inline double cubicFeetToMeters(const double v)   { return v * CFT2CM; }

inline double litersToCubicMeters(const double v) { return v * L2CM; }
inline double cubicMetersToLiters(const double v) { return v * CM2L; }

inline double cubicFeetToInches(const double v)   { return (v * CFT2CM) * CM2CIN; }
inline double cubicInchesToFeet(const double v)   { return (v * CIN2CM) * CM2CFT; }

}
}
}

#endif
