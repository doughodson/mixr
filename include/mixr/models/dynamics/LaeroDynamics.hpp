
#ifndef __mixr_models_LaeroDynamics_HPP__
#define __mixr_models_LaeroDynamics_HPP__

#include "mixr/models/dynamics/IAeroDynamics.hpp"

#include "mixr/base/util/constants.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: LaeroDynamics
// Description:
//    A small, simple, reconfigurable 4 degree of freedom aerodynamic model
//    written by Larry Buckner (i.e., Larry's aerodynamics model)
//------------------------------------------------------------------------------
class LaeroDynamics final: public IAeroDynamics
{
   DECLARE_SUBCLASS(LaeroDynamics, IAeroDynamics )

public:
   LaeroDynamics();

   void dynamics(const double dt) final;  // One pass model update; called from Player::dynamics()
   bool setCommandedHeadingD(const double h, const double hDps = 0, const double maxBank = 0) final;
   bool setCommandedAltitude(const double a, const double aMps = 0, const double maxPitch = 0) final;
   bool setCommandedVelocityKts(const double v, const double vNps = 0) final;

   void reset() final;

protected:
   //-----------------------------------
   // flight interface
   //-----------------------------------
   // max bank defaults to 10
   bool flyPhi(const double phiCmdDeg, const double phiDotCmdDps=10.0);
   // max pitch defaults to 10
   bool flyTht(const double thtCmdDeg, const double thtDotCmdDps=10.0);
   // max roll rate at 3.0 degs per second
   bool flyPsi(const double psiCmdDeg, const double psiDotCmdDps=3.0);
   // max acceleration at 5.0 Nps
   //bool flyVel(const double velCmdKts, const double velDotCmdNps=5.0);
   //bool flyTGS(const double tgsCmdKts) { return flyVel(tgsCmdKts); }

private:
   void update4DofModel(const double dt);

   double dT{};

   // Body angular vel, acc components
   double p{};
   double q{};
   double r{};
   double pDot{};
   double qDot{};
   double rDot{};

   // Euler rotation angle, rate components
   double phi{};
   double tht{};
   double psi{};
   double phiDot{};
   double thtDot{};
   double psiDot{};

   // Body linear vel, acc components
   double u{};
   double v{};
   double w{};
   double uDot{};
   double vDot{};
   double wDot{};

   // NED pos, vel, acc components
   double refPosN{};
   double refPosE{};
   double refPosD{};
   double posN{};
   double posE{};
   double posD{};

   double velN{};
   double velE{};
   double velD{};
   double accN{};
   double accE{};
   double accD{};

   // Hold components
   double phiDot1{};
   double thtDot1{};
   double psiDot1{};
   double uDot1{};
   double vDot1{};
   double wDot1{};
};

}
}

#endif
