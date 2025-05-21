
#ifndef __mixr_models_common_RfConstantSignature_HPP__
#define __mixr_models_common_RfConstantSignature_HPP__

#include "mixr/models/signature/IRfSignature.hpp"

namespace mixr {
namespace base { class INumber; class IArea; }
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: RfConstantSignature
// Descriptions: Concrete constant RCS signature
// Factory name: RfConstantSignature
// Slots:
//    rcs   <base::INumber> ! Constant Radar Cross Section value (default: 0.0)
//                          !  base::Number(square meters) or base::Decibel(square meters) or base::Area()
//------------------------------------------------------------------------------
class RfConstantSignature final: public IRfSignature
{
    DECLARE_SUBCLASS(RfConstantSignature, IRfSignature)
public:
    RfConstantSignature();
    RfConstantSignature(const double);
    RfConstantSignature(const base::INumber* const);

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
