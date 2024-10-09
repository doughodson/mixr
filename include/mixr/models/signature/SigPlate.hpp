
#ifndef __mixr_models_common_SigPlate_HPP__
#define __mixr_models_common_SigPlate_HPP__

#include "mixr/models/signature/RfSignature.hpp"

namespace mixr {
namespace base { class Number; class Length; }
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: SigPlate
// Descriptions: RfSignature of a simple plate based on length & width, and always
//               normal to the transmitter.
// Factory name: SigPlate
// Slots:
//    a    <base::Number>    ! length of the plate (default: 0.0)
//    b    <base::Number>    ! width of the plate (default: 0.0)
//
//------------------------------------------------------------------------------
class SigPlate : public RfSignature
{
    DECLARE_SUBCLASS(SigPlate, RfSignature)

public:
    SigPlate();
    SigPlate(const double a, const double b);

    double getA() const                             { return a; }
    double getB() const                             { return b; }

    virtual bool setA(base::Number* const);
    virtual bool setA(base::Length* const);
    virtual bool setB(base::Number* const);
    virtual bool setB(base::Length* const);

    double getRCS(const Emission* const) override;

private:
    double a{};       // Length dimension
    double b{};       // Width dimension
};

}
}

#endif
