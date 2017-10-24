
#include "mixr/base/Transforms.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/units/Angles.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Transform, "Transform")

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

void Transform::computeMatrix()
{
}


//==============================================================================
// Class: Translation
//==============================================================================

IMPLEMENT_SUBCLASS(Translation, "Translation")
EMPTY_SLOTTABLE(Translation)

Translation::Translation()
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Translation)
EMPTY_DELETEDATA(Translation)

void Translation::computeMatrix()
{
    m.makeIdentity();
    if (nv == 2) {
        // Translate X and Y
        Matrixd tt;
        tt.makeTranslate(v[0], v[1], 0.0f);
        m.preMult(tt);
    } else if (nv >= 3) {
        // Translate X, Y and Z
        Matrixd tt;
        tt.makeTranslate(v[0], v[1], v[2]);
        m.preMult(tt);
    }

}


//==============================================================================
// Class: Rotation
//==============================================================================

IMPLEMENT_SUBCLASS(Rotation, "Rotation")
EMPTY_SLOTTABLE(Rotation)

Rotation::Rotation()
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Rotation)
EMPTY_DELETEDATA(Rotation)

void Rotation::computeMatrix()
{
    m.makeIdentity();
    if (nv == 1) {
        // Single value: rotate about the Z axis
        Matrixd rr;
        rr.makeRotate(v[0], 0.0f, 0.0f, 1.0f);
        m.preMult(rr);
    } else if (nv == 4) {
        // Four values: rotate about vector [ v[0] v[1] v[2] ] by v[3] degrees
        Matrixd rr;
        rr.makeRotate(v[3], v[0], v[1], v[2]);
        m.preMult(rr);
    }
}


//==============================================================================
// Class: Scale
//==============================================================================

IMPLEMENT_SUBCLASS(Scale, "Scale")
EMPTY_SLOTTABLE(Scale)

Scale::Scale()
{
   STANDARD_CONSTRUCTOR()
}

EMPTY_COPYDATA(Scale)
EMPTY_DELETEDATA(Scale)

void Scale::computeMatrix()
{
    m.makeIdentity();
    if (nv == 1) {
       // Single value: scale X and Y by the value and hold Z constant
        Matrixd ss;
        ss.makeScale(v[0], v[0], 1.0f);
        m.preMult(ss);
    }

    if (nv == 2) {
        // Two values: scale X and Y by the values and hold Z constant
        Matrixd ss;
        ss.makeScale(v[0], v[1], 1.0f);
        m.preMult(ss);
    } else if (nv >= 3) {
        // Three values: scale X, Y and Z
        Matrixd ss;
        ss.makeScale(v[0], v[1], v[2]);
        m.preMult(ss);
    }
}


//------------------------------------------------------------------------------
// setSlotComputematrix1 () - for Angle types
//------------------------------------------------------------------------------
bool Transform::setSlotComputematrix1(const Angle* const sc1obj)
{
    bool ok = true;
    if (nv == 0 && isClassType(typeid(Rotation))) {
        Radians rad;
        v[nv++] = static_cast<double>(rad.convert(*sc1obj));
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix1: Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotComputematrix1 () - for Number types
//------------------------------------------------------------------------------
bool Transform::setSlotComputematrix1(const Number* const sc1obj)
{
    bool ok = true;
    if (sc1obj != nullptr) {
        v[nv++] = sc1obj->getReal();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix1:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotComputematrix2 () - for Angle types
//------------------------------------------------------------------------------
bool Transform::setSlotComputematrix2(const Angle* const sc2obj)
{
    bool ok = true;
    if (nv == 0 && isClassType(typeid(Rotation))) {
        Radians rad;
        v[nv++] = static_cast<double>(rad.convert(*sc2obj));
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix2:  Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotComputematrix2 () - for Number types
//------------------------------------------------------------------------------
bool Transform::setSlotComputematrix2(const Number* const sc2obj)
{
    bool ok = true;
    if (sc2obj != nullptr) {
        v[nv++] = sc2obj->getReal();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix2:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotComputematrix3 () - for Angle types
//------------------------------------------------------------------------------
bool Transform::setSlotComputematrix3(const Angle* const sc3obj)
{
    bool ok = true;
    if (nv == 0 && isClassType(typeid(Rotation))) {
        Radians rad;
        v[nv++] = static_cast<double>(rad.convert(*sc3obj));
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix3:  Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotComputematrix3 () - for Number types
//------------------------------------------------------------------------------
bool Transform::setSlotComputematrix3(const Number* const sc3obj)
{
    bool ok = true;
    if (sc3obj != nullptr) {
        v[nv++] = sc3obj->getReal();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix3:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotComputematrix4 () - for Angle types
//------------------------------------------------------------------------------
bool Transform::setSlotComputematrix4(const Angle* const sc4obj)
{
    bool ok = true;
    if (nv == 0 && isClassType(typeid(Rotation))) {
        Radians rad;
        v[nv++] = static_cast<double>(rad.convert(*sc4obj));
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix4:  Invalid Angle type or input" << std::endl;
        ok = false;
    }
    return ok;
}

//------------------------------------------------------------------------------
// setSlotComputematrix4 () - for Number types
//------------------------------------------------------------------------------
bool Transform::setSlotComputematrix4(const Number* const sc4obj)
{
    bool ok = true;
    if (sc4obj != nullptr) {
        v[nv++] = sc4obj->getReal();
        computeMatrix();
    } else {
        std::cerr << "Transform::setComputematrix4:  Invalid Number type or input" << std::endl;
        ok = false;
    }
    return ok;
}

}
}
