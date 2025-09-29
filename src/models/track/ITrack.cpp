
#include "mixr/models/track/ITrack.hpp"

#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/RfEmission.hpp"
#include "mixr/models/IrQueryMsg.hpp"
#include "mixr/models/ISensorMsg.hpp"

#include "mixr/base/qty/util/angle_utils.hpp"
#include "mixr/base/qty/util/length_utils.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(ITrack, "ITrack")
EMPTY_SLOTTABLE(ITrack)

ITrack::ITrack()
{
    STANDARD_CONSTRUCTOR()
    clear();
}

void ITrack::copyData(const ITrack& org, const bool)
{
   BaseClass::copyData(org);

   setTarget( org.tgt );

   // General track ID and status
   id = org.id;
   type = org.type;
   iffCode = org.iffCode;
   age = org.age;
   quality = org.quality;

   //Track Class
   trackClass = org.trackClass;

   // Track State Vector
   latitude = org.latitude;
   longitude = org.longitude;
   llValid = org.llValid;
   pos = org.pos;
   los = org.los;
   vel = org.vel;
   accel = org.accel;
   rng = org.rng;
   rngRate = org.rngRate;
   cErr = org.cErr;
   vErr = org.vErr;

   // Data computed from state vector
   gndRng = org.gndRng;
   taz = org.taz;
   aa  = org.aa;
   gndSpd = org.gndSpd;
   gndTrk = org.gndTrk;
   relGndTrk = org.relGndTrk;
   for (unsigned int i = 0; i < 3; i++) {
      raz[i] = org.raz[i];
   }
   for (unsigned int i = 0; i < 3; i++) {
      rel[i] = org.rel[i];
   }
   for (unsigned int i = 0; i < 3; i++) {
      predictedRaz[i] = org.predictedRaz[i];
   }
   for (unsigned int i = 0; i < 3; i++) {
      predictedRel[i] = org.predictedRel[i];
   }

   // Shoot list support
   shootList = org.shootList;
   wpnRel = org.wpnRel;
   rejected = org.rejected;

   // Ownship data
   osGndTrk = org.osGndTrk;
   osVel = org.osVel;
   osAccel = org.osAccel;
}

void ITrack::deleteData()
{
   setTarget(nullptr);
   clear();
}

//------------------------------------------------------------------------------
// clear() -- clear the track
//------------------------------------------------------------------------------
void ITrack::clear()
{
   // General track ID and status
   id = 0;
   type = 0;
   iffCode = UNKNOWN;
   age = 0.0;
   quality = 0.0;

   // Track State Vector
   latitude = 0.0;
   longitude = 0.0;
   llValid = false;
   pos.set(0,0,0);
   los.set(0,0,0);
   vel.set(0,0,0);
   accel.set(0,0,0);
   rng = 0.0;
   rngRate = 0.0;
   cErr = 0.0;
   vErr = 0.0;

   // Data computed from state vector
   gndRng = 0.0;
   taz = 0.0;
   aa  = 0.0;
   gndSpd = 0.0;
   gndTrk = 0.0;
   relGndTrk = 0.0;
   for (int i = 0; i < 3; i++) {
      raz[i] = 0.0;
   }
   for (int i = 0; i < 3; i++) {
      rel[i] = 0.0;
   }
   for (int i = 0; i < 3; i++) {
      predictedRaz[i] = 0.0;
   }
   for (int i = 0; i < 3; i++) {
      predictedRel[i] = 0.0;
   }

   // Shoot list support
   shootList = 0;
   wpnRel = false;
   rejected = false;

   // Ownship data
   osGndTrk = 0;
   osVel.set(0,0,0);
   osAccel.set(0,0,0);

   setTarget(nullptr);
}


//------------------------------------------------------------------------------
// ownshipDynamics() -- apply ownship dynamics to predicted track position
//------------------------------------------------------------------------------
void ITrack::ownshipDynamics(const double gtrk, const base::Vec3d velOS, const base::Vec3d accelOS, const double)
{
   osGndTrk = gtrk;
   osVel = velOS;
   osAccel = accelOS;
}

//------------------------------------------------------------------------------
// getLatLonPosition() -- set track's Lat/Lon position; returns true if latitude
// and longitude are valid, and 'lat' and 'lon' are set; returns false if latitude
// and longitude are not valid, and 'lat' and 'lon' are not changed.
//------------------------------------------------------------------------------
bool ITrack::getLatLonPosition(double* const lat, double* const lon) const
{
   bool ok{};
   if (llValid && lat != nullptr && lon != nullptr) {
      *lat = latitude;
      *lon = longitude;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// setTrackID() -- set local track ID
bool ITrack::setTrackID(const int v)
{
   id = v;
   return true;
}
// setType() -- set all of the track type bits to 't'
bool ITrack::setType(const short t)
{
   type = t;
   return true;
}

// setSubtype() -- bitwise-OR the type bits with 't'
bool ITrack::setSubtype(const short t)
{
   type = (t | type);
   return true;
}

// setClass() -- set all of the track class with 'c'
bool ITrack::setClass(const TrackClass c)
{
    trackClass = c;
    return true;
}

// updateTrackAge() -- Update the age of the track by delta time, dt (seconds)
bool ITrack::updateTrackAge(const double dt)
{
   age += dt;
   return true;
}

// resetTrackAge() -- Reset the age of the track to zero
bool ITrack::resetTrackAge()
{
   age = 0.0;
   return true;
}

// setQuality() -- Sets the normalized track quality value
bool ITrack::setQuality(const double v)
{
   quality = v;
   return true;
}

// setLatLonPosition() -- set track's position vector
bool ITrack::setLatLonPosition(const double lat, const double lon)
{
   llValid = false;
   latitude = lat;
   longitude = lon;
   if (latitude <= 90.0 && latitude >= -90.0) {
      if (longitude <= 180.0 && longitude >= -180.0) {
         llValid = true;
      }
   }
   return llValid;
}

// clearLatLonValid() -- clears the track's latitude/longitude position validity
void ITrack::clearLatLonValid()
{
   llValid = false;
}

// setPosition() -- set track's position vector
bool ITrack::setPosition(const base::Vec3d& p)
{
   // set position vector
   pos = p;

   // compute ranges
   const double gndRng2{pos.x()*pos.x() + pos.y()*pos.y()};
   gndRng = std::sqrt(gndRng2);
   rng = std::sqrt(gndRng2 +  pos.z()*pos.z());

   // compute angles
   taz = std::atan2(pos.y(),pos.x());
   raz[0] = base::angle::aepcdRad(taz - osGndTrk);
   rel[0] = std::atan2(-pos.z(), gndRng);

   // Set LOS unit vector
   if (rng > 0) los.set( pos.x()/rng, pos.y()/rng, pos.z()/rng );
   else los.set(0,0,0);

   return true;
}


// setPosition() -- set track's position vector with a 4D vector; use only x, y and z
bool ITrack::setPosition(const base::Vec4d& p)
{
   base::Vec3d pos3(p.x(),p.y(),p.z());
   return setPosition(pos3);
}

// setCircularError() -- sets the track's estimated position error, meters,
bool ITrack::setCircularError(const double err)
{
   cErr = err;
   return true;
}

// setVerticalError() -- sets the track's estimated altitude error, meters
bool ITrack::setVerticalError(const double err)
{
   vErr = err;
   return true;
}

// setVelocity() -- set track's velocity vector and compute ground speed & track
bool ITrack::setVelocity(const base::Vec3d v)
{
   vel = v;

   // Total velocity (NED) (m/s)
   base::Vec3d totalVel{vel + osVel};

   gndSpd = std::sqrt(totalVel[0]*totalVel[0] + totalVel[1]*totalVel[1]);
   gndTrk = std::atan2(totalVel[1], totalVel[0]);
   relGndTrk = base::angle::aepcdRad(gndTrk - osGndTrk);

   const double tmp1{pos[1] * totalVel[0] - pos[0] * totalVel[1]};
   const double tmp2{pos[0] * totalVel[0] + pos[1] * totalVel[1]};
   aa = std::atan2(-tmp1,tmp2);

   return true;
}

// setAcceleration() -- set track's acceleration vector
bool ITrack::setAcceleration(const base::Vec3d a)
{
   accel = a;
   return true;
}

// setRange() -- sets slant range to track, meters
bool ITrack::setRange(const double r)
{
   rng = r;
   return true;
}

// setRangeRate() -- sets the rate of change of the slate range to target, m/s
bool ITrack::setRangeRate(const double rr)
{
   rngRate = rr;
   return true;
}

// setIffCode() -- sets the IFF code
bool ITrack::setIffCode(const IffCode s)
{
   iffCode = s;
   return true;
}

// setShootListIndex() -- sets the track's shootlist index
bool ITrack::setShootListIndex(const int idx)
{
   shootList = idx;
   return true;
}

// setWeaponReleased() -- flags that we've released a weapon at this track
bool ITrack::setWeaponReleased(const bool f)
{
   wpnRel = f;
   return true;
}

// setRejected() -- rejects this track from the shootlist
bool ITrack::setRejected(const bool f)
{
   rejected = f;
   return true;
}

// setTarget() -- set the target pointer
bool ITrack::setTarget(IPlayer* const p)
{
   if (tgt != nullptr) tgt->unref();
   tgt = p;
   if (tgt != nullptr) tgt->ref();
   return true;
}

}
}
