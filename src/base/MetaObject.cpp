
#include "mixr/base/MetaObject.hpp"

#include "mixr/base/SlotTable.hpp"

namespace mixr {
namespace base {

MetaObject::MetaObject(
      const char* const cname,
      const char* const fname,
      const SlotTable* const slottbl,
      const MetaObject* const metaobj
   ) : slottable(slottbl), baseMetaObject(metaobj), class_name(cname), factory_name(fname)
{}

}
}

