
#include "mixr/models/system/Antenna.hpp"
#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/RfSystem.hpp"
#include "mixr/models/Emission.hpp"
#include "mixr/models/Tdb.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"

#include "mixr/base/relations/Func1.hpp"
#include "mixr/base/relations/Func2.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Decibel.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/units/angles.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/units/powers.hpp"

#include "mixr/base/util/math_utils.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_PARTIAL_SUBCLASS(Antenna, "Antenna")

BEGIN_SLOTTABLE(Antenna)
    "polarization",         //  1: Antenna Polarization  { none, vertical, horizontal, slant, RHC, LHC }
    "threshold",            //  2: Antenna threshold                (base::Power)
    "gain",                 //  3: Gain                             (no units)
    "gainPattern",          //  4: Gain pattern (base::Func1 or base::Func2) (db)
    "gainPatternDeg",       //  5: Gain pattern in degrees flag (true: degrees, false(default): radians)
    "recycle",              //  6: Recycle emissions flag (default: true)
    "beamWidth",            //  7: Beam Width              (Angle) or (Number: Radian)
END_SLOTTABLE(Antenna)

BEGIN_SLOT_MAP(Antenna)
    ON_SLOT(1,  setSlotPolarization,      base::Identifier)
    ON_SLOT(2,  setSlotThreshold,         base::Power)
    ON_SLOT(3,  setSlotGain,              base::Number)
    ON_SLOT(4,  setSlotGainPattern,       base::Function)
    ON_SLOT(5,  setSlotGainPatternDeg,    base::Boolean)
    ON_SLOT(6,  setSlotRecycleFlg,        base::Boolean)
    ON_SLOT(7,  setSlotBeamWidth,         base::Angle)      // Check for base::Angle before base::Number
    ON_SLOT(7,  setSlotBeamWidth,         base::Number)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Antenna)
    ON_EVENT_OBJ(RF_EMISSION_RETURN,onRfEmissionReturnEventAntenna,Emission)
END_EVENT_HANDLER()

Antenna::Antenna()
{
   STANDARD_CONSTRUCTOR()
}

Antenna::Antenna(const Antenna& org)
{
    STANDARD_CONSTRUCTOR()
    copyData(org, true);
}

Antenna::~Antenna()
{
   STANDARD_DESTRUCTOR()
}

Antenna& Antenna::operator=(const Antenna& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

Antenna* Antenna::clone() const
{
    return new Antenna(*this);
}

void Antenna::copyData(const Antenna& org, const bool)
{
   BaseClass::copyData(org);

   setSystem(nullptr);
   polar = org.polar;
   threshold = org.threshold;
   gain = org.gain;
   gainPatternDeg = org.gainPatternDeg;

   if (org.gainPattern != nullptr) {
      base::Function* copy{org.gainPattern->clone()};
      setSlotGainPattern( copy );
      copy->unref();
   } else {
      setSlotGainPattern(nullptr);
   }

   recycle = org.recycle;
   beamWidth = org.beamWidth;
}

void Antenna::deleteData()
{
   setSystem(nullptr);
   setSlotGainPattern(nullptr);

   clearQueues();
}

void Antenna::reset()
{
    BaseClass::reset();
    clearQueues();
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool Antenna::shutdownNotification()
{
    setSystem(nullptr);
    clearQueues();

    return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void Antenna::process(const double dt)
{
   BaseClass::process(dt);

   // ---
   // Recycle emissions ...
   // Update emission queues: from 'in-use' to 'free'
   // ---
   if (recycle) {
      unsigned int n{inUseEmQueue.entries()};

      for (unsigned int i = 0; i < n; i++) {

         base::lock(inUseEmLock);
         Emission* em{inUseEmQueue.get()};
         base::unlock(inUseEmLock);

         if (em != nullptr && em->getRefCount() > 1) {
            // Others are still referencing the emission, put back on in-use queue
            base::lock(inUseEmLock);
            inUseEmQueue.put(em);
            base::unlock(inUseEmLock);
         } else if (em != nullptr && em->getRefCount() <= 1) {
            // No one else is referencing the emission, push to the free stack
            em->clear();
            base::lock(freeEmLock);
            if (freeEmStack.isNotFull()) freeEmStack.push(em);
            else em->unref();
            base::unlock(freeEmLock);
         }
      }
   }
}

//------------------------------------------------------------------------------
// setSystem() -- Set pointer to our companion system
//------------------------------------------------------------------------------
bool Antenna::setSystem(RfSystem* const s)
{
   if (sys != nullptr) sys->unref();
   sys = s;
   if (sys != nullptr) sys->ref();
   return true;
}

//------------------------------------------------------------------------------
// clearQueues() -- clear out all queues
//------------------------------------------------------------------------------
void Antenna::clearQueues()
{
   base::lock(freeEmLock);
   Emission* em{freeEmStack.pop()};
   while (em != nullptr) {
      em->unref();
      em = freeEmStack.pop();
   }
   base::unlock(freeEmLock);

   base::lock(inUseEmLock);
   em = inUseEmQueue.get();
   while (em != nullptr) {
      em->unref();
      em = inUseEmQueue.get();
   }
   base::unlock(inUseEmLock);
}

bool Antenna::setPolarization(base::Identifier* const x)
{
   if (x == nullptr) return false;

   bool ok{true};
   if (*x == "none")            ok = setPolarization(Polarization::NONE);
   else if (*x == "vertical")   ok = setPolarization(Polarization::VERTICAL);
   else if (*x == "horizontal") ok = setPolarization(Polarization::HORIZONTAL);
   else if (*x == "slant")      ok = setPolarization(Polarization::SLANT);
   else if (*x == "RHC")        ok = setPolarization(Polarization::RHC);
   else if (*x == "LHC")        ok = setPolarization(Polarization::LHC);
   else ok = false;

   return ok;
}

//------------------------------------------------------------------------------
// setSlotThreshold() -- converts a power to watts and sets our antenna threshold
//------------------------------------------------------------------------------
bool Antenna::setThreshold(base::Power* const p)
{
   bool ok{};
   const double x{p->getValueInWatts()};

   // Test and set the threshold
   if (x >= 0.0) ok = setThreshold(x);

   return ok;
}

//------------------------------------------------------------------------------
// setSlotGain() -- calls setGain()
//------------------------------------------------------------------------------
bool Antenna::setGain(const base::Number* const g)
{
   bool ok{};
   double value{-1.0};

   if (g != nullptr) {
      value = g->asDouble();
   }

   if (value >= 0.0) {
      ok = setGain(value);
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Antenna::setGain: invalid gain" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotGainPattern() -- sets our gain pattern
//------------------------------------------------------------------------------
bool Antenna::setGainPattern(base::Function* const tbl)
{
    bool ok{true};
    if (gainPattern != nullptr) gainPattern->unref();
    gainPattern = tbl;
    if (gainPattern != nullptr) gainPattern->ref();
    return ok;
}

//------------------------------------------------------------------------------
// setSlotGainPatternDeg() -- sets the gain pattern is in degrees flag
//------------------------------------------------------------------------------
bool Antenna::setGainPatternDeg(const base::Boolean* const msg)
{
    bool ok{true};
    if (msg != nullptr) {
        gainPatternDeg = msg->asBool();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotRecycleFlg() -- sets the emission recycle flag
//------------------------------------------------------------------------------
bool Antenna::setRecycleFlg(const base::Boolean* const msg)
{
    bool ok{true};
    if (msg != nullptr) {
        ok = setEmissionRecycleFlag( msg->asBool() );
    }
    return ok;
}

//------------------------------------------------------------------------------
// Sets beam width as an base::Angle
//------------------------------------------------------------------------------
bool Antenna::setBeamWidth(const base::Angle* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setBeamWidth( msg->getValueInRadians() );
      if (!ok) {
         std::cerr << "Antenna::setSlotBeamWidth: Error setting beam width!" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets beam width in radians
//------------------------------------------------------------------------------
bool Antenna::setBeamWidth(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setBeamWidth( msg->asDouble() );
      if (!ok) {
         std::cerr << "Antenna::setSlotBeamWidth: Error setting beam width!" << std::endl;
      }

   }
   return ok;
}

//------------------------------------------------------------------------------
// setThreshold() - sets our antenna threshold
//------------------------------------------------------------------------------
bool Antenna::setThreshold(const double th)
{
    bool ok{};
    if (th > 0.0) {
        threshold = th;
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setGain: sets the gain
//------------------------------------------------------------------------------
bool Antenna::setGain(const double value)
{
    bool ok{};
    if (value >= 0.0) {
        gain = value;
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setEmissionRecycleFlag: sets the recycle flag
//------------------------------------------------------------------------------
bool Antenna::setEmissionRecycleFlag(const bool enable)
{
   recycle = enable;
   return true;
}

//------------------------------------------------------------------------------
// Sets the beam width (radians; must be greater than 0)
//------------------------------------------------------------------------------
bool Antenna::setBeamWidth(const double radians)
{
   bool ok{};
   if (radians > 0.0) {
      beamWidth = radians;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// TRANSMIT AND RECEIVE FUNCTION (SENSOR STUFF)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// rfTransmit() -- Transmit a RF emission packet at all active players.
//------------------------------------------------------------------------------
void Antenna::rfTransmit(Emission* const xmit)
{
   // Need something to transmit and someone to send to
   Tdb* tdb{getCurrentTDB()};
   Player* ownship{getOwnship()};
   if (xmit == nullptr || tdb == nullptr || ownship == nullptr) {
      // Clean up and leave
      if (tdb != nullptr) tdb->unref();
      return;
   }

   // ---
   // Compute gimbal boresight data for our targets
   // ---
   unsigned int ntgts{tdb->computeBoresightData()};
   if (ntgts > MAX_PLAYERS) ntgts = MAX_PLAYERS;

   // ---
   // If we have targets
   // ---
   const base::Vec3d* losG{tdb->getGimbalLosVectors()};
   if (ntgts > 0 && losG != nullptr) {

      // ---
      // Lookup gain from antenna gain pattern, compute antenna
      // effective gain and effective radiated power.
      // ---
      bool haveGainTgt{};
      double gainTgt[MAX_PLAYERS]{};
      if (gainPattern != nullptr) {
         const auto gainFunc1 = dynamic_cast<base::Func1*>(gainPattern);
         const auto gainFunc2 = dynamic_cast<base::Func2*>(gainPattern);
         if (gainFunc2 != nullptr) {
            // ---
            // Antenna pattern: 2D table (az & el off antenna boresight)
            // ---

            // Compute azimuth off boresight (radians)
            const double* aazr{tdb->getBoresightAzimuthErrors()};

            // Compute elevation off boresight (radians)
            const double* aelr{tdb->getBoresightElevationErrors()};

            // Lookup gain in 2D table and convert from dB
            double gainTgt0[MAX_PLAYERS]{};
            if (gainPatternDeg) {
               for (unsigned int i1 = 0; i1 < ntgts; i1++) {
                  gainTgt0[i1] = gainFunc2->f( (aazr[i1] * base::angle::R2DCC), (aelr[i1] * base::angle::R2DCC) )/10.0;
               }
            } else {
               for (unsigned int i1 = 0; i1 < ntgts; i1++) {
                  gainTgt0[i1] = gainFunc2->f( aazr[i1], aelr[i1] )/10.0f;
               }
            }
            base::pow10Array(gainTgt0, gainTgt, ntgts);
            haveGainTgt = true;
         } else if (gainFunc1 != nullptr) {
            // ---
            // Antenna Pattern: 1D table (off antenna boresight only
            // ---

            // Compute angles off antenna boresight (radians)
            const double* aar{tdb->getBoresightErrorAngles()};

            // Lookup gain in 1D table and convert from dB
            double gainTgt0[MAX_PLAYERS]{};
            if (gainPatternDeg) {
               for (unsigned int i2 = 0; i2 < ntgts; i2++) {
                  gainTgt0[i2] = gainFunc1->f( aar[i2] * base::angle::R2DCC )/10.0;
               }
            } else {
               for (unsigned int i2 = 0; i2 < ntgts; i2++) {
                  gainTgt0[i2] = gainFunc1->f( aar[i2] )/10.0f;
               }
            }
            base::pow10Array(gainTgt0, gainTgt, ntgts);
            haveGainTgt = true;
         }
      }
      if (!haveGainTgt) {
         // ---
         // No antenna pattern table
         // ---
         for (unsigned int i = 0; i < ntgts; i++) {
            gainTgt[i] = 1.0;
         }
      }

      // Compute antenna effective gain
      double aeGain[MAX_PLAYERS]{};
      base::multArrayConst(gainTgt, getGain(), aeGain, ntgts);

      // Compute Effective Radiated Power (watts) (Equation 2-1)
      double erp[MAX_PLAYERS]{};
      base::multArrayConst(aeGain, xmit->getPower(), erp, ntgts);

      // Fetch the required data arrays from the TargetDataBlock
      const double* ranges{tdb->getTargetRanges()};
      const double* rngRates{tdb->getTargetRangeRates()};
      const base::Vec3d* losO2T{tdb->getLosVectors()};
      const base::Vec3d* losT2O{tdb->getTargetLosVectors()};
      Player** targets{tdb->getTargets()};

      // ---
      // Send emission packets to the targets
      // ---
      for (unsigned int i = 0; i < ntgts; i++) {

         // Only of power exceeds an optional threshold
         if (erp[i] > threshold) {

            // Get a free emission packet
            Emission* em{};
            if (recycle) {
               base::lock(freeEmLock);
               em = freeEmStack.pop();
               base::unlock(freeEmLock);
            }

            bool cloned{};
            if (em == nullptr) {
               // Otherwise, clone a new one
               em = xmit->clone();
               cloned = true;
            }

            // Send the emission to the other player
            if (em != nullptr) {

               // a) Copy the template emission
               if (!cloned) *em = *xmit;

               // b) Set target unique data
               em->setGimbal(this);
               em->setOwnship(ownship);

               em->setRange( static_cast<double>(ranges[i]) );
               em->setLosVec(losO2T[i]);
               em->setTgtLosVec(losT2O[i]);
               em->setRangeRate( static_cast<double>(rngRates[i]) );
               em->setTarget(targets[i]);

               em->setGimbalAzimuth( static_cast<double>(getAzimuth()) );
               em->setGimbalElevation( static_cast<double>(getElevation()) );
               em->setPower( static_cast<double>(erp[i]) );
               em->setGain( static_cast<double>(aeGain[i]) );
               em->setPolarization(getPolarization());
               em->setLocalPlayersOnly( isLocalPlayersOfInterestOnly() );

               // c) Send the emission to the target
               targets[i]->event(RF_EMISSION, em);

               // d) Recycle the emission (we don't bother)
               //bool recycled = false;
               //if (recycle) {
               //   base::lock(inUseEmLock);
               //   if (inUseEmQueue.isNotFull()) {
                     // Store for future reference
               //      inUseEmQueue.put(em);
               //      recycled = true;
               //   }
               //   base::unlock(inUseEmLock);
               //}

               // or just forget it
               //else {
                  em->unref();
               //}

            } else {
               // When we couldn't get a free emission packet
               if (isMessageEnabled(MSG_ERROR)) {
                  std::cerr << "Antenna: OUT OF EMISSIONS!" << std::endl;
               }
            }
         }

      }
   }

   // Unref() the TDB
   tdb->unref();
}

//------------------------------------------------------------------------------
// onStartScanEvent() -- process the start of a scan
//------------------------------------------------------------------------------
bool Antenna::onStartScanEvent(base::Integer* const bar)
{
   // Pass the event to our system
   RfSystem* p{getSystem()};
   if (p != nullptr) p->event(SCAN_START, bar);
   return true;
}

//------------------------------------------------------------------------------
// onEndScanEvent() -- process the end of a scan
//------------------------------------------------------------------------------
bool Antenna::onEndScanEvent(base::Integer* const bar)
{
   // Pass the event to our sensor
   RfSystem* p{getSystem()};
   if (p != nullptr) p->event(SCAN_END, bar);
   return true;
}

//------------------------------------------------------------------------------
// onRfEmissionEventAntenna() -- process events for RF Emission not sent by us.
//
// 1) Build a list of emission packets from the queue and compute the
//    Line-Of-Sight (LOS) vectors back to the transmitter.
//
// 2) Transform LOS vectors to antenna coordinates
//
// 3) Compute antenna gains in the direction of the transmitter
//
// 4) Compute Antenna Effective Gains
//
// 5) Compute Antenna Effective Area and Polarization Gains
//
// 6) Compute total receiving antenaa gain and send the emission to our sensor
//------------------------------------------------------------------------------
bool Antenna::onRfEmissionEvent(Emission* const em)
{
   // Is this emission from a player of interest?
   if (fromPlayerOfInterest(em)) {

      Player* ownship{getOwnship()};
      RfSystem* sys1{getSystem()};
      if (ownship != nullptr && sys1 != nullptr) {
         sys1->ref();

         // Line-Of-Sight (LOS) vectors back to the transmitter.
         const base::Vec3d xlos{em->getTgtLosVec()};
         const base::Vec4d los0( xlos.x(), xlos.y(), xlos.z(), 0.0);

         // 2) Transform local NED LOS vectors to antenna coordinates
         base::Matrixd mm{getRotMat()};
         mm *= ownship->getRotMat();
         base::Vec4d losA{mm * los0};

         // ---
         // Compute antenna gains in the direction of the transmitter
         // ---
         double rGainDb{};
         if (gainPattern != nullptr) {

            const auto gainFunc1 = dynamic_cast<base::Func1*>(gainPattern);
            const auto gainFunc2 = dynamic_cast<base::Func2*>(gainPattern);
            if (gainFunc2 != nullptr) {
               // ---
               // 3-a) Antenna pattern: 2D table (az & el off antenna boresight)
               // ---

               // Get component arrays and ground range squared
               const double xa{losA.x()};
               const double ya{losA.y()};
               const double za{-losA.z()};
               const double ra2{xa*xa + ya*ya};

               // Compute range along antenna x-y plane
               const double ra{std::sqrt(ra2)};

               // Compute azimuth off boresight
               const double aazr{std::atan2(ya,xa)};

               // Compute elevation off boresight
               const double aelr{std::atan2(za,ra)};

               // Lookup gain in 2D table and convert from dB
               if (gainPatternDeg)
                  rGainDb = gainFunc2->f( aazr * base::angle::R2DCC, aelr * base::angle::R2DCC );
               else
                  rGainDb = gainFunc2->f( aazr, aelr );

            } else if (gainFunc1 != nullptr) {
               // ---
               // 3-b) Antenna Pattern: 1D table (off antenna boresight only
               // ---

               // Compute angle off antenna boresight
               const double aar{std::acos(losA.x())};

               // Lookup gain in 1D table and convert from dB
               if (gainPatternDeg)
                  rGainDb = gainFunc1->f( aar * base::angle::R2DCC );
               else
                  rGainDb = gainFunc1->f(aar);

            }
         }

         // Compute off-boresight gain
         const double rGain{std::pow(10.0,rGainDb/10.0)};

         // Compute Antenna Effective Gain
         const double aeGain{rGain * getGain()};
         const double lambda{em->getWavelength()};
         const double aea{getEffectiveArea(aeGain, lambda)};

         const double pGain{getPolarizationGain(em->getPolarization())};
         const double raGain{aea * pGain};

         sys1->rfReceivedEmission(em, this, static_cast<double>(raGain));

         sys1->unref();
      }

   }

   return BaseClass::onRfEmissionEvent(em);
}

//------------------------------------------------------------------------------
// onRfEmissionReturnEventAntenna() -- process Returned RF Emission Events
//------------------------------------------------------------------------------
bool Antenna::onRfEmissionReturnEventAntenna(Emission* const em)
{
    bool used{};
    // Pass all returned emissions to our sensor
    RfSystem* sys1{getSystem()};
    if (sys1 != nullptr) {
        sys1->ref();

        // Compute antenna effective area
        double aea{getEffectiveArea(em->getGain(), em->getWavelength())};
        // Same antenna -- polarization match -- polarization gain is 1.0
        // So just use Antenna effective area
        sys1->rfReceivedEmission(em, this, static_cast<double>(aea));
        used = true;

        sys1->unref();
    }
    return used;
}

//------------------------------------------------------------------------------
// getPolarizationGain() -- Get the gain from the antenna polarization match
//------------------------------------------------------------------------------
double Antenna::getPolarizationGain(const Polarization p1) const
{
    const int n{static_cast<int>(Polarization::LHC) + 1};
    static double table[n][n] {
      //   NONE     VERTICAL   HORIZONTAL   SLANT       RHC        LHC
        {   1.0,       1.0,       1.0,       1.0,       1.0,       1.0 },     // NONE
        {   1.0,       1.0,       0.0,       0.5,       0.5,       0.5 },     // VERTICAL
        {   1.0,       0.0,       1.0,       0.5,       0.5,       0.5 },     // HORIZONTAL
        {   1.0,       0.5,       0.5,       1.0,       0.5,       0.5 },     // SLANT
        {   1.0,       0.5,       0.5,       0.5,       1.0,       0.0 },     // RHC
        {   1.0,       0.5,       0.5,       0.5,       0.0,       1.0 },     // LHC
    };
    return table[static_cast<int>(polar)][static_cast<int>(p1)];
}

}
}
