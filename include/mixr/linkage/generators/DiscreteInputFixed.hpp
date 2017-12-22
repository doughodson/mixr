
#ifndef __mixr_linkage_DiscreteInputFixed_H__
#define __mixr_linkage_DiscreteInputFixed_H__

#include "mixr/linkage/generators/AbstractGenerator.hpp"

namespace mixr {
namespace base { class AbstractIoData; class AbstractIoDevice; class Number; class Angle; class Frequency; class String; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: DiscreteInputFixed
//
// Description: Discrete input signal generator.  Generates either 'on' or 'off'
//              values.
//
// Factory name: DiscreteInputFixed
// Slots:
//      signal    <Identifier>   ! Signal type { ON, OFF }
//                               !  (default: OFF )
//
//      di        <Number>       ! AbstractIoData's DI channel index
//------------------------------------------------------------------------------
class DiscreteInputFixed final: public AbstractGenerator
{
   DECLARE_SUBCLASS(DiscreteInputFixed, AbstractGenerator)

public:
   enum class Signal { ON, OFF };

public:
   DiscreteInputFixed();

   void reset() final    {}

private:
   void processInputsImpl(const double dt, base::AbstractIoData* const) final;

   // AbstractIoData's AI channel index
   int getChannel() const                              { return channel; }
   bool setChannel(const int x)                        { channel = x; return true; }
   int channel {};

   Signal signal {Signal::OFF};  // Signal type

   bool setSignalType(const Signal x)                  { signal = x;   return true; }

private:
   // slot table helper methods
   bool setSlotChannel(const base::Number* const);
   bool setSlotSignal(const base::String* const);
};

}
}

#endif
