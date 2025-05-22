
#ifndef __mixr_models_common_IRfPlateSignature_HPP__
#define __mixr_models_common_IRfPlateSignature_HPP__

#include "mixr/models/signature/IRfSignature.hpp"

namespace mixr {
namespace base { class INumber; class ILength; }
namespace models {
class RfEmission;

//------------------------------------------------------------------------------
// Class: IRfPlateSignature
// Descriptions: Interface to RF signature of a simple plate based on length & width, and always
//               normal to the transmitter.
// Factory name: IRfPlateSignature
// Slots:
//    a    <base::INumber>    ! length of the plate (default: 0.0)
//    b    <base::INumber>    ! width of the plate (default: 0.0)
//
//------------------------------------------------------------------------------
class IRfPlateSignature : public IRfSignature
{
    DECLARE_SUBCLASS(IRfPlateSignature, IRfSignature)

public:
    IRfPlateSignature();
    IRfPlateSignature(const double a, const double b);

    double getA() const                             { return a; }
    double getB() const                             { return b; }

    virtual bool setA(base::INumber* const);
    virtual bool setA(base::ILength* const);
    virtual bool setB(base::INumber* const);
    virtual bool setB(base::ILength* const);

    double getRCS(const RfEmission* const) override;

private:
    double a{};       // Length dimension
    double b{};       // Width dimension
};

}
}

#endif
