
#ifndef __mixr_base_IIoHandler_HPP__
#define __mixr_base_IIoHandler_HPP__

#include "mixr/base/Component.hpp"

namespace mixr {
namespace base {
class IIoData;

//------------------------------------------------------------------------------
// Class: IIoHandler
// Description: Handles the flow of data to/from devices.
//
//    Data flowing to/from devices is buffered using application specific
//    data objects (IIoData).
//
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: IIoHandler
// Slots: none
//------------------------------------------------------------------------------
// Notes:
//    1) Data received from the hardware and sent to the simulation
//       software is "input" data, and data received from the simulation
//       software and sent to the hardware is "output" data.
//
//    2) As an option, asynchronous processing of the flow of data to/from
//       devices be started by calling the startAsyncProcessing() method
//------------------------------------------------------------------------------
class IIoHandler : public Component
{
   DECLARE_SUBCLASS(IIoHandler, Component)

public:
   IIoHandler();

   // updates all of the input devices (call ignored if processing asynchronously)
   void inputDevices(const double dt)                   { if (!async()) inputDevicesImpl(dt); }

   // updates all of the output devices (call ignored if processing asunchronously)
   void outputDevices(const double dt)                  { if (!async()) outputDevicesImpl(dt); }

   // return the input data buffer
   IIoData* getInputData()                       { return getInputDataImpl();  }
   const IIoData* getInputData() const           { return getInputDataImpl();  }

   // return the output data buffer
   IIoData* getOutputData()                      { return getOutputDataImpl(); }
   const IIoData* getOutputData() const          { return getOutputDataImpl(); }

   // starts asynchronous processing of i/o (this is optional)
   void startAsyncProcessing()                          { startAsyncProcessingImpl(); }

private:
   // return state of asynchronous i/o processing
   virtual bool async() = 0;

   virtual void inputDevicesImpl(const double dt)  =0;
   virtual void outputDevicesImpl(const double dt) =0;

   // return input data buffer implementation
   virtual IIoData* getInputDataImpl() =0;
   virtual const IIoData* getInputDataImpl() const =0;

   // return output data buffer implementation
   virtual IIoData* getOutputDataImpl() =0;
   virtual const IIoData* getOutputDataImpl() const =0;

   // starts the processing of data to/from devices
   virtual void startAsyncProcessingImpl() = 0;
};

}
}

#endif
