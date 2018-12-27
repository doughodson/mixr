
#include "mixr/base/MetaObject.hpp"

#include "mixr/base/SlotTable.hpp"

namespace mixr {
namespace base {

MetaObject::MetaObject(
      const std::string& cname,
      const std::string& fname,
      const SlotTable* const slottbl,
      const MetaObject* const metaobj
   ) : slottable(slottbl), baseMetaObject(metaobj), className(cname), factoryName(fname)
{}

}
}

