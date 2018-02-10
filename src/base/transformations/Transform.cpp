
#include "mixr/base/transformations/Transform.hpp"

#include "mixr/base/numeric/Number.hpp"
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
    ON_SLOT(1, setSlotComputematrix1, Angle)
    ON_SLOT(1, setSlotComputematrix1, Number)
    ON_SLOT(2, setSlotComputematrix2, Angle)
    ON_SLOT(2, setSlotComputematrix2, Number)
    ON_SLOT(3, setSlotComputematrix3, Angle)
    ON_SLOT(3, setSlotComputematrix3, Number)
    ON_SLOT(4, setSlotComputematrix4, Angle)
    ON_SLOT(4, setSlotComputematrix4, Number)
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

bool Transform::setSlotComputematrix1(const Angle* const x)
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

bool Transform::setSlotComputematrix1(const Number* const x)
{
    bool ok{true};
    if (x != nullptr) {
        v[nv++] = x->asDouble();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix1:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

bool Transform::setSlotComputematrix2(const Angle* const x)
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

bool Transform::setSlotComputematrix2(const Number* const sc2obj)
{
    bool ok{true};
    if (sc2obj != nullptr) {
        v[nv++] = sc2obj->asDouble();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix2:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

bool Transform::setSlotComputematrix3(const Angle* const x)
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

bool Transform::setSlotComputematrix3(const Number* const sc3obj)
{
    bool ok{true};
    if (sc3obj != nullptr) {
        v[nv++] = sc3obj->asDouble();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix3:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

bool Transform::setSlotComputematrix4(const Angle* const x)
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

bool Transform::setSlotComputematrix4(const Number* const sc4obj)
{
    bool ok{true};
    if (sc4obj != nullptr) {
        v[nv++] = sc4obj->asDouble();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix4:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

}
}
