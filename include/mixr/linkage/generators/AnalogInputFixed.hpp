
#ifndef __mixr_linkage_AnalogInputFixed_H__
#define __mixr_linkage_AnalogInputFixed_H__

#include "mixr/linkage/generators/AbstractGenerator.hpp"

namespace mixr {
namespace base { class AbstractIoData; class Number; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: AnalogInputFixed
//
// Description: Analog input signal generator to generator fixed values.
//
// Factory name: AnalogInputFixed
// Slots:
//      ai        <Number>       ! AbstractIoData's AI channel index
//      value     <Number>       ! Default is 0.0
//------------------------------------------------------------------------------
class AnalogInputFixed final: public AbstractGenerator
{
   DECLARE_SUBCLASS(AnalogInputFixed, AbstractGenerator)

public:
   AnalogInputFixed();

   void reset() final    {}

private:
   void processInputsImpl(const double dt, base::AbstractIoData* const) final;

   // AbstractIoData's AI channel index
   int getChannel() const                              { return channel; }
   bool setChannel(const int x)                        { channel = x; return true; }
   int channel{};

   // value that will be set to channel index
   double getValue() const                             { return channel; }
   bool setValue(const double x)                       { value = x; return true; }
   double value{};

private:
   // slot table helper methods
   bool setSlotChannel(const base::Number* const);
   bool setSlotValue(const base::Number* const);
};

}
}

#endif
