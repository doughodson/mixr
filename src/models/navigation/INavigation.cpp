
#include "mixr/models/navigation/INavigation.hpp"

#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/navigation/Bullseye.hpp"
#include "mixr/models/navigation/Route.hpp"
#include "mixr/models/navigation/Steerpoint.hpp"
#include "mixr/models/WorldModel.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/IList.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/String.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/times.hpp"
#include "mixr/base/util/nav_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(INavigation, "INavigation")

BEGIN_SLOTTABLE(INavigation)
    "route",            // 1) Primary route
    "utc",              // 2) initial UTC time
    "feba",             // 3) FEBA: List of distance vectors [ North East ];
                        //    default unit is Nautical Miles; Example:
                        //    feba: {
                        //      [ 10 -10 ]  // in nautical miles
                        //      [ ( Kilometers 20 ) ( NauticalMiles -15 ) ]
                        //    }
    "bullseye",         // 4) Bullseye (just one for now)
END_SLOTTABLE(INavigation)

BEGIN_SLOT_MAP(INavigation)
    ON_SLOT(1, setSlotRoute,    Route)
    ON_SLOT(2, setSlotUtc,      base::ITime)
    ON_SLOT(3, setSlotFeba,     base::PairStream)
    ON_SLOT(4, setSlotBullseye, Bullseye)
END_SLOT_MAP()

INavigation::INavigation() : rm()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void INavigation::initData()
{
   velVec.set(0,0,0);
   accelVec.set(0,0,0);
}

void INavigation::copyData(const INavigation& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   if (org.priRoute != nullptr) {
      Route* p{org.priRoute->clone()};
      priRoute = p;
      p->container(this);
      p->unref();  // safe_ptr<> has it
   } else {
      priRoute = nullptr;
   }
   if (org.initRoute != nullptr) {
      Route* p{org.initRoute->clone()};
      initRoute = p;
      p->unref();  // safe_ptr<> has it
   } else {
      initRoute = nullptr;
   }
   if (org.bull != nullptr) {
      Bullseye* b{org.bull->clone()};
      bull = b;
      b->container(this);
      b->unref();  // safe_ptr<> has it
   } else {
      bull = nullptr;
   }

   latitude = org.latitude;
   longitude = org.longitude;
   altitude = org.altitude;
   posValid = org.posValid;
   heading = org.heading;
   pitch = org.pitch;
   roll = org.roll;
   attValid = org.attValid;
   velVec = org.velVec;
   accelVec = org.accelVec;
   rm = org.rm;
   gs = org.gs;
   tas = org.tas;
   tk = org.tk;
   velValid = org.velValid;
   magvar = org.magvar;
   mhdg = org.mhdg;
   magVarValid = org.magVarValid;
   windDirD = org.windDirD;
   windSpdKts = org.windSpdKts;
   windsValid = org.windsValid;
   navStrValid = org.navStrValid;
   tbrg = org.tbrg;
   mbrg = org.mbrg;
   dst = org.dst;
   ttg = org.ttg;
   tcrs = org.tcrs;
   mcrs = org.mcrs;
   xte = org.xte;
   eta = org.eta;
   utc = org.utc;
   initUTC = org.initUTC;
   utcValid = org.utcValid;
   refLat = org.refLat;
   refLon = org.refLon;

   // Copy Feba
   setFeba(org.feba, org.nFeba);
}

void INavigation::deleteData()
{
    priRoute = nullptr;
    initRoute = nullptr;
    bull = nullptr;

    // Delete the FEBA
    setFeba(nullptr, 0);
}

void INavigation::reset()
{
   BaseClass::reset();

   // And not nothin' valid
   posValid = false;
   attValid = false;
   velValid = false;
   magVarValid = false;
   navStrValid = false;

   // Except UTC
   utc = initUTC;
   utcValid = true;

   // Reset the route to the initial route and reset
   if (priRoute != nullptr) {
      priRoute->container(nullptr);
      priRoute = nullptr;
   }
   if (initRoute != nullptr) {
      priRoute = initRoute->clone();
      priRoute->unref();  // safe_ptr<> has it
   }
   if (priRoute != nullptr) {
      priRoute->container(this);
      priRoute->event(RESET_EVENT);
   }

   // Reset our bullseye
   if (bull != nullptr) bull->event(RESET_EVENT);

   // Set the reference center of our gaming area
   const WorldModel* sim{getWorldModel()};
   if (sim != nullptr) {
      refLat = sim->getRefLatitude();
      refLon = sim->getRefLongitude();
   }
}

//------------------------------------------------------------------------------
// updateData() -- update Non-time critical stuff here
//------------------------------------------------------------------------------
void INavigation::updateData(const double dt)
{
   // ---
   // Update the BaseClass and our primary route
   // ---
   if (priRoute != nullptr) priRoute->updateData(dt);
}

//------------------------------------------------------------------------------
// Process phase
//------------------------------------------------------------------------------
void INavigation::process(const double dt)
{
   BaseClass::process(dt);

   // ---
   // Update our position, attitude and velocities
   // ---
   if (getOwnship() != nullptr) {
      velValid = updateSysVelocity();
      posValid = updateSysPosition();
      attValid = updateSysAttitude();
      magVarValid = updateMagVar();
   }
   else {
      posValid = false;
      attValid = false;
      velValid = false;
      magVarValid = false;
   }

   // Update UTC
   double v {utc + dt};
   if (v >= base::time::D2S) {
      v = (v - base::time::D2S);
   }
   setUTC(v);

   // ---
   // Update our primary route
   // ---
   if (priRoute != nullptr) priRoute->tcFrame(dt);

   // Update our bullseye
   if (bull != nullptr) bull->compute(this);

   // ---
   // Update our navigational steering data
   // ---
   updateNavSteering();
}


//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Is system position valid?
bool INavigation::isPositionDataValid() const
{
   return posValid;
}

// Returns system latitude (degs)
double INavigation::getLatitude() const
{
   return latitude;
}

// Returns system longitude (degs)
double INavigation::getLongitude() const
{
   return longitude;
}

// Returns system altitude (ft)
double INavigation::getAltitudeFt() const
{
    return getAltitudeM() * base::length::M2FT;
}

// Returns system altitude (m)
double INavigation::getAltitudeM() const
{
   return altitude;
}

// Is system attitude valid?
bool INavigation::isAttitudeDataValid() const
{
   return attValid;
}

// Returns system pitch (degs)
double INavigation::getPitchDeg() const
{
   return pitch;
}

// Returns system roll  (degs)
double INavigation::getRollDeg() const
{
   return roll;
}

// Returns system true heading (degs)
double INavigation::getHeadingDeg() const
{
   return heading;
}

// Returns directional cosines
const base::Matrixd& INavigation::getRotMat() const
{
   return rm;
}

// Is the magnetic variation valid?
bool INavigation::isMagVarValid() const
{
   return magVarValid;
}

// Returns the magnetic variation
double INavigation::getMagVarDeg() const
{
   return magvar;
}

// Returns the mag heading
double INavigation::getMagHeadingD() const
{
   return base::angle::aepcdDeg(getHeadingDeg() + getMagVarDeg());
}

// Are the winds valid?
bool INavigation::areWindsValid() const
{
   return windsValid;
}

// Returns the wind 'from' direction (degs)
double INavigation::getWindDirD() const
{
   return windDirD;
}

// Returns the wind speed (kts)
double INavigation::getWindSpeedKts() const
{
   return windSpdKts;
}

// Is the UTC time valid?
bool INavigation::isUtcDataValid() const
{
   return utcValid;
}

// Returns UTC time (sec)
double INavigation::getUTC() const
{
   return utc;
}

// Is system velocity valid?
bool INavigation::isVelocityDataValid() const
{
   return velValid;
}

// Returns ground speed (kts)
double INavigation::getGroundSpeedKts() const
{
   return gs;
}

// Returns true airspeed (kts)
double INavigation::getTrueAirspeedKts() const
{
   return tas;
}

// Returns true ground track (degs)
double INavigation::getGroundTrackDeg() const
{
   return tk;
}

// Returns velocity vector (m/s)
const base::Vec3d& INavigation::getVelocity() const
{
   return velVec;
}

// Returns acceleration vector (m/s/s)
const base::Vec3d& INavigation::getAcceleration() const
{
   return accelVec;
}

// Is system steering data valid?
bool INavigation::isNavSteeringValid() const
{
   return navStrValid;
}

// Returns true bearing to dest (degs)
double INavigation::getTrueBrgDeg() const
{
   return tbrg;
}

// Returns mag bearing to dest (degs)
double INavigation::getMagBrgDeg() const
{
   return mbrg;
}

// Returns distance to dest (NM)
double INavigation::getDistNM() const
{
   return dst;
}

// Returns true course to dest (degs)
double INavigation::getTrueCrsDeg() const
{
   return tcrs;
}

// Returns mag course to dest (degs)
double INavigation::getMagCrsDeg() const
{
   return mcrs;
}

// Returns cross track error (NM)
double INavigation::getCrossTrackErrorNM() const
{
   return xte;
}

// Returns Time-To-Go to dest (sec)
double INavigation::getTTG() const
{
   return ttg;
}

// Returns Est-Time-of-Arrival at dest (sec)
double INavigation::getETA() const
{
   return eta;
}

// Returns the ref latitude (degs)
double INavigation::getRefLatitude() const
{
   return refLat;
}

// Returns the ref longitude (degs)
double INavigation::getRefLongitude() const
{
   return refLon;
}

// Returns the primary route (non-const version)
Route* INavigation::getPriRoute()
{
   return priRoute;
}

// Returns the primary route (const version)
const Route* INavigation::getPriRoute() const
{
   return priRoute;
}

// Returns the current bullseye (non-const version)
Bullseye* INavigation::getBullseye()
{
   return bull;
}

// Returns the current bullseye (const version)
const Bullseye* INavigation::getBullseye() const
{
   return bull;
}


//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool INavigation::setPosition(const double lat, const double lon, const double alt)
{
   latitude  =  lat;
   longitude =  lon;
   altitude  =  alt;
   posValid  =  true;
   return true;
}

bool INavigation::setPosition(const bool flg)
{
   posValid  =  flg;
   return true;
}

bool INavigation::setAttitude(const double roll0, const double pitch0, const double thdg0)
{
   roll = roll0;
   pitch = pitch0;
   heading = thdg0;
   base::nav::computeRotationalMatrixDeg(roll0, pitch0, thdg0, &rm);

   if (magVarValid) mhdg = base::angle::aepcdDeg(heading - magvar);
   else mhdg = heading;

   attValid = true;

   return true;
}

bool INavigation::setAttitude(const bool flg)
{
   attValid = flg;
   return true;
}

bool INavigation::setMagVar(const double mvDeg)
{
   magvar = mvDeg;
   if (attValid) mhdg = base::angle::aepcdDeg(heading - magvar);
   magVarValid = true;
   return true;
}

bool INavigation::setMagVar(const bool flg)
{
   magVarValid = flg;
   return true;
}

bool INavigation::setWinds(const double dirDeg, const double speedKts)
{
   windDirD = dirDeg;
   windSpdKts = speedKts;
   windsValid = true;
   return true;
}

bool INavigation::setWinds(const bool flg)
{
   windsValid = flg;
   return true;
}

bool INavigation::setUTC(const double time)
{
   utc = time;
   utcValid = true;
   return true;
}

bool INavigation::setUTC(const bool flg)
{
   utcValid = flg;
   return true;
}

bool INavigation::setGroundSpeedKts(const double kts)
{
   gs = kts;
   return true;
}

bool INavigation::setTrueAirspeedKts(const double kts)
{
   tas = kts;
   return true;
}

bool INavigation::setGroundTrackDeg(const double degs)
{
   tk = degs;
   return true;
}

bool INavigation::setVelocity(const base::Vec3d& v)
{
   velVec = v;
   return true;
}

bool INavigation::setAcceleration(const base::Vec3d& v)
{
   accelVec = v;
   return true;
}

bool INavigation::setVelocityDataValid(const bool flg)
{
   velValid = flg;
   return true;
}

bool INavigation::setTrueBrgDeg(const double v)
{
   tbrg = v;
   return true;
}

bool INavigation::setMagBrgDeg(const double v)
{
   mbrg = v;
   return true;
}

bool INavigation::setDistNM(const double v)
{
   dst = v;
   return true;
}

bool INavigation::setTrueCrsDeg(const double v)
{
   tcrs = v;
   return true;
}

bool INavigation::setMagCrsDeg(const double v)
{
   mcrs = v;
   return true;
}

bool INavigation::setCrossTrackErrorNM(const double v)
{
   xte = v;
   return true;
}

bool INavigation::setTTG(const double v)
{
   ttg = v;
   return true;
}

bool INavigation::setETA(const double v)
{
   eta = v;
   return true;
}

bool INavigation::setNavSteeringValid(const bool flg)
{
   navStrValid = flg;
   return true;
}


//------------------------------------------------------------------------------
// Default navigation methods
//------------------------------------------------------------------------------

// (default) System position (using truth data from ownship)
bool INavigation::updateSysPosition()
{
    bool ok{};
    if (getOwnship() != nullptr) {
        // -- convert ownship's position vector to lat/lon/alt
        double lat0{};
        double lon0{};
        double alt0{};
        ok = base::nav::convertPosVec2LL(refLat, refLon, getOwnship()->getPosition(), &lat0, &lon0, &alt0);
        setPosition(lat0, lon0, alt0);
    }
    return ok;
}

// (default) System attitude function  (using truth data from ownship)
bool INavigation::updateSysAttitude()
{
    bool ok{};
    if (getOwnship() != nullptr) {
        setAttitude(getOwnship()->getRollD(), getOwnship()->getPitchD(), getOwnship()->getHeadingD());
        ok = true;
    }
    return ok;
}

// Mag var data
bool INavigation::updateMagVar()
{
   // No default magnetic variation
   setMagVar(false);
   return false;
}

// (default) Velocity data function (using truth data from ownship)
bool INavigation::updateSysVelocity()
{
    bool ok{};
    if (getOwnship() != nullptr) {
        setVelocity( getOwnship()->getVelocity() );
        setAcceleration( getOwnship()->getAcceleration() );
        setGroundSpeedKts( getOwnship()->getGroundSpeedKts() );
        setTrueAirspeedKts( getOwnship()->getTotalVelocityKts() );
        setGroundTrackDeg( getOwnship()->getGroundTrackD() );
        setVelocityDataValid( true );
        ok = true;
    } else {
        setVelocityDataValid( false );
   }
   return ok;
}

// (default) Nav steering function (pull data from the 'to' steerpoint)
bool INavigation::updateNavSteering()
{
   if (getPriRoute() != nullptr) {
      const Steerpoint* to{getPriRoute()->getSteerpoint()};
      if (to != nullptr) {
         if (to->isNavDataValid()) {
            setTrueBrgDeg( to->getTrueBrgDeg() );
            setMagBrgDeg( to->getMagBrgDeg() );
            setDistNM( to->getDistNM()) ;
            setTrueCrsDeg( to->getTrueCrsDeg() );
            setMagCrsDeg( to->getMagCrsDeg() );
            setTTG( to->getTTG() );
            setETA( to->getETA() );
            setCrossTrackErrorNM( to->getCrossTrackErrNM() );
            setNavSteeringValid( true );
         } else {
            setNavSteeringValid( false );
         }
      }
   }
   return isNavSteeringValid();
}

//------------------------------------------------------------------------------
// getFeba() -- FEBA [ North East ] (Nautical Miles)
//------------------------------------------------------------------------------
std::size_t INavigation::getFeba(base::Vec2d* const points, const std::size_t max) const
{
    std::size_t n{};
    if (points != nullptr && max > 0 && feba != nullptr && nFeba > 0) {

        // Number of points; limited by 'max'
        n = nFeba;
        if (n > max) n = max;

        // Copy the FEBA points
        for (std::size_t i{}; i < n; i++) {
            points[i] = feba[i];
        }
    }
    return n;
}

//------------------------------------------------------------------------------
// getFeba() -- FEBA [ North East ] (Nautical Miles)
//------------------------------------------------------------------------------
bool INavigation::setFeba(base::Vec2d* const points, const std::size_t n)
{
    // First delete any old FEBA lines
    if (feba != nullptr) delete[] feba;
    feba = nullptr;
    nFeba = 0;

    if (points != nullptr && n >= 2) { // Need at least two points
        nFeba = n;
        feba = new base::Vec2d[nFeba];
        for (std::size_t i{}; i < nFeba; i++) {
            feba[i] = points[i];
        }
    }
    return true;
}

//------------------------------------------------------------------------------
// setRoute() - sets a new route on the fly
//------------------------------------------------------------------------------
bool INavigation::setRoute(Route* const msg)
{
   // we are a new route, but our last new route wasn't the original route
   if (priRoute != nullptr) priRoute->container(nullptr);
   priRoute = msg;
   if (priRoute != nullptr) priRoute->container(this);
   return true;
}


//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool INavigation::setSlotRoute(const Route* const msg)
{
   initRoute = msg;

   if (priRoute != nullptr) {
      priRoute->container(nullptr);
      priRoute = nullptr;
   }
   if (initRoute != nullptr) {
      priRoute = initRoute->clone();
      priRoute->unref();  // safe_ptr<> has it
   }
   if (priRoute != nullptr) {
      priRoute->container(this);
   }
   return true;
}

bool INavigation::setSlotUtc(const base::ITime* const x)
{
    bool ok{};
    if (x != nullptr) {
        initUTC = x->getValueInSeconds();
        ok = true;
    }
    return ok;
}

// set the Forward Edge of the Battle Area (FEBA)
bool INavigation::setSlotFeba(const base::PairStream* const msg)
{
    bool ok{true};

    if (msg != nullptr) {
        // allocate space for the points
        const std::size_t max{msg->entries()};
        const auto tmpFeba = new base::Vec2d[max];

        // Get the points from the pair stream
        std::size_t np{};
        const base::IList::Item* item{msg->getFirstItem()};
        while (item != nullptr && np < max && ok) {
            bool validFlg{};
            const auto p = dynamic_cast<const base::Pair*>(item->getValue());
            if (p != nullptr) {
                const base::IObject* obj2{p->object()};
                const auto msg2 = dynamic_cast<const base::IList*>(obj2);
                if (msg2 != nullptr) {
                    double values[2]{};
                    int n{};

                    { // Get the north (first) distance
                        const base::INumber* pNum{};
                        const auto pair2 = dynamic_cast<const base::Pair*>(msg2->getPosition(1));
                        if (pair2 != nullptr) {
                            pNum = dynamic_cast<const base::INumber*>(pair2->object());
                        } else {
                            pNum = dynamic_cast<const base::INumber*>(msg2->getPosition(1));
                        }
                        if (pNum != nullptr) {
                            const auto pDist = dynamic_cast<const base::ILength*>(pNum);
                            if (pDist != nullptr) {
                                values[n++] = pDist->getValueInNauticalMiles();
                            } else {
                                values[n++] = pNum->asDouble();
                            }
                        }
                    }

                    { // Get the east (second) distance
                        const base::INumber* pNum{};
                        const auto pair2 = dynamic_cast<const base::Pair*>(msg2->getPosition(2));
                        if (pair2 != nullptr) {
                            pNum = dynamic_cast<const base::INumber*>(pair2->object());
                        } else {
                            pNum = dynamic_cast<const base::INumber*>(msg2->getPosition(2));
                        }
                        if (pNum != nullptr) {
                            const auto pDist = dynamic_cast<const base::ILength*>(pNum);
                            if (pDist != nullptr) {
                                values[n++] = pDist->getValueInNauticalMiles();
                            } else {
                                values[n++] = pNum->asDouble();
                            }
                        }
                    }

                    if (n == 2) {
                        tmpFeba[np++].set(values[0],values[1]);
                        validFlg = true;
                    }
                }
            }

            if (!validFlg) {
                std::cerr << "Navigation::setSlotFeba: Invalid FEBA coordinate!" << std::endl;
                ok = false;
            }
            item = item->getNext();
        }

        // If we have a valid FEBA, then save it
        if (ok) {
            setFeba(tmpFeba, np);
        }

        delete[] tmpFeba;
    }

    return ok;
}

bool INavigation::setSlotBullseye(Bullseye* const msg)
{
   if (bull != nullptr) {
      bull->container(nullptr);
   }
   bull = msg;
   if (bull != nullptr) {
      bull->container(this);
   }
   return true;
}

}
}
