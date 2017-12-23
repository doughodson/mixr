
#ifndef __mixr_map_vpf_VMap0MainDirectory_H__
#define __mixr_map_vpf_VMap0MainDirectory_H__

#include "mixr/map/vpf/VpfDirectory.hpp"
#include "mixr/map/vpf/VMap0LibDirectory.hpp"
#include "mixr/map/vpf/VMap0ReferenceDirectory.hpp"

namespace mixr {
namespace vpf {
class VMap0ReferenceDirectory;
class VMap0LibDirectory;

// -------------------------------------------------------------------------------
// Class: VMap0MainDirectory
// Description: VMAP level 0 main directory which contains
// - dht (database header table)
// - lat (library attribute table)
// -------------------------------------------------------------------------------
class VMap0MainDirectory : public VpfDirectory
{
   DECLARE_SUBCLASS(VMap0MainDirectory, VpfDirectory)

public:
    VMap0MainDirectory();

    void loadTables() override;

    VMap0ReferenceDirectory* getReferenceDir()      { return rference; }

private:
    // Holds our reference library
    VMap0ReferenceDirectory* rference {};    // Reference library directory
    VMap0LibDirectory* libDirectory {};      // Library directory (just one for now)
};

}
}

#endif
