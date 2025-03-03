
#ifndef __mixr_base_TableStorage_HPP__
#define __mixr_base_TableStorage_HPP__

#include "mixr/base/relations/IFStorage.hpp"

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
class TableStorage : public IFStorage
{
   DECLARE_SUBCLASS(TableStorage, IFStorage)

public:
   TableStorage();
   unsigned int xbp{}, ybp{}, zbp{}, wbp{}, vbp{};
};

}
}

#endif
