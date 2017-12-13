
#ifndef __mixr_linkage_AbstractAdapter_H__
#define __mixr_linkage_AbstractAdapter_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base { class AbstractIoData; class AbstractIoDevice; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: AbstractIoAdapter
//
// Description: Abstract class that manages the flow of individual I/O items
//              between the I/O devices (AbstractIoDevice) and the I/O data buffers
//              (AbstractIoData). AbstractIoAdapter objects are owned and managed
//              by I/O device objects (AbstractIoDevice).
//
//------------------------------------------------------------------------------
class AbstractAdapter : public base::Object
{
   DECLARE_SUBCLASS(AbstractAdapter, base::Object)

public:
   AbstractAdapter();

   // Process input data item(s) from the input device to the input buffer
   void processInputs(
         const base::AbstractIoDevice* const device,
         base::AbstractIoData* const inData
      )                                        { processInputsImpl(device, inData); }

   // Process output data item(s) from the output buffer to the output device
   void processOutputs(
         const base::AbstractIoData* const outData,
         base::AbstractIoDevice* const device
      )                                         { processOutputsImpl(outData, device); }

private:
   // Process input data item(s) from the input device to the input buffer
   virtual void processInputsImpl(
         const base::AbstractIoDevice* const device,
         base::AbstractIoData* const inData
      ) =0;

   // Process output data item(s) from the output buffer to the output device
   virtual void processOutputsImpl(
         const base::AbstractIoData* const outData,
         base::AbstractIoDevice* const device
      ) =0;
};

}
}

#endif
