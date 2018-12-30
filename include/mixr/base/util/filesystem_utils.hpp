#ifndef __mixr_base_util_filesystem_utils_HPP__
#define __mixr_base_util_filesystem_utils_HPP__

//------------------------------------------------------------------------------
// General purpose functions associated with the file system
//------------------------------------------------------------------------------

#include <string>

namespace mixr {
namespace base {

// builds a full path to a specific file (with a given filename)
const std::string buildPath(const std::string& directory, const std::string& filename);

}
}

#endif
