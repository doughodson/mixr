
#include "mixr/base/LatLon.hpp"
#include "mixr/base/String.hpp"

#include <cmath>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(LatLon,"LatLon")
EMPTY_DELETEDATA(LatLon)

// DPG_NOTE -- This needs to be handled in the lexical section of the parser

BEGIN_SLOTTABLE(LatLon)
    "direction",    // { "n", "s", "e", "w" }
    "degrees",
    "minutes",
    "seconds",
END_SLOTTABLE(LatLon)

BEGIN_SLOT_MAP(LatLon)
    ON_SLOT(1, setSlotDirection, String)
    ON_SLOT(2, setSlotDegrees,   Number)
    ON_SLOT(3, setSlotMinutes,   Number)
    ON_SLOT(4, setSlotSeconds,   Number)
END_SLOT_MAP()

LatLon::LatLon()
{
   STANDARD_CONSTRUCTOR()
}

void LatLon::copyData(const LatLon& org, const bool)
{
   BaseClass::copyData(org);
   dir = org.dir;
   deg = org.deg;
   min = org.min;
   sec = org.sec;
}

//------------------------------------------------------------------------------
// computeVal() --
//     Converts direction, degrees, minutes, and seconds to a lat/long value.
//------------------------------------------------------------------------------
void LatLon::computeVal()
{
    val = deg + (static_cast<double>(min) + static_cast<double>(sec)/60.0) / 60.0;
    if (dir == Dir::SOUTH || dir == Dir::WEST) val = -val;
}

//------------------------------------------------------------------------------
// Support Functions
//------------------------------------------------------------------------------

// Sets direction
bool LatLon::setDir(const char* const d)
{
    dir = Dir::NONE;
    if (d != nullptr) {
        if (d[0] == 'N' || d[0] == 'n')      dir = Dir::NORTH;
        else if (d[0] == 'S' || d[0] == 's') dir = Dir::SOUTH;
        else if (d[0] == 'E' || d[0] == 'e') dir = Dir::EAST;
        else if (d[0] == 'W' || d[0] == 'w') dir = Dir::WEST;
    }

    if (dir == Dir::NONE && d != nullptr) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "LatLon::setDir: invalid lat/long direction: " << d << std::endl;
        }
    }
    return (dir != Dir::NONE);
}

// Sets degrees, minutes, seconds -- then calls computeVal().
bool LatLon::setDeg(const double d)
{
   bool ok{true};

   double dd{std::fabs(d)};
   if (dir == Dir::NORTH || dir == Dir::SOUTH) {
      // check for valid degree numbers
      if (dd > 90.0) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "LatLon::setDeg: invalid latitude: " << d << std::endl;
         }
         ok = false;
      }
   } else if (dir == Dir::EAST || dir == Dir::WEST) {
      // check for valid degree numbers
      if (dd > 180.0) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "LatLon::setDeg: invalid longitude: " << d << std::endl;
         }
         ok = false;
      }
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LatLon::setDeg: invalid degrees value: " << d << std::endl;
      }
      ok = false;
   }

   if (ok) {
      deg = static_cast<int>(dd);
      double m{(dd - static_cast<double>(deg)) * 60.0};
      min = static_cast<int>(m);
      sec = static_cast<double>(m - static_cast<double>(min)) * 60.0;
      computeVal();
   }
   return ok;
}

// Sets minutes, seconds -- then calls computeVal().
bool LatLon::setMin(const double m) //
{
   bool ok{true};

   double mm{std::fabs(m)};
   // check for valid minute numbers
   if (mm >= 0.0 && mm < 60.0) {
      min = static_cast<int>(mm);
      sec = (mm - static_cast<double>(min)) * 60.0;
      computeVal();
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LatLon::setMin: invalid minutes value: " << m << std::endl;
      }
      ok = false;
   }

   return ok;
}

// Sets seconds -- then calls computeVal().
bool LatLon::setSec(const double s)
{
   bool ok{true};

   const double ss{std::fabs(s)};
   // check for valid second numbers
   if (ss >= 0.0 && ss < 60.0) {
      sec = ss;
      computeVal();
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "LatLon::setMin: invalid seconds value: " << s << std::endl;
      }
      ok = false;
   }

   return ok;
}

//------------------------------------------------------------------------------
// setSlotDirection() -- sets the initial direction "n", "s", "e", "w"
//------------------------------------------------------------------------------
bool LatLon::setSlotDirection(const String* const sdobj)
{
   bool ok{};
   if (sdobj != nullptr) ok = setDir(*sdobj);
   return ok;
}

//------------------------------------------------------------------------------
//  setSlotDegrees() -- sets the initial degrees
//------------------------------------------------------------------------------
bool LatLon::setSlotDegrees(const Number* const sdeobj)
{
   bool ok{};
   if (sdeobj != nullptr) ok = setDeg(sdeobj->getReal());
   return ok;
}

//------------------------------------------------------------------------------
// setSlotMinutes() -- sets the minutes
//------------------------------------------------------------------------------
bool LatLon::setSlotMinutes(const Number* const smobj)
{
   bool ok{};
   if (smobj != nullptr) ok = setMin(smobj->getReal());
   return ok;
}

//------------------------------------------------------------------------------
// setSlotSeconds() --
//------------------------------------------------------------------------------
bool LatLon::setSlotSeconds(const Number* const ssobj)
{
   bool ok{};
   if (ssobj != nullptr) ok = setSec(ssobj->getReal());
   return ok;
}

}
}
