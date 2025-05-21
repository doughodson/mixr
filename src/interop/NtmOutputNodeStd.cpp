
#include "NtmOutputNodeStd.hpp"

#include "mixr/interop/INtm.hpp"
#include "mixr/base/util/str_utils.hpp"
#include "mixr/models/player/IPlayer.hpp"
#include "mixr/base/List.hpp"

namespace mixr {
namespace interop {

IMPLEMENT_SUBCLASS(NtmOutputNodeStd, "NtmOutputNodeStd")
EMPTY_SLOTTABLE(NtmOutputNodeStd)

NtmOutputNodeStd::NtmOutputNodeStd(const models::IPlayer* const p, const char* const name)
{
   STANDARD_CONSTRUCTOR()

   nodeFactoryName = nullptr;
   tp = nullptr;

   if (name != nullptr) {
      const std::size_t LENGTH{std::strlen(name) + 1};
      nodeFactoryName = new char[LENGTH];
      base::utStrcpy(nodeFactoryName,LENGTH,name);
   }

   if (p != nullptr) {
      p->ref();
      tp = p;
}

   ntmList = new base::List();
   subnodeList = new base::List();
}

void NtmOutputNodeStd::copyData(const NtmOutputNodeStd& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      nodeFactoryName = nullptr;
      tp = nullptr;
      ntmList = nullptr;
      subnodeList = nullptr;
   }

   if (nodeFactoryName != nullptr) {
      delete[] nodeFactoryName;
      nodeFactoryName = nullptr;
   }
   if (org.nodeFactoryName != nullptr) {
      const std::size_t LENGTH{std::strlen(org.nodeFactoryName) + 1};
      nodeFactoryName = new char[LENGTH];
      base::utStrcpy(nodeFactoryName,LENGTH,org.nodeFactoryName);
   }

   if (tp != nullptr) {
      tp->unref();
      tp = nullptr;
   }
   if (org.tp != nullptr) {
      tp = org.tp->clone();
   }

   if (ntmList != nullptr) {
      ntmList->unref();
      ntmList = nullptr;
   }
   if (org.ntmList != nullptr) {
      ntmList = org.ntmList->clone();
   }

   if (subnodeList != nullptr) {
      subnodeList->unref();
      subnodeList = nullptr;
   }
   if (org.subnodeList != nullptr) {
      subnodeList = org.subnodeList->clone();
   }

}

void NtmOutputNodeStd::deleteData()
{
   if (nodeFactoryName != nullptr) {
      delete[] nodeFactoryName;
      nodeFactoryName = nullptr;
   }

   if (tp != nullptr) {
      tp->unref();
      tp = nullptr;
   }

   if (ntmList != nullptr) {
      ntmList->unref();
      ntmList = nullptr;
   }

   if (subnodeList != nullptr) {
      subnodeList->unref();
      subnodeList = nullptr;
   }
}

//------------------------------------------------------------------------------
// Find the NTM node for the target player, 'p'.
//------------------------------------------------------------------------------
const INtm* NtmOutputNodeStd::findNetworkTypeMapper(const models::IPlayer* const p) const
{
   const INtm* result{};

   // Using factory names, is the target player either the same or derived from
   // our node?  (the root node, which has no factory name, will always match)
   if ( nodeFactoryName == nullptr || p->isFactoryName(nodeFactoryName) ) {

      // First, we'll search our subnodes to see if they'll find a match
      // (i.e., if it's derived from our node then there may be a better match)
      const base::List::Item* item{subnodeList->getFirstItem()};
      while (item != nullptr && result == nullptr) {
         const NtmOutputNodeStd* subnode{static_cast<const NtmOutputNodeStd*>(item->getValue())};
         result = subnode->findNetworkTypeMapper(p);
         item = item->getNext();
      }

      // Second, we'll search our NTM list for a template player with a
      // type string matching the target player.
      if (result == nullptr && nodeFactoryName != nullptr) {

         // Target player's type string and length
         const base::String* const pType{p->getType_old()};
         const std::size_t pTypeLen{pType->len()};

         // Search the NTM for a match with the most matching type string characters,
         // but not more than the target player's type string.
         const base::List::Item* item{ntmList->getFirstItem()};
         while (item != nullptr && result == nullptr) {

            // Get the template player and its type string with length
            const INtm* tstNtm{static_cast<const INtm*>(item->getValue())};
            const models::IPlayer* const tp{tstNtm->getTemplatePlayer()};
            const base::String* const tpType{tp->getType_old()};
            const std::size_t tpTypeLen{tpType->len()};

            if (tpTypeLen <= pTypeLen) {
               bool match{(std::strncmp( pType->c_str(), tpType->c_str(), tpTypeLen ) == 0)};
               if (match) {
                  result = tstNtm;
               }
            }
            item = item->getNext();
         }

      }

   }

   return result;
}

//------------------------------------------------------------------------------
// Check if the target NTM's template player is the same or derived from
// this node's player using factory names.
//  1) if factory name match: the Ntm is added to the NTM list, 'ntmList'
//  2) if the target factory is derived from this factory then a new node is created
//  3) if there's not match then nothing changes and false is returned.
//------------------------------------------------------------------------------
bool NtmOutputNodeStd::add2OurLists(INtm* const tgtNtm)
{
   bool ok{};

   if (tgtNtm != nullptr) {

      ok = true;  // default is true (root node matches all)
      if (nodeFactoryName != nullptr) {
         // Using factory names, check if the target Ntm's
         // template player our form
         const models::IPlayer* const p{tgtNtm->getTemplatePlayer()};
         ok = p->isFactoryName(nodeFactoryName);
      }

      if (ok) {
         // Yes -- check to see if it really belongs to one of our subnodes.
         bool found{};
         base::List::Item* item{subnodeList->getFirstItem()};
         while (item != nullptr && !found) {
            NtmOutputNodeStd* subnode{static_cast<NtmOutputNodeStd*>(item->getValue())};
            found = subnode->add2OurLists(tgtNtm);
            item = item->getNext();
         }

         // If not found then add it to our local lists
         if (!found) ok = checkAndAddNtm(tgtNtm);
      }

   }

   return ok;
}

//------------------------------------------------------------------------------
// Add the target NTM to either (#1) the NTM list or (#2) our list of subnodes.
//------------------------------------------------------------------------------
bool NtmOutputNodeStd::checkAndAddNtm(INtm* const tgtNtm)
{
   bool ok{};
   if (tgtNtm != nullptr) {

      const models::IPlayer* const tp{tgtNtm->getTemplatePlayer()};
      const std::string& tpfn{tp->getFactoryName()};

      // Case #1 : when the Ntm's template player has the same
      // factory name as this node then we just add it to the list
      // of Ntm objects assigned to this node.
      if (nodeFactoryName != nullptr) {
         ok = ( tpfn == nodeFactoryName );
         if (ok) addNtmSorted(tgtNtm);
      }

      // Case #2 : when the Ntm's template player does not have the
      // same factory name as this node (i.e., it's derived from this
      // node's player class) then create a new NtmOutputNodeStd node for
      // this Ntm object and add it to our list of subnodes.
      if (!ok) {
         // Create a new node and add the NTM
         const auto newNode = new NtmOutputNodeStd(tp, tpfn.c_str());
         newNode->addNtmSorted(tgtNtm);

         // Case #2A : check if any of our subnodes is really a subnode of the new node.
         base::List::Item* item{subnodeList->getFirstItem()};
         while (item != nullptr) {
            const auto subnode = static_cast<NtmOutputNodeStd*>(item->getValue());
            item = item->getNext();

            if ( subnode->tp->isFactoryName( tpfn ) ) {
               // Move it from our subnode list to the new node's subnode list
               newNode->subnodeList->put(subnode);
               this->subnodeList->remove(subnode);
            }

         }

         // add the new node to our subnode list
         subnodeList->put(newNode);
         newNode->unref();
         ok = true;
      }
   }

   return ok;
}

//------------------------------------------------------------------------------
// Add a NTM to the list in sorted order
//------------------------------------------------------------------------------
bool NtmOutputNodeStd::addNtmSorted(INtm* const newNtm)
{
   bool ok{};
   if (newNtm != nullptr) {
      newNtm->ref();

      // Create a new List::Item to contain this Ntm
      const auto newItem = new base::List::Item();
      newItem->value = newNtm;

      // Get the template player's type string from the 'new' Ntm
      const models::IPlayer* newP{newNtm->getTemplatePlayer()};
      const base::String* newTypeStr{newP->getType_old()};
      const std::size_t newTypeLen{newTypeStr->len()};

      bool inserted{};
      bool err{};
      base::List::Item* refItem{ntmList->getFirstItem()};
      while (refItem != nullptr && !inserted && !err) {

         // Get the ref player's string from the 'ref' Ntm.
         const INtm* refNtm{static_cast<const INtm*>(refItem->getValue())};
         const models::IPlayer* refP{refNtm->getTemplatePlayer()};
         const base::String* refTypeStr{refP->getType_old()};
         const std::size_t refTypeLen{refTypeStr->len()};

         // compare to the shortest string length
         std::size_t len{newTypeLen};
         if (refTypeLen < len) len = refTypeLen;

         int r{std::strncmp(newTypeStr->c_str(), refTypeStr->c_str(), len)};
         if ( r == 0 && refTypeLen < newTypeLen) {
            ntmList->insert(newItem, refItem);
            inserted = true;
         } else if (r < 0) {
            ntmList->insert(newItem, refItem);
            inserted = true;
         } else if ( r == 0 && refTypeLen == newTypeLen) {
            if (isMessageEnabled(MSG_WARNING)) {
               std::cerr << "Warning: duplicate outgoing NTM( ";
               std::cerr << newP->getFactoryName() << " type: " << *newTypeStr;
               std::cerr << " ), second ignored" << std::endl;
               err = true;
            }
            err = true;
         }

         refItem = refItem->getNext();
      }

      if (!inserted && !err) ntmList->insert(newItem,nullptr);

      ok = true;
   }

   return ok;
}

}
}
