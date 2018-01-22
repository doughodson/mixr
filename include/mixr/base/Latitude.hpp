
#ifndef __mixr_base_Latitude_H__
#define __mixr_base_Latitude_H__

#include "mixr/base/Object.hpp"

#include <string>

namespace mixr {
namespace base {
class Number;
class Identifier;

//------------------------------------------------------------------------------
// Class: Latitude
//
// Description: Stores a latitude coordinate expressed in terms of a direction
//              {north or south} and degrees, minutes and seconds.  Provides
//              a useful conversion of the coordinate expressed in this form
//              a decimal representation (a signal value).
//
// Factory name: Latitude
// Slots:
//    direction <Identifier>  ! Set the direction { north, south } (default: none)
//    degrees   <Number>      ! Set the degrees component (default: 0)
//    minutes   <Number>      ! Set the minutes component (default: 0)
//    seconds   <Number>      ! Set the seconds component (default: 0.0)
//
//------------------------------------------------------------------------------
class Latitude : public Object
{
   DECLARE_SUBCLASS(Latitude, Object)

public:
   enum class Dir { NONE, NORTH, SOUTH };

   Latitude();

   double getValue() const         { return val; }
   double getDouble() const        { return val; }

   Dir getDir() const              { return dir; }
   int getDeg() const              { return deg; }
   int getMin() const              { return min; }
   double getSec() const           { return sec; }

   bool setDirection(const std::string&);
   bool setDeg(const double);
   bool setMin(const double);
   bool setSec(const double);

private:
   void computeVal();
   double val{};

   Dir    dir{Dir::NONE};  // direction
   int    deg{};           // degrees
   int    min{};           // minutes
   double sec{};           // seconds

private:
   // slot table helper methods
   bool setSlotDirection(const Identifier* const);
   bool setSlotDegrees(const Number* const);
   bool setSlotMinutes(const Number* const);
   bool setSlotSeconds(const Number* const);
};

}
}

#endif
