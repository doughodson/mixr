#ifndef __mixr_models_ActionImagingSar_HPP__
#define __mixr_models_ActionImagingSar_HPP__

#include "mixr/models/action/IAction.hpp"
#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace base { class Integer; class Latitude; class ILength; class Longitude; }
namespace models {
class OnboardComputer;
class Sar;
class Steerpoint;

//------------------------------------------------------------------------------
// Class: ActionImagingSar
// Description: Concrete - SAR capture, ATR run and Image download
// Factory name: ActionImagingSar
// Slots:
//   sarLatitude   <Latitude>  ! Target's latitude (default: 0)
//   sarLongitude  <Longitude> ! Target's longitude (default: 0)
//   sarElevation  <ILength>   ! Target's elevation (Length) (default: 0)
//   resolution    <ILength>   ! Image (pixel) resolution (default: 3)
//   imageSize     <Integer>   ! Image size: height & width (pixels) (default: 512)
//
//------------------------------------------------------------------------------
class ActionImagingSar final: public IAction
{
    DECLARE_SUBCLASS(ActionImagingSar, IAction)

public:
   ActionImagingSar();

   double getSarLatitude() const       { return sarLatitude;  }   // SAR latitude in degrees
   double getSarLongitude() const      { return sarLongitude; }   // SAR longitude in degrees
   double getSarElevation() const      { return sarElevation; }   // SAR elevation in meters
   double getResolution() const        { return resolution;   }   // Image resolution in meters
   unsigned int getImageSize() const   { return imgSize;      }   // Image size
   double getOrientation() const       { return orientation;  }   // Planned image orientation (Deg) (true)

   virtual bool setSarLatitude(const double);                     // Sets the SAR latitude in degrees
   virtual bool setSarLongitude(const double);                    // Sets the SAR longitude in degrees
   virtual bool setSarElevation(const double);                    // Sets the SAR elevation in meters
   virtual bool setResolution(const double);                      // Sets the SAR image resolution (meters)
   virtual bool setImageSize(const unsigned int);                 // Sets the image size

   // Computes the planned image orientation (degs)
   virtual double computeOrientation(const Steerpoint* const);

   bool trigger(OnboardComputer* const) override;
   bool cancel() override;
   void process(const double dt) override;

protected:
   Sar* getSarSystem()                         { return sar; }
   virtual void setSarSystem(Sar* const);

   void setCompleted(const bool) override;

private:
   double sarLatitude {};        // Latitude (deg)
   double sarLongitude {};       // Longitude (deg)
   double sarElevation {};       // Elevation (meters)
   double resolution {3};        // Image resolution (meters)
   double orientation {};        // orientation (true) (degrees)
   unsigned int imgSize {512};   // Image size (pixels)
   double timer {};              // Time-Out timer
   Sar* sar {};                  // SAR system

private:
   // slot table helper methods
   bool setSlotSarLat(const base::Latitude* const);
   bool setSlotSarLon(const base::Longitude* const);
   bool setSlotSarElev(const base::ILength* const);
   bool setSlotResolution(const base::ILength* const);
   bool setSlotImageSize(const base::Integer* const);
};

}
}

#endif
