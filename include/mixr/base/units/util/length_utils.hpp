
#ifndef __mixr_base_util_length_H__
#define __mixr_base_util_length_H__

namespace mixr {
namespace base {
namespace length {

// conversion constants
constexpr double FT2M{0.30480};        // Feet => Meters
constexpr double M2FT{1.0 / FT2M};     // Meters => Feet
constexpr double IN2M{0.02540};        // Inches => Meters
constexpr double M2IN{1.0 / IN2M};     // Meters => Inches

constexpr double NM2M{1852.0};         // Nautical Miles => Meters
constexpr double M2NM{1.0 / NM2M};     // Meters => Nautical Miles
constexpr double NM2FT{NM2M * M2FT};   // Nautical Miles => Feet
constexpr double FT2NM{1.0 / NM2FT};   // Feet => Nautical Miles

constexpr double SM2M{1609.344};       // Statue Miles => Meters
constexpr double M2SM{1.0 / SM2M};     // Meters => Statue Miles
constexpr double SM2FT{5280.0};        // Statue Miles => Feet
constexpr double FT2SM{1.0 / SM2FT};   // Feet => Statue Miles

constexpr double KM2M{1000.0};         // Kilometers => Meters
constexpr double M2KM{1.0 / KM2M};     // Meters => Kilometers
constexpr double CM2M{0.01};           // Centimeters => Meters
constexpr double M2CM{1.0 / CM2M};     // Meters => Centimeters
constexpr double UM2M{0.000001};       // Micrometer (Micron) => Meters
constexpr double M2UM{1.0 / UM2M};     // Meters => Micrometer (Micron)

// conversion functions
inline double MetersToMicroMeters(const double v)          { return v * M2UM; }
inline double MicroMetersToMeters(const double v)          { return v * UM2M; }
inline double MetersToCentiMeters(const double v)          { return v * M2CM; }
inline double CentiMetersToMeters(const double v)          { return v * CM2M; }
inline double MetersToKiloMeters(const double v)           { return v * M2KM; }
inline double KiloMetersToMeters(const double v)           { return v * KM2M; }
inline double MetersToInches(const double v)               { return v * M2IN; }
inline double InchesToMeters(const double v)               { return v * IN2M; }
inline double MetersToFeet(const double v)                 { return v * M2FT; }
inline double FeetToMeters(const double v)                 { return v * FT2M; }
inline double KiloMetersToNauticalMiles(const double v)    { return (v * KM2M) * M2NM; }
inline double NauticalMilesToKiloMeters(const double v)    { return (v * NM2M) * M2KM; }
inline double KiloMetersToStatuteMiles(const double v)     { return (v * KM2M) * M2SM; }
inline double StatuteMilesToKiloMeters(const double v)     { return (v * SM2M) * M2KM; }
inline double NauticalMilesToStatuteMiles(const double v)  { return (v * NM2M) * M2SM; }
inline double StatuteMilesToNauticalMiles(const double v)  { return (v * SM2M) * M2NM; }

}
}
}

#endif
