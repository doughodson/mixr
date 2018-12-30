
#ifndef __mixr_base_TableStorage_HPP__
#define __mixr_base_TableStorage_HPP__

#include "mixr/base/relations/FStorage.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: TableStorage
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: TableStorage
// Slots: none
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
