
#ifndef __mixr_interop_dis_NtmInputNode_HPP__
#define __mixr_interop_dis_NetInputNode_HPP__

#include "mixr/interop/NtmInputNode.hpp"

namespace mixr {
namespace base { class List; }
namespace interop { class Ntm; class INib; }
namespace dis {
class Ntm;

//------------------------------------------------------------------------------
// Class: NtmInputNode
// Description: Distributed-Interactive-Simulation (DIS) incoming NTM class
//------------------------------------------------------------------------------
class NtmInputNode : public interop::NtmInputNode
{
   DECLARE_SUBCLASS(NtmInputNode, interop::NtmInputNode)

public:
   enum { ROOT_LVL, KIND_LVL, DOMAIN_LVL, COUNTRYCODE_LVL,
         CATEGORY_LVL, SUBCATEGORY_LVL, SPECIFIC_LVL, EXTRA_LVL };

public:
   NtmInputNode(const unsigned int level, const unsigned int code, const Ntm* ntm = nullptr);

   virtual const Ntm* findNtmByTypeCodes(
         const unsigned char  kind,
         const unsigned char  domain,
         const unsigned short countryCode,
         const unsigned char  category,
         const unsigned char  subcategory = 0,
         const unsigned char  specific = 0,
         const unsigned char  extra = 0
      ) const;

   // inherited methods
   const interop::INtm* findNetworkTypeMapper(const interop::INib* const nib) const override;
   bool add2OurLists(interop::INtm* const ntm) override;

private:
   unsigned int level {};        // Level
   unsigned int code {};         // Code for this level
   const Ntm* ourNtm {};         // Our default NTM
   base::List* subnodeList {};   // List of NtmInputNode nodes below this level
};

}
}

#endif

