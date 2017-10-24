
#ifndef __mixr_base_LatLon_H__
#define __mixr_base_LatLon_H__

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace base {
class String;
class Number;

//------------------------------------------------------------------------------
// Class:  LatLon
//
// Description:  For latitude or longitude numbers.
//
// Factory name: LatLon
// Slots:
//    direction <String>  ! Set the direction ("North, South", "East", or "West") (default: NONE)
//    degrees   <Number>  ! Set the degrees component. (default: 0)
//    minutes   <Number>  ! Set the minutes component. (default: 0)
//    seconds   <Number>  ! Set the seconds component. (default: 0.0)
//
// Events: None
//
//
// Constructor:
//
//  LatLon()
//      Instantiates an LatLon instance, and sets its attributes as follows:
//          dir = none
//          deg = 0
//          min = 0
//          sec = 0.0
//
//
// Public methods:
//
//  Conversion operator:
//
//      double()
//          Is C++ equivalent.
//
//      Dir getDir()
//      int getDeg()
//      int getMin();
//      double getSec();
//          Return the direction { north, south, east, west }, degrees,
//          minutes, and seconds components, respectively.
//
//      void LatLon::computeVal()
//          Converts direction, degrees, minutes, and seconds to a lat/long value.
//
//      void LatLon::setDir(const char* d)
//          Sets Direction - N,S,E,W
//
//      void LatLon::setDeg(const double d)
//          Sets degrees, minutes, seconds -- then calls computeVal().
//
//      void LatLon::setMin(const double m)
//          Sets minutes, seconds -- then calls computeVal().
//
//      void LatLon::setSec(const double s)
//          Sets seconds -- then calls computeVal().
//
//------------------------------------------------------------------------------
class LatLon : public Number
{
   DECLARE_SUBCLASS(LatLon, Number)

public:
   enum class Dir { NONE, NORTH, SOUTH, EAST, WEST };

   LatLon();

   operator double() const         { return Number::getDouble(); }

   Dir getDir() const              { return dir; }
   int getDeg() const              { return deg; }
   int getMin() const              { return min; }
   double getSec() const           { return sec; }

protected:
   void computeVal();
   bool setDir(const char* const direction);
   bool setDeg(const double degrees);
   bool setMin(const double minutes);
   bool setSec(const double seconds);

private:
   Dir    dir {Dir::NONE};  // direction
   int    deg {};           // degrees
   int    min {};           // minutes
   double sec {};           // seconds

private:
   // slot table helper methods
   bool setSlotDirection(const String* const);
   bool setSlotDegrees(const Number* const);
   bool setSlotMinutes(const Number* const);
   bool setSlotSeconds(const Number* const);
};

}
}

#endif
