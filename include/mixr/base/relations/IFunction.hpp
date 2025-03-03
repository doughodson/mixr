
#ifndef __mixr_base_IFunction_HPP__
#define __mixr_base_IFunction_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base {
class FStorage;
class ITable;

//------------------------------------------------------------------------------
// Class: IFunction
// Description: Interface for multi-dimensional function classes.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: IFunction
// Slots:
//    table    <Table>     ! Optional LFI table
//------------------------------------------------------------------------------
// Notes:
//    1) Use the storageFactory() function to create the FStorage object that
//       will maintain the previous function call values (i.e., integration).
//------------------------------------------------------------------------------
class IFunction : public IObject
{
   DECLARE_SUBCLASS(IFunction, IObject)

public:
   IFunction();

   // data storage factory (pre-ref()'d)
   virtual FStorage* storageFactory() const;    // Data storage factory (pre-ref()'d)

protected:
   const ITable* getTable() const {  return table; }

private:
   safe_ptr<const ITable> table;   // Optional LFI Table

protected:
   // slot table helper methods
   virtual bool setSlotLfiTable(const ITable* const);
};

}
}

#endif
