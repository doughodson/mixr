
#ifndef __mixr_models_common_Antenna_HPP__
#define __mixr_models_common_Antenna_HPP__

#include "mixr/models/system/IScanGimbal.hpp"

#include "mixr/base/safe_queue.hpp"
#include "mixr/base/safe_stack.hpp"
#include "mixr/base/util/constants.hpp"

namespace mixr {
namespace base { class IAngle; class Boolean; class IFunction; class Identifier; class INumber; class IPower; }
namespace models {
class IPlayer;
class IRfSystem;

//------------------------------------------------------------------------------
// Class: Antenna
//
// Description: Simple Antenna model: provides rate & position servo
//              control, as well as 1, 2 and 4 bar scans
//
//
// Factory name: Antenna
// Slots:
//      polarization    <base::Identifier>      ! Antenna polarization  { none, vertical, horizontal, slant, RHC, LHC }
//                                              ! (default: none)
//
//      threshold       <base::IPower>          ! Antenna threshold  (default: 0.0)
//
//      gain            <base::INumber>         ! Gain (default: 1.0)              (no units)
//
//      gainPattern     <base::Function>        ! Gain pattern (base::Func1 or base::Func2) (db)
//                                              ! (default: 0)
//
//      gainPatternDeg  <base::Boolean>         ! Gain pattern is in degrees (true) or radians (false) (default: false)
//
//      recycle         <base::Boolean>         ! Recycle emissions flag (default: true)
//
//      beamWidth       <base::IAngle>          ! Beam Width  (must be greater than zero) (default: 3.5 degrees)
//                      <base::INumber>         ! Beam width in radians
//
//
// Note
//    1) Other defaults:
//          maxPlayersOfInterest: 200
//          playerOfInterestTypes: { air weapon }
//
//    2) When the Emission 'recycle' flag is enabled (default behavior), the
//       system will try to reuse Emission objects, which removes the overhead
//       of creating and deleting them.
//
//------------------------------------------------------------------------------
class Antenna final: public IScanGimbal
{
   DECLARE_SUBCLASS(Antenna, IScanGimbal)

public:
   enum class Polarization: int { NONE, VERTICAL, HORIZONTAL, SLANT, RHC, LHC };

public:
   Antenna();

   virtual void rfTransmit(Emission* const em);

   IRfSystem* getSystem()                                 { return sys; }
   const IRfSystem* getSystem() const                     { return sys; }
   virtual bool setSystem(IRfSystem* const);

   // System limits
   int getMaxEmissions() const                           { return MAX_EMISSIONS; }

   // Antenna polarization matching gain
   double getPolarizationGain(const Polarization) const;
   Polarization getPolarization() const                  { return polar; }

   // Antenna gain
   virtual double getGain() const                        { return gain; }

   // Gain pattern
   const base::IFunction* gainPatternTable() const       { return gainPattern; }
   bool isGainPatternDegrees() const                     { return gainPatternDeg; }

   // Antenna threshold (watts)
   double getTransmitThreshold() const                   { return threshold; }

   // Antenna effective area (m^2)
   static double getEffectiveArea(const double gain, const double lambda) {
      return (gain * lambda * lambda)/(4.0 * base::PI);
   }

   // Recycle emissions flag (reuse old emission structure instead of creating new ones)
   bool isEmissionRecycleEnabled() const                 { return recycle; }

   // Beam width (radians)
   double getBeamWidth() const                           { return beamWidth; }

   // Member functions
   virtual bool setPolarization(const Polarization p)    { polar = p; return true; }
   virtual bool setThreshold(const double);
   virtual bool setGain(const double);
   virtual bool setEmissionRecycleFlag(const bool enable);
   virtual bool setBeamWidth(const double radians);

   virtual bool setPolarization(base::Identifier* const);
   virtual bool setThreshold(base::IPower* const);
   virtual bool setGain(const base::INumber* const);
   virtual bool setGainPattern(base::IFunction* const);
   virtual bool setGainPatternDeg(const base::Boolean* const);
   virtual bool setRecycleFlg(const base::Boolean* const);
   virtual bool setBeamWidth(const base::IAngle* const);
   virtual bool setBeamWidth(const base::INumber* const);

   // event handler(s)
   virtual bool onRfEmissionReturnEventAntenna(Emission* const);

   bool onStartScanEvent(base::Integer* const bar) override;
   bool onEndScanEvent(base::Integer* const bar) override;

   bool onRfEmissionEvent(Emission* const) override;

   bool event(const int event, base::IObject* const obj = nullptr) override;
   void reset() override;

protected:
   void clearQueues();

   void process(const double dt) override;                   // phase 3

   bool shutdownNotification() override;

   base::safe_stack<Emission*> freeEmStack{MAX_EMISSIONS};   // free emission stack
   mutable long freeEmLock{};                                // semaphore to protect 'freeEmStack'

   base::safe_queue<Emission*> inUseEmQueue{MAX_EMISSIONS};  // in use emission queue
   mutable long inUseEmLock{};                               // semaphore to protect 'inUseEmQueue'

private:
   static const int MAX_EMISSIONS{10000};       // max size of emission queues and arrays

   IRfSystem* sys{};                            // assigned R/F system (e.g., sensor, radio)

   // antenna parameters
   Polarization polar{Polarization::NONE};      // polarization  (enum)
   double gain{1.0};                            // gain          (no units)
   base::IFunction* gainPattern{};              // gain pattern  (Function)

   double threshold{};                          // antenna threshold; don't send emission if
                                                // power is below this threshold (watts)

   double beamWidth{base::angle::D2RCC * 3.5};  // beamwidth (radians)

   bool gainPatternDeg{};                       // gain pattern is in degrees flag (else radians)

   bool recycle{true};                          // recycle emissions flag

private:
   // slot table helper methods
   bool setSlotPolarization(base::Identifier* const x)              { return setPolarization(x);   }
   bool setSlotThreshold(base::IPower* const x)                     { return setThreshold(x);      }
   bool setSlotGain(const base::INumber* const x)                   { return setGain(x);           }
   bool setSlotGainPattern(base::IFunction* const x)                { return setGainPattern(x);    }
   bool setSlotGainPatternDeg(const base::Boolean* const x)         { return setGainPatternDeg(x); }
   bool setSlotRecycleFlg(const base::Boolean* const x)             { return setRecycleFlg(x);     }
   bool setSlotBeamWidth(const base::IAngle* const x)               { return setBeamWidth(x);      }
   bool setSlotBeamWidth(const base::INumber* const x)              { return setBeamWidth(x);      }
};

}
}

#endif
