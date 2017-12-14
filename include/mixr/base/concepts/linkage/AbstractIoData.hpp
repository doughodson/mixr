
#ifndef __mixr_base_AbstractIoData_H__
#define __mixr_base_AbstractIoData_H__

#include "mixr/base/Object.hpp"

namespace mixr {
namespace base {

//------------------------------------------------------------------------------
// Class: AbstractIoData
// Description: I/O data buffer
//
//    Abstract interface to I/O data organized as channels that flow to/from
//    devices.
//
//    The main input and output data buffers are typically managed by an I/O
//    handler class (see AbstractIoHander).
//
// Notes:
//    1) Inputs from hardware are called Analog Inputs (AI) and Discrete Inputs (DI)
//
//    2) Outputs to hardware are called Analog Outputs (AO) and Discrete Outputs (DO)
//
//    3) Channel numbering is application dependent; some applications may start
//       numbering at one(1) and some may start at zero(0).
//
//    4) All functions return true if successful.
//
//------------------------------------------------------------------------------
class AbstractIoData : public Object
{
   DECLARE_SUBCLASS(AbstractIoData, Object)

public:
   AbstractIoData();

   // ---
   // Input channels
   // ---
   virtual int getNumAnalogInputChannels() const =0;
   virtual int getNumDiscreteInputChannels() const =0;

   // Returns the value of the analog input 'channel' in 'value'
   virtual bool getAnalogInput(const int channel, double* const value) const =0;

   // Returns the state of the discrete input bit 'channel' in 'value'
   virtual bool getDiscreteInput(const int channel, bool* const value) const =0;

   // Sets an analog input 'channel' to 'value'.
   virtual bool setAnalogInput(const int channel, const double value) =0;

   // Sets the discrete input 'channel' to 'value'.
   virtual bool setDiscreteInput(const int channel, const bool value) =0;

   // ---
   // Output channels
   // ---

   virtual int getNumAnalogOutputChannels() const =0;
   virtual int getNumDiscreteOutputChannels() const =0;

   // Returns the value of the analog output 'channel' in 'value'
   virtual bool getAnalogOutput(const int channel, double* const value) const =0;

   // Returns the state of the output bit 'channel' in 'value'
   virtual bool getDiscreteOutput(const int channel, bool* const value) const =0;

   // Sets an analog output 'channel' to 'value'.
   virtual bool setAnalogOutput(const int channel, const double value) =0;

   // Sets the discrete output 'channel' to 'value'.
   virtual bool setDiscreteOutput(const int channel, const bool value) =0;

   // clear data
   virtual void clear() = 0;
};

}
}

#endif
