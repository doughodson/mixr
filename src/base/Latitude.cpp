
#include "mixr/base/Latitude.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/numeric/Number.hpp"

#include <cmath>
#include <string>

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Latitude, "Latitude")
EMPTY_DELETEDATA(Latitude)

BEGIN_SLOTTABLE(Latitude)
    "direction",    // { north, south }
    "degrees",
    "minutes",
    "seconds",
END_SLOTTABLE(Latitude)

BEGIN_SLOT_MAP(Latitude)
    ON_SLOT(1, setSlotDirection, Identifier)
    ON_SLOT(2, setSlotDegrees,   Number)
    ON_SLOT(3, setSlotMinutes,   Number)
    ON_SLOT(4, setSlotSeconds,   Number)
END_SLOT_MAP()

Latitude::Latitude()
{
   STANDARD_CONSTRUCTOR()
}

void Latitude::copyData(const Latitude& org, const bool)
{
   BaseClass::copyData(org);
   dir = org.dir;
   deg = org.deg;
   min = org.min;
   sec = org.sec;
}

// converts direction, degrees, minutes, and seconds to a latitude value
void Latitude::computeVal()
{
   val = deg + (static_cast<double>(min) + static_cast<double>(sec)/60.0) / 60.0;
   if (dir == Dir::SOUTH) {
      val = -val;
   }
}

// sets direction { north, south, east, west }
bool Latitude::setDirection(const std::string& d)
{
    dir = Dir::NONE;
    if (d == "north")      dir = Dir::NORTH;
    else if (d == "south") dir = Dir::SOUTH;

    if (dir == Dir::NONE) {
        if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Latitude::setDirection: invalid latitude direction, expected { north or south } " << std::endl;
        }
    }
    return (dir != Dir::NONE);
}

// Sets degrees, minutes, seconds -- then calls computeVal().
bool Latitude::setDeg(const double d)
{
   bool ok{true};

   double dd{std::fabs(d)};
   if (dir == Dir::NORTH || dir == Dir::SOUTH) {
      // check for valid degree numbers
      if (dd > 90.0) {
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Latitude::setDeg: invalid latitude: " << d << std::endl;
         }
         ok = false;
      }
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Latitude::setDeg: invalid degrees value: " << d << std::endl;
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
bool Latitude::setMin(const double m) //
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
         std::cerr << "Latitude::setMin: invalid minutes value: " << m << std::endl;
      }
      ok = false;
   }

   return ok;
}

// Sets seconds -- then calls computeVal().
bool Latitude::setSec(const double s)
{
   bool ok{true};

   const double ss{std::fabs(s)};
   // check for valid second numbers
   if (ss >= 0.0 && ss < 60.0) {
      sec = ss;
      computeVal();
   } else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Latitude::setMin: invalid seconds value: " << s << std::endl;
      }
      ok = false;
   }

   return ok;
}

// sets the initial direction { north or south }
bool Latitude::setSlotDirection(const Identifier* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setDirection(x->to_string());
   }
   return ok;
}

// sets the degrees
bool Latitude::setSlotDegrees(const Number* const x)
{
   bool ok{};
   if (x != nullptr) ok = setDeg(x->to_double());
   return ok;
}

// sets the minutes
bool Latitude::setSlotMinutes(const Number* const x)
{
   bool ok{};
   if (x != nullptr) ok = setMin(x->to_double());
   return ok;
}

bool Latitude::setSlotSeconds(const Number* const x)
{
   bool ok{};
   if (x != nullptr) ok = setSec(x->to_double());
   return ok;
}

}
}
