
#ifndef __mixr_interop_common_NtmInputNode_HPP__
#define __mixr_interop_common_NtmInputNode_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace interop {
class INib;
class Ntm;

//------------------------------------------------------------------------------
// Class: NtmInputNode
//------------------------------------------------------------------------------
class NtmInputNode : public base::Object
{
   DECLARE_SUBCLASS(NtmInputNode, base::Object)
public:
   NtmInputNode();
   virtual const Ntm* findNetworkTypeMapper(const INib* const) const =0;
   virtual bool add2OurLists(Ntm* const) =0;
};

}
}

#endif

