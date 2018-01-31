
#include "mixr/base/Longitude.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/numeric/Number.hpp"

#include <cmath>
#include <string>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Longitude, "Longitude")
EMPTY_DELETEDATA(Longitude)

BEGIN_SLOTTABLE(Longitude)
    "direction",    // { east, west }
    "degrees",
    "minutes",
    "seconds",
END_SLOTTABLE(Longitude)

BEGIN_SLOT_MAP(Longitude)
    ON_SLOT(1, setSlotDirection, Identifier)
    ON_SLOT(2, setSlotDegrees,   Number)
    ON_SLOT(3, setSlotMinutes,   Number)
    ON_SLOT(4, setSlotSeconds,   Number)
END_SLOT_MAP()

Longitude::Longitude()
{
   STANDARD_CONSTRUCTOR()
}

void Longitude::copyData(const Longitude& org, const bool)
{
   BaseClass::copyData(org);
   dir = org.dir;
   deg = org.deg;
   min = org.min;
   sec = org.sec;
}

// converts direction, degrees, minutes, and seconds to a longitude value
void Longitude::computeVal()
{
   val = deg + (static_cast<double>(min) + static_cast<double>(sec)/60.0) / 60.0;
   if (dir == Dir::WEST) {
      val = -val;
   }
}

// sets direction { east, west }
bool Longitude::setDirection(const std::string& d)
{
    dir = Dir::NONE;
    if (d == "east")  dir = Dir::EAST;
    else if (d == "west")  dir = Dir::WEST;

    if (dir == Dir::NONE) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Longitude::setDir: invalid longitude direction, expected { east or west }" << std::endl;
        }
    }
    return (dir != Dir::NONE);
}

// Sets degrees, minutes, seconds -- then calls computeVal().
bool Longitude::setDeg(const double d)
{
   bool ok{true};

   double dd{std::fabs(d)};
   if (dir == Dir::EAST || dir == Dir::WEST) {
      // check for valid degree numbers
      if (dd > 180.0) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Longitude::setDeg: invalid longitude: " << d << std::endl;
         }
         ok = false;
      }
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Longitude::setDeg: invalid degrees value: " << d << std::endl;
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
bool Longitude::setMin(const double m) //
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
         std::cerr << "Longitude::setMin: invalid minutes value: " << m << std::endl;
      }
      ok = false;
   }

   return ok;
}

// Sets seconds -- then calls computeVal().
bool Longitude::setSec(const double s)
{
   bool ok{true};

   const double ss{std::fabs(s)};
   // check for valid second numbers
   if (ss >= 0.0 && ss < 60.0) {
      sec = ss;
      computeVal();
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Longitude::setMin: invalid seconds value: " << s << std::endl;
      }
      ok = false;
   }

   return ok;
}

// sets the initial direction { east, west }
bool Longitude::setSlotDirection(const Identifier* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setDirection(x->asString());
   }
   return ok;
}

// sets the degrees
bool Longitude::setSlotDegrees(const Number* const x)
{
   bool ok{};
   if (x != nullptr) ok = setDeg(x->asDouble());
   return ok;
}

// sets the minutes
bool Longitude::setSlotMinutes(const Number* const x)
{
   bool ok{};
   if (x != nullptr) ok = setMin(x->asDouble());
   return ok;
}

bool Longitude::setSlotSeconds(const Number* const x)
{
   bool ok{};
   if (x != nullptr) ok = setSec(x->asDouble());
   return ok;
}

}
}
