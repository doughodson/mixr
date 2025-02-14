
#include "mixr/base/transformations/Transform.hpp"

#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/osg/Matrixd"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/transformations/Rotation.hpp"
#include "mixr/base/units/angles.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(Transform, "AbstractTransform")

BEGIN_SLOTTABLE(Transform)
        "x", "y", "z", "w"
END_SLOTTABLE(Transform)

BEGIN_SLOT_MAP(Transform)
    ON_SLOT(1, setSlotComputematrix1, IAngle)
    ON_SLOT(1, setSlotComputematrix1, INumber)
    ON_SLOT(2, setSlotComputematrix2, IAngle)
    ON_SLOT(2, setSlotComputematrix2, INumber)
    ON_SLOT(3, setSlotComputematrix3, IAngle)
    ON_SLOT(3, setSlotComputematrix3, INumber)
    ON_SLOT(4, setSlotComputematrix4, IAngle)
    ON_SLOT(4, setSlotComputematrix4, INumber)
END_SLOT_MAP()

Transform::Transform()
{
    STANDARD_CONSTRUCTOR()
    m.makeIdentity();
}

void Transform::copyData(const Transform& org, const bool)
{
   BaseClass::copyData(org);
   m  = org.m;
   nv = org.nv;
   for (int i = 0; i < nv; i++) {
      v[i] = org.v[i];
   }
}

void Transform::deleteData()
{
   m.makeIdentity();
   nv = 0;
}

bool Transform::setSlotComputematrix1(const IAngle* const x)
{
    bool ok{true};
    if (nv == 0 && isClassType(typeid(Rotation))) {
        v[nv++] = x->getValueInRadians();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix1: Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

bool Transform::setSlotComputematrix1(const INumber* const x)
{
    v[nv++] = x->asDouble();
    computeMatrix();
    return true;
}

bool Transform::setSlotComputematrix2(const IAngle* const x)
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

bool Transform::setSlotComputematrix2(const INumber* const x)
{
    v[nv++] = x->asDouble();
    computeMatrix();
    return true;
}

bool Transform::setSlotComputematrix3(const IAngle* const x)
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

bool Transform::setSlotComputematrix3(const INumber* const x)
{
    v[nv++] = x->asDouble();
    computeMatrix();
    return true;
}

bool Transform::setSlotComputematrix4(const IAngle* const x)
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

bool Transform::setSlotComputematrix4(const INumber* const x)
{
    v[nv++] = x->asDouble();
    computeMatrix();
    return true;
}

}
}
