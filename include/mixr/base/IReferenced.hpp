
#ifndef __mixr_base_IReferenced_HPP__
#define __mixr_base_IReferenced_HPP__

// framework configuration file
#include "mixr/config.hpp"
// lock/unlock, etc - reference system needs it
#include "mixr/base/util/atomics.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: IReferenced
// Description: Interface that provides reference counting
//------------------------------------------------------------------------------
// Reference counting:
//
//    Reference counting is a technique of counting the number of users of an
//    instantiated object.  It is used to delete objects that are no
//    longer referenced.  Two functions (ref(), unref() control the reference
//    count of an object.
//
//    Using ref() increments the reference count, which indicates another user
//    of the object.  When an object is created using 'new', a copy operator, copy
//    constructor or the clone() function, the reference count is initialized to
//    one (1) by the constructor (i.e., it's a pre-referenced object).
//
//    Using unref() decrements the reference count, which indicates one less
//    user.  If the number of references becomes zero, the object is deleted.
//    Beware - Do not use 'delete' to destroy an object; only use unref()!
//
//    Using getRefCount() returns the current value of the Object's reference count
//------------------------------------------------------------------------------

class IReferenced
{
public:
   IReferenced() =default;
   IReferenced(const IReferenced&) =delete;
   IReferenced& operator=(const IReferenced&) =delete;
   virtual ~IReferenced() =0;

   int getRefCount() const       { return refCount; }

   // ---
   // ref() --
   //    Increments the number of references to this object.  An object
   //    is pre-referenced at creation and therefore does not need to be
   //    referenced by the creator (i.e., the reference count is
   //    initialized to one (1) by the constructor).  ExpInvalidRefCount
   //    is thrown if the reference count is invalid.
   // ---
   void ref() const;

   // ---
   // unref() --
   //    Decrements the number of references to this object.
   //    And, when the number of references becomes zero, deletes this object.
   // ---
   void unref() const;

   // general exception class
   class Exception {
   public:
      Exception()                                { }
      virtual ~Exception()                       { }
      virtual const char* getDescription() const {
         return "Unknown";
      }
   };

   class ExpInvalidRefCount : public Exception {
   public:
      ExpInvalidRefCount() : Exception() {}
      const char* getDescription() const override {
         return "invalid reference count";
      }
   };

   class ExpInvalidRefCountDelete : public Exception {
   public:
      ExpInvalidRefCountDelete() : Exception() {}
      const char* getDescription() const override {
         return "deleting object with positive reference count";
      }
   };

private:
   mutable long semaphore{};  // ref(), unref() semaphore
   mutable int refCount{1};   // reference count
};

inline IReferenced::~IReferenced() {}

inline void IReferenced::ref() const
{
   lock(semaphore);
   if (++(refCount) <= 1) throw new ExpInvalidRefCount();
   else unlock(semaphore);

   #ifdef MAX_REF_COUNT_ERROR
   static int maxRefCount = MAX_REF_COUNT_ERROR;
   if (which->refCount > maxRefCount) {
      std::cout << "ref(" << this << "): refCount(" << which->refCount << ") exceeded max refCount(" << maxRefCount << ")." << std::endl;
   }
   #endif
}

inline void IReferenced::unref() const
{
   lock(semaphore);
   if (--refCount == 0) delete this;
   else unlock(semaphore);
}

}
}

#endif
