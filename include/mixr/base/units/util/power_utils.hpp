
#ifndef __mixr_base_util_power_HPP__
#define __mixr_base_util_power_HPP__

namespace mixr {
namespace base {
namespace power {

// conversion constants
constexpr double W2KW{0.001};       // Watts => KiloWatts
constexpr double KW2W{1.0 / W2KW};  // KiloWatts => Watts
constexpr double W2HP{1341.0};      // Watts => Horsepower
constexpr double HP2W{1.0 / W2HP};  // Horsepower => Watts
constexpr double W2MW{1000.0};      // Watts => MilliWatts
constexpr double MW2W{1.0 / W2MW};  // MilliWatts => Watts

// conversion functions
inline double kiloWattsToWatts(const double v)       { return v * KW2W; }
inline double kiloWattsToHorsepower(const double v)  { return (v * KW2W) * W2HP; }
inline double kiloWattsToMilliWatts( const double v) { return (v * KW2W) * W2MW; }
inline double wattsToKiloWatts(const double v)       { return v * W2KW; }
inline double wattsToHorsepower(const double v)      { return v * W2HP; }
inline double wattsToMilliWatts(const double v)      { return v * W2MW; }
inline double horsepowerToWatts(const double v)      { return v * HP2W; }
inline double horsepowerToKiloWatts(const double v)  { return (v * HP2W) * W2KW; }
inline double horsepowerToMilliWatts(const double v) { return (v * HP2W) * W2MW; }
inline double milliWattsToWatts(const double v)      { return v * MW2W; }
inline double milliWattsToKiloWatts(const double v)  { return (v * MW2W) * W2KW; }
inline double milliWattsToHorsepower(const double v) { return (v * MW2W) * W2HP; }

}
}
}

#endif
