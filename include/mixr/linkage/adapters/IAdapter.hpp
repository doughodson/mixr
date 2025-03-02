
#ifndef __mixr_linkage_IAdapter_HPP__
#define __mixr_linkage_IAdapter_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace base { class IIoData; class IIoDevice; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: IAdapter
//
// Description: Interface class that manages the flow of individual I/O items
//              between the I/O devices (AbstractIoDevice) and the I/O data buffers
//              (AbstractIoData). AbstractIoAdapter objects are owned and managed
//              by I/O device objects (AbstractIoDevice).
//
//------------------------------------------------------------------------------
class IAdapter : public base::IObject
{
   DECLARE_SUBCLASS(IAdapter, base::IObject)

public:
   IAdapter();

   // Process input data item(s) from the input device to the input buffer
   void processInputs(
         const base::IIoDevice* const device,
         base::IIoData* const inData
      )                                        { processInputsImpl(device, inData); }

   // Process output data item(s) from the output buffer to the output device
   void processOutputs(
         const base::IIoData* const outData,
         base::IIoDevice* const device
      )                                         { processOutputsImpl(outData, device); }

private:
   // Process input data item(s) from the input device to the input buffer
   virtual void processInputsImpl(
         const base::IIoDevice* const device,
         base::IIoData* const inData
      ) =0;

   // Process output data item(s) from the output buffer to the output device
   virtual void processOutputsImpl(
         const base::IIoData* const outData,
         base::IIoDevice* const device
      ) =0;
};

}
}

#endif
