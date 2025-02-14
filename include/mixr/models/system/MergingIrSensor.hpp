
#ifndef __mixr_models_common_MergingIrSensor_HPP__
#define __mixr_models_common_MergingIrSensor_HPP__

#include "mixr/models/system/IrSensor.hpp"

namespace mixr {
namespace base { class Integer; class INumber; class String; class IAngle; }
namespace models {
class IrSeeker;
class IrQueryMsg;
class Player;
class TrackManager;

//------------------------------------------------------------------------------
// Class: MergingIrSensor
//
// Description: I/R Sensor Model that models limited IrSensor that can only distinguish targets
//              if they are not within specified az & el bin.
//              Merges targets that are within bins in receive frame.
//------------------------------------------------------------------------------
class MergingIrSensor : public IrSensor
{
    DECLARE_SUBCLASS(MergingIrSensor, IrSensor)

public:
   MergingIrSensor();

   void receive(const double dt) override;

   virtual double getAzimuthBin() const {return azimuthBin; };         // Returns min azimuth distance
   virtual bool setAzimuthBin(const double azimuthBin);                // Sets min Azimuth Distance

   virtual double getElevationBin() const {return elevationBin; };     // Returns min azimuth distance
   virtual bool setElevationBin(const double elevationBin);            // Sets min Azimuth Distance

   void reset() override;

protected:
   virtual void mergeIrReturns();

private:
   double azimuthBin {};      // minimum azimuth we can distinguish -- two signals whose
                              // azimuth differs by less than this will be merged

   double elevationBin {};    // minimum azimuth we can distinguish -- two signals whose
                              // azimuth differs by less than this will be merged

private:
   // slot table helper methods
   bool setSlotAzimuthBin(const base::INumber* const);     // Sets the Azimuth Bin
   bool setSlotAzimuthBin(const base::IAngle* const);
   bool setSlotElevationBin(const base::INumber* const);   // Sets the Elevation Bin
   bool setSlotElevationBin(const base::IAngle* const);
};

}
}

#endif

