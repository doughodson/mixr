
#ifndef __mixr_base_AbstractIoHandler_H__
#define __mixr_base_AbstractIoHandler_H__

#include "mixr/base/Component.hpp"

namespace mixr {
namespace base {
class AbstractIoData;

//------------------------------------------------------------------------------
// Class: AbstractIoHandler
//
// Description: Handles the flow of data to/from devices.
//
//    Data flowing to/from devices is buffered using application specific
//    data objects (AbstractIoData).
//
// Notes:
//    1) Data received from the hardware and sent to the simulation
//       software is "input" data, and data received from the simulation
//       software and sent to the hardware is "output" data.
//
//    2) As an option, asynchronous processing of the flow of data to/from
//       devices be started by calling the startAsyncProcessing() method
//
//------------------------------------------------------------------------------
class AbstractIoHandler : public Component
{
   DECLARE_SUBCLASS(AbstractIoHandler, Component)

public:
   AbstractIoHandler();

   // updates all of the input devices (call ignored if processing asynchronously)
   void inputDevices(const double dt)                   { if (!async()) inputDevicesImpl(dt); }

   // updates all of the output devices (call ignored if processing asunchronously)
   void outputDevices(const double dt)                  { if (!async()) outputDevicesImpl(dt); }

   // return the input data buffer
   AbstractIoData* getInputData()                       { return getInputDataImpl();  }
   const AbstractIoData* getInputData() const           { return getInputDataImpl();  }

   // return the output data buffer
   AbstractIoData* getOutputData()                      { return getOutputDataImpl(); }
   const AbstractIoData* getOutputData() const          { return getOutputDataImpl(); }

   // starts asynchronous processing of i/o (this is optional)
   void startAsyncProcessing()                          { startAsyncProcessingImpl(); }

private:
   // return state of asynchronous i/o processing
   virtual bool async() = 0;

   virtual void inputDevicesImpl(const double dt)  =0;
   virtual void outputDevicesImpl(const double dt) =0;

   // return input data buffer implementation
   virtual AbstractIoData* getInputDataImpl() =0;
   virtual const AbstractIoData* getInputDataImpl() const =0;

   // return output data buffer implementation
   virtual AbstractIoData* getOutputDataImpl() =0;
   virtual const AbstractIoData* getOutputDataImpl() const =0;

   // starts the processing of data to/from devices
   virtual void startAsyncProcessingImpl() = 0;
};

}
}

#endif
