
#ifndef __mixr_base_List_HPP__
#define __mixr_base_List_HPP__

#include "mixr/base/IList.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: List
// Description: Concrete list class.
//
// Factory name: List
//------------------------------------------------------------------------------
// Notes: Example of looping through the list:
//
//      List* list = <some list>
//      List::Item* item = list->getFirstItem();
//      while (item != nullptr) {
//          IObject* obj = item->getValue();
//          <... code to use the object ...>
//          item = item->getNext();
//      }
//------------------------------------------------------------------------------
class List final: public IList
{
   DECLARE_SUBCLASS(List, IList)

public:
   List();
};

}
}

#endif
