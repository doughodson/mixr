#ifndef __mixr_interop_common_NtmOutputNode_HPP__
#define __mixr_interop_common_NtmOutputNode_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace models { class Player; }
namespace interop {
class INtm;

//------------------------------------------------------------------------------
// Class: NtmOutputNode
//------------------------------------------------------------------------------
class NtmOutputNode : public base::Object
{
   DECLARE_SUBCLASS(NtmOutputNode, base::Object)
public:
   NtmOutputNode();
   virtual const INtm* findNetworkTypeMapper(const models::Player* const) const =0;
   virtual bool add2OurLists(INtm* const) =0;
};

}
}

#endif
