
#ifndef __mixr_models_common_Radar_HPP__
#define __mixr_models_common_Radar_HPP__

#include "mixr/models/system/IRfSensor.hpp"
#include "mixr/base/safe_queue.hpp"

#include <cmath>
#include <array>

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Radar
// Description: Interface for Radar models
//
// Default R/F sensor type ID is "RADAR"
//
// Factory name: Radar
// Slots:
//    igain    <base::INumber>    ! Integrator gain (no units; default: 1.0f)
//             <base::Decibel>    ! Integrator gain (dB)
//
//------------------------------------------------------------------------------
class Radar : public IRfSensor
{
   DECLARE_SUBCLASS(Radar, IRfSensor)

public:
   // Max number of reports (per scan)
   static const unsigned int MAX_REPORTS{MIXR_CONFIG_MAX_REPORTS};

   static const unsigned int NUM_SWEEPS{121};          // Number of sweeps in Real-Beam display
   static const unsigned int PTRS_PER_SWEEP{128};      // Number of points per sweep in RB display

public:
   Radar();

   const double* getSweep(const unsigned int n) const     { return (n < NUM_SWEEPS ?  sweeps[n] : 0); }
   const double* getClosure(const unsigned int n) const   { return (n < NUM_SWEEPS ?  vclos[n]  : 0); }
   unsigned int getNumSweeps() const                      { return NUM_SWEEPS; }
   unsigned int getPtrsPerSweep() const                   { return PTRS_PER_SWEEP; }

   unsigned int getMaxReports() const                     { return MAX_REPORTS; }
   unsigned int getNumReports() const                     { return numReports; }

   // Returns the number of emission reports, up to 'max', that are loaded into the 'list'
   // Emission pointers are pre-ref()'d, so unref() when finished.
   unsigned int getReports(const Emission** list, const unsigned int max) const;

   // For debugging purposes
   // returns the amount of jamming signal to be considered, 0 if no jamming
   double getRecvJamSignal() const                 { return currentJamSignal; }
   double getRecvJamSignalDb() const               { return 10.0f * std::log10(currentJamSignal); }

   // Returns integration gain
   double getIGain() const                         { return rfIGain; }

   // return the current number of emissions that have been jammed.
   int getNumberOfJammedEmissions() const          { return numberOfJammedEmissions; }

   // Sets integration gain
   virtual bool setIGain(const double);

   bool killedNotification(Player* const killedBy = nullptr) override;

   void updateData(const double dt = 0.0) override;
   void reset() override;

protected:
   bool onEndScanEvent(const base::Integer* const bar) override;

   void transmit(const double dt) override;
   void receive(const double dt) override;
   void process(const double dt) override;

   bool shutdownNotification() override;

protected: // (#temporary#) allow subclasses to access and use report queue

   // Semaphore to protect 'rptQueue', 'rptSnQueue', 'reports' and 'rptMaxSn'
   mutable long myLock {};

   // Queues
   base::safe_queue<Emission*> rptQueue {MAX_EMISSIONS};    // Reporting emission queue
   base::safe_queue<double> rptSnQueue {MAX_EMISSIONS};     // Reporting Signal/Nose queue  (dB)

   // Reports
   std::array<Emission*, MAX_REPORTS> reports {};  // Best emission for this report
   std::array<double, MAX_REPORTS> rptMaxSn {};    // Signal/Nose value (dB)
   unsigned int numReports {};                     // Number of reports this sweep

private:
   void clearTracksAndQueues();
   void clearSweep(const unsigned int i);
   void ageSweeps();
   unsigned int computeSweepIndex(const double az);
   unsigned int computeRangeIndex(const double rng);

   bool endOfScanFlg {};               // End of scan flag

   double sweeps[NUM_SWEEPS][PTRS_PER_SWEEP] {};
   double vclos[NUM_SWEEPS][PTRS_PER_SWEEP] {};
   int    csweep;                     // Current sweep

   double currentJamSignal {};
   int    numberOfJammedEmissions {};

   double rfIGain {1.0};              // Integrator gain (default: 1.0) (no units)

private:
   // slot table helper methods
   bool setSlotIGain(base::INumber* const);
};

}
}

#endif
