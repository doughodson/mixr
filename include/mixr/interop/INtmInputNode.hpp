
#ifndef __mixr_interop_common_INtmInputNode_HPP__
#define __mixr_interop_common_INtmInputNode_HPP__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace interop {
class INib;
class INtm;

//------------------------------------------------------------------------------
// Class: INtmInputNode
//------------------------------------------------------------------------------
class INtmInputNode : public base::Object
{
   DECLARE_SUBCLASS(INtmInputNode, base::Object)
public:
   INtmInputNode();
   virtual const INtm* findNetworkTypeMapper(const INib* const) const =0;
   virtual bool add2OurLists(INtm* const) =0;
};

}
}

#endif

