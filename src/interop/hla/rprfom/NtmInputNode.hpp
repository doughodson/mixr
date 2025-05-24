#ifndef __mixr_interop_rprfom_NtmInputNode_HPP__
#define __mixr_interop_rprfom_NetInputNode_HPP__

#include "mixr/interop/INtmInputNode.hpp"

namespace mixr {
namespace base { class IList; }
namespace rprfom {
class Ntm;

//------------------------------------------------------------------------------
// Class: rprfom::NtmInputNode
// Description: RPR FOM incoming NTM class
//------------------------------------------------------------------------------
class NtmInputNode : public interop::INtmInputNode
{
   DECLARE_SUBCLASS(NtmInputNode, interop::INtmInputNode)

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

   // class functions
   const interop::INtm* findNetworkTypeMapper(const interop::INib* const nib) const override;
   bool add2OurLists(interop::INtm* const ntm) override;

private:
   unsigned int level {};        // Level
   unsigned int code {};         // Code for this level
   const Ntm* ourNtm {};         // Our default NTM
   base::IList* subnodeList {};  // List of NtmInputNode nodes below this level
};

}
}

#endif

