
#include "NtmInputNode.hpp"

#include "mixr/base/List.hpp"
#include "mixr/interop/dis/Nib.hpp"
#include "mixr/interop/dis/Ntm.hpp"

namespace mixr {
namespace dis {

IMPLEMENT_SUBCLASS(NtmInputNode, "NtmInputNode")
EMPTY_SLOTTABLE(NtmInputNode)

NtmInputNode::NtmInputNode(const unsigned int l, const unsigned int c, const Ntm* ntm)
   : level(l), code(c), ourNtm(nullptr), subnodeList(nullptr)
{
   STANDARD_CONSTRUCTOR()

   if (ntm != nullptr) {
      ourNtm = ntm;
      ourNtm->ref();
   }
   subnodeList = new base::List();
}

void NtmInputNode::copyData(const NtmInputNode& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      ourNtm = nullptr;
      subnodeList = nullptr;
   }

   level = org.level;
   code = org.code;

   if (ourNtm != nullptr) {
      ourNtm->unref();
      ourNtm = nullptr;
   }
   if (org.ourNtm != nullptr) {
      ourNtm = org.ourNtm->clone();
   }

   if (subnodeList != nullptr) {
      subnodeList->unref();
      subnodeList = nullptr;
   }
   if (org.subnodeList != nullptr) {
      subnodeList = org.subnodeList->clone();
   }
}

void NtmInputNode::deleteData()
{
   if (ourNtm != nullptr) {
      ourNtm->unref();
      ourNtm = nullptr;
   }

   if (subnodeList != nullptr) {
      subnodeList->unref();
      subnodeList = nullptr;
   }
}

//------------------------------------------------------------------------------
// Find the NTM based on the incoming entity type codes in the NIB
//------------------------------------------------------------------------------
const interop::INtm* NtmInputNode::findNetworkTypeMapper(const interop::INib* const nib) const
{
   const interop::INtm* result{};

   const auto disNib = dynamic_cast<const dis::Nib*>( nib );
   if (disNib != nullptr) {
      result = findNtmByTypeCodes(
            disNib->getEntityKind(),
            disNib->getEntityDomain(),
            disNib->getEntityCountry(),
            disNib->getEntityCategory(),
            disNib->getEntitySubcategory(),
            disNib->getEntitySpecific(),
            disNib->getEntityExtra()
         );
   }
   return result;
}

//------------------------------------------------------------------------------
// Find the NTM based on the incoming entity type codes in the NIB
//------------------------------------------------------------------------------
const Ntm* NtmInputNode::findNtmByTypeCodes(
      const unsigned char  kind,
      const unsigned char  domain,
      const unsigned short countryCode,
      const unsigned char  category,
      const unsigned char  subcategory,
      const unsigned char  specific,
      const unsigned char  extra
   ) const
{
   const Ntm* result{};

   {
      // Yes we have the proper type of NIB ...

      // Make sure that the NIB's code for this level matches our code
      bool match{true};
      switch (level) {
         case ROOT_LVL :         match = true;                   break; // the 'root' node always matches
         case KIND_LVL :         match = (code == kind);         break;
         case DOMAIN_LVL :       match = (code == domain);       break;
         case COUNTRYCODE_LVL :  match = (code == countryCode);  break;
         case CATEGORY_LVL :     match = (code == category);     break;
         case SUBCATEGORY_LVL :  match = (code == subcategory);  break;
         case SPECIFIC_LVL :     match = (code == specific);     break;
         case EXTRA_LVL :        match = (code == extra);        break;
      }

      if (match) {

         // First, if we're not the last 'extra' level then search
         // our subnodes to see if they can find a match
         if (level < EXTRA_LVL) {
            const base::IList::Item* item{subnodeList->getFirstItem()};
            while (item != nullptr && result == nullptr) {
               const NtmInputNode* subnode = static_cast<const NtmInputNode*>(item->getValue());
               result = subnode->findNtmByTypeCodes(kind, domain, countryCode, category, subcategory, specific, extra);
               item = item->getNext();
            }
         }

         // Second, we can use our NTM object, but only if we're at the category
         // level or higher (i.e., we must have match at the kind, domain, country
         // code and category levels)
         if (result == nullptr && level >= CATEGORY_LVL) {
            result = ourNtm;
         }
      }

   }

   return result;
}


//------------------------------------------------------------------------------
// Add the NTM to our sublist of nodes.
//------------------------------------------------------------------------------
bool NtmInputNode::add2OurLists(interop::INtm* const ntm)
{
   bool ok {};

   // Make sure we have the correct kind of NTM ...
   const auto disNtm = dynamic_cast<dis::Ntm*>( ntm );
   if (disNtm != nullptr) {

      // Make sure that the NTM's code for this level matches our code
      unsigned int currLevelCode {};
      unsigned int nextLevelCode {};
      switch (level) {
         case ROOT_LVL : {
            currLevelCode = 0;
            nextLevelCode = disNtm->getEntityKind();
            break;
          }
         case KIND_LVL : {
            currLevelCode = disNtm->getEntityKind();
            nextLevelCode = disNtm->getEntityDomain();
            break;
          }
         case DOMAIN_LVL : {
            currLevelCode = disNtm->getEntityDomain();
            nextLevelCode = disNtm->getEntityCountry();
            break;
          }
         case COUNTRYCODE_LVL : {
            currLevelCode = disNtm->getEntityCountry();
            nextLevelCode = disNtm->getEntityCategory();
            break;
          }
         case CATEGORY_LVL : {
            currLevelCode = disNtm->getEntityCategory();
            nextLevelCode = disNtm->getEntitySubcategory();
            break;
          }
         case SUBCATEGORY_LVL : {
            currLevelCode = disNtm->getEntitySubcategory();
            nextLevelCode = disNtm->getEntitySpecific();
            break;
          }
         case SPECIFIC_LVL : {
            currLevelCode = disNtm->getEntitySpecific();
            nextLevelCode = disNtm->getEntityExtra();
            break;
          }
         case EXTRA_LVL : {
            currLevelCode = disNtm->getEntityExtra();
            nextLevelCode = 0;
            break;
          }
      }

      // Does our code match the NIB's entity type code for this level?
      // And the 'root' node always matches.
      bool match {(code == currLevelCode) || (level == ROOT_LVL)};

      if (match) {
         bool err {};

         // Case #1; if we're at the 'category' level or above, and all remaining codes are
         // zero, then this becomes a wild card terminal node.
         {
            bool wild {(level >= CATEGORY_LVL)};

            if (wild && level < EXTRA_LVL)        wild = (disNtm->getEntityExtra() == 0);
            if (wild && level < SPECIFIC_LVL)     wild = (disNtm->getEntitySpecific() == 0);
            if (wild && level < SUBCATEGORY_LVL)  wild = (disNtm->getEntitySubcategory() == 0);

            if (wild) {
               // wild card terminal node
               if (ourNtm == nullptr) {
                  ourNtm = disNtm;
                  ourNtm->ref();
                  ok = true;
               } else if (isMessageEnabled(MSG_WARNING)) {
                  std::cerr << "Warning: duplicate incoming NTM(";
                  std::cerr << static_cast<int>(disNtm->getEntityKind()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntityDomain()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntityCountry()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntityCategory()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntitySubcategory()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntitySpecific()) << ",";
                  std::cerr << static_cast<int>(disNtm->getEntityExtra()) << ")";
                  std::cerr << ", second ignored" << std::endl;
                  err = true;
               }
            }
         }

         // Case #2; if we're at the 'specific' level, then create a terminal node
         // for the Ntm.  The wild card case was handle in case #1.
         if (!ok && !err && level == SPECIFIC_LVL) {

            // make sure the terminal node doesn't already exist.
            bool alreadyExists {};
            const base::IList::Item* item {subnodeList->getFirstItem()};
            while (item != nullptr && !alreadyExists) {
               //NtmInputNode* subnode = (NtmInputNode*) item->getValue();
               const NtmInputNode* csubnode {static_cast<const NtmInputNode*>(item->getValue())};
               NtmInputNode* subnode {const_cast<NtmInputNode*>(csubnode)};
               alreadyExists = (nextLevelCode == subnode->code);
               item = item->getNext();
            }

            if (!alreadyExists) {
               const auto newNode = new NtmInputNode( (level+1), nextLevelCode, disNtm );
               subnodeList->put(newNode);
               newNode->unref();   // ref()'d when put into the subnodelist
               ok = true;
            }
            else if (isMessageEnabled(MSG_WARNING)) {
               std::cerr << "Warning: duplicate incoming NTM(";
               std::cerr << static_cast<int>(disNtm->getEntityKind()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntityDomain()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntityCountry()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntityCategory()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntitySubcategory()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntitySpecific()) << ",";
               std::cerr << static_cast<int>(disNtm->getEntityExtra()) << ")";
               std::cerr << ", second ignored" << std::endl;
               err = true;
            }
         }

         // Case #3; if we're at a level less than the 'specific' level, so try
         // to add the NTM to one of our existing subnodes.
         if (!ok && !err && level < SPECIFIC_LVL) {
            const base::IList::Item* item {subnodeList->getFirstItem()};
            while (item != nullptr && !ok) {
               //NtmInputNode* subnode = (NtmInputNode*) item->getValue();
               const NtmInputNode* csubnode = static_cast<const NtmInputNode*>(item->getValue());
               NtmInputNode* subnode = const_cast<NtmInputNode*>(csubnode);
               if (nextLevelCode == subnode->code) {
                  ok = subnode->add2OurLists(disNtm);
               }
               item = item->getNext();
            }
         }

         // Case #4; We didn't create a terminal node, and the NTM was added to
         // one of our existing subnodes, then create a new subnode for it.
         if (!ok && !err) {
            // Create a new node and add the NTM
            const auto newNode = new NtmInputNode( (level+1), nextLevelCode );
            subnodeList->put(newNode);
            ok = newNode->add2OurLists(disNtm);
            newNode->unref();   // ref()'d when put into the subnodelist
         }
      }

   }

   return ok;
}

}
}
