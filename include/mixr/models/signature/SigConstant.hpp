
#ifndef __mixr_models_common_SigConstant_HPP__
#define __mixr_models_common_SigConstant_HPP__

#include "mixr/models/signature/RfSignature.hpp"

namespace mixr {
namespace base { class INumber; class IArea; }
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: SigConstant
// Descriptions: Constant RCS signature
// Factory name: SigConstant
// Slots:
//    rcs   <base::INumber> ! Constant Radar Cross Section value (default: 0.0)
//                          !  base::Number(square meters) or base::Decibel(square meters) or base::Area()
//------------------------------------------------------------------------------
class SigConstant : public RfSignature
{
    DECLARE_SUBCLASS(SigConstant, RfSignature)
public:
    SigConstant();
    SigConstant(const double);
    SigConstant(const base::INumber* const);

    bool setRCS(const base::INumber* const);
    bool setRCS(const base::IArea* const);

    double getRCS(const Emission* const) override;

private:
    double rcs{};         // Constant RCS value

private:
    // slot table helper methods
    bool setSlotRCS(const base::INumber* const x)     { return setRCS(x); }
    bool setSlotRCS(const base::IArea* const x)       { return setRCS(x); }
};

}
}

#endif
