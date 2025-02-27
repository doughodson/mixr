
#include "mixr/interop/INtmOutputNode.hpp"

namespace mixr {
namespace interop {

IMPLEMENT_PARTIAL_SUBCLASS(INtmOutputNode, "INtmOutputNode")
EMPTY_SLOTTABLE(INtmOutputNode)
EMPTY_COPYDATA(INtmOutputNode)
EMPTY_DELETEDATA(INtmOutputNode)

INtmOutputNode::INtmOutputNode()
{
   STANDARD_CONSTRUCTOR()
}

INtmOutputNode::~INtmOutputNode()
{
   STANDARD_DESTRUCTOR()
}

INtmOutputNode& INtmOutputNode::operator=(const INtmOutputNode& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

INtmOutputNode* INtmOutputNode::clone() const
{
   return nullptr;
}

}
}
