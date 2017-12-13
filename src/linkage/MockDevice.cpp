
#include "mixr/linkage/MockDevice.hpp"

#include "mixr/linkage/generators/AbstractGenerator.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(MockDevice, "MockDevice")
EMPTY_DELETEDATA(MockDevice)

BEGIN_SLOTTABLE(MockDevice)
   "generators"              // 1) List of generator objects (default: none)
END_SLOTTABLE(MockDevice)

BEGIN_SLOT_MAP(MockDevice)
   ON_SLOT(1, setSlotGenerators,  base::PairStream)
END_SLOT_MAP()

MockDevice::MockDevice()
{
   STANDARD_CONSTRUCTOR()
}

void MockDevice::copyData(const MockDevice& org, const bool)
{
   BaseClass::copyData(org);

   // ---
   // copy the list of generators
   // ---
   if (org.generators != nullptr) {
      const auto copy = static_cast<base::PairStream*>(org.generators->clone());
      setSlotGenerators(copy);
      copy->unref();
   }

}

// Process device input channels
void MockDevice::processInputsImpl(const double dt, base::AbstractIoData* const inData)
{
   // process any input generators
   if (generators != nullptr) {
      base::List::Item* item{generators->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = static_cast<AbstractGenerator*>(pair->object());
         p->processInputs(dt, inData);
         item = item->getNext();
      }
   }
}

// read in list of generators
bool MockDevice::setSlotGenerators(base::PairStream* const list)
{
   bool ok{true};

   if (list != nullptr) {
      // check to make sure all objects on the list are I/O adapters
      int cnt{};
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr) {
         cnt++;
         const auto pair = static_cast<base::Pair*>(item->getValue());
         ok = pair->object()->isClassType(typeid(AbstractGenerator));
         if (ok) {
//            static_cast<AbstractIoAdapter*>(pair->object())->container(this);
         } else {
            std::cerr << "MockIoDevice::setSlotGenerators(): Item number " << cnt;
            std::cerr << " on the list is a non-AbstractGenerator component!" << std::endl;
         }
         item = item->getNext();
      }
   }

   if (ok) generators = list;

   return ok;
}

}
}
