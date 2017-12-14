
#ifndef __mixr_base_AbstractIoDevice_H__
#define __mixr_base_AbstractIoDevice_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base {
class AbstractIoData;

//------------------------------------------------------------------------------
// Class: AbstractIoDevice
//
// Description: Abstract interface to an I/O device
//
//    I/O devices communicate with I/O hardware, usually via device drivers.
//    Most of the time, these drivers are operating specific.  This class
//    abstracts communication to I/O devices for both different devices
//    and different platforms.
//
//    I/O devices are typically managed by a handler class (AbstractIoHander)
//    which provides either synchronous or asynchronous movement of data to/from
//    data buffers (AbstractIoData) and devices (AbstractIoDevice).
//
// Notes:
//    1) All Channel (and port) numbers start with zero;
//       example: AI channels range from 0 to (getNumAnalogInputs() - 1)
//
//    2) Use the reset() function to open the device, and the
//       deleteData() function, base::Object::deleteData(), to close the
//       device (both as required).
//
//    3) Both processInputs() and processOutputs() are passed an I/O data
//       buffer that are application (not device) specific.  This buffer
//       should be passed to the I/O adapters.
//
//------------------------------------------------------------------------------
class AbstractIoDevice : public Object
{
   DECLARE_SUBCLASS(AbstractIoDevice, Object)

public:
   AbstractIoDevice();

   virtual void reset() =0;

   // DI methods
   virtual int getNumDiscreteInputChannels() const =0;
   virtual int getNumDiscreteInputPorts() const =0;
   virtual bool getDiscreteInput(bool* const value, const int channel, const int port) const =0;

   // DO methods
   virtual int getNumDiscreteOutputChannels() const =0;
   virtual int getNumDiscreteOutputPorts() const =0;
   virtual bool setDiscreteOutput(const bool value, const int channel, const int port) =0;

   // AI methods
   virtual int getNumAnalogInputs() const =0;
   virtual bool getAnalogInput(double* const value, const int channel) const =0;

   // AO methods
   virtual int getNumAnalogOutputs() const =0;
   virtual bool setAnalogOutput(const double value, const int channel) =0;

   // process device inputs
   void processInputs(const double dt, AbstractIoData* const inData)                   { processInputsImpl(dt, inData);   }
   // process device outputs
   void processOutputs(const double dt, const AbstractIoData* const outData)           { processOutputsImpl(dt, outData); }

private:
   virtual void processInputsImpl(const double dt, AbstractIoData* const inData) =0;
   virtual void processOutputsImpl(const double dt, const AbstractIoData* const outData) =0;
};

}
}

#endif
