
#include "mixr/linkage/IoDevice.hpp"

#include "mixr/linkage/adapters/AbstractAdapter.hpp"

#include "mixr/base/concepts/linkage/AbstractIoHandler.hpp"

#include "mixr/base/List.hpp"

#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

#include <iostream>

namespace mixr {
namespace linkage {

IMPLEMENT_ABSTRACT_SUBCLASS(IoDevice, "BaseIoDevice")
EMPTY_DELETEDATA(IoDevice)

BEGIN_SLOTTABLE(IoDevice)
   "adapters",               // 1) List of adapter objects (default: none)
END_SLOTTABLE(IoDevice)

BEGIN_SLOT_MAP(IoDevice)
   ON_SLOT(1, setSlotAdapters,    base::PairStream)
END_SLOT_MAP()

IoDevice::IoDevice()
{
   STANDARD_CONSTRUCTOR()
}

void IoDevice::copyData(const IoDevice& org, const bool)
{
   BaseClass::copyData(org);

   // clear our old lists of adapters and devices
   setSlotAdapters(nullptr);

   // ---
   // copy the list of adapters
   // ---
   if (org.adapters != nullptr) {
      const auto copy = static_cast<base::PairStream*>(org.adapters->clone());
      setSlotAdapters(copy);
      copy->unref();
   }
}

void IoDevice::processInputAdapters(base::AbstractIoData* const inData)
{
   // ### Since we'll process all of the input adapters, our derived I/O device
   // classes should process their device inputs BEFORE calling this base
   // class functions. ###

   // process any input adapters
   if (adapters != nullptr) {
      base::List::Item* item{adapters->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = static_cast<AbstractAdapter*>(pair->object());
         p->processInputs(this, inData);
         item = item->getNext();
      }
   }
}

void IoDevice::processOutputAdapters(const base::AbstractIoData* const outData)
{
   // ### Since we'll process all of the output I/O adapters, our derived I/O
   // device classes should process their device outputs AFTER calling this
   // base class functions. ###

   // process any output adapters
   if (adapters != nullptr) {

      if (outData != nullptr) {
         base::List::Item* item{adapters->getFirstItem()};
         while (item != nullptr) {
            const auto pair = static_cast<base::Pair*>(item->getValue());
            const auto p = static_cast<AbstractAdapter*>(pair->object());
            p->processOutputs(outData, this);
            item = item->getNext();
         }
      }
   }
}

// read list of adapters
bool IoDevice::setSlotAdapters(base::PairStream* const list)
{
   bool ok{true};

   if (list != nullptr) {
      // check to make sure all objects on the list are I/O adapters
      int cnt{};
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr) {
         cnt++;
         const auto pair = static_cast<base::Pair*>(item->getValue());
         ok = pair->object()->isClassType(typeid(AbstractAdapter));
         if (ok) {
//            static_cast<AbstractIoAdapter*>(pair->object())->container(this);
         } else {
            std::cerr << "IoDevice::setSlotAdapters(): Item number " << cnt;
            std::cerr << " on the list is a non-AbstractAdapter component!" << std::endl;
         }
         item = item->getNext();
      }
   }

   if (ok) adapters = list;

   return ok;
}

}
}
