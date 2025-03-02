
#ifndef __mixr_linkage_IGenerator_HPP__
#define __mixr_linkage_IGenerator_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace base { class IIoData; class IIoDevice; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: IGenerator
//
// Description: Interface class that is used to generate both analog or signal
//              values to be stored into the applications i/o data buffer.
//------------------------------------------------------------------------------
class IGenerator : public base::IObject
{
   DECLARE_SUBCLASS(IGenerator, base::IObject)

public:
   IGenerator();

   // general purpose reset
   virtual void reset() =0;

   // generate a signal value to set in the input data buffer
   void processInputs(const double dt, base::IIoData* const inData)          { processInputsImpl(dt, inData); }

private:
   // implementation method for generating and setting a signal value in input data buffer
   virtual void processInputsImpl(const double dt, base::IIoData* const) =0;
};

}
}

#endif
