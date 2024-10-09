
#include "mixr/models/dynamics/LaeroModel.hpp"

#include "mixr/models/player/Player.hpp"

#include "mixr/base/String.hpp"
#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/units/util/time_utils.hpp"

#include "mixr/base/osg/Vec3d"
#include "mixr/base/osg/Quat"

#include <iostream>
#include <iomanip>
#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(LaeroModel, "LaeroModel")
EMPTY_SLOTTABLE(LaeroModel)
EMPTY_DELETEDATA(LaeroModel)

//----------------------------------------------------------
// conversion constants
//----------------------------------------------------------
constexpr double HALF_PI{base::PI / 2.0};
constexpr double EPSILON{1.0E-10};

LaeroModel::LaeroModel()
{
   STANDARD_CONSTRUCTOR()
}

void LaeroModel::copyData(const LaeroModel& org, const bool)
{
   BaseClass::copyData(org);

   dT       = org.dT;

   // Body angular vel, acc components
   p        = org.p;
   q        = org.q;
   r        = org.r;
   pDot     = org.pDot;
   qDot     = org.qDot;
   rDot     = org.rDot;

   // Euler rotation angle, rate components
   phi      = org.phi;
   tht      = org.tht;
   psi      = org.psi;
   phiDot   = org.phiDot;
   thtDot   = org.thtDot;
   psiDot   = org.psiDot;

   // Body linear vel, acc components
   u        = org.u;
   v        = org.v;
   w        = org.w;
   uDot     = org.uDot;
   vDot     = org.vDot;
   wDot     = org.wDot;

   // NED pos, vel, acc components
   refPosN  = org.refPosN;
   refPosE  = org.refPosE;
   refPosD  = org.refPosD;
   posN     = org.posN;
   posE     = org.posE;
   posD     = org.posD;

   velN     = org.velN;
   velE     = org.velE;
   velD     = org.velD;
   accN     = org.accN;
   accE     = org.accE;
   accD     = org.accD;

   // Hold components
   phiDot1  = org.phiDot1;
   thtDot1  = org.thtDot1;
   psiDot1  = org.psiDot1;
   uDot1    = org.uDot1;
   vDot1    = org.vDot1;
   wDot1    = org.wDot1;
}

void LaeroModel::dynamics(const double dt)
{
    update4DofModel(dt);
    dT = dt;
}

void LaeroModel::reset()
{
   BaseClass::reset();

   const auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );
   if (pPlr != nullptr) {
      const double initVel{pPlr->getInitVelocity()};
      u = initVel * base::length::NM2M / base::time::H2S;
   }
}

//----------------------------------------------------------
// update equations of motion
//----------------------------------------------------------
void LaeroModel::update4DofModel(const double dt)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   const auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );

   if (pPlr != nullptr) {

      //==============================================================
      // ROTATIONAL EOM
      //==============================================================

      //----------------------------------------------------
      // integrate Euler angles using Adams-Bashforth
      //----------------------------------------------------
      phi += 0.5 * (3.0 * phiDot - phiDot1) * dT;
      if (phi >  base::PI) phi = -base::PI;
      if (phi < -base::PI) phi =  base::PI;

      tht += 0.5 * (3.0 * thtDot - thtDot1) * dT;
      if (tht >=  HALF_PI) tht =  (HALF_PI - EPSILON);
      if (tht <= -HALF_PI) tht = -(HALF_PI - EPSILON);

      psi += 0.5 * (3.0 * psiDot - psiDot1) * dT;
      if (psi >  base::PI) psi = -base::PI;
      if (psi < -base::PI) psi =  base::PI;

      //----------------------------------------------------
      // update Euler angles
      //----------------------------------------------------
      pPlr->setEulerAngles(phi, tht, psi);

      //----------------------------------------------------
      // hold current rotational control values for next iteration
      //----------------------------------------------------
      phiDot1 = phiDot;
      thtDot1 = thtDot;
      psiDot1 = psiDot;

      //----------------------------------------------------
      // compute sin, cos of Euler angles
      //----------------------------------------------------
      double sinPhi{std::sin(phi)};
      double cosPhi{std::cos(phi)};
      double sinTht{std::sin(tht)};
      double cosTht{std::cos(tht)};
      double sinPsi{std::sin(psi)};
      double cosPsi{std::cos(psi)};

      //----------------------------------------------------
      // compute local to body axes matrix
      //----------------------------------------------------
      double l1{cosTht * cosPsi};
      double l2{cosTht * sinPsi};
      double l3{-sinTht};
      double m1{sinPhi * sinTht * cosPsi - cosPhi * sinPsi};
      double m2{sinPhi * sinTht * sinPsi + cosPhi * cosPsi};
      double m3{sinPhi * cosTht};
      double n1{cosPhi * sinTht * cosPsi + sinPhi * sinPsi};
      double n2{cosPhi * sinTht * sinPsi - sinPhi * cosPsi};
      double n3{cosPhi * cosTht};

      //----------------------------------------------------
      // update p,q,r angular velocity components (body)
      //----------------------------------------------------
      p = phiDot                    +       (-sinTht)*psiDot;
      q =           (cosPhi)*thtDot + (cosTht*sinPhi)*psiDot;
      r =          (-sinPhi)*thtDot + (cosTht*cosPhi)*psiDot;

      //----------------------------------------------------
      // update angular velocities
      //----------------------------------------------------
      pPlr->setAngularVelocities(p, q, r);


      //==============================================================
      // TRANSLATIONAL EOM
      //==============================================================

      //----------------------------------------------------
      // integrate u,v,w velocity components (body)
      //----------------------------------------------------
      u  += 0.5*(3.0*uDot - uDot1)*dT;
      v  += 0.5*(3.0*vDot - vDot1)*dT;
      w  += 0.5*(3.0*wDot - wDot1)*dT;

      //----------------------------------------------------
      // hold current translational control values for next iteration
      //----------------------------------------------------
      uDot1   = uDot;
      vDot1   = vDot;
      wDot1   = wDot;

      //----------------------------------------------------
      // update velocity in NED system
      //----------------------------------------------------
      velN  = l1*u + m1*v + n1*w;
      velE  = l2*u + m2*v + n2*w;
      velD  = l3*u + m3*v + n3*w;

      pPlr->setVelocity(velN, velE, velD);

      //----------------------------------------------------
      // update acceleration in NED system
      //----------------------------------------------------
      //double A = uDot + w*q - v*r;
      //double B = vDot + u*r - w*p;
      //double C = wDot + v*p - u*q;

      //accN  = l1*A + m1*B + n1*C;
      //accE  = l2*A + m2*B + n2*C;
      //accD  = l3*A + m3*B + n3*C;

      //pPlr->setAcceleration(accN, accE, accD);
   }
}

//
// flight interface
//

bool LaeroModel::flyPhi(const double phiCmdDeg, const double phiDotCmdDps)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   const auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );
   bool ok{(pPlr != nullptr)};
   if (ok) {

      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------
      double phiCmdRad{phiCmdDeg * base::angle::D2RCC};
      double phiDotCmdRps{phiDotCmdDps * base::angle::D2RCC};

      //-------------------------------------------------------
      // current phi error (rad)
      //-------------------------------------------------------
      double phiRad{pPlr->getRollR()};
      double phiErrRad{phiCmdRad - phiRad};

      //-------------------------------------------------------
      // phi error break point (rad)
      //-------------------------------------------------------
      const double TAU{1.0};                 // time constant [sec]
      double phiErrBrkRad{phiDotCmdRps * TAU};

      //-------------------------------------------------------
      // control signal for commanded phi (rps)
      //-------------------------------------------------------
      double phiDotRps{base::sign(phiErrRad) * phiDotCmdRps};
      if (std::abs(phiErrRad) < phiErrBrkRad) {
         phiDotRps = (phiErrRad / phiErrBrkRad) * phiDotCmdRps;
      }

      //-------------------------------------------------------
      // assign result to roll control
      //-------------------------------------------------------
      phiDot = phiDotRps;
   }

   return ok;
}

bool LaeroModel::flyTht(const double thtCmdDeg, const double thtDotCmdDps)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   const auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );
   bool ok{(pPlr != nullptr)};
   if (ok) {

      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------
      double thtCmdRad{thtCmdDeg * base::angle::D2RCC};
      double thtDotCmdRps{thtDotCmdDps * base::angle::D2RCC};

      //-------------------------------------------------------
      // current tht error (rad)
      //-------------------------------------------------------
      double thtRad{pPlr->getPitchR()};
      double thtErrRad{thtCmdRad - thtRad};

      //-------------------------------------------------------
      // tht error break point (rad)
      //-------------------------------------------------------
      const double TAU{1.0};  // time constant [sec]
      double thtErrBrkRad{thtDotCmdRps * TAU};

      //-------------------------------------------------------
      // control signal for commanded tht (rps)
      //-------------------------------------------------------
      double thtDotRps{base::sign(thtErrRad) * thtDotCmdRps};
      if (std::abs(thtErrRad) < thtErrBrkRad) {
         thtDotRps = (thtErrRad / thtErrBrkRad) * thtDotCmdRps;
      }

      //-------------------------------------------------------
      // assign result to pitch control
      //-------------------------------------------------------
      thtDot = thtDotRps;
   }

   return ok;
}

bool LaeroModel::flyPsi(const double psiCmdDeg, const double psiDotCmdDps)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   const auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );
   bool ok{(pPlr != nullptr)};
   if (ok) {

      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------
      double psiCmdRad{psiCmdDeg * base::angle::D2RCC};
      double psiDotCmdRps{psiDotCmdDps * base::angle::D2RCC};

      //-------------------------------------------------------
      // current psi error (rad)
      //-------------------------------------------------------
      double psiRad{pPlr->getHeadingR()};
      double psiErrRad{psiCmdRad - psiRad};

      //-------------------------------------------------------
      // psi error break point (rad)
      //-------------------------------------------------------
      const double TAU{1.0};  // time constant [sec]
      double psiErrBrkRad{psiDotCmdRps * TAU};

      //-------------------------------------------------------
      // control signal for commanded psi (rps)
      //-------------------------------------------------------
      double psiDotRps{base::sign(psiErrRad) * psiDotCmdRps};
      if (std::abs(psiErrRad) < psiErrBrkRad) {
         psiDotRps = (psiErrRad / psiErrBrkRad) * psiDotCmdRps;
      }

      //-------------------------------------------------------
      // assign result to azimuth control
      //-------------------------------------------------------
      psiDot = psiDotRps;
   }

   return ok;
}

////==============================================================================
//bool LaeroModel::flyVel(const double velCmdKts, const double velDotCmdNps)
//{
//   //-------------------------------------------------------
//   // get data pointers
//   //-------------------------------------------------------
//   simulation::Player* pPlr = static_cast<simulation::Player*>( findContainerByType(typeid(simulation::Player)) );
//   bool ok = (pPlr != nullptr);
//   if (ok) {
//
//      //-------------------------------------------------------
//      // define local constants
//      //-------------------------------------------------------
//      const double KTS2MPS = base::Distance::NM2M / base::Time::H2S;
//
//      //-------------------------------------------------------
//      // convert argument units (deg -> rad)
//      //-------------------------------------------------------
//      double velCmdMps     = velCmdKts * KTS2MPS;
//      double velDotCmdMps2 = velDotCmdNps * KTS2MPS;
//
//      //-------------------------------------------------------
//      // current vel error (rad)
//      //-------------------------------------------------------
//      double velMps    = pPlr->getTotalVelocityKts() * KTS2MPS;
//      double velErrMps = velCmdMps - velMps;
//
//      //-------------------------------------------------------
//      // vel error break point (rad)
//      //-------------------------------------------------------
//      const double TAU = 1.0;  // time constant [sec]
//      double velErrBrkMps = velDotCmdMps2 * TAU;
//
//      //-------------------------------------------------------
//      // control signal for commanded vel (rps)
//      //-------------------------------------------------------
//      double velDotMps2 = sign(velErrMps) * velDotCmdMps2;
//      if (std::abs(velErrMps) < velErrBrkMps) {
//         velDotMps2 = (velErrMps / velErrBrkMps) * velDotCmdMps2;
//      }
//
//      //-------------------------------------------------------
//      // assign result to velocity control
//      //-------------------------------------------------------
//      uDot = velDotMps2;
//   }
//
//   return ok;
//}

//==============================================================================
//bool LaeroModel::fly2LL(const double latDeg, const double lonDeg)
//{
//   //-------------------------------------------------------
//   // get data pointers
//   //-------------------------------------------------------
//   simulation::Player* pPlr = static_cast<simulation::Player*>( findContainerByType(typeid(simulation::Player)) );
//
//   bool ok = (pPlr != nullptr);
//   if (ok) {
//
//      //----------------------------------------------------
//      // get current data
//      //----------------------------------------------------
//      double osLatDeg  = pPlr->getLatitude();
//      double osLonDeg  = pPlr->getLongitude();
//      double brgDeg = 0.0;
//      double rngMtr = 0.0;
//      base::nav::gll2bd(osLatDeg, osLonDeg, latDeg, lonDeg, &brgDeg,&rngMtr);
//
//      //-------------------------------------------------------
//      // fly to heading necessary to intercept lat/lon
//      //-------------------------------------------------------
//      ok = flyHDG(brgDeg);
//   }
//
//   return ok;
//}


// Dynamics model interface
bool LaeroModel::setCommandedHeadingD(const double h, const double hDps, const double maxBank)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   const auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );

   bool ok{(pPlr != nullptr)};
   if (ok) {

      //----------------------------------------------------
      // define local constants
      //----------------------------------------------------
      const double MAX_BANK_RAD{maxBank * base::angle::D2RCC};
      //const double TAU = 2.0;  // time constant [sec]
      const double TAU{1.0};  // time constant [sec]

      //-------------------------------------------------------
      // get current data
      //-------------------------------------------------------
      double velMps{pPlr->getTotalVelocity()};
      double hdgDeg{pPlr->getHeadingD()};
      double hdgErrDeg{base::angle::aepcdDeg(h - hdgDeg)};
      double hdgErrAbsDeg{std::fabs(hdgErrDeg)};

      //-------------------------------------------------------
      // get absolute heading rate of change (hdgDotAbsDps)
      //-------------------------------------------------------
      double hdgDotMaxAbsRps{base::ETHGM * std::tan(MAX_BANK_RAD) / velMps};
      double hdgDotMaxAbsDps{hdgDotMaxAbsRps * base::angle::R2DCC};

      double hdgDotAbsDps{hDps};
      if (hdgDotAbsDps > hdgDotMaxAbsDps) {
         hdgDotAbsDps = hdgDotMaxAbsDps;
      }

      double hdgErrBrkAbsDeg{TAU * hdgDotAbsDps};
      if (hdgErrAbsDeg < hdgErrBrkAbsDeg) {
         hdgDotAbsDps = hdgErrAbsDeg / TAU;
      }

      //-------------------------------------------------------
      // define direction of heading rate of change (hdgDotDps)
      //-------------------------------------------------------
      double hdgDotDps{base::sign(hdgErrDeg) * hdgDotAbsDps};
      psiDot = hdgDotDps * base::angle::D2RCC;

      //-------------------------------------------------------
      // define bank angle as a function of turn rate
      //-------------------------------------------------------
      double phiCmdDeg{std::atan2(psiDot * velMps, base::ETHGM) * base::angle::R2DCC};
      ok = flyPhi(phiCmdDeg);
   }

   return ok;
}

// Dynamics model interface - all input values in meters
bool LaeroModel::setCommandedAltitude(const double a, const double aMps, const double maxPitch)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   const auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );

   bool ok{(pPlr != nullptr)};
   if (ok) {

      //-------------------------------------------------------
      // define local constants
      //-------------------------------------------------------
      const double TAU{4.0};        // time constant [sec]

      //-------------------------------------------------------
      // get current alt error (mtr)
      //-------------------------------------------------------
      double altMtr{pPlr->getAltitude()};
      double altErrMtr{a - altMtr};

      //-------------------------------------------------------
      // get alt error break point (mtr)
      //-------------------------------------------------------
      double altDotCmdMps{aMps};
      double altErrBrkMtr{altDotCmdMps * TAU};

      //-------------------------------------------------------
      // get commanded altDot (mps)
      //-------------------------------------------------------
      double altDotMps{base::sign(altErrMtr) * altDotCmdMps};
      if (std::abs(altErrMtr) < altErrBrkMtr) {
         altDotMps = altErrMtr * (altDotCmdMps / altErrBrkMtr);
      }

      //-------------------------------------------------------
      // assign result to altitude control
      //-------------------------------------------------------
      double thtCmdDeg{(altDotMps / u) * base::angle::R2DCC};
      // SLS - TO DO: Limit commanded pitch to max pitch angle as well.
      ok = flyTht(thtCmdDeg);
   }

   return ok;
}

// setCommandedVelocityKts() - also can limit velocity rate of acceleration
bool LaeroModel::setCommandedVelocityKts(const double v, const double vNps)
{
   //-------------------------------------------------------
   // get data pointers
   //-------------------------------------------------------
   const auto pPlr = static_cast<Player*>( findContainerByType(typeid(Player)) );
   bool ok{(pPlr != nullptr)};
   if (ok) {

      //-------------------------------------------------------
      // define local constants
      //-------------------------------------------------------
      const double KTS2MPS{base::length::NM2M / base::time::H2S};

      //-------------------------------------------------------
      // convert argument units (deg -> rad)
      //-------------------------------------------------------
      double velCmdMps{v * KTS2MPS};
      double velDotCmdMps2{vNps * KTS2MPS};

      //-------------------------------------------------------
      // current vel error (rad)
      //-------------------------------------------------------
      double velMps{pPlr->getTotalVelocityKts() * KTS2MPS};
      double velErrMps{velCmdMps - velMps};

      //-------------------------------------------------------
      // vel error break point (rad)
      //-------------------------------------------------------
      const double TAU{1.0};  // time constant [sec]
      double velErrBrkMps{velDotCmdMps2 * TAU};

      //-------------------------------------------------------
      // control signal for commanded vel (rps)
      //-------------------------------------------------------
      double velDotMps2{base::sign(velErrMps) * velDotCmdMps2};
      if (std::abs(velErrMps) < velErrBrkMps) {
         velDotMps2 = (velErrMps / velErrBrkMps) * velDotCmdMps2;
      }

      //-------------------------------------------------------
      // assign result to velocity control
      //-------------------------------------------------------
      uDot = velDotMps2;
   }

   return true;
}

}
}
