
#ifndef __mixr_graphics_readout_utils_HPP__
#define __mixr_graphics_readout_utils_HPP__

#include <string>

namespace mixr {
namespace graphics {

enum class DataType { number, octal, hex };
enum class DirMode { invalid, ddmmss, ddmm, dd };
enum class TimeMode { invalid, hhmmss, hhmm, hh, mmss, mm, ss };

std::string processInteger(const DataType, const std::string&, bool* postSign);
std::string processFloat(const DataType, const std::string&, bool* postSign);
std::string processTime(const TimeMode, const std::string&, bool* postSign);
std::string processDirection(const DirMode, const std::string&, bool* postSign);

}
}

#endif
