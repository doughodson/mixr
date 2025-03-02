
#ifndef __mixr_models_common_Designator_HPP__
#define __mixr_models_common_Designator_HPP__

#include "mixr/base/IObject.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Designator
//
// Description: General designator (e.g., laser)
//------------------------------------------------------------------------------
class Designator : public base::IObject
{
    DECLARE_SUBCLASS(Designator, base::IObject)

public:

   // Since we're not using DIS Designator PDUs yet, use this to send
   // data over the network.
   class Data {
   public:
      Data() = default;
      Data(const Data&) = delete;
      Data& operator=(const Data&) = delete;

      double latitude {};         // Latitude                         (degs)
      double longitude {};        // Longitude                        (degs)
      double elevation {};        // Elevation                        (m)
      double freq {};             // Frequency                        (Hz)
      double power {};            // Effective Radiated Power (ERP)   (Watts)
      unsigned short code {};     // LASER code
      unsigned short padding {};  // LASER code
   };

   Designator();

   double getLatitude() const       { return data.latitude; }
   double getLongitude() const      { return data.longitude; }
   double getElevation() const      { return data.elevation; }
   double getFrequency() const      { return data.freq; }
   double getPower() const          { return data.power; }
   unsigned short getCode() const   { return data.code; }
   const Data* getDataBuffer() const   { return &data; }

   // set functions
   virtual void setPosition(const double lat, const double lon, const double elev) {
      data.latitude = lat;
      data.longitude = lon;
      data.elevation = elev;
   }
   virtual void setFrequency(const double v)       { data.freq = v; }
   virtual void setPower(const double v)           { data.power = v; }
   virtual void setCode(const unsigned short c)    { data.code = c; }

   virtual void setDataBuffer(const Data* const d) {
      if (d != nullptr) {
         setPosition( d->latitude, d->longitude, d->elevation );
         setFrequency( d->freq );
         setPower( d->power );
         setCode( d->code );
      }
   }

private:
   Data data;          // Designator data (see above)
};

}
}

#endif
