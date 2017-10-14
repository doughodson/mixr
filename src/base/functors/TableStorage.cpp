
#include "mixr/base/functors/TableStorage.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(TableStorage, "TableStorage")
EMPTY_SLOTTABLE(TableStorage)
EMPTY_DELETEDATA(TableStorage)

TableStorage::TableStorage()
{
   STANDARD_CONSTRUCTOR()
}

void TableStorage::copyData(const TableStorage& org, const bool)
{
   BaseClass::copyData(org);
   xbp = org.xbp;
   ybp = org.ybp;
   zbp = org.zbp;
   wbp = org.wbp;
   vbp = org.vbp;
}

}
}
