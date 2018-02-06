
#ifndef __mixr_base_util_mass_H__
#define __mixr_base_util_mass_H__

namespace mixr {
namespace base {
namespace mass {

// conversion constants
constexpr double KG2G{1000.0};            // KiloGrams => Grams
constexpr double G2KG{1.0 / KG2G};        // Grams => KiloGrams
constexpr double KG2SL{0.06852176585};    // KiloGrams => Slugs
constexpr double SL2KG{1.0 / KG2SL};      // Slugs => KiloGrams
constexpr double KG2PM{2.2046};           // KiloGrams => PoundsMass
constexpr double PM2KG{1.0 / KG2PM};      // PoundsMass => KiloGrams

// conversion functions
inline double gramsToKiloGrams(const double v) { return v * G2KG; }
inline double gramsToSlugs(const double v)     { return (v * G2KG) * KG2SL; }
inline double kiloGramsToGrams(const double v) { return v * KG2G; }
inline double kiloGramsToSlugs(const double v) { return v * KG2SL;}
inline double slugsToGrams(const double v)     { return (v * SL2KG) * KG2G; }
inline double slugsToKiloGrams(const double v) { return v * SL2KG; }

}
}
}

#endif
