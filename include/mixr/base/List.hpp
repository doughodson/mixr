
#ifndef __mixr_base_List_HPP__
#define __mixr_base_List_HPP__

#include "mixr/base/Object.hpp"

#include <cstddef>

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: List
// Description: General purpose list of objects.  The linked list and next-
//              previous pointers are maintained using a List::Item structure.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: List
// Slots: none
//------------------------------------------------------------------------------
// Notes: Example of looping through the list:
//
//      List* list = <some list>
//      List::Item* item = list->getFirstItem();
//      while (item != nullptr) {
//          Object* obj = item->getValue();
//          <... code to use the object ...>
//          item = item->getNext();
//      }
//
//------------------------------------------------------------------------------
class List : public Object
{
   DECLARE_SUBCLASS(List, Object)

public:
   struct Item {
      Item() = default;
      Item(const Item&) = delete;
      Item& operator=(const Item&) = delete;

      // returns the next item in the list.
      Item* getNext()                  { return next; }
      const Item* getNext() const      { return next; }

      // returns the previous item in the list.
      Item* getPrevious()              { return previous; }
      const Item* getPrevious() const  { return previous; }

      // returns the item's value: a pointer to the Object.
      Object* getValue()               { return value; }
      const Object* getValue() const   { return value; }

      Item* next{};                   // Pointer to next entry
      Item* previous{};               // Pointer to previous entry
      Object* value{};                // Data
   };

public:
   List();
   // Constructors; construct a list of 'nv' Float (Integer)
   // objects each containing a value from 'values'.
   List(const double values[], const std::size_t nv);
   List(const int values[], const std::size_t nv);


   // Fills the array, values, with values from the list.  Positions
   // of the objects on the list correspond to the values stored in
   // the array.  List objects that are not of type Number will
   // have a value of zero.  The maximum number of values stored in
   // the array is set by 'max'.  getNumberList() returns the number
   // of values stored in the array.
   std::size_t getNumberList(double values[], const std::size_t max) const;
   std::size_t getNumberList(float values[], const std::size_t max) const;
   std::size_t getNumberList(int values[], const std::size_t max) const;

   // operators to compare the List with other lists.
   bool operator==(const List&) const;
   bool operator!=(const List&) const;

   // returns true if entries() is zero
   bool isEmpty() const                            { return (num == 0); }
   // returns the number of entries in this list
   std::size_t entries() const                     { return num; }
   // clears or empties the list
   void clear();

   // Returns a pointer to the object at the head of this list and
   // the object IS REMOVED from the list.  Empty lists will return
   // nullptr.  Ownership of Object is passed to the caller
   // (i.e., this routine does not unref() the object and the
   // caller should not ref() the object).
   Object* get()                                   { return removeHead(); }

   // Put 'obj' at the end of the list.  The Object, obj, is referenced,
   // ref(), by this routine.
   void put(Object* obj)                           { if (obj == nullptr) return;  addTail(obj); }

   // Finds the object 'obj' in the list and returns the position
   // index.  If the object is not found, zero is returned.
   std::size_t getIndex(const Object* const) const;

   // returns a pointer to the n'th object on the list.
   Object* getPosition(const std::size_t n)              { return const_cast<Object*>(getPosition1(n)); }
   const Object* getPosition(const std::size_t n) const  { return getPosition1(n); }

   // Adds an object to the head (tail) of this list.
   // The Object is referenced, ref(), by these routines.
   void addHead(Object* const);
   void addTail(Object* const);

   // Removes 'obj' from this list and true is returned.  If the
   // object 'obj' is not found then false is returned.  The Object
   // is unref() and therefore possibly deleted.
   bool remove(const Object* const);

   // Removes the object at the head (tail) of this list.  Ownership of
   // Object is passed to the caller (i.e., these routines do not
   // unref() the object and the caller should not ref() the object).
   Object* removeHead();
   Object* removeTail();

   // Return pointers to the first/last List::Item.  The List::Item
   // member functions below are used to traverse the list.
   Item* getFirstItem()                { return headP; }
   const Item* getFirstItem() const    { return headP; }
   Item* getLastItem()                 { return tailP; }
   const Item* getLastItem() const     { return tailP; }

   // Adds a List::Item to the first or last positions (head or tail)
   // of the list.
   void addHead(Item*);
   void addTail(Item*);

   // Inserts the new List::Item, newItem, before the reference
   // List::Item, refItem.
   bool insert(Item* newItem, Item* refItem);

   // finds and removes the Item from the list.
   Object* remove(Item*);

   bool isValid() const override;

private:
   const Object* getPosition1(const std::size_t) const;

   Item* headP{};       // Pointer to head object
   Item* tailP{};       // Pointer to last object

   std::size_t num{};   // Number of list objects
};

}
}

#endif

