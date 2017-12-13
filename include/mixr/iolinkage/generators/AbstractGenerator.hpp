
#ifndef __mixr_linkage_AbstractGenerator_H__
#define __mixr_linkage_AbstractGenerator_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base { class AbstractIoData; class AbstractIoDevice; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: AbstractGenerator
//
// Description: Abstract class that is used to generate both analog or signal
//              values to be stored into the applications i/o data buffer.
//------------------------------------------------------------------------------
class AbstractGenerator : public base::Object
{
   DECLARE_SUBCLASS(AbstractGenerator, base::Object)

public:
   AbstractGenerator();

   // general purpose reset
   virtual void reset() =0;

   // generate a signal value to set in the input data buffer
   void processInputs(const double dt, base::AbstractIoData* const inData)          { processInputsImpl(dt, inData); }

private:
   // implementation method for generating and setting a signal value in input data buffer
   virtual void processInputsImpl(const double dt, base::AbstractIoData* const) =0;
};

}
}

#endif
