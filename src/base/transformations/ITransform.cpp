
#include "mixr/base/transformations/ITransform.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/osg/Matrixd"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/transformations/Rotation.hpp"
#include "mixr/base/units/angles.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(ITransform, "ITransform")

BEGIN_SLOTTABLE(ITransform)
        "x", "y", "z", "w"
END_SLOTTABLE(ITransform)

BEGIN_SLOT_MAP(ITransform)
    ON_SLOT(1, setSlotComputematrix1, IAngle)
    ON_SLOT(1, setSlotComputematrix1, INumber)
    ON_SLOT(2, setSlotComputematrix2, IAngle)
    ON_SLOT(2, setSlotComputematrix2, INumber)
    ON_SLOT(3, setSlotComputematrix3, IAngle)
    ON_SLOT(3, setSlotComputematrix3, INumber)
    ON_SLOT(4, setSlotComputematrix4, IAngle)
    ON_SLOT(4, setSlotComputematrix4, INumber)
END_SLOT_MAP()

ITransform::ITransform()
{
    STANDARD_CONSTRUCTOR()
    m.makeIdentity();
}

void ITransform::copyData(const ITransform& org, const bool)
{
   BaseClass::copyData(org);
   m  = org.m;
   nv = org.nv;
   for (int i = 0; i < nv; i++) {
      v[i] = org.v[i];
   }
}

void ITransform::deleteData()
{
   m.makeIdentity();
   nv = 0;
}

bool ITransform::setSlotComputematrix1(const IAngle* const x)
{
    bool ok{true};
    if (nv == 0 && isClassType(typeid(Rotation))) {
        v[nv++] = x->getValueInRadians();
        computeMatrix();
    } else {
        std::cerr << "ITransform::setComputematrix1: Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

bool ITransform::setSlotComputematrix1(const INumber* const x)
{
    v[nv++] = x->asDouble();
    computeMatrix();
    return true;
}

bool ITransform::setSlotComputematrix2(const IAngle* const x)
{
    bool ok{true};
    if (nv == 0 && isClassType(typeid(Rotation))) {
        v[nv++] = x->getValueInRadians();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix2:  Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

bool ITransform::setSlotComputematrix2(const INumber* const x)
{
    v[nv++] = x->asDouble();
    computeMatrix();
    return true;
}

bool ITransform::setSlotComputematrix3(const IAngle* const x)
{
    bool ok{true};
    if (nv == 0 && isClassType(typeid(Rotation))) {
        v[nv++] = x->getValueInRadians();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix3:  Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

bool ITransform::setSlotComputematrix3(const INumber* const x)
{
    v[nv++] = x->asDouble();
    computeMatrix();
    return true;
}

bool ITransform::setSlotComputematrix4(const IAngle* const x)
{
    bool ok{true};
    if (nv == 0 && isClassType(typeid(Rotation))) {
        v[nv++] = x->getValueInRadians();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix4:  Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

bool ITransform::setSlotComputematrix4(const INumber* const x)
{
    v[nv++] = x->asDouble();
    computeMatrix();
    return true;
}

}
}
