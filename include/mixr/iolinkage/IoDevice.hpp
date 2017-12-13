
#ifndef __mixr_linkage_IoDevice_H__
#define __mixr_linkage_IoDevice_H__

#include "mixr/base/concepts/linkage/AbstractIoDevice.hpp"

#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class PairStream; class AbstractIoData; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: IoDevice
//
// Description: Base class to manage a list of adapters to map and shape
//              data values between a data buffer and a device. Subclasses
//              only need to implement methods to read and write to a
//              particular device.
//
// Slots:
//    adapters <PairStream>   A list of adapters
//------------------------------------------------------------------------------
class IoDevice : public base::AbstractIoDevice
{
    DECLARE_SUBCLASS(IoDevice, base::AbstractIoDevice)

public:
   IoDevice();

private:
   void processInputsImpl(const double, base::AbstractIoData* const inData) final {
      readInputs();
      processInputAdapters(inData);
   }

   void processOutputsImpl(const double, const base::AbstractIoData* const outData) final {
      processOutputAdapters(outData);
      writeOutputs();
   }

   void processInputAdapters(base::AbstractIoData* const);
   void processOutputAdapters(const base::AbstractIoData* const);

   virtual void readInputs() = 0;
   virtual void writeOutputs() = 0;

   base::safe_ptr<base::PairStream> adapters;     // list of adapters

private:
   // slot table helper methods
   bool setSlotAdapters(base::PairStream* const);
};

}
}

#endif
