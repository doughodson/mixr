
#include "mixr/models/IWorldModel.hpp"

#include "mixr/base/EarthModel.hpp"
#include "mixr/base/Identifier.hpp"
#include "mixr/base/IPairStream.hpp"
#include "mixr/base/Pair.hpp"

#include "mixr/base/Latitude.hpp"
#include "mixr/base/Longitude.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/INumber.hpp"

#include "mixr/base/util/nav_utils.hpp"

// environment models
#include "mixr/models/environment/IAtmosphere.hpp"
#include "mixr/terrain/ITerrain.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(IWorldModel, "IWorldModel")

BEGIN_SLOTTABLE(IWorldModel)
   "latitude",                // 1) Ref latitude
   "longitude",               // 2) Ref longitude

   "gamingAreaRange",         // 3) Max valid range of the simulation's gaming area or zero for unlimited
                              //    (default: zero -- unlimited range)

   "earthModel",              // 4) Earth model for geodetic lat/lon (default is WGS-84)

   "gamingAreaUseEarthModel", // 5) If true, use the 'earthModel' or its WGS-84 default for flat
                              //    earth projections between geodetic lat/lon and the gaming
                              //    area's NED coordinates.  Otherwise, use a standard spherical
                              //    earth with a radius of nav::ERAD60. (default: false)

   "terrain",                 //  6) Terrain elevation database
   "atmosphere",              //  7) Atmospheric model
END_SLOTTABLE(IWorldModel)

BEGIN_SLOT_MAP(IWorldModel)
    ON_SLOT( 1, setSlotRefLatitude,          base::Latitude)
    ON_SLOT( 1, setSlotRefLatitude,          base::INumber)

    ON_SLOT( 2, setSlotRefLongitude,         base::Longitude)
    ON_SLOT( 2, setSlotRefLongitude,         base::INumber)

    ON_SLOT( 3, setSlotGamingAreaRange,      base::ILength)

    ON_SLOT( 4, setSlotEarthModel,           base::EarthModel)
    ON_SLOT( 4, setSlotEarthModel,           base::Identifier)

    ON_SLOT( 5, setSlotGamingAreaEarthModel, base::Boolean)

    ON_SLOT( 6, setSlotTerrain,              terrain::ITerrain)
    ON_SLOT( 7, setSlotAtmosphere,           IAtmosphere)
END_SLOT_MAP()

IWorldModel::IWorldModel()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void IWorldModel::initData()
{
   base::nav::computeWorldMatrix(refLat, refLon, &wm);
}

void IWorldModel::copyData(const IWorldModel& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setEarthModel( org.em );

   // Center of the gaming area
   refLat = org.refLat;
   refLon = org.refLon;
   sinRlat = org.sinRlat;
   cosRlat = org.cosRlat;
   maxRefRange = org.maxRefRange;
   gaUseEmFlg = org.gaUseEmFlg;
   wm = org.wm;


   if (org.terrain != nullptr) {
      terrain::ITerrain* copy = org.terrain->clone();
      setSlotTerrain( copy );
      copy->unref();
   }
   else {
      setSlotTerrain(nullptr);
   }

   if (org.atmosphere != nullptr) {
      IAtmosphere* copy = org.atmosphere->clone();
      setSlotAtmosphere( copy );
      copy->unref();
   }
   else {
      setSlotAtmosphere(nullptr);
   }
}

void IWorldModel::deleteData()
{
   setSlotAtmosphere( nullptr );
   setSlotTerrain( nullptr );
}

void IWorldModel::reset()
{
   BaseClass::reset();

   // ---
   // First time reset of terrain database will load the data
   // ---
   if (terrain != nullptr) {
      std::cout << "Loading Terrain Data..." << std::endl;
      terrain->reset();
      std::cout << "Finished!" << std::endl;
   }

   // ---
   // Reset atmospheric model
   // ---
   if (atmosphere != nullptr) atmosphere->reset();
}

bool IWorldModel::shutdownNotification()
{
   // ---
   // Shutdown our baseclass, which will notify our components
   // ---
   BaseClass::shutdownNotification();

   // ---
   // Tell the environments ...
   // ---
   if (atmosphere != nullptr) atmosphere->event(SHUTDOWN_EVENT);
   if (terrain != nullptr) terrain->event(SHUTDOWN_EVENT);

   return true;
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Returns a pointer to the EarthModel
const base::EarthModel* IWorldModel::getEarthModel() const
{
   return em;
}

// Gaming area using the earth model?
bool IWorldModel::isGamingAreaUsingEarthModel() const
{
   return gaUseEmFlg;
}

// Returns the reference latitude
double IWorldModel::getRefLatitude() const
{
   return refLat;
}

// Returns the reference longitude
double IWorldModel::getRefLongitude() const
{
   return refLon;
}

// Returns the sine of the reference latitude
double IWorldModel::getSinRefLat() const
{
   return sinRlat;
}

// Returns the cosine of the reference latitude
double IWorldModel::getCosRefLat() const
{
   return cosRlat;
}

// Max valid range (meters) of the gaming area or zero if there's no limit.
double IWorldModel::getMaxRefRange() const
{
   return maxRefRange;
}

// World transformation matrix:
const base::Matrixd& IWorldModel::getWorldMat() const
{
   return wm;
}

//------------------------------------------------------------------------------
// Data set routines
//------------------------------------------------------------------------------

bool IWorldModel::setEarthModel(const base::EarthModel* const msg)
{
   if (em != nullptr) {
      em->unref();
      em = nullptr;
   }

   if (msg != nullptr) {
      em = msg->clone();
   }

   return true;
}

bool IWorldModel::setGamingAreaUseEarthModel(const bool flg)
{
   gaUseEmFlg = flg;
   return true;
}

// Sets Ref latitude
bool IWorldModel::setRefLatitude(const double v)
{
   bool ok{v <= 90.0 && v >= -90.0};
   if (ok) {
      // Set the latitude and compute the world matrix
      refLat = v;
      const double r{base::angle::D2RCC * refLat};
      sinRlat = std::sin(r);
      cosRlat = std::cos(r);
      base::nav::computeWorldMatrix(refLat, refLon, &wm);
   }
   return ok;
}

// Sets Ref longitude
bool IWorldModel::setRefLongitude(const double v)
{
   bool ok{v <= 180.0 && v >= -180.0};
   if (ok) {
      // Set the longitude and compute the world matrix
      refLon = v;
      base::nav::computeWorldMatrix(refLat, refLon, &wm);
   }
   return ok;
}

// Sets the max range (meters) of the gaming area or zero if there's no limit.
bool IWorldModel::setMaxRefRange(const double v)
{
   bool ok{v >= 0};
   if (ok) maxRefRange = v;
   return ok;
}

//------------------------------------------------------------------------------
// Set Slot routines
//------------------------------------------------------------------------------

bool IWorldModel::setSlotRefLatitude(const base::Latitude* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = setRefLatitude(msg->getDecimalDegrees());
    }
    return ok;
}

bool IWorldModel::setSlotRefLatitude(const base::INumber* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = setRefLatitude(msg->asDouble());
    }
    return ok;
}

bool IWorldModel::setSlotRefLongitude(const base::Longitude* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = setRefLongitude(msg->getDecimalDegrees());
    }
    return ok;
}

bool IWorldModel::setSlotRefLongitude(const base::INumber* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = setRefLongitude(msg->asDouble());
    }
    return ok;
}

bool IWorldModel::setSlotGamingAreaRange(const base::ILength* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setMaxRefRange(x->getValueInMeters());
   }
   return ok;
}

bool IWorldModel::setSlotEarthModel(const base::EarthModel* const msg)
{
   return setEarthModel(msg);
}

bool IWorldModel::setSlotEarthModel(const base::Identifier* const msg)
{
   bool ok{};
   if (msg != nullptr && msg->asString().length() > 0) {
      const base::EarthModel* p{base::EarthModel::getEarthModel((*msg).c_str())};
      if (p != nullptr) {
         // found the earth model
         ok = setEarthModel(p);
      } else {
         // Earth model not found by name
         std::cerr << "simulation::setSlotEarthModel() earth model not found: " << *msg << std::endl;
      }
   } else {
      // clear the earth model with a empty string
      ok = setEarthModel(nullptr);
   }
   return ok;
}

bool IWorldModel::setSlotGamingAreaEarthModel(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = setGamingAreaUseEarthModel(msg->asBool());
   }
   return ok;
}

// returns the terrain elevation database
const terrain::ITerrain* IWorldModel::getTerrain() const
{
   return terrain;
}

terrain::ITerrain* IWorldModel::getTerrain()
{
   return terrain;
}

// returns the atmosphere model
IAtmosphere* IWorldModel::getAtmosphere()
{
   return atmosphere;
}

// returns the atmospheric model
const IAtmosphere* IWorldModel::getAtmosphere() const
{
   return atmosphere;
}

bool IWorldModel::setSlotTerrain(terrain::ITerrain* const msg)
{
   if (terrain != nullptr) terrain->unref();
   terrain = msg;
   if (terrain != nullptr) terrain->ref();
   return true;
}

bool IWorldModel::setSlotAtmosphere(IAtmosphere* const msg)
{
   if (atmosphere != nullptr) atmosphere->unref();
   atmosphere = msg;
   if (atmosphere != nullptr) atmosphere->ref();
   return true;
}

}
}
