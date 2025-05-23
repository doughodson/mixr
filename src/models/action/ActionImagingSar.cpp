
#include "mixr/models/action/ActionImagingSar.hpp"

#include "mixr/models/player/weapon/Bomb.hpp"
#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/system/OnboardComputer.hpp"
#include "mixr/models/system/Sar.hpp"
#include "mixr/models/system/IStoresMgr.hpp"
#include "mixr/models/navigation/Steerpoint.hpp"

#include "mixr/models/WorldModel.hpp"

#include "mixr/base/IComponent.hpp"
#include "mixr/base/Latitude.hpp"
#include "mixr/base/Longitude.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/lengths.hpp"
#include "mixr/base/util/nav_utils.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(ActionImagingSar,"ActionImagingSar")

BEGIN_SLOTTABLE(ActionImagingSar)
   "sarLatitude",    //  1) Target's latitude
   "sarLongitude",   //  2) Target's longitude
   "sarElevation",   //  3) Target's elevation (Length) (default: 0)
   "resolution",     //  4) Image (pixel) resolution   (Length) (default: 1 meter)
   "imageSize",      //  5) Image size: height & width (pixels) (default: 512)
END_SLOTTABLE(ActionImagingSar)

BEGIN_SLOT_MAP(ActionImagingSar)
    ON_SLOT(1, setSlotSarLat,     base::Latitude)
    ON_SLOT(2, setSlotSarLon,     base::Longitude)
    ON_SLOT(3, setSlotSarElev,    base::ILength)
    ON_SLOT(4, setSlotResolution, base::ILength)
    ON_SLOT(5, setSlotImageSize,  base::Integer)
END_SLOT_MAP()

ActionImagingSar::ActionImagingSar()
{
   STANDARD_CONSTRUCTOR()
}

void ActionImagingSar::copyData(const ActionImagingSar& org, const bool)
{
   BaseClass::copyData(org);

   sarLatitude = org.sarLatitude;
   sarLongitude = org.sarLongitude;
   sarElevation = org.sarElevation;
   resolution = org.resolution;
   imgSize = org.imgSize;
   orientation = org.orientation;

   timer = 0.0;
   setSarSystem(nullptr);
}

void ActionImagingSar::deleteData()
{
}

//------------------------------------------------------------------------------
// Default function: Start the SAR capture, ATR run and Image download process
//------------------------------------------------------------------------------
bool ActionImagingSar::trigger(OnboardComputer* const mgr)
{
   bool ok{};

   if (mgr != nullptr) {

      // Find our ownship player & SAR system
      IPlayer* ownship{static_cast<IPlayer*>(mgr->findContainerByType(typeid(IPlayer)))};
      if (ownship != nullptr) {
         base::Pair* pair{ownship->getSensorByType(typeid(Sar))};
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Looking for SAR: pair = " << pair << std::endl;
         }
         if (pair != nullptr) {
            setSarSystem( static_cast<Sar*>(pair->object()) );
         }
      }

      if (sar != nullptr && ownship != nullptr) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Requesting an image from the SAR: refId: " << getRefId() << std::endl;
         }
         sar->setStarePoint( getSarLatitude(), getSarLongitude(), static_cast<double>(getSarElevation()) );
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "And resolution: " << getResolution() << std::endl;
         }
         sar->requestImage(getImageSize(), getImageSize(), getResolution() );
         ok = true;
      }

      timer = 0;
   }

   if (ok) setManager(mgr);
   else setManager(nullptr);

   return ok;
}

//------------------------------------------------------------------------------
// Cancels this action
//------------------------------------------------------------------------------
bool ActionImagingSar::cancel()
{
   if (getSarSystem() != nullptr) {
      getSarSystem()->cancel();
   }
   return BaseClass::cancel();
}

//------------------------------------------------------------------------------
// process() -- action processing
//------------------------------------------------------------------------------
void ActionImagingSar::process(const double dt)
{
   BaseClass::process(dt);

   if (isInProgress() && getSarSystem() != nullptr) {

      // Check if SAR has finished
      if ( !getSarSystem()->isImagingInProgress()) {
         // Imaging is NOT in progress, so it must have completed ...
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Processing SAR -- completed!" << std::endl;
         }
         setCompleted(true);
      }

      // Process safety time-out
      timer += dt;
      static const double MAX_SAR_TIME{120.0};
      if (timer > MAX_SAR_TIME) {
         // Cancel the SAR
         cancel();
         setCompleted(true);
      }
   }
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

// Sets the SAR latitude in degrees
bool ActionImagingSar::setSarLatitude(const double v)
{
   bool ok{};
   if (v >= -90.0 && v <= 90.0) {
      sarLatitude = v;
      ok = true;
   }
   return ok;
}

// Sets the SAR longitude in degrees
bool ActionImagingSar::setSarLongitude(const double v)
{
   bool ok{};
   if (v >= -180.0 && v <= 180.0) {
      sarLongitude = v;
      ok = true;
   }
   return ok;
}

// Sets the SAR elevation in meters
bool ActionImagingSar::setSarElevation(const double v)
{
   sarElevation = v;
   return true;
}

// Sets the SAR image resolution (meters)
bool ActionImagingSar::setResolution(const double r)
{
   resolution = r;
   return true;
}

// Sets the image size
bool ActionImagingSar::setImageSize(const unsigned int v)
{
   bool ok{};
   if (v == 512 || v == 1024 || v == 2048 || v == 4096) {
      imgSize = v;
      ok = true;
   }
   return ok;
}

// Sets the completed flag
void ActionImagingSar::setCompleted(const bool flg)
{
   if (flg) setSarSystem(nullptr);
   BaseClass::setCompleted(flg);
}

// Set the SAR system
void ActionImagingSar::setSarSystem(Sar* const p)
{
   if (sar != nullptr) {
      sar->unref();
   }
   sar = p;
   if (sar != nullptr) {
      sar->ref();
   }
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool ActionImagingSar::setSlotSarLat(const base::Latitude* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setSarLatitude(x->getDecimalDegrees());
   }
   return ok;
}

bool ActionImagingSar::setSlotSarLon(const base::Longitude* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setSarLongitude(x->getDecimalDegrees());
   }
   return ok;
}

bool ActionImagingSar::setSlotSarElev(const base::ILength* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setSarElevation(x->getValueInMeters());
   }
   return ok;
}

bool ActionImagingSar::setSlotResolution(const base::ILength* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setResolution(x->getValueInMeters());
   }
   return ok;
}

bool ActionImagingSar::setSlotImageSize(const base::Integer* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setImageSize(x->asInt());
      if (!ok) {
         if (isMessageEnabled(MSG_INFO)) {
         std::cout << "  ActionImagingSar::setSlotImageSize must be 512, 1024, 2048 or 4096, not " << x->asInt() << std::endl;
      }
   }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Computes the planned image orientation (degs)
//------------------------------------------------------------------------------
double ActionImagingSar::computeOrientation(const Steerpoint* const wp)
{
   if (wp != nullptr) {
      double slat{wp->getLatitude()};
      double slon{wp->getLongitude()};
      double dlat{getSarLatitude()};
      double dlon{getSarLongitude()};
      double brg{}, distNM{};
      base::nav::gll2bd(slat, slon, dlat, dlon, &brg, &distNM);
      orientation = static_cast<double>(-brg);
   }
   return orientation;
}

}
}
