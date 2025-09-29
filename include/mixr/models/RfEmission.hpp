
#ifndef __mixr_models_RfEmission_HPP__
#define __mixr_models_RfEmission_HPP__

#include "mixr/models/ISensorMsg.hpp"
#include "mixr/models/system/Antenna.hpp"

namespace mixr {
namespace models {
class IRfSystem;

//------------------------------------------------------------------------------
// Class: RfEmission
//
// Description: Concrete RF Emission class -- Used to pass RF emission data
//              between antennas and players.
// Factory name: RfEmission
//------------------------------------------------------------------------------
class RfEmission final: public ISensorMsg
{
   DECLARE_SUBCLASS(RfEmission, ISensorMsg)

public:
   // ECM enumerations (can be expanded by derived classes)
   enum {
      ECM_OFF,
      ECM_NOISE,
      ECM_LAST       // Hook for subclasses to expand
   };

public:
   RfEmission();

   // Frequency (hz)
   double getFrequency() const                                { return freq; }

   // Wavelength (meters)
   double getWavelength() const                               { return lambda; }

   // Sets the frequency (hz) [and wavelength (meters)]
   void setFrequency(const double f) {
      if (f > 0.0) {
         freq = f;
         lambda = static_cast<float>(base::LIGHTSPEED)/f;
      }
   }

   // Sets the wavelength (meters) [and Frequency (hz)]
   void setWavelength(const double l) {
      if (l > 0.0) {
         lambda = l;
         freq = static_cast<float>(base::LIGHTSPEED)/l;
      }
   }

   // Bandwidth (hz)
   double getBandwidth() const                                { return bw; }

   // Sets the beam width (hz)
   void setBandwidth(const double f)                          { bw = f; }

   // Pulse width (sec)
   double getPulseWidth() const                               { return pw; }

   // Sets the pulse width (sec)
   void setPulseWidth(const double f)                         { pw = f; }

   // Pulse Repetition Freq (hz)
   double getPRF() const                                      { return prf; }

   // Sets the PRF (hz)
   void setPRF(const double f)                                { prf = f; }

   // Number of pulses in this emission packet
   //  (e.g., at 50HZ and a PRF of 2000hz, there are 40 pulses per emission instance)
   unsigned int getPulses() const                             { return pulses; }

   // Sets the number of pulses in this emission packet
   void setPulses(const unsigned int i)                       { pulses = i; }

   // Effective Radiated Power (ERP) (watts)
   double getPower() const                                    { return power; }

   // Sets the Effective Radiated Power (ERP) (watts)
   void setPower(const double f)                              { power = f; }

   // (one way) Range loss (no qty)
   double getRangeLoss() const                                { return lossRng; }

   // Antenna Polarization (see the Antenna class)
   Antenna::Polarization getPolarization() const              { return polar; }

   // Sets the antenna Polarization (see the Antenna class)
   void setPolarization(const Antenna::Polarization p)        { polar = p; }

   // Effective antenna gain (no qty)
   double getGain() const                                     { return gain; }

   // Sets the effective antenna gain (no qty)
   void setGain(const double g)                               { gain = g; }

   // Doppler shift (hz)
   // Source: S.A. Hovanessian, Radar Detection & Tracking Systems (Equation 1-3)
   double getDopplerShift() const  {
      if (lambda > 0.0f) return (-2.0 * getRangeRate()/lambda);
      else return 0.0;
   }

   // Atmospheric attenuation loss (no qty)
   double getAtmosphericAttenuationLoss() const               { return lossAtmos; }

   // Sets the atmospheric attenuation loss (no qty)
   void setAtmosphericAttenuationLoss(const double loss)      { lossAtmos = loss; }

   // Returns the transmit loss (no qty)
   double getTransmitLoss() const                             { return lossXmit; }

   // Sets the transmit loss (no qty)
   void setTransmitLoss(const double v)                       { lossXmit = v; }

   // Target's Radar Cross Section (RCS) as a function of this emission data (m^2)
   double getRCS() const                                      { return rcs; }

   // Sets the Radar Cross Section (RCS)
   void setRCS(const double f)                                { rcs = f; }

   // Pointer to the R/F system that transmitted this emission
   IRfSystem* getTransmitter()                                 { return transmitter; }

   // Pointer to the R/F system that transmitted this emission
   const IRfSystem* getTransmitter() const                     { return transmitter; }

   // Sets the pointer to the R/F system that transmitted this emission
   void setTransmitter(IRfSystem* const t);

   // ECM emission flag (this is an ECM emission)
   bool isECM() const                                         { return (ecmFlag!=ECM_OFF); }
   virtual bool isECMType(const unsigned int ecm) const       { return (ecmFlag==ecm); }

   // Sets the ECM emission flag
   virtual void setECM(const unsigned int b)                  { ecmFlag = b; }

   void setRange(const double r) override;   // Sets the range to the target (meters) (which we use to set the range loss)
   void clear() override;                    // Clear this emission's data

private:
   double freq{};           // Frequency                        (Hz)
   double lambda{};         // Wavelength                       (meters)
   double pw{};             // Pulse Width                      (Sec)
   double bw{};             // Bandwidth                        (Hz)
   double prf{};            // Pulse Repetition Frequency       (Hz)
   unsigned int pulses{1};  // Number of Pulses this packet     (no qty)
   double power{};          // Effective Radiated Power (erp)   (Watts)
   double gain{};           // Effective antenna gain           (no qty)
   double lossRng{1.0};     // Loss due to range                (m^-2)
   double lossAtmos{1.0};   // Atmospheric Attenuation Loss     (no qty)
   double lossXmit{1.0};    // Transmit loss (default: 1.0)     (no qty)
   double rcs{};            // Radar Cross Section (RCS)        (m^2)
   Antenna::Polarization polar{Antenna::Polarization::NONE};  // Antenna polarization   (enum)
   IRfSystem* transmitter{};         // The system that transmitted the emission
   unsigned int ecmFlag{ECM_OFF};    // ECM enumeration
};

}
}

#endif
