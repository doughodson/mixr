
#ifndef __mixr_linkage_MockDevice_H__
#define __mixr_linkage_MockDevice_H__

#include "mixr/base/concepts/linkage/AbstractIoDevice.hpp"

#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class PairStream; class AbstractIoData; }
namespace linkage {

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
class MockDevice final: public base::AbstractIoDevice
{
    DECLARE_SUBCLASS(MockDevice, base::AbstractIoDevice)

public:
   MockDevice();

   void reset() final                                                                          {}

   // DI methods
   int getNumDiscreteInputChannels() const final                                               { return 0; }
   int getNumDiscreteInputPorts() const final                                                  { return 0; }
   bool getDiscreteInput(bool* const value, const int channel, const int port) const final     { return false; }

   // DO methods
   int getNumDiscreteOutputChannels() const final                                              { return 0; }
   int getNumDiscreteOutputPorts() const final                                                 { return 0; }
   bool setDiscreteOutput(const bool value, const int channel, const int port) final           { return false; }

   // AI methods
   int getNumAnalogInputs() const final                                                        { return 0; }
   bool getAnalogInput(double* const value, const int channel) const final                     { return false; }

   // AO methods
   int getNumAnalogOutputs() const final                                                       { return 0;     }
   bool setAnalogOutput(const double value, const int channel) final                           { return false; }

private:
   // mock device executes all generators to create values to store in input data buffer
   void processInputsImpl(const double dt, base::AbstractIoData* const inData) final;
   // mock device looks like a null device, it has no output
   void processOutputsImpl(const double dt, const base::AbstractIoData* const outData) final       { }

   base::safe_ptr<base::PairStream> generators;   // list of adapters used to generate values

private:
   // slot table helper methods
   bool setSlotGenerators(base::PairStream* const);
};

}
}

#endif
