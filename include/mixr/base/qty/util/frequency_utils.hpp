
#ifndef __mixr_base_util_frequency_HPP__
#define __mixr_base_util_frequency_HPP__

namespace mixr {
namespace base {
namespace frequency {

// conversion constants
constexpr double KHz2Hz{1000.0};                 // KiloHertz => Hertz
constexpr double Hz2KHz{1.0 / 1000.0};           // Hertz => KiloHertz

constexpr double MHz2Hz{1000000.0};              // MegaHertz => Hertz
constexpr double Hz2MHz{1.0 / 1000000.0};        // Hertz => MegaHertz

constexpr double GHz2Hz{1000000000.0};           // GigaHertz => Hertz
constexpr double Hz2GHz{1.0 / 1000000000.0};     // Hertz => GigaHertz

constexpr double THz2Hz{1000000000000.0};        // TeraHertz => Hertz
constexpr double Hz2THz{1.0 / 1000000000000.0};  // Hertz => TeraHertz

}
}
}

#endif
