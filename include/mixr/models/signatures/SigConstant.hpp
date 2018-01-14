
#ifndef __mixr_models_SigConstant_H__
#define __mixr_models_SigConstant_H__

#include "mixr/models/signatures/RfSignature.hpp"

namespace mixr {
namespace base { class Number; }
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: SigConstant
// Descriptions: Constant RCS signature
// Factory name: SigConstant
// Slots:
//    rcs   <base::Number>  ! Constant Radar Cross Section value (default: 0.0)
//                          !  base::Number(square meters) or base::Decibel(square meters) or base::Area()
//------------------------------------------------------------------------------
class SigConstant : public RfSignature
{
    DECLARE_SUBCLASS(SigConstant, RfSignature)
public:
    SigConstant();
    SigConstant(const double);
    SigConstant(const base::Number* const);

    bool setRCS(const base::Number* const);

    double getRCS(const Emission* const) override;

private:
    double rcs{};         // Constant RCS value

private:
    // slot table helper methods
    bool setSlotRCS(const base::Number* const x)     { return setRCS(x); }
};

}
}

#endif
