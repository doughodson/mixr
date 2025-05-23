
#ifndef __mixr_models_IrSignature_HPP__
#define __mixr_models_IrSignature_HPP__

#include "mixr/models/signature/IIrSignature.hpp"

namespace mixr {
namespace models {

//------------------------------------------------------------------------------
// Class: IrSignature
// Description: Concrete IR signature class
//
// Factory name: IrSignature
//------------------------------------------------------------------------------
class IrSignature final: public IIrSignature
{
   DECLARE_SUBCLASS(IrSignature, IIrSignature)

public:
   IrSignature();
};

}
}

#endif
