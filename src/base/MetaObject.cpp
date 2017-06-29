
#include "mixr/base/MetaObject.hpp"

#include "mixr/base/SlotTable.hpp"

namespace mixr {
namespace base {

MetaObject::MetaObject(
      const char* const cn,
      const char* const fn,
      const SlotTable* const slottbl,
      const MetaObject* const metaobj
   ) : slottable(slottbl), baseMetaObject(metaobj), cname(cn), fname(fn)
{}

const char* MetaObject::getClassName() const
{
    return cname;
}

const char* MetaObject::getFactoryName() const
{
    return fname;
}

}
}

