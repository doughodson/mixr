
#include "mixr/terrain/factory.hpp"

#include "mixr/base/Object.hpp"

#include "mixr/terrain/QuadMap.hpp"
#include "mixr/terrain/ded/DedFile.hpp"
#include "mixr/terrain/dted/DtedFile.hpp"
#include "mixr/terrain/srtm/SrtmHgtFile.hpp"

#include <string>

namespace mixr {
namespace terrain {

base::Object* factory(const std::string& name)
{
    base::Object* obj {};

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
