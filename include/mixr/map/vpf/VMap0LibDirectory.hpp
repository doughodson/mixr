
#ifndef __mixr_map_vpf_VMap0LibDirectory_H__
#define __mixr_map_vpf_VMap0LibDirectory_H__

#include "mixr/map/vpf/VpfDirectory.hpp"

#include <array>

namespace mixr {
namespace vpf {
class VMap0RefCoverageDirectory;

// -------------------------------------------------------------------------------
// Class: VMap0LibDirectory
// Description: VMAP level 0 library directory which contains
// 
// - lat (library attribute table)
//
// Subroutines:
// loadTables() - 
//      void VMap0LibDirectory::loadTables()
//
// -------------------------------------------------------------------------------
class VMap0LibDirectory : public VpfDirectory
{
   DECLARE_SUBCLASS(VMap0LibDirectory, VpfDirectory)

public:
    VMap0LibDirectory();

    void loadTables() override;

private:
    // Holds table information for this directory
    static const int MAX_COVERAGES {15};
    std::array<VMap0RefCoverageDirectory*, MAX_COVERAGES> coverages {};
    int numCoverages {};
};

}
} 

#endif
