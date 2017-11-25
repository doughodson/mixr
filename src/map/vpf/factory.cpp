
#include "mixr/base/Object.hpp"

#include "mixr/map/vpf/factory.hpp"

#include "mixr/map/vpf/VMap0FeatureClass.hpp"
#include "mixr/map/vpf/VMap0LibDirectory.hpp"
#include "mixr/map/vpf/VMap0MainDirectory.hpp"
#include "mixr/map/vpf/VMap0RefCoverageDirectory.hpp"
#include "mixr/map/vpf/VMap0ReferenceDirectory.hpp"
#include "mixr/map/vpf/VpfDataType.hpp"
#include "mixr/map/vpf/VpfDirectory.hpp"
#include "mixr/map/vpf/VpfIndexTable.hpp"
#include "mixr/map/vpf/VpfRecord.hpp"
#include "mixr/map/vpf/VpfSpatialIndexTable.hpp"
#include "mixr/map/vpf/VpfTable.hpp"

#include <string>

namespace mixr {
namespace vpf {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

    // Feature Class
    if ( name == VMap0FeatureClass::getFactoryName() )              { obj = new VMap0FeatureClass(); }
    // Library directory
    else if ( name == VMap0LibDirectory::getFactoryName() )         { obj = new VMap0LibDirectory(); }
    // Main directory
    else if ( name == VMap0MainDirectory::getFactoryName() )        { obj = new VMap0MainDirectory(); }
    // Reference Coverage directory
    else if ( name == VMap0RefCoverageDirectory::getFactoryName() ) { obj = new VMap0RefCoverageDirectory(); }
    // Reference directory
    else if ( name == VMap0ReferenceDirectory::getFactoryName() )   { obj = new VMap0ReferenceDirectory();   }
    // Data Type
    else if ( name == VpfDataType::getFactoryName() )               { obj = new VpfDataType(); }
    // Top Level Directory
    else if ( name == VpfDirectory::getFactoryName() )              { obj = new VpfDirectory();}
    // Index Table
    else if ( name == VpfIndexTable::getFactoryName() )             { obj = new VpfIndexTable(); }
    // Record
    else if ( name == VpfRecord::getFactoryName() )                 { obj = new VpfRecord(); }
    // Spatial Index File
    else if ( name == VpfSpatialIndexTable::getFactoryName() )      { obj = new VpfSpatialIndexTable(); }
    // Table
    else if ( name == VpfTable::getFactoryName() )                  { obj = new VpfTable(); }

    return obj;
}

}
}
