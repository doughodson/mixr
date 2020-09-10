#ifndef __mixr_interop_common_NtmOutputNode_HPP__
#define __mixr_interop_common_NtmOutputNode_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace models { class Player; }
namespace interop {
class Ntm;

//------------------------------------------------------------------------------
// Class: NtmOutputNode
//------------------------------------------------------------------------------
class NtmOutputNode : public base::Object
{
   DECLARE_SUBCLASS(NtmOutputNode, base::Object)
public:
   NtmOutputNode();
   virtual const Ntm* findNetworkTypeMapper(const models::Player* const) const =0;
   virtual bool add2OurLists(Ntm* const) =0;
};

}
}

#endif
