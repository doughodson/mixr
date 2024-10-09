
#include "mixr/models/system/Sar.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/Antenna.hpp"
#include "mixr/models/system/ScanGimbal.hpp"
#include "mixr/models/Image.hpp"

#include "mixr/models/WorldModel.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

#include "mixr/base/util/nav_utils.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Sar, "Sar")

BEGIN_SLOTTABLE(Sar)
    "chipSize",         // 1) Chip size (pixels)   <base::Number>
END_SLOTTABLE(Sar)

BEGIN_SLOT_MAP(Sar)
    ON_SLOT( 1, setSlotChipSize,  base::Integer)
END_SLOT_MAP()

// Default parameters
static const double DEFAULT_SAR_TIME{10.0};

Sar::Sar() : imgList(nullptr)
{
   STANDARD_CONSTRUCTOR()

   setReceiverEnabledFlag(false);
   setTransmitterEnableFlag(false);
}

void Sar::copyData(const Sar& org, const bool cc)
{
   BaseClass::copyData(org);

   if (cc) {
      setReceiverEnabledFlag(false);
      setTransmitterEnableFlag(false);
   }

   // Copy our data
   nextId = org.nextId;
   stareLatitude = org.stareLatitude;
   stareLongitude = org.stareLongitude;
   stareElevation = org.stareElevation;
   resolution = org.resolution;
   chipSize = org.chipSize;
   busy = org.busy;
   haveLL = org.haveLL;

   width = org.width;
   height = org.height;
   timer = org.timer;

   // Copy the images
   if (imgList != nullptr) {
      imgList->unref();
      imgList = nullptr;
   }
   if (org.imgList != nullptr) {
      imgList = org.imgList->clone();
   }
}

void Sar::deleteData()
{
    // Clear the images
    if (imgList != nullptr) {
        imgList->unref();
        imgList = nullptr;
    }
}

//------------------------------------------------------------------------------
// Access functions
//------------------------------------------------------------------------------

// Is a SAR being imaged?
bool Sar::isImagingInProgress() const
{
    return busy || timer > 0;
}

// Is the SAR system ready?
bool Sar::isSystemReady() const
{
    return haveStarePoint() && !isImagingInProgress();
}

// Return a list of all images
base::PairStream* Sar::getImages()
{
    base::PairStream* p{imgList};
    if (p != nullptr) p->ref();
    return p;
}

// Returns the last image
const Image* Sar::getImage() const
{
    const Image* p{};
    if (imgList != nullptr) {
        const base::Pair* pair{imgList->getPosition( imgList->entries() )};    // Last item
        p = dynamic_cast<const Image*>(pair->object());
        if (p != nullptr) p->ref();
    }
    return p;
}

//------------------------------------------------------------------------------
// setStarePoint() -- Set the stare point (lat and lon are in degrees, elev is in meters)
//------------------------------------------------------------------------------
bool Sar::setStarePoint(const double lat, const double lon, const double elev)
{
    stareLatitude = lat;
    stareLongitude = lon;
    stareElevation = elev;
    haveLL = true;
    return true;
}

bool Sar::setSlotChipSize(const base::Integer* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        const int n{msg->asInt()};
        if (n >= 0) {
            ok = setChipSize( n );
        }
    }
    return ok;
}

bool Sar::setResolution(const double res)
{
   resolution = res;
   return true;
}

//------------------------------------------------------------------------------
// requestImage() -- Request a new image:
//------------------------------------------------------------------------------
bool Sar::requestImage(
        const unsigned int w,           // Image width (pixels)
        const unsigned int h,           // Image height (pixels)
        const double r)                 // Image Resolution (meters/pixel)
{
   bool ok{};
   if ( isSystemReady() ) {
      if (isMessageEnabled(MSG_INFO)) {
         std::cout << "starting new SAR (" << w << "," << h << ") at " << r << std::endl;
      }
      width = w;
      height = h;
      resolution = r;
      timer = DEFAULT_SAR_TIME;
      setTransmitterEnableFlag(true);
   }
   return ok;
}

//------------------------------------------------------------------------------
// cancel() -- Cancel the current SAR imaging
//------------------------------------------------------------------------------
void Sar::cancel()
{
   setTransmitterEnableFlag(false);
   timer = 0;
}

//------------------------------------------------------------------------------
// process() --
//------------------------------------------------------------------------------
void Sar::process(const double dt)
{
    BaseClass::process(dt);

    // Imaging in progress?
   if (timer > 0) {

      // ---
      // Point the beam
      // ---
      Antenna* ant{getAntenna()};
      if (ant != nullptr) {

         const WorldModel* s{getWorldModel()};
         const double refLat{s->getRefLatitude()};
         const double refLon{s->getRefLongitude()};

         base::Vec3d pos;
         base::nav::convertLL2PosVec(
            refLat, refLon,                           // Ref point (at sea level)
            getStarePointLatitude(), getStarePointLongitude(), getStarePointElevation(),
            &pos); // x,y,z  NED

         // Platform (ownship) coord and then body
         const base::Vec3d posP{pos - getOwnship()->getPosition()};
         const base::Vec3d posB{getOwnship()->getRotMat() * posP};

         // Convert to az/el
         double tgt_az{};   // Angle (degs)
         double tgt_el{};   // Angle (degs)
         xyz2AzEl(posB, &tgt_az, &tgt_el);

         // Command to that position
         const double az{tgt_az * static_cast<double>(base::angle::D2RCC)};
         const double el{tgt_el * static_cast<double>(base::angle::D2RCC)};

         ant->setRefAzimuth(az);
         ant->setRefElevation(el);
         ant->setScanMode(ScanGimbal::ScanMode::CONICAL_SCAN);
      }

      // ---
      // Process timer
      // ---
      double ttimer{timer - dt};
      if (ttimer <= 0) {

         // ### test -- Generate a test image ###
         const auto p = new Image();
         p->testImage(width,height);
         p->setImageId(getNextId());
         p->setLatitude(getStarePointLatitude());
         p->setLongitude(getStarePointLongitude());
         p->setElevation(getStarePointElevation());
         p->setOrientation(0);
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Sar:: Generating test image: resolution: " << getResolution() << std::endl;
         }
         if (getResolution() > 0) p->setResolution( getResolution() );
         else p->setResolution( 3.0 * base::length::FT2M );
         const auto pp = new base::Pair("image", p);
         addImage(pp);
         // ### TEST

         // Just finished!
         ttimer = 0;
         setTransmitterEnableFlag(false);
      }
      timer = ttimer;
   }

   BaseClass::updateData(dt);
}


//------------------------------------------------------------------------------
// getNextId() -- Get the next image ID and increment the counter
//------------------------------------------------------------------------------
int Sar::getNextId()
{
    return nextId++;
}

//------------------------------------------------------------------------------
// Set chip size (square -- pixels)
//------------------------------------------------------------------------------
bool Sar::setChipSize(const unsigned int pixels)
{
    chipSize = pixels;
    return true;
}

//------------------------------------------------------------------------------
// addImage() -- Add an image to the end of the list of images
//------------------------------------------------------------------------------
bool Sar::addImage(base::Pair* const newImage)
{
    bool ok{};
    if (newImage != nullptr) {
        if (imgList == nullptr) {
            imgList = new base::PairStream();
        }
        imgList->put(newImage);
    }

    return ok;
}

//------------------------------------------------------------------------------
// xyz2AzEl() -- converts relative position vector to azimuth and elevation (degs)
//------------------------------------------------------------------------------
void Sar::xyz2AzEl(const double x, const double y, const double z, double* const az, double* const el)
{
   // Compute azimuth (degs)
   if (az != nullptr) {
      *az = std::atan2(y, x) * static_cast<double>(base::angle::R2DCC);
   }

   if (el != nullptr) {
      const double r{std::sqrt(x * x + y * y)};
      *el = std::atan2(-z, r) * static_cast<double>(base::angle::R2DCC);
   }
}

void Sar::xyz2AzEl(const base::Vec3d& vec, double* const az, double* const el)
{
   xyz2AzEl(vec.x(), vec.y(), vec.z(), az, el);
}

}
}
