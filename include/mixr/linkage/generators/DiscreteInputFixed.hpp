
#ifndef __mixr_linkage_DiscreteInputFixed_HPP__
#define __mixr_linkage_DiscreteInputFixed_HPP__

#include "mixr/linkage/generators/IGenerator.hpp"

namespace mixr {
namespace base { class AbstractIoData; class AbstractIoDevice; class Angle; class Frequency; class Integer; class Identifier; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: DiscreteInputFixed
//
// Description: Discrete input signal generator.  Generates either 'on' or 'off'
//              values.
//
// Factory name: DiscreteInputFixed
// Slots:
//      signal    <Identifier>   ! Type identifiers { on, off }
//                               !  (default: off )
//
//      di        <Integer>      ! AbstractIoData's DI channel index
//------------------------------------------------------------------------------
class DiscreteInputFixed final: public IGenerator
{
   DECLARE_SUBCLASS(DiscreteInputFixed, IGenerator)

public:
   enum class Signal { ON, OFF };

public:
   DiscreteInputFixed();

   void reset() final    {}

private:
   void processInputsImpl(const double dt, base::IIoData* const) final;

   // AbstractIoData's AI channel index
   int getChannel() const                              { return channel; }
   bool setChannel(const int x)                        { channel = x; return true; }
   int channel{};

   Signal signal{Signal::OFF};  // Signal type

   bool setSignalType(const Signal x)                  { signal = x;   return true; }

private:
   // slot table helper methods
   bool setSlotChannel(const base::Integer* const);
   bool setSlotSignal(const base::Identifier* const);
};

}
}

#endif
