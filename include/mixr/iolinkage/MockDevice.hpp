
#ifndef __mixr_iolinkage_MockDevice_H__
#define __mixr_iolinkage_MockDevice_H__

#include "mixr/base/concepts/io/AbstractIoDevice.hpp"

#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class PairStream; class AbstractIoData; }
namespace iolinkage {

//------------------------------------------------------------------------------
// Class: MockDevice
//
// Description: This is not a real physical device, it's a fake one. It acts
//              much like a NULL or fake device concerning outputs. For inputs,
//              it generates signal values by executing the list of generators
//              provided.
//
// Factory name: MockIoDevice
//
// Slots:
//    generators <PairStream>   : A list of generators
//------------------------------------------------------------------------------
class MockDevice : public base::AbstractIoDevice
{
    DECLARE_SUBCLASS(MockDevice, base::AbstractIoDevice)

public:
   MockDevice();

   void reset() override                                                                          {}

   // DI methods
   int getNumDiscreteInputChannels() const override                                               { return 0; }
   int getNumDiscreteInputPorts() const override                                                  { return 0; }
   bool getDiscreteInput(bool* const value, const int channel, const int port) const override     { return false; }

   // DO methods
   int getNumDiscreteOutputChannels() const override                                              { return 0; }
   int getNumDiscreteOutputPorts() const override                                                 { return 0; }
   bool setDiscreteOutput(const bool value, const int channel, const int port) override           { return false; }

   // AI methods
   int getNumAnalogInputs() const override                                                        { return 0; }
   bool getAnalogInput(double* const value, const int channel) const override                     { return false; }

   // AO methods
   int getNumAnalogOutputs() const override                                                       { return 0;     }
   bool setAnalogOutput(const double value, const int channel) override                           { return false; }

private:
   // mock device executes all generators to create values to store in input data buffer
   void processInputsImpl(const double dt, base::AbstractIoData* const inData) override;
   // mock device looks like a null device, it has no output
   void processOutputsImpl(const double dt, const base::AbstractIoData* const outData) override       { }

   base::safe_ptr<base::PairStream> generators;   // list of adapters used to generate values

private:
   // slot table helper methods
   bool setSlotGenerators(base::PairStream* const);
};

}
}

#endif
