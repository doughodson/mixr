
#ifndef __mixr_base_util_force_HPP__
#define __mixr_base_util_force_HPP__

namespace mixr {
namespace base {
namespace force {

// conversion constants
constexpr double N2KN{0.001};        // Newtons => KiloNewtons
constexpr double KN2N{1.0 / N2KN};   // KiloNewtons => Newtons
constexpr double N2PF{0.224809};     // Newtons => PoundForces
constexpr double PF2N{1.0 / N2PF};   // PoundForces => Newtons
constexpr double N2PD{7.23301};      // Newtons => Poundals
constexpr double PD2N{1.0 / N2PD};   // Poundals => Newtons

// conversion functions
inline double newtonsToKiloNewtons(const double v)     { return v * N2KN; }
inline double newtonsToPoundForces(const double v)     { return v * N2PF; }
inline double newtonsToPoundals(const double v)        { return v * N2PD; }
inline double kiloNewtonsToNewtons(const double v)     { return v * KN2N; }
inline double kiloNewtonsToPoundForces(const double v) { return (v * KN2N) * N2PF; }
inline double kiloNewtonsToPoundals(const double v)    { return (v * KN2N) * N2PD; }
inline double poundForcesToNewtons(const double v)     { return v * PF2N; }
inline double poundForcesToKiloNewtons(const double v) { return (v * PF2N) * N2KN; }
inline double poundForcesToPoundals(const double v)    { return (v * PF2N) * N2PD; }
inline double poundalsToNewtons(const double v)        { return v * PD2N; }
inline double poundalsToKiloNewtons(const double v)    { return (v * PD2N) * N2KN; }
inline double poundalsToPoundForces(const double v)    { return (v * PD2N) * N2PF; }

}
}
}

#endif
