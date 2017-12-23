
#ifndef __mixr_base_TableStorage_H__
#define __mixr_base_TableStorage_H__

#include "mixr/base/functors/FStorage.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: TableStorage
//------------------------------------------------------------------------------
class TableStorage : public FStorage
{
   DECLARE_SUBCLASS(TableStorage, FStorage)

public:
   TableStorage();
   unsigned int xbp{}, ybp{}, zbp{}, wbp{}, vbp{};
};

}
}

#endif
