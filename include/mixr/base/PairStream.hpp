
#ifndef __mixr_base_PairStream_HPP__
#define __mixr_base_PairStream_HPP__

#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"

#include <string>

namespace mixr {
namespace base {
class Identifier;

//------------------------------------------------------------------------------
// Class: PairStream
// Description: A PairStream is a list of Pair objects. Pair objects consist of
//              an object and the name of the object.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: PairStream
// Slots: none
// Notes: EDL syntax (a list of Pairs)
//      ident: <object>
//      ...
//      ident: <object>
//------------------------------------------------------------------------------
class PairStream : public List
{
    DECLARE_SUBCLASS(PairStream, List)

public:
   PairStream();

   bool operator==(const PairStream& stream) const;
   bool operator!=(const PairStream& stream) const;

   // finds the first pair by object type
   Pair* findByType(const std::type_info& type);
   const Pair* findByType(const std::type_info& type) const;

   // finds a pair by name
   Pair* findByName(const char* const slotname);
   const Pair* findByName(const char* const slotname) const;

   // finds the name associated with an object
   const std::string findName(const IObject* const obj) const;

   // returns the n'th pair
   Pair* getPosition(const unsigned int n)    {
      return static_cast<Pair*>(List::getPosition(n));
   }
   const Pair* getPosition(const unsigned int n) const {
      return static_cast<const Pair*>(List::getPosition(n));
   }

   // Returns a pointer to the pair at the head of this stream and the pair IS REMOVED
   // from the stream.  Empty streams will return null(0).  Ownership of the Pair is passed
   // to the caller (i.e., this routine does not unref() the pair and the caller should
   // not ref() the pair).
   Pair* get() {
      return static_cast<Pair*>(List::get());
   }

   // Put 'pair' at the tail of the stream.  The Pair is referenced, ref(), by this routine.
   void put(Pair* pair1) {
      List::put(static_cast<IObject*>(pair1));
   }

   // removes 'pair' from this list and true is returned.  If 'pair' 'obj' is not found
   // then false is returned.  (Note: The Pair is unref() and therefore possible to delete)
   bool remove(Pair* pair1) {
      return List::remove(static_cast<IObject*>(pair1));
   }

};

}
}

#endif
