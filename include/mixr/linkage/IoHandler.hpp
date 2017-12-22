
#ifndef __mixr_linkage_IoHandler_H__
#define __mixr_linkage_IoHandler_H__

#include "mixr/base/concepts/linkage/AbstractIoHandler.hpp"

#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class PairStream; class Frequency; class Number; class AbstractIoData; }
namespace linkage {
class IoPeriodicThread;

//------------------------------------------------------------------------------
// Class: IoHandler
//
// Description: Abstract base class that implements some of AbstractIoHanders
//    functionality, and defines common code useful to subclasses.
//
//    This I/O handler class will contain (via the 'devices' slot) a number of
//    devices (AbstractIoDevice) to interface with the physical
//    devices (e.g., joysticks, linkage systems).  Typically these devices
//    will contain a number of adapters that control the flow of
//    data between a device and a data buffer (see AbstractIoAdapter).
//
//    For an example, see the example 'testIoLinkage'.
//
//
// Notes:
//    1) Data received from the hardware and sent to the simulation
//       software is "input" data, and data received from the simulation
//       software and sent to the hardware is "output" data.
//
//    2) The 'ioData' slot sets both the input and output handler pointers
//       to one combined I/O data buffer; or you can set just an input or
//       output data buffer; or set separate input/output data buffers, using
//       the 'inputData' and 'outputData' slots.
//
//    3) As an option, use createDataThread() to create a thread that will
//       call the I/O process functions for the devices; in which case, the
//       inputDevices() and outputDevices() are overridden.  This thread will
//       terminate when a SHUTDOWN_EVENT is sent to this object.
//
//
// Factory name: IoHandler
// Slots:
//    ioData      <IoData>       ! Combined input/output data (default: none)
//    inputData   <IoData>       ! Individual input data (default: none)
//    outputData  <IoData>       ! Individual output data (default: none)
//    devices     <PairStream>   ! List of I/O devices (AbstractIoDevice objects) (default: none)
//    rate        <Frequency>    ! Optional thread's update rate (default: 50hz)
//    priority    <Number>       ! Optional thread's priority: lowest(0.0) to highest(1.0)  (default: 0.5 )
//
//------------------------------------------------------------------------------
class IoHandler : public base::AbstractIoHandler
{
   DECLARE_SUBCLASS(IoHandler, base::AbstractIoHandler)

public:
   IoHandler();

   void reset() override;

protected:
   // using adapters, reads inputs from device to AbstractIoData
   void readDeviceInputs(const double dt);
   // using adapters, writes data from AbstractIoData to device
   void writeDeviceOutputs(const double dt);

   bool shutdownNotification() override;

private:
   // return state of asynchronous periodic i/o processing
   bool async() override                                             { return periodicThread != nullptr; }

   // return input data buffer implementation
   base::AbstractIoData* getInputDataImpl() override                 { return inData; }
   const base::AbstractIoData* getInputDataImpl() const override     { return inData; }

   // return output data buffer implementation
   base::AbstractIoData* getOutputDataImpl() override                { return outData; }
   const base::AbstractIoData* getOutputDataImpl() const override    { return outData; }

   // create thread(s) to process i/o asynchronous
   void startAsyncProcessingImpl() override;

   friend class IoThread;
   double getPriority() const     { return pri;  }      // Thread priority (0 low to 1 high)
   double getRate() const         { return rate; }      // Thread rate (hz)

   // data i/o
   base::safe_ptr<base::AbstractIoData> inData;         // "input" data received from the hardware
   base::safe_ptr<base::AbstractIoData> outData;        // "output" data sent to the hardware
   base::safe_ptr<base::PairStream> devices;            // Device list

   double rate {50};                                    // Thread Rate (hz)
   double pri {0.5};                                    // Priority of the thread (0->lowest, 1->highest)
   base::safe_ptr<IoPeriodicThread> periodicThread;     // periodic thread to process I/O

private:
   // slot table helper methods
   bool setSlotIoData(base::AbstractIoData* const);
   bool setSlotInputData(base::AbstractIoData* const);
   bool setSlotOutputData(base::AbstractIoData* const);
   bool setSlotDevices(base::PairStream* const);
   bool setSlotRate(const base::Frequency* const);
   bool setSlotPriority(const base::Number* const);
};

}
}

#endif
