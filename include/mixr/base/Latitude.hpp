
#ifndef __mixr_base_Latitude_HPP__
#define __mixr_base_Latitude_HPP__

#include "mixr/base/IObject.hpp"

#include <string>

namespace mixr {
namespace base {
class INumber;
class Identifier;

//------------------------------------------------------------------------------
// Class: Latitude
//
// Description: Stores a latitude coordinate expressed in terms of a direction
//              {north or south}, degrees, minutes and seconds.  Can convert
//              this representation into decimal degrees.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Latitude
// Slots:
//    direction <Identifier>  ! Set the direction { north, south } (default: none)
//    degrees   <INumber>     ! Set the degrees component (default: 0)
//    minutes   <INumber>     ! Set the minutes component (default: 0)
//    seconds   <INumber>     ! Set the seconds component (default: 0.0)
//------------------------------------------------------------------------------
class Latitude : public IObject
{
   DECLARE_SUBCLASS(Latitude, IObject)

public:
   enum class Dir { NONE, NORTH, SOUTH };

   Latitude();

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
   bool setSlotDegrees(const INumber* const);
   bool setSlotMinutes(const INumber* const);
   bool setSlotSeconds(const INumber* const);
};

}
}

#endif
