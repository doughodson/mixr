
#include "mixr/interop/common/NtmInputNode.hpp"

namespace mixr {
namespace interop {

IMPLEMENT_PARTIAL_SUBCLASS(NtmInputNode, "AbstractNtmInputNode")
EMPTY_SLOTTABLE(NtmInputNode)
EMPTY_COPYDATA(NtmInputNode)
EMPTY_DELETEDATA(NtmInputNode)

NtmInputNode::NtmInputNode()
{
   STANDARD_CONSTRUCTOR()
}

NtmInputNode::~NtmInputNode()
{
   STANDARD_DESTRUCTOR()
}

NtmInputNode& NtmInputNode::operator=(const NtmInputNode& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

NtmInputNode* NtmInputNode::clone() const
{
   return nullptr;
}

}
}
