
#ifndef __mixr_models_SynchronizedState_HPP__
#define __mixr_models_SynchronizedState_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/base/osg/Vec3d"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: SynchronizedState
//
// Description: Concrete - a snapshot of position state at a given time
//------------------------------------------------------------------------------
class SynchronizedState final: public base::IObject
{
   DECLARE_SUBCLASS(SynchronizedState, base::IObject)

public:
   SynchronizedState();

   const base::Vec3d& getGeocPosition() const           { return posVecECEF; }    // Geocentric (ECEF) position vector [ x y z ] (meters)
   const base::Vec3d& getGeocVelocity() const           { return velVecECEF; }    // Geocentric (ECEF) velocity vector [ x y z ] (meters/second)
   const base::Vec3d& getGeocAcceleration() const       { return accelVecECEF; }  // Geocentric (ECEF) acceleration vector [ x y z ] ((meters/second)/second)
   const base::Vec3d& getGeocEulerAngles() const        { return anglesW; }       // Geocentric (body/ECEF) Euler angles
   const base::Vec3d& getAngularVelocities() const      { return angVel; }        // Body angular rates (radians/second)
   double getTimeExec() const                           { return timeExec; }      // Sim exec time of state data (seconds)
   double getTimeUtc() const                            { return timeUtc; }       // Sim time of day for the state data (seconds)
   bool isValid() const override                        { return valid; }

   virtual bool setGeocPosition(const base::Vec3d& gcPos);
   virtual bool setGeocVelocity(const base::Vec3d& newVelEcef);
   virtual bool setGeocAcceleration(const base::Vec3d& newAccelEcef);
   virtual bool setGeocEulerAngles(const base::Vec3d& newAngles);
   virtual bool setAngularVelocities(const base::Vec3d& newAngVel);
   void setValid(const bool b)                                        { valid = b; }
   void setTimeExec(const double d)                                   { timeExec = d; }
   void setTimeUtc(const double d)                                    { timeUtc = d; }

   virtual void clear();         // clear state data

private:

   base::Vec3d posVecECEF;       // Geocentric position vector     (meters)  (ECEF)
   base::Vec3d velVecECEF;       // Geocentric velocity vector     (meters/second)  (ECEF)
   base::Vec3d accelVecECEF;     // Geocentric acceleration vector ((meters/second)/second) (ECEF)
   base::Vec3d anglesW;          // World (body/ECEF) Euler angles (radians)
   base::Vec3d angVel;           // Body angular velocities (radians/seconds)
   double timeExec {};
   double timeUtc {};
   bool valid {};
};

}
}

#endif

