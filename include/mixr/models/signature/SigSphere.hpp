
#ifndef __mixr_models_common_SigSphere_HPP__
#define __mixr_models_common_SigSphere_HPP__

#include "mixr/models/signature/RfSignature.hpp"

#include "mixr/base/util/constants.hpp"

namespace mixr {
namespace base { class INumber; class ILength; }
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: SigSphere
// Descriptions: RfSignature of a simple sphere based on radius
// Factory name: SigSphere
// Slots:
//    radius   <base::INumber>   ! Radius of the sphere (default: 0)
//    radius   <base::Length>    ! Radius of the sphere (default: 0)
//
//------------------------------------------------------------------------------
class SigSphere : public RfSignature
{
    DECLARE_SUBCLASS(SigSphere, RfSignature)
public:
    SigSphere();
    SigSphere(const double r);
    double computeRcs(const double r)                          { return static_cast<double>(base::PI * r * r); }
    void setRadius(const double r)                             { radius = r; rcs = computeRcs(radius); }

    double getRCS(const Emission* const) override;

private:
    double radius {};      // Sphere radius
    double rcs {};         // RCS of sphere

private:
   // slot table helper methods
   bool setSlotRadius(base::INumber* const);
   bool setSlotRadius(base::ILength* const);
};

}
}

#endif
