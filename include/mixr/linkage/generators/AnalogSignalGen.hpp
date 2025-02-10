
#ifndef __mixr_linkage_AnalogSignalGen_HPP__
#define __mixr_linkage_AnalogSignalGen_HPP__

#include "mixr/linkage/generators/IGenerator.hpp"

namespace mixr {
namespace base { class IIoData; class IIoDevice; class Angle; class Frequency; class Integer; class Identifier; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: AnalogSignalGen
//
// Description: Analog Input (AI) Signal Generator: sine, cosine, square and
//              saw tooth waves.
//
//  Cycle Angle     SINE    COSINE    SQUARE     SAW
//  -----------    ------   ------    ------    ------
//      0           0.0      1.0       1.0       0.0
//     90           1.0      0.0       1.0       0.25
//    180           0.0     -1.0       0.0       0.5
//    270          -1.0      0.0       0.0       0.75
//   <360           0.0      1.0       1.0       1.0
//
//
// Factory name: AnalogSignalGen
// Slots:
//      signal    <Identifier>   ! Type identifiers { sine, cosine, square, saw }
//                               !  (default: sine )
//
//      frequency <Frequency>    ! Signal frequency (should be <much> less
//                               !   than 1/2 of our core update rate)
//                               !   (default: 1/2 HZ or 2 seconds per cycle)
//
//      phase     <Angle>        ! Phase shift (default: 0 degrees)
//
//      ai        <Integer>    Integer  ! AbstractIoData's AI channel index
//------------------------------------------------------------------------------
class AnalogSignalGen final: public IGenerator
{
   DECLARE_SUBCLASS(AnalogSignalGen, IGenerator)

public:
   enum class Signal { SINE, COSINE, SQUARE, SAW };

public:
   AnalogSignalGen();

   void reset() final;

private:
   void processInputsImpl(const double dt, base::IIoData* const) final;

   // AbstractIoData's AI channel index
   int getChannel() const                      { return channel; }
   bool setChannel(const int x)                { channel = x; return true; }
   int channel {};

   Signal signal {Signal::SINE};  // Signal type
   double phase {};               // Phase shift (rad)
   double freq {};                // Frequency (rad/sec)
   double time {};                // Time since last reset

   Signal getSignalType() const                { return signal;   }
   double getFrequency() const                 { return freq;     }     // Hz
   double getPhase() const                     { return phase;    }     // Radians

   bool setSignalType(const Signal x)          { signal = x;   return true; }
   bool setFrequency(const double x)           { freq = x;     return true; }  // Hz
   bool setPhase(const double x)               { phase = x;    return true; }  // Radians

   double calc(const double dt);

private:
   // slot table helper methods
   bool setSlotChannel(const base::Integer* const);
   bool setSlotSignal(const base::Identifier* const);
   bool setSlotFrequency(const base::Frequency* const);
   bool setSlotPhase(const base::Angle* const);
};

}
}

#endif
