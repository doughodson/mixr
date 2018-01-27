
#ifndef __mixr_base_Longitude_H__
#define __mixr_base_Longitude_H__

#include "mixr/base/Object.hpp"

#include <string>

namespace mixr {
namespace base {
class Number;
class Identifier;

//------------------------------------------------------------------------------
// Class: Longitude
//
// Description: Stores a longitude coordinate expressed in terms of a direction
//              {east or west}, degrees, minutes and seconds.  Can convert
//              this representation into decimal degrees.
//
// Factory name: Longitude
// Slots:
//    direction <Identifier>  ! Set the direction { east, west } (default: none)
//    degrees   <Number>      ! Set the degrees component (default: 0)
//    minutes   <Number>      ! Set the minutes component (default: 0)
//    seconds   <Number>      ! Set the seconds component (default: 0.0)
//
//------------------------------------------------------------------------------
class Longitude : public Object
{
   DECLARE_SUBCLASS(Longitude, Object)

public:
   enum class Dir { NONE, EAST, WEST };

   Longitude();

   double getDecimalDegrees() const         { return val; }

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
