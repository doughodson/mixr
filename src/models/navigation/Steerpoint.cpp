
#include "mixr/models/navigation/Steerpoint.hpp"

#include "mixr/models/action/IAction.hpp"
#include "mixr/models/navigation/INavigation.hpp"
#include "mixr/models/player/IPlayer.hpp"

#include "mixr/terrain/ITerrain.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/String.hpp"

#include "mixr/base/Latitude.hpp"
#include "mixr/base/Longitude.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"

#include "mixr/base/units/angles.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/units/times.hpp"

#include "mixr/base/util/nav_utils.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Steerpoint, "Steerpoint")

BEGIN_SLOTTABLE(Steerpoint)
    "stptType",         //  1) Steerpoint type          (Identifier) { ROUTE, DEST, MARK, FIX, OAP, TGT }; default: DEST
    "latitude",         //  2) Latitude                 (latLon)
    "longitude",        //  3) Longitude                (latLon)
    "xPos",             //  4) X position               (Distance)
    "yPos",             //  5) Y position               (Distance)
    "elevation",        //  6) Terrain Elevation        (Distance)
    "altitude",         //  7) Commanded altitude       (Distance)
    "airspeed",         //  8) Commanded airspeed (Kts) (Number)
    "pta",              //  9) Planned Time of Arrival  (Time)
    "sca",              // 10) Safe Clearance Altitude  (Distance)
    "description",      // 11) Description              (String)
    "magvar",           // 12) Magnetic Var @ point     (Angle)
    "next",             // 13) Next steerpoint number: by name (Identifier) or index (Number)
    "action",           // 14) Steerpoint action        (IAction)
END_SLOTTABLE(Steerpoint)

BEGIN_SLOT_MAP(Steerpoint)
    ON_SLOT( 1, setSlotSteerpointType, base::Identifier)

    ON_SLOT( 2, setSlotLatitude,       base::Latitude)
    ON_SLOT( 2, setSlotLatitude,       base::INumber)

    ON_SLOT( 3, setSlotLongitude,      base::Longitude)
    ON_SLOT( 3, setSlotLongitude,      base::INumber)

    ON_SLOT( 4, setSlotXPos,           base::ILength)

    ON_SLOT( 5, setSlotYPos,           base::ILength)

    ON_SLOT( 6, setSlotElevation,      base::ILength)
    ON_SLOT( 6, setSlotElevation,      base::INumber)

    ON_SLOT( 7, setSlotCmdAltitude,    base::ILength)
    ON_SLOT( 7, setSlotCmdAltitude,    base::INumber)

    ON_SLOT( 8, setSlotCmdAirspeed,    base::INumber)

    ON_SLOT( 9, setSlotPTA,            base::ITime)
    ON_SLOT( 9, setSlotPTA,            base::INumber)

    ON_SLOT(10, setSlotSCA,            base::ILength)
    ON_SLOT(10, setSlotSCA,            base::INumber)

    ON_SLOT(11, setSlotDescription,    base::String)

    ON_SLOT(12, setSlotMagVar,         base::IAngle)
    ON_SLOT(12, setSlotMagVar,         base::INumber)

    ON_SLOT(13, setSlotNext,           base::Identifier)
    ON_SLOT(13, setSlotNext,           base::Integer)

    ON_SLOT(14, setSlotAction,         IAction)
END_SLOT_MAP()

Steerpoint::Steerpoint()
{
    STANDARD_CONSTRUCTOR()

    posVec.set(0,0,0);
    initPosVec.set(0,0,0);
}

void Steerpoint::copyData(const Steerpoint& org, const bool)
{
    BaseClass::copyData(org);

    next = nullptr; // find it using 'initNextStptName' or 'initNexStptIdx'

    {
       base::Identifier* n{};
       if (org.initNextStptName != nullptr) n = org.initNextStptName->clone();
       initNextStptName = n;
       if (n != nullptr) n->unref();
    }

    initNextStptIdx = org.initNextStptIdx;

    {
       base::String* s{};
       if (org.description != nullptr) s = org.description->clone();
       description = s;
       if (s != nullptr) s->unref();
    }

    {
       IAction* aa{};
       if (org.action != nullptr) aa = org.action->clone();
       action = aa;
       if (aa != nullptr) aa->unref();
    }

    latitude = org.latitude;
    longitude = org.longitude;
    elevation = org.elevation;
    posVec = org.posVec;
    stptType = org.stptType;
    pta = org.pta;
    sca = org.sca;
    magvar = org.magvar;
    needPosVec = org.needPosVec;
    needLL = org.needLL;
    cmdAlt = org.cmdAlt;
    haveCmdAlt = org.haveCmdAlt;
    cmdAirspeed = org.cmdAirspeed;
    haveCmdAs = org.haveCmdAs;

    initLatitude = org.initLatitude;
    initLongitude = org.initLongitude;
    initElev = org.initElev;
    initPosVec = org.initPosVec;
    initMagVar = org.initMagVar;
    haveInitLat = org.haveInitLat;
    haveInitLon = org.haveInitLon;
    haveInitPos = org.haveInitPos;
    haveInitMagVar = org.haveInitMagVar;
    haveInitElev = org.haveInitElev;
    initCmdAlt = org.initCmdAlt;
    haveInitCmdAlt = org.haveInitCmdAlt;
    initCmdAirspeed = org.initCmdAirspeed;
    haveInitCmdAs = org.haveInitCmdAs;

    tbrg = org.tbrg;
    mbrg = org.mbrg;
    dst = org.dst;
    ttg = org.ttg;
    xte = org.xte;

    tcrs = org.tcrs;
    mcrs = org.mcrs;
    tlt = org.tlt;
    tld = org.tld;

    tde = org.tde;
    ete = org.ete;
    eta = org.eta;
    elt = org.elt;
    scaWarn = org.scaWarn;
    navDataValid = org.navDataValid;
}

void Steerpoint::deleteData()
{
    next = nullptr;
    initNextStptName = nullptr;
    description = nullptr;
}

void Steerpoint::reset()
{
    if (haveInitElev) {
        elevation  = initElev;
        initPosVec[IPlayer::IDOWN] = -initElev;
        posVec[IPlayer::IDOWN] = -initElev;
    }

    // ---
    // Set initial positions -- give priority to lat/lon entries
    // ---
    if (haveInitLat && haveInitLon) {
        setLatitude(initLatitude);
        setLongitude(initLongitude);
        setElevation(initElev);
    } else if (haveInitPos) {
        setPosition(initPosVec);
    }

    if (haveInitCmdAlt) {
       setCmdAltitude( initCmdAlt );
    }

    if (haveInitCmdAs) {
       setCmdAirspeedKts( initCmdAirspeed );
    }

    BaseClass::reset();
}

//------------------------------------------------------------------------------
// Data access functions
//------------------------------------------------------------------------------

double Steerpoint::getElevationFt() const
{
    return getElevationM() * base::length::M2FT;
}

const char* Steerpoint::getDescription() const
{
    const char* p{};
    if (description != nullptr) p = description->c_str();
    return p;
}

double Steerpoint::getLatitude() const
{
    double ll{};
    // Make sure we have a valid lat/lon
    if (!needLL) ll = latitude;
    return ll;
}

double Steerpoint::getLongitude() const
{
    double ll = 0;
    // Make sure we have a valid lat/lon
    if (!needLL) ll = longitude;
    return ll;
}

double Steerpoint::getCmdAltitudeFt() const
{
    return getCmdAltitude() * base::length::M2FT;
}

//------------------------------------------------------------------------------
// Set the ground elevation at the steerpoint from this terrain database
// Returns true if successful.
//------------------------------------------------------------------------------
bool Steerpoint::setElevation(const terrain::ITerrain* const terrain, const bool interp)
{
   bool ok{};
   if (!needLL) {
      double elev{};
      ok = terrain->getElevation(&elev, latitude, longitude, interp);
      if (ok) setElevation(elev);
   }
   return ok;
}

//------------------------------------------------------------------------------
// Data set functions
//------------------------------------------------------------------------------
void Steerpoint::setPosition(const double x, const double y, const double z)
{
    posVec.set(x, y, z);
    needPosVec = false;
    needLL = true;
}

void Steerpoint::setPosition(const base::Vec3d& newPos)
{
    posVec = newPos;
    needPosVec = false;
    needLL = true;
}

void Steerpoint::setLatitude(const double v)
{
    latitude = v;
    needPosVec = true;
    needLL = false;
}

void Steerpoint::setLongitude(const double v)
{
    longitude = v;
    needPosVec = true;
    needLL = false;
}

void Steerpoint::setElevation(const double x)
{
    elevation = x;
    posVec[IPlayer::IDOWN] = -x;
}

void Steerpoint::setDescription(const base::String* const d)
{
    description = d;
}

void Steerpoint::setCmdAltitude(const double x)
{
   cmdAlt = x;
   haveCmdAlt = true;
}

void Steerpoint::setCmdAirspeedKts(const double x)
{
   cmdAirspeed = x;
   haveCmdAs = true;
}

// Sets the action to be performed
bool Steerpoint::setAction(IAction* const aa)
{
   action = aa;
   return true;
}

//------------------------------------------------------------------------------
// Set slot functions
//------------------------------------------------------------------------------
bool Steerpoint::setSlotSteerpointType(const base::Identifier* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = true;  // assume that it's valid
        if      (*msg == "DEST"  || *msg == "dest")  setSteerpointType(StptType::DEST);
        else if (*msg == "MARK"  || *msg == "mark")  setSteerpointType(StptType::MARK);
        else if (*msg == "FIX"   || *msg == "fix")   setSteerpointType(StptType::FIX);
        else if (*msg == "OAP"   || *msg == "oap")   setSteerpointType(StptType::OAP);
        else if (*msg == "TGT"   || *msg == "tgt")   setSteerpointType(StptType::TGT);
        else if (*msg == "IP"    || *msg == "ip")    setSteerpointType(StptType::IP);
        else {
            std::cerr << "Steerpoint::setSlotSteerpointType(): invalid steerpoint type: " << *msg << std::endl;
            std::cerr << " -- valid types are { dest, mark, fix, oap, tgt }" << std::endl;
            ok = false; // it's no longer ok
        }
    }
    return ok;
}

bool Steerpoint::setSlotLatitude(const base::Latitude* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        initLatitude = msg->getDecimalDegrees();
        haveInitLat = true;
        setLatitude( initLatitude );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotLatitude(const base::INumber* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        initLatitude = msg->asDouble();
        haveInitLat = true;
        setLatitude( initLatitude );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotLongitude(const base::Longitude* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        initLongitude = msg->getDecimalDegrees();
        haveInitLon = true;
        setLongitude( initLongitude );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotLongitude(const base::INumber* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        initLongitude = msg->asDouble();
        haveInitLon = true;
        setLongitude( initLongitude );
        ok = true;
    }
    return ok;
}

/*
bool Steerpoint::setSlotPosition(const base::List* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        double values[3];
        const int n = msg->getNumberList(values, 3);
        if (n == 3) {
            initPosVec[0] = values[0];
            initPosVec[1] = values[1];
            initPosVec[2] = values[2];
            haveInitPos = true;
            setPosition( initPosVec[0], initPosVec[1], initPosVec[2] );
            ok = true;
        }
    }
    return ok;
}
*/

bool Steerpoint::setSlotXPos(const base::ILength* const x)
{
    bool ok{};
    if (x != nullptr) {
        initPosVec[IPlayer::INORTH] = x->getValueInMeters();
        haveInitPos = true;
        setPosition( initPosVec[0], initPosVec[1], initPosVec[2] );
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotYPos(const base::ILength* const x)
{
    bool ok{};
    if (x != nullptr) {
        initPosVec[IPlayer::IEAST] = x->getValueInMeters();
        haveInitPos = true;
        setPosition( initPosVec[0], initPosVec[1], initPosVec[2] );
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotElevation(const base::ILength* const x)
{
    bool ok{};
    if (x != nullptr) {
        initElev = x->getValueInMeters();
        elevation  = initElev;
        initPosVec[IPlayer::IDOWN] = -initElev;
        posVec[IPlayer::IDOWN] = -initElev;
        haveInitElev = true;
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotElevation(const base::INumber* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        initElev = msg->asDouble();
        elevation  = initElev;
        initPosVec[IPlayer::IDOWN] = -initElev;
        posVec[IPlayer::IDOWN] = -initElev;
        haveInitElev = true;
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotPTA(const base::ITime* const x)
{
    bool ok{};
    if (x != nullptr) {
        setPTA(x->getValueInSeconds());
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotPTA(const base::INumber* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        // assumes seconds
        setPTA( msg->asDouble() );
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotSCA(const base::ILength* const x)
{
    bool ok{};
    if (x != nullptr) {
        setSCA(x->getValueInFeet());
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotSCA(const base::INumber* const x)
{
    bool ok{};
    if (x != nullptr) {
        // assumes feet
        setSCA(x->asDouble());
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotDescription(const base::String* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        setDescription(msg);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotMagVar(const base::IAngle* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        initMagVar = msg->getValueInDegrees();
        haveInitMagVar = true;
        ok = true;
    }
    return ok;
}
bool Steerpoint::setSlotMagVar(const base::INumber* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        // assumes degrees
        initMagVar = msg->asDouble();
        haveInitMagVar = true;
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotCmdAltitude(const base::ILength* const x)
{
    bool ok{};
    if (x != nullptr) {
        initCmdAlt = x->getValueInMeters();
        haveInitCmdAlt = true;
        setCmdAltitude(initCmdAlt);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotCmdAltitude(const base::INumber* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        initCmdAlt = msg->asDouble();
        haveInitCmdAlt = true;
        setCmdAltitude(initCmdAlt);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotCmdAirspeed(const base::INumber* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        initCmdAirspeed = msg->asDouble();
        haveInitCmdAs = true;
        setCmdAirspeedKts(initCmdAirspeed);
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotNext(const base::Identifier* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        initNextStptName = msg;
        ok = true;
    }
    return ok;
}

bool Steerpoint::setSlotNext(const base::Integer* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        initNextStptIdx = msg->asInt();
        ok = true;
    }
    return ok;
}

//------------------------------------------------------------------------------
// clearNavData() -- Clear the nav data
//------------------------------------------------------------------------------
void Steerpoint::clearNavData()
{
    setTrueBrgDeg(0);
    setMagBrgDeg(0);
    setDistNM(0);
    setTTG(0);
    setCrossTrackErrNM(0);
    setTrueCrsDeg(0);
    setMagCrsDeg(0);
    setLegDistNM(0);
    setLegTime(0);
    setDistEnrouteNM(0);
    setETE(0);
    setETA(0);
    setELT(0);
    navDataValid = false;
}

//------------------------------------------------------------------------------
// compute() -- Compute steerpoint data
//------------------------------------------------------------------------------
bool Steerpoint::compute(const INavigation* const nav, const Steerpoint* const from)
{
    bool ok{};
    if (nav != nullptr) {

        // ---
        // Update Mag Var (if needed)
        // ---
        if (haveInitMagVar) {
            magvar = initMagVar;
        } else {
            magvar = static_cast<double>(nav->getMagVarDeg());
        }

        // ---
        // Make sure we have a position vector and compute lat/lon, if needed
        // ---
        if ( !isLatLonValid() && isPosVecValid() ) {
            // Compute our lat/lon when we only have the Pos Vec
            double elev = 0.0;
            base::nav::convertPosVec2LL(nav->getRefLatitude(), nav->getRefLongitude(), posVec, &latitude, &longitude, &elev);
            elevation  = static_cast<double>(elev);
            needLL = false;
        }
        if ( isLatLonValid() && !isPosVecValid() ) {
            // Compute our Pos Vec when we only have the lat/lon
            base::nav::convertLL2PosVec(nav->getRefLatitude(), nav->getRefLongitude(), latitude, longitude, elevation, &posVec);
            needPosVec = false;
        }

        // ## Note: at this point we need a valid lat/lon position

        if (isLatLonValid()) {

            // ---
            // Compute 'direct-to' bearing,  distance & time
            // ---
            double toBrg{};
            double toDist{};
            double toTTG{};
            base::nav::gll2bd(nav->getLatitude(), nav->getLongitude(), getLatitude(), getLongitude(), &toBrg, &toDist);

            setTrueBrgDeg( static_cast<double>(toBrg) );
            setDistNM( static_cast<double>(toDist) );
            setMagBrgDeg( base::angle::aepcdDeg( getTrueBrgDeg() - getMagVarDeg() ) );

            if (nav->isVelocityDataValid() && nav->getGroundSpeedKts() > 0.0) {
                toTTG = (toDist/nav->getGroundSpeedKts()) * base::time::H2S;
            }
            setTTG(static_cast<double>(toTTG));

            // ---
            // Compute 'leg' course, distance & time, as well as enroute distance & times
            // ---
            toBrg = 0.0;
            toDist = 0.0;
            toTTG = 0.0;
            if (from != nullptr) {
                // When we have a 'from' steerpoint, we can compute this leg's data
                base::nav::gll2bd(from->getLatitude(), from->getLongitude(), getLatitude(), getLongitude(), &toBrg, &toDist);
                setTrueCrsDeg( static_cast<double>(toBrg) );
                setMagCrsDeg( base::angle::aepcdDeg( getTrueCrsDeg() - getMagVarDeg() ) );
                setLegDistNM( static_cast<double>(toDist) );
                if (nav->isVelocityDataValid() && nav->getGroundSpeedKts() > 0.0) {
                    toTTG = (toDist/nav->getGroundSpeedKts()) * base::time::H2S;
                }
                setLegTime( static_cast<double>(toTTG) );
                setDistEnrouteNM( from->getDistEnrouteNM() + getLegDistNM() );
                setETE( from->getETE() + getLegTime() );
            } else {
                // When we don't have a 'from' steerpoint, this leg is the same as the direct-to data
                setTrueCrsDeg( getTrueBrgDeg() );
                setMagCrsDeg( getMagBrgDeg() );
                setLegDistNM( getDistNM() );
                setLegTime( getTTG() );
                setDistEnrouteNM( getDistNM() );
                setETE( getTTG() );
            }

            // ---
            // Compute Est Time of Arrival and the PTA Early/Late time
            // ---
            setETA( static_cast<double>(getETE() + nav->getUTC()) );
            double delta{getPTA() - getETA()};
            if (delta >= base::time::D2S) delta -= base::time::D2S;
            setELT( delta );

            // ---
            // Compute Cross-track error (NM); negative values are when the desired track
            //  to this point is left of our navigation position
            // ---
            double aa{base::angle::aepcdDeg( getTrueBrgDeg() - getTrueCrsDeg() ) * static_cast<double>(base::angle::D2RCC)};
            setCrossTrackErrNM( getDistNM() * std::sin(aa) );

            // ---
            // Update our component steerpoint list (from NAV data, or 'direct-to' only)
            // ---
            base::PairStream* steerpoints{getComponents()};
            if (steerpoints != nullptr) {
                base::List::Item* item{steerpoints->getFirstItem()};
                while (item != nullptr) {
                    base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
                    Steerpoint* p{static_cast<Steerpoint*>(pair->object())};
                    p->compute(nav);
                    item = item->getNext();
                }
                steerpoints->unref();
                steerpoints = nullptr;
            }

            // ---
            // Check for safe clearance altitude warning
            // ---
            scaWarn = nav->getAltitudeFt() < getSCA();
            navDataValid = true;
        } else {
            navDataValid = false;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// processComponets() -- process our components; make sure the are all of
// type Steerpoint (or derived); tell them that we are their container
//------------------------------------------------------------------------------
void Steerpoint::processComponents(
      base::PairStream* const list,
      const std::type_info&,
      base::Pair* const add,
      base::IComponent* const remove
   )
{
   base::IComponent::processComponents(list,typeid(Steerpoint),add,remove);
}

}
}
