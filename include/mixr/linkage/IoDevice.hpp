
#ifndef __mixr_linkage_IoDevice_HPP__
#define __mixr_linkage_IoDevice_HPP__

#include "mixr/base/concepts/linkage/IIoDevice.hpp"

#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class IPairStream; class AbstractIoData; }
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
class IoDevice : public base::IIoDevice
{
    DECLARE_SUBCLASS(IoDevice, base::IIoDevice)

public:
   IoDevice();

private:
   void processInputsImpl(const double, base::IIoData* const inData) final {
      readInputs();
      processInputAdapters(inData);
   }

   void processOutputsImpl(const double, const base::IIoData* const outData) final {
      processOutputAdapters(outData);
      writeOutputs();
   }

   void processInputAdapters(base::IIoData* const);
   void processOutputAdapters(const base::IIoData* const);

   virtual void readInputs() = 0;
   virtual void writeOutputs() = 0;

   base::safe_ptr<base::IPairStream> adapters;     // list of adapters

private:
   // slot table helper methods
   bool setSlotAdapters(base::IPairStream* const);
};

}
}

#endif
