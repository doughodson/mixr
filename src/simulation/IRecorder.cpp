
#include "mixr/simulation/IRecorder.hpp"

#include "mixr/base/IList.hpp"
#include <iostream>

namespace mixr {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(IRecorder, "IRecorder")

BEGIN_SLOTTABLE(IRecorder)
    "enabledList",         // 1)  List of enabled data records
    "disabledList",        // 2)  List of disabled data records
END_SLOTTABLE(IRecorder)

BEGIN_SLOT_MAP(IRecorder)
    ON_SLOT( 1, setSlotEnabledList,  base::IList)
    ON_SLOT( 2, setSlotDisabledList, base::IList)
END_SLOT_MAP()

IRecorder::IRecorder()
{
   STANDARD_CONSTRUCTOR()
}

void IRecorder::copyData(const IRecorder& org, const bool)
{
   BaseClass::copyData(org);

   setEnabledList(org.enabledList, org.numEnabled);
   setDisabledList(org.disabledList, org.numDisabled);
}

void IRecorder::deleteData()
{
   setEnabledList(nullptr, 0);
   setDisabledList(nullptr, 0);
}

//------------------------------------------------------------------------------
// Set a list of 'n' of data records enabled for processing
//------------------------------------------------------------------------------
bool IRecorder::setEnabledList(const unsigned int* const list, const unsigned int n)
{
   // Remove the old list
   if (enabledList != nullptr) {
      delete[] enabledList;
      enabledList = nullptr;
   }
   numEnabled = 0;

   // Create the new list
   if (list != nullptr && n > 0) {
      enabledList = new unsigned int[n];
      for (unsigned int i = 0; i < n; i++) {
         enabledList[i] = list[i];
      }
      numEnabled = n;
   }

   return true;
}


//------------------------------------------------------------------------------
// Set a list of 'n' of data records disabled from being processed
//------------------------------------------------------------------------------
bool IRecorder::setDisabledList(const unsigned int* const list, const unsigned int n)
{
   // Remove the old list
   if (disabledList != nullptr) {
      delete[] disabledList;
      disabledList = nullptr;
   }
   numDisabled = 0;

   // Create the new list
   if (list != nullptr && n > 0) {
      disabledList = new unsigned int[n];
      for (unsigned int i = 0; i < n; i++) {
         disabledList[i] = list[i];
      }
      numDisabled = n;
   }

   return true;
}


//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------
bool IRecorder::setSlotEnabledList(const base::IList* const list)
{
   unsigned int n2{};
   unsigned int* p2{};

   // Items in the list ...
   std::size_t n {list->entries()};
   if (n > 0) {
      // get the numbers from the list and set them as the filter list
      auto p1 = new int[n];
      std::size_t n1 {list->getNumberList(p1, n)};
      if (n1 > 0) {
         // Make sure they're positive
         p2 = new unsigned int[n1];
         for (unsigned int i = 0; i < n1; i++) {
            if (p1[i] >= 0) {
               p2[n2++] = static_cast<unsigned int>(p1[i]);
            }
         }
      }
      delete[] p1;
      p1 = nullptr;
   }

   // Set the list --
   setEnabledList(p2, n2);
   if (p2 != nullptr) {
      delete[] p2;
      p2 = nullptr;
   }

   return true;
}

bool IRecorder::setSlotDisabledList(const base::IList* const list)
{
   unsigned int n2{};
   unsigned int* p2{};

   // Items in the list ...
   std::size_t n {list->entries()};
   if (n > 0) {
      // get the numbers from the list and set them as the filter list
      auto p1 = new int[n];
      std::size_t n1 {list->getNumberList(p1, n)};
      if (n1 > 0) {
         // Make sure they're positive
         p2 = new unsigned int[n1];
         for (unsigned int i = 0; i < n1; i++) {
            if (p1[i] >= 0) {
               p2[n2++] = static_cast<unsigned int>(p1[i]);
            }
         }
      }
      delete[] p1;
      p1 = nullptr;
   }

   // Set the list --
   setDisabledList(p2, n2);
   if (p2 != nullptr) {
      delete[] p2;
      p2 = nullptr;
   }

   return true;
}

}
}
