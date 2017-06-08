
#include "mxrp/terrain/factory.hpp"

#include "mxrp/base/Object.hpp"

#include "mxrp/terrain/QuadMap.hpp"
#include "mxrp/terrain/ded/DedFile.hpp"
#include "mxrp/terrain/dted/DtedFile.hpp"
#include "mxrp/terrain/srtm/SrtmHgtFile.hpp"

#include <string>

namespace mxrp {
namespace terrain {

base::Object* factory(const std::string& name)
{
    base::Object* obj = nullptr;

    if ( name == QuadMap::getFactoryName() ) {
        obj = new QuadMap();
    }
    else if ( name == DedFile::getFactoryName() ) {
        obj = new DedFile();
    }
    else if ( name == DtedFile::getFactoryName() ) {
        obj = new DtedFile();
    }
    else if ( name == SrtmHgtFile::getFactoryName() ) {
        obj = new SrtmHgtFile();
    }

    return obj;
}

}
}
