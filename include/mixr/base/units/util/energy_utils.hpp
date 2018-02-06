
#ifndef __mixr_base_util_energy_H__
#define __mixr_base_util_energy_H__

namespace mixr {
namespace base {
namespace energy {

// conversion constants
constexpr double J2KWH{0.0000002777777778};  // Joules => KiloWattHours
constexpr double KWH2J{1.0 / J2KWH};         // KiloWattHours => Joules
constexpr double J2BTU{0.0009478};           // Joules => BTUs
constexpr double BTU2J{1.0 / J2BTU};         // BTUs => Joules
constexpr double J2FP{0.7376};               // Joules => FootPounds
constexpr double FP2J{1.0 / J2FP};           // FootPounds => Joules
constexpr double J2C{0.2388888889};          // Joules => Calories
constexpr double C2J{1.0 / J2C};             // Calories => Joules

// conversion functions
inline double btusToFootPounds(const double v)          { return (v * BTU2J) * J2FP;  }
inline double btusToJoules(const double v)              { return v * BTU2J;           }
inline double btusToKiloWattHours(const double v)       { return (v * BTU2J) * J2KWH; }
inline double btusToCalories(const double v)            { return (v * BTU2J) * J2C;   }
inline double footPoundsToBTUs(const double v)          { return (v * FP2J) * J2BTU;  }
inline double footPoundsToJoules(const double v)        { return v * FP2J;            }
inline double footPoundsToKiloWattHours(const double v) { return (v * FP2J) * J2KWH;  }
inline double footPoundsToCalories(const double v)      { return (v * FP2J) * J2C;    }
inline double joulesToBTUs(const double v)              { return v * J2BTU;           }
inline double joulesToFootPounds(const double v)        { return v * J2FP;            }
inline double joulesToKiloWattHours(const double v)     { return v * J2KWH;           }
inline double joulesToCalories(const double v)          { return v * J2C;             }
inline double kiloWattHoursToBTUs(const double v)       { return (v * KWH2J) * J2BTU; }
inline double kiloWattHoursToFootPounds(const double v) { return (v * KWH2J) * J2FP;  }
inline double kiloWattHoursToJoules(const double v)     { return v * KWH2J;           }
inline double kiloWattHoursToCalories(const double v)   { return (v * KWH2J) * J2C;   }
inline double caloriesToBTUs(const double v)            { return (v * C2J) * J2BTU;   }
inline double caloriesToFootPounds(const double v)      { return (v * C2J) * J2FP;    }
inline double caloriesToJoules(const double v)          { return v * C2J;             }
inline double caloriesToKiloWattHours(const double v)   { return (v * C2J) * J2KWH;   }

}
}
}

#endif
