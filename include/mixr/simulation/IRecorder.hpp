
#ifndef __mixr_simulation_IRecorder_HPP__
#define __mixr_simulation_IRecorder_HPP__

#include "mixr/base/IComponent.hpp"
#include "mixr/simulation/dataRecorderTokens.hpp"

namespace mixr {
namespace base { class IList; }
namespace simulation {

//------------------------------------------------------------------------------
// Class: IRecorder
// Description: Interface base class for data recorder components
//
// Notes:
//    1) Use the setEnabledList() function to limit the processing to only
//    DataRecords with matching recorder event IDs.  Default is to process
//    all DataRecords.
//
//
// Slots:
//    enabledList <base::IList>  ! List of data records that are enabled for processing
//                               ! Overrides the disabledList!
//                               ! (default: all records are enabled -- except those
//                               !  listed in 'disabledList')
//
//    disabledList <base::IList> ! List of data records that are disabled from processing
//                               ! Only valid if 'enabledList' is NOT set!
//                               ! (default: no records are disabled)
//
//------------------------------------------------------------------------------
class IRecorder : public base::IComponent
{
   DECLARE_SUBCLASS(IRecorder, base::IComponent)

public:
   IRecorder();

   // Checks the data filters and returns true if the record should be processed.
   bool isDataEnabled(const unsigned int id) const;

   // Set a list of 'n' of data records enabled for processing,
   // or set 'n' to zero to enable all data records.
   bool setEnabledList(const unsigned int* const list, const unsigned int n);

   // Set a list of 'n' of data records disabled from being processed
   bool setDisabledList(const unsigned int* const list, const unsigned int n);

private:
   unsigned int* enabledList{};    // List of data records enabled for processing (default: all)
   unsigned int numEnabled{};      // Number of enabled record IDs, or zero for all records enabled

   unsigned int* disabledList{};   // List of data records disabled from being processed (default: none)
   unsigned int numDisabled{};     // Number of disabled record IDs

private:
   // slot table helper methods
   bool setSlotEnabledList(const base::IList* const);
   bool setSlotDisabledList(const base::IList* const);
};


// Data record enabled function
inline bool IRecorder::isDataEnabled(const unsigned int id) const
{
   bool ok{true};   // default is enabled

   if (id != REID_END_OF_DATA) { // END_OF_DATA message is always enabled
      // Do we have an enabled list?
      if (numEnabled > 0 && enabledList != nullptr) {
         ok = false; // yes -- then check to see if this message is enabled
         for (unsigned int i = 0; !ok && i < numEnabled; i++) {
            ok = (id == enabledList[i]);
         }
      }
      // Otherwise, do we have an disabled list?
      else if (numDisabled > 0 && disabledList != nullptr) {
         // yes -- then check to see if this message should be disabled
         for (unsigned int i = 0; ok && i < numDisabled; i++) {
            ok =  (id != disabledList[i]);
         }
      }
   }
   return ok;
}

}
}

#endif
