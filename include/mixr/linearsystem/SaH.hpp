
#ifndef __mixr_linearsystem_SaH_H__
#define __mixr_linearsystem_SaH_H__

#include "mixr/linearsystem/ScalerFunc.hpp"

namespace mixr {
namespace linearsystem {

//------------------------------------------------------------------------------
// Class: SaH
//
// Description: Sample and Hold.  The g() function's X input is sampled at
//              a rate of 'sampleRate' and held as the output Y until the
//              next sample is taken.
//
//
// Factory name: SaH
// Slots:
//    rate  <Frequency>  Sample rate
//    rate  <Number>     Sample rate Hz
//
//------------------------------------------------------------------------------
class SaH : public ScalerFunc
{
    DECLARE_SUBCLASS(SaH, ScalerFunc)

public:
   SaH();
   SaH(const unsigned int rate, const unsigned int sampleRate);

   unsigned int getSampleRate() const         { return sampleRate; } // hz

   // Time since last sample (sec) [ 0 .. 1/getSampleRate() ]
   double getAge() const                      { return time; }

   // Set Parameter
   virtual bool setSampleRate(const unsigned int);   // hz

   double g(const double x) override;
   bool isValid() const override;

protected:
   void initialize() override;

private:
   void initData();

   static const unsigned int ORDER{1};

   double       time {};       // Current time since last sample (s)
   double       stime {};      // Sample time: time of next sample (s)
   unsigned int sampleRate {}; // Sample rate (hz)

private:
   // slot table helper methods
   bool setSlotSampleRate(const base::Frequency* const);
   bool setSlotSampleRate(const base::Number* const);
};

}
}

#endif
