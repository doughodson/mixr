
#include "mixr/graphics/MapPage.hpp"
#include "mixr/graphics/Display.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/INumber.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(MapPage, "MapPage")
EMPTY_DELETEDATA(MapPage)

BEGIN_SLOTTABLE(MapPage)
    "outerRadius",              // 1) radius (centered) of our outer circle (units)
    "outerRadiusDecentered",    // 2) radius (de-centered) of our outer arc (units)
    "range",                    // 3) our range (NM) of our outer circle or arc
    "displacement",             // 4) how far to translate when we are decentered (units)
    "centered",                 // 5) are we centered, or de-centered (default: centered)
    "refLat",                   // 6) reference latitude (degs)
    "refLon",                   // 7) reference longitude (degs)
    "refHdg",                   // 8) reference heading (degs)
    "northUp",                  // 9) north up mode
END_SLOTTABLE(MapPage)

BEGIN_SLOT_MAP(MapPage)
    ON_SLOT(1, setSlotOuterRadius,   base::INumber)
    ON_SLOT(2, setSlotOuterRadiusDC, base::INumber)
    ON_SLOT(3, setSlotRange,         base::INumber)
    ON_SLOT(4, setSlotDisplacement,  base::INumber)
    ON_SLOT(5, setSlotCentered,      base::Boolean)
    ON_SLOT(6, setSlotRefLat,        base::INumber)
    ON_SLOT(7, setSlotRefLon,        base::INumber)
    ON_SLOT(8, setSlotRefHdg,        base::INumber)
    ON_SLOT(9, setSlotNorthUp,       base::Boolean)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(MapPage)
    ON_EVENT_OBJ(UPDATE_VALUE,  onUpdateRange,         base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE2, onUpdateHeading,       base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE3, onUpdateReferenceLat,  base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE4, onUpdateReferenceLon,  base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE5, onUpdateCentered,      base::Boolean)
    ON_EVENT_OBJ(UPDATE_VALUE6, onUpdateOuterRadius,   base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE7, onUpdateOuterRadiusDC, base::INumber)
    ON_EVENT_OBJ(UPDATE_VALUE8, onUpdateDisplacement,  base::INumber)
END_EVENT_HANDLER()

MapPage::MapPage()
{
   STANDARD_CONSTRUCTOR()
}

void MapPage::copyData(const MapPage& org, const bool)
{
   BaseClass::copyData(org);

   heading = org.heading;
   headingSin = org.headingSin;
   headingCos = org.headingCos;
   referenceLat = org.referenceLat;
   referenceLon = org.referenceLon;
   cosineLatReference = org.cosineLatReference;

   range = org.range;
   outerRadius = org.outerRadius;
   outerRadiusDC = org.outerRadiusDC;
   nm2Screen = org.nm2Screen;
   displacement = org.displacement;
   isCentered = org.isCentered;
   northUp = org.northUp;
}

void MapPage::updateData(const double dt)
{
   // find our nearest map page above us and get the data from it!
   const auto page = static_cast<MapPage*>(findContainerByType(typeid(MapPage)));
   if (page != nullptr) {
      setHeadingDeg(page->getHeadingDeg());
      setDisplacement(page->getDisplacement());
      setReferenceLatDeg(page->getReferenceLatDeg());
      setReferenceLonDeg(page->getReferenceLonDeg());
      setOuterRadius(page->getOuterRadius());
      setOuterRadiusDC(page->getOuterRadiusDC());
      setCentered(page->getCentered());
      setRange(page->getRange());
      setNorthUp(page->getNorthUp());
      nm2Screen = page->getScale();
   }
   // if we are the top map page, we do the calculations ourself
   else {
      // determine if we are centered or not, and set our displacement and radius accordingly
      if (isCentered) nm2Screen = outerRadius / range;
      else nm2Screen = outerRadiusDC / range;
   }

   // update base class stuff
   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// pixelsToLatLon() - determine our lat lon position from a pixel X/Y
//------------------------------------------------------------------------------
void MapPage::pixelsToLatLon(const int x, const int y, double &lat, double &lon)
{
   double xpos {static_cast<double>(x)};
   double ypos {static_cast<double>(y)};

   // convert pixels position from top left to movement from center, so we
   // can base our cals from the center.
   if (getDisplay() != nullptr) {
      double myLat {}, myLon {};
      double dLat {}, dLon {};
      GLsizei vpW {}, vpH {};
      getDisplay()->getViewportSize(&vpW, &vpH);
      GLdouble tl {}, tr {}, tb {}, tt {}, tn {}, tf {};
      getDisplay()->getOrtho(tl, tr, tb, tt, tn, tf);

      // determine the middle of our viewing screen
      const double startX {vpW / 2.0};
      const double startY {vpH / 2.0};

      // find the position from our screen center (in pixels)
      xpos -= startX;
      ypos = startY - ypos;

      // we have to find our units per pixel
      const double unitsPerPixE {(tr * 2) / vpW};
      const double unitsPerPixN {(tt * 2) / vpH};

      // now from that we can determine our position in inches
      xpos *= unitsPerPixE;
      ypos *= unitsPerPixN;

      if (!isCentered) ypos -= displacement;

      // ok, now we can find our xpos and ypos in nautical miles
      xpos /= nm2Screen;
      ypos /= nm2Screen;

      if (!getNorthUp()) {
         double acX {};
         double acY {};
         earth2Aircraft( static_cast<double>(xpos), static_cast<double>(ypos), &acX, &acY);
         xpos = acX;
         ypos = acY;
      }

      // reference lat to figure off of (if centered())
      myLat = referenceLat;
      myLon = referenceLon;

      // now convert to degree from our reference lat/lon
      dLat = ypos / 60.0;
      dLon = xpos / (60.0 * cosineLatReference);

      myLat += dLat;
      myLon += dLon;

      lat = myLat;
      lon = myLon;
   }
}

//------------------------------------------------------------------------------
// Moves our reference lat lon based on the distance moved.
//------------------------------------------------------------------------------
void MapPage::moveMap(const int startX, const int startY, const int endX, const int endY)
{
   double eLat {}, eLon {}, lat {}, lon {}, sLat {}, sLon {}, deltaLat {}, deltaLon {};

   // figure our lat/lon positioning
   pixelsToLatLon(startX, startY, sLat, sLon);
   pixelsToLatLon(endX, endY, eLat, eLon);

   // get our delta movement
   deltaLat = sLat - eLat;
   deltaLon = sLon - eLon;

   lat = getReferenceLatDeg();
   lon = getReferenceLonDeg();

   lat += deltaLat;
   lon += deltaLon;

   // As the world turns.... we need to flip the maps!
   if (lon > 180) lon = -180;
   else if (lon < -180) lon = 180;
   if (lat > 90) lat = -90;
   else if (lat < -90) lat = 90;

   setReferenceLatDeg(lat);
   setReferenceLonDeg(lon);
}


//------------------------------------------------------------------------------
// converts lat/lon to screen coordinates
//------------------------------------------------------------------------------
bool MapPage::latLon2Screen(const double lat, const double lon,
                              double* const x, double* const y) const
{
   bool ok {};
   if (x != nullptr && y != nullptr) {

      // Convert to nautical miles (NED) centered on ownship
      const double earthX {(lat - getReferenceLatDeg()) * 60.0};
      const double earthY {(lon - getReferenceLonDeg()) * 60.0 * getCosRefLat()};

      // Rotate to aircraft coordinates
      double acX {};
      double acY {};
      if (getNorthUp()) {
         acX = earthX;
         acY = earthY;
      }
      else {
         acX =  (earthX * headingCos) + (earthY * headingSin);
         acY = -(earthX * headingSin) + (earthY * headingCos);
      }

      // Scale from nautical miles to inches and
      // transpose the X and Y from A/C to screen
      double screenY {acX * getScale()};
      const double screenX {acY * getScale()};

      // Adjust for the decentered displayment
      if ( !getCentered() ) screenY += getDisplacement();

      *x = static_cast<double>(screenX);
      *y = static_cast<double>(screenY);

      ok = true;
   }

   return ok;
}

//------------------------------------------------------------------------------
// converts screen to lat/lon coordinates
//------------------------------------------------------------------------------
bool MapPage::screen2LatLon(const double x, const double y,
                              double* const lat, double* const lon) const
{
   bool ok {};

   const double scale {getScale()};
   const double cosLat {getCosRefLat()};
   if (lat != nullptr && lon != nullptr && scale != 0 && cosLat != 0) {

      // buffer the inputs
      double screenX {x};
      double screenY {y};

      // Adjust for the decentered displayment
      if ( !getCentered() ) screenY -= getDisplacement();

      // Scale from screen (inches) to A/C (NM) and
      // transpose the X and Y from screen to A/C
      const double acX {screenY/scale};
      const double acY {screenX/scale};

      // Rotate A/C to NED
      double earthX {};
      double earthY {};
      if (getNorthUp()) {
         earthX = acX;
         earthY = acY;
      } else {
         earthX = (acX * headingCos) - (acY * headingSin);
         earthY = (acX * headingSin) + (acY * headingCos);
      }

      // Convert nautical miles (NED) from ref point to lat/lon
      *lat = (earthX/60.0) + getReferenceLatDeg();
      *lon = (earthY/(60.0*cosLat)) + getReferenceLonDeg();

      ok = true;
   }
   return ok;
}


//------------------------------------------------------------------------------
// aircraft2Screen() - converts real world coordinates to X, Y in inches.
//------------------------------------------------------------------------------
bool MapPage::aircraft2Screen(const double acX, const double acY, double* const screenX, double* const screenY) const
{
   bool ok {};

   if (screenX != nullptr && screenY != nullptr) {

      const double newX {acX * nm2Screen};
      const double newY {acY * nm2Screen};

      // now set the pointers to the new X and Y values
      *screenX = static_cast<double>(newY);
      *screenY = static_cast<double>(newX);

      ok = true;
   }

   return ok;
}

//------------------------------------------------------------------------------
// earth2Aircraft() - converts earth coordinates to aircraft coordinates.
//------------------------------------------------------------------------------
bool MapPage::earth2Aircraft(const double earthX, const double earthY, double* const acX, double* const acY) const
{
   bool ok {};
   if (acX != nullptr && acY != nullptr) {
      const double x {(earthX * headingCos) + (earthY * headingSin)};
      const double y {-(earthX * headingSin) + (earthY * headingCos)};
      *acX = static_cast<double>(x);
      *acY = static_cast<double>(y);
      ok = true;
   }

   return ok;
}

//------------------------------------------------------------------------------
// latLonToEarth() - converts a latitude/longitude to earth coordinates.
//------------------------------------------------------------------------------
bool MapPage::latLon2Earth(const double lat, const double lon, double* const earthX, double* const earthY) const
{
   bool ok {};
   if (earthX != nullptr && earthY != nullptr) {
      *earthX = static_cast<double>((lat - referenceLat) * 60.0);
      *earthY = static_cast<double>((lon - referenceLon) * 60.0 * cosineLatReference);
      ok = true;
   }

   return ok;
}

//------------------------------------------------------------------------------
// event functions
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// onUpdateRange() - set our new range
//------------------------------------------------------------------------------
bool MapPage::onUpdateRange(const base::INumber* const newR)
{
   bool ok {};
   if (newR != nullptr) ok = setRange(newR->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// onUpdateHeading() - set our new heading
//------------------------------------------------------------------------------
bool MapPage::onUpdateHeading(const base::INumber* const newH)
{
   bool ok {};
   if (newH != nullptr) ok = setHeadingDeg(newH->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// onUpdateReferenceLat() - set our new reference latitude
//------------------------------------------------------------------------------
bool MapPage::onUpdateReferenceLat(const base::INumber* const newOL)
{
   bool ok {};
   if (newOL != nullptr) ok = setReferenceLatDeg(newOL->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// onUpdateReferenceLon() - set our new reference longitude
//------------------------------------------------------------------------------
bool MapPage::onUpdateReferenceLon(const base::INumber* const newOL)
{
   bool ok {};
   if (newOL != nullptr) ok = setReferenceLonDeg(newOL->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// onUpdateCentered() - set our centered / decentered flag
//------------------------------------------------------------------------------
bool MapPage::onUpdateCentered(const base::Boolean* const newC)
{
   bool ok {};
   if (newC != nullptr) ok = setCentered(newC->asBool());
   return ok;
}

//------------------------------------------------------------------------------
// onUpdateOuterRadius() - set our new outer map area
//------------------------------------------------------------------------------
bool MapPage::onUpdateOuterRadius(const base::INumber* const newR)
{
   bool ok {};
   if (newR != nullptr) ok = setOuterRadius(newR->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// onUpdateOuterRadiusDC() - set our new outer map area, decentered
//------------------------------------------------------------------------------
bool MapPage::onUpdateOuterRadiusDC(const base::INumber* const newRDC)
{
   bool ok {};
   if (newRDC != nullptr) ok = setOuterRadiusDC(newRDC->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// onUpdateDisplacement() - set our new displacement
//------------------------------------------------------------------------------
bool MapPage::onUpdateDisplacement(const base::INumber* const newD)
{
   bool ok {};
   if (newD != nullptr) ok = setDisplacement(newD->asDouble());
   return ok;
}

// SLOT FUNCTIONS
//------------------------------------------------------------------------------
// setSlotOuterRadius() - sets the outer radius of our compass rose, if we
// are centered
//------------------------------------------------------------------------------
bool MapPage::setSlotOuterRadius(const base::INumber* const newRadius)
{
   bool ok {};
   if (newRadius != nullptr) ok = setOuterRadius(newRadius->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// setSlotOuterRadiusDC() - sets the outer radius of our compass rose, if
// we are de-centered
//------------------------------------------------------------------------------
bool MapPage::setSlotOuterRadiusDC(const base::INumber* const newDCRadius)
{
   bool ok {};
   if (newDCRadius != nullptr) ok = setOuterRadiusDC(newDCRadius->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// setSlotRange() - sets the range of our viewing areas, in nautical miles
//------------------------------------------------------------------------------
bool MapPage::setSlotRange(const base::INumber* const newR)
{
   bool ok {};
   if (newR != nullptr) ok = setRange(newR->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// setSlotDisplacement() - sets how far we translate before we draw
//------------------------------------------------------------------------------
bool MapPage::setSlotDisplacement(const base::INumber* const newD)
{
   bool ok {};
   if (newD != nullptr) ok = setDisplacement(newD->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// setSlotCentered() - sets if we are centered or decentered
//------------------------------------------------------------------------------
bool MapPage::setSlotCentered(const base::Boolean* const newC)
{
   bool ok {};
   if (newC != nullptr) ok = setCentered(newC->asBool());
   return ok;
}

//------------------------------------------------------------------------------
// setSlotRefLat() - sets ref lat from slot
//------------------------------------------------------------------------------
bool MapPage::setSlotRefLat(const base::INumber* const x)
{
   bool ok {};
   if (x != nullptr) ok = setReferenceLatDeg(x->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// setSlotRefLon() - sets ref lon from slot
//------------------------------------------------------------------------------
bool MapPage::setSlotRefLon(const base::INumber* const x)
{
   bool ok {};
   if (x != nullptr) ok = setReferenceLonDeg(x->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// setSlotRefHdg() - sets ref heading from slot
//------------------------------------------------------------------------------
bool MapPage::setSlotRefHdg(const base::INumber* const x)
{
   bool ok {};
   if (x != nullptr) ok = setHeadingDeg(x->asDouble());
   return ok;
}

//------------------------------------------------------------------------------
// setSlotNorthUp() - sets north up / track up from slot
//------------------------------------------------------------------------------
bool MapPage::setSlotNorthUp(const base::Boolean* const x)
{
   bool ok {};
   if (x != nullptr) ok = setNorthUp(x->asBool());
   return ok;
}

}
}

