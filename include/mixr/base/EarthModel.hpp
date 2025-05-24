
#ifndef __mixr_base_EarthModel_HPP__
#define __mixr_base_EarthModel_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace base {
class ILength;
class INumber;

//------------------------------------------------------------------------------
// Class: EarthModel
// Description: Concrete class that contains the major axis, minor axis and
//              flattening terms used to describe an ellipsoidal or a
//              spherical earth model.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: EarthModel
// Slots:
//     a    <ILength>    ! Semi major axis
//     b    <ILength>    ! Semi minor axis
//     f    <INumber>    ! Flattening
//------------------------------------------------------------------------------
class EarthModel final: public IObject
{
   DECLARE_SUBCLASS(EarthModel, IObject)

public:
   // Some pre-defined earth models             -- getEarthModel() by name list
   static const EarthModel airy;                // "airy"
   static const EarthModel australianNational;  // "australianNational"
   static const EarthModel bessel1841;          // "bessel1841"
   static const EarthModel clark1866;           // "clark1866"
   static const EarthModel clark1880;           // "clark1880"
   static const EarthModel everest;             // "everest"
   static const EarthModel fischer1960;         // "fischer1960"
   static const EarthModel fischer1968;         // "fischer1968"
   static const EarthModel grs1967;             // "grs1967"
   static const EarthModel grs1980;             // "grs1980"
   static const EarthModel helmert1906;         // "helmert1906"
   static const EarthModel hough;               // "hough"
   static const EarthModel international;       // "international"
   static const EarthModel kravosky;            // "kravosky"
   static const EarthModel modAiry;             // "modairy"
   static const EarthModel modEverest;          // "modeverest"
   static const EarthModel modFischer;          // "modfischer"
   static const EarthModel southAmerican1969;   // "southAmerican1969"
   static const EarthModel wgs60;               // "wgs60"
   static const EarthModel wgs66;               // "wgs66"
   static const EarthModel wgs72;               // "wgs72"
   static const EarthModel wgs84;               // "wgs84"

public:
   EarthModel();   // Defaults to WGS-84
   EarthModel(const double a0, const double f0);

   double getA() const;     // meters
   double getB() const;     // meters
   double getF() const;
   double getE2() const;

   bool setA(const double meters);
   bool setB(const double meters);
   bool setF(const double);

   // Get an earth model by name
   // (see the above list of pre-defined earth models)
   static const EarthModel* getEarthModel(const char* const name);

private:
   void initData(const double a0, const double f0);

   double a {};   // semi major axis (meters)
   double b {};   // semi minor axis (meters)
   double f {};   // flattening
   double e2 {};  // Eccentricity Squared

private:
   // slot table helper methods
   bool setSlotA(const ILength* const);
   bool setSlotB(const ILength* const);
   bool setSlotF(const INumber* const);
};

inline double EarthModel::getA() const  { return a; }
inline double EarthModel::getB() const  { return b; }
inline double EarthModel::getF() const  { return f; }
inline double EarthModel::getE2() const { return e2; }

}
}

#endif
