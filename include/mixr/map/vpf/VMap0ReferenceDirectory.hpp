
#ifndef __mixr_map_vpf_VMap0ReferenceDirectory_H__
#define __mixr_map_vpf_VMap0ReferenceDirectory_H__

#include "mixr/map/vpf/VpfDirectory.hpp"
#include "mixr/map/vpf/VMap0RefCoverageDirectory.hpp"

#include <array>

namespace mixr {
namespace vpf {
class VMap0RefCoverageDirectory;

// -------------------------------------------------------------------------------
// Class: VMap0ReferenceDirectory
// Description: VMAP Level 0 reference library, contains tables
// - cat
// - lat
// - grt
// - dqx
// - dqt
// - lineage.doc
// and other libraries
// -------------------------------------------------------------------------------
class VMap0ReferenceDirectory : public VpfDirectory
{
   DECLARE_SUBCLASS(VMap0ReferenceDirectory, VpfDirectory)

public:
    VMap0ReferenceDirectory();

    void loadTables() override;

    // Defined coverages for VMAP level 0 reference directories
    enum { CVG_DBREF = 0, CVG_LIBREF, CVG_PLACENAM, CVG_POLBND, MAX_COVERAGES };

    // Get a specific coverage (VMAP0 has 4)
    VMap0RefCoverageDirectory* getCoverage(const int cov);

private:
    std::array<VMap0RefCoverageDirectory*, MAX_COVERAGES> coverages {};
};

}
}

#endif
