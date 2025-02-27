
#include "mixr/interop/INtmInputNode.hpp"

namespace mixr {
namespace interop {

IMPLEMENT_PARTIAL_SUBCLASS(INtmInputNode, "INtmInputNode")
EMPTY_SLOTTABLE(INtmInputNode)
EMPTY_COPYDATA(INtmInputNode)
EMPTY_DELETEDATA(INtmInputNode)

INtmInputNode::INtmInputNode()
{
   STANDARD_CONSTRUCTOR()
}

INtmInputNode::~INtmInputNode()
{
   STANDARD_DESTRUCTOR()
}

INtmInputNode& INtmInputNode::operator=(const INtmInputNode& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

INtmInputNode* INtmInputNode::clone() const
{
   return nullptr;
}

}
}
