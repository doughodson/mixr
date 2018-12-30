
#ifndef __mixr_base_util_time_HPP__
#define __mixr_base_util_time_HPP__

#include "mixr/base/util/math_utils.hpp"

namespace mixr {
namespace base {
namespace time {

// conversion constants
constexpr double S2NS{1000000000.0};       // Seconds => nanoseconds
constexpr double NS2S{1.0 / 1000000000.0}; // nanoseconds => Seconds
constexpr double S2US{1000000.0};          // Seconds => microseconds
constexpr double US2S{1.0 / 1000000.0};    // microseconds => Seconds
constexpr double S2MS{1000.0};             // Seconds => MilliSeconds
constexpr double MS2S{1.0 / 1000.0};       // MilliSeconds => Seconds
constexpr double M2S{60.0};                // Minutes => Seconds
constexpr double S2M{1.0 / 60.0};          // Seconds => Minutes
constexpr double H2S{3600.0};              // Hours => Seconds
constexpr double S2H{1.0 / 3600.0};        // Seconds => Hours
constexpr double D2M{1440.0};              // Days => Minutes
constexpr double M2D{1.0 / 1440.0};        // Minutes => Days
constexpr double D2S{86400.0};             // Days => Seconds
constexpr double S2D{1.0 / 86400.0};       // Seconds => Days

// conversion functions
inline double SecondsToMilliSeconds(const double v)  { return v * S2MS; }
inline double MilliSecondsToSeconds(const double v)  { return v * MS2S; }

inline double MinutesToSeconds(const double v)       { return v * M2S; }
inline double SecondsToMinutes(const double v)       { return v * S2M; }

inline double HoursToMinutes(const double v)         { return (v * H2S) * S2M; }
inline double MinutesToHours(const double v)         { return (v * M2S) * S2H; }

inline double DaysToHours(const double v)            { return (v * D2S) * S2H; }
inline double HoursToDays(const double v)            { return (v * H2S) * S2D; }

// convert time in seconds to integer hours, minutes and seconds
inline void getHHMMSS(const double sec, int* const hh, int* const mm, int* const ss)
{
    double rem = sec;

    *hh  = int(rem / 3600.0f);
    rem -= static_cast<double>(*hh) * 3600.0f;
    *mm  = int(rem / 60.0f);
    rem -= static_cast<double>(*mm) * 60.0f;
    *ss  = nint(rem);
}

// convert time in seconds to integer hours, minutes and seconds
inline void getHHMMSS(const double sec, int* const hh, int* const mm, double* const ss)
{
    double rem = sec;

    *hh  = int(rem / 3600.0f);
    rem -= static_cast<double>(*hh) * 3600.0f;
    *mm  = int(rem / 60.0f);
    rem -= static_cast<double>(*mm) * 60.0f;
    *ss  = rem;
}

// convert integer hours, minutes and seconds to time in seconds
inline double putHHMMSS(const int hh, const int mm, const int ss)
{
    return static_cast<double>((3600.0f * hh) + (60.0f * mm) + ss);
}

}
}
}

#endif
