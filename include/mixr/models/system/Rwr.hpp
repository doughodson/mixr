
#ifndef __mixr_models_Rwr_H__
#define __mixr_models_Rwr_H__

#include "mixr/models/system/RfSensor.hpp"
#include "mixr/base/safe_queue.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: Rwr
//
// Description: General Radar Warning Receiver (RWR) Model
// Factory name: Rwr
//------------------------------------------------------------------------------
class Rwr : public RfSensor
{
    DECLARE_SUBCLASS(Rwr, RfSensor)

public:
    Rwr();

    double getDegreesPerRay() const         { return 360.0f / static_cast<double>(NUM_RAYS); }
    int getNumberOfRays() const             { return NUM_RAYS; }
    double getRay(const int i) const        { return (i >= 0 && i < NUM_RAYS) ? rays[1][i] : 0.0f; }

    int getRayIndex(const double az) const;
    double getRayAzimuth(const int idx) const;

    void clearRays(const int ibuff)
    {
       if (ibuff == 0 || ibuff == 1) {
          int i = 0;
          while (i < NUM_RAYS) rays[ibuff][i++] = 0.0;
       }
    }

    bool killedNotification(Player* const killedBy = 0) override;

protected:
   static const int MAX_EMISSIONS{1000};
   static const int NUM_RAYS{360};

   void receive(const double dt) override;
   void process(const double dt) override;

   bool shutdownNotification() override;

   void xferRays()
   {
      for (int i = 0; i < NUM_RAYS; i++) {
         rays[1][i] = rays[0][i];
      }
   }

private:
   base::safe_queue<Emission*> rptQueue {MAX_EMISSIONS};   // Report queue

   double rays[2][NUM_RAYS] {};     // Back (sensor) buffer [0][*] and front (graphics) buffer [1][*]
};

}
}

#endif
