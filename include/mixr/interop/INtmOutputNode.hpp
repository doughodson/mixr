#ifndef __mixr_interop_common_INtmOutputNode_HPP__
#define __mixr_interop_common_INtmOutputNode_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace models { class IPlayer; }
namespace interop {
class INtm;

//------------------------------------------------------------------------------
// Class: INtmOutputNode
//------------------------------------------------------------------------------
class INtmOutputNode : public base::IObject
{
   DECLARE_SUBCLASS(INtmOutputNode, base::IObject)
public:
   INtmOutputNode();
   virtual const INtm* findNetworkTypeMapper(const models::IPlayer* const) const =0;
   virtual bool add2OurLists(INtm* const) =0;
};

}
}

#endif
