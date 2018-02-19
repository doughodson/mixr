#include "mixr/base/util/filesystem_utils.hpp"

#include <string>

namespace mixr {
namespace base {


// builds a full path to a specific file (with a given filename)
const std::string buildPath(const std::string& directory, const std::string& filename)
{
   return directory + filename;
}

}
}

