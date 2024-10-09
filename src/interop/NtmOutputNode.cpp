
#include "mixr/interop/NtmOutputNode.hpp"

namespace mixr {
namespace interop {

IMPLEMENT_PARTIAL_SUBCLASS(NtmOutputNode, "AbstractNtmOutputNode")
EMPTY_SLOTTABLE(NtmOutputNode)
EMPTY_COPYDATA(NtmOutputNode)
EMPTY_DELETEDATA(NtmOutputNode)

NtmOutputNode::NtmOutputNode()
{
   STANDARD_CONSTRUCTOR()
}

NtmOutputNode::~NtmOutputNode()
{
   STANDARD_DESTRUCTOR()
}

NtmOutputNode& NtmOutputNode::operator=(const NtmOutputNode& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

NtmOutputNode* NtmOutputNode::clone() const
{
   return nullptr;
}

}
}
