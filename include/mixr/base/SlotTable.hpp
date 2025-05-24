
#ifndef __mixr_base_SlotTable_HPP__
#define __mixr_base_SlotTable_HPP__

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: SlotTable
// Description: Concrete class that defines a table of slot names (see IObject.hpp)
//------------------------------------------------------------------------------
// Notes:
//    Slot tables define the names of the slots (i.e., attributes) accepted by
//    the class of objects and maps these slot names to slot index numbers.
//
//    The mapping of slot names and index numbers includes all base class slot
//    tables that may exist.  Therefore, the first slot of the first base class
//    that contains a slot table is slot number one, and the last slot in this
//    table is slot N, where N is the total number of slot in this table and all
//    base class tables.  See function n() below.
//
//    Slot tables are usually defined using the macros BEGIN_SLOTTABLE and
//    END_SLOTTABLE (see macros.hpp).
//------------------------------------------------------------------------------
class SlotTable final
{
public:
   SlotTable() = default;
   SlotTable(const char* slotnames[], const int nslots, const SlotTable& baseTable);
   SlotTable(const char* slotnames[], const int nslots);
   SlotTable(const SlotTable&) = delete;
   SlotTable& operator=(const SlotTable&) = delete;
   virtual ~SlotTable();

   // Returns the last slot index number, which includes all base class slots.
   int n() const;

   // Returns the index, [ 1 .. n() ], for slot name 'slotname', or zero if not found
   int index(const char* const slotname) const;

   // Returns the name of the slot at index 'slotindex', range [ 1 .. n() ],
   // or zero is returned if the index is out of range.
   const char* name(const int slotindex) const;

private:
   SlotTable* baseTable {};   // Pointer to base class's slot table
   char** slots1 {};          // Array of slot names
   int nslots1 {};            // Number of slots in table
};

}
}

#endif

