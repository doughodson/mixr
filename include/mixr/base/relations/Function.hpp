
#ifndef __mixr_base_Function_HPP__
#define __mixr_base_Function_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base {
class FStorage;
class Table;

//------------------------------------------------------------------------------
// Class: Function
// Description: Base for the derived, multi-dimensional function classes.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Function
// Slots:
//    table    <Table>     ! Optional LFI table
//------------------------------------------------------------------------------
// Notes:
//    1) Use the storageFactory() function to create the FStorage object that
//       will maintain the previous function call values (i.e., integration).
//------------------------------------------------------------------------------
class Function : public IObject
{
   DECLARE_SUBCLASS(Function, IObject)

public:
   Function();

   // data storage factory (pre-ref()'d)
   virtual FStorage* storageFactory() const;    // Data storage factory (pre-ref()'d)

protected:
   const Table* getTable() const {  return table; }

private:
   safe_ptr<const Table> table;   // Optional LFI Table

protected:
   // slot table helper methods
   virtual bool setSlotLfiTable(const Table* const);
};

}
}

#endif
