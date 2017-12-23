#ifndef __mixr_models_Gmti_H__
#define __mixr_models_Gmti_H__

#include "mixr/models/system/Radar.hpp"
#include "mixr/base/osg/Vec3d"

namespace mixr {
namespace base { class List; }
namespace models {

//------------------------------------------------------------------------------
// Class: Gmti
// Description: Very simple, Ground-Moving-Target-Indication (GMTI) mode radar
//------------------------------------------------------------------------------
class Gmti : public Radar
{
   DECLARE_SUBCLASS(Gmti, Radar)

public:
   Gmti();

   const base::Vec3d& getPoi() const                                { return poiVec; }
   void setPoi(const double x, const double y, const double z);
   void setPoi(const base::Vec3d& newPos);

protected:
   void dynamics(const double dt) override;

private:
   base::Vec3d poiVec;    // Point Of Interest vector  (m) [ x, y, z ] NED

private:
   // slot table helper methods
   bool setSlotPoi(base::List* const);
};

}
}

#endif
