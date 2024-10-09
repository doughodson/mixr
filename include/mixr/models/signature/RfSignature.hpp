
#ifndef __mixr_models_common_RfSignature_HPP__
#define __mixr_models_common_RfSignature_HPP__

#include "mixr/base/Component.hpp"

namespace mixr {
namespace models {
class Emission;

//------------------------------------------------------------------------------
// Class: RfSignature
// Descriptions: Abstract class for signatures
//------------------------------------------------------------------------------
class RfSignature : public base::Component
{
    DECLARE_SUBCLASS(RfSignature, base::Component)
public:
    RfSignature();

    // computes the Radar Cross Section for the emission
    virtual double getRCS(const Emission* const em) =0;
};

}
}

#endif
