
#ifndef __mixr_instruments_CompassRose_HPP__
#define __mixr_instruments_CompassRose_HPP__

#include "mixr/graphics/Graphic.hpp"
#include "mixr/base/qty/util/angle_utils.hpp"

namespace mixr {
namespace base { class Boolean; class INumber; }
namespace instruments {

//------------------------------------------------------------------------------
// Class: CompassRose
//
// Description: Generic compass used as a direction indicator.  It will
// rotate about a heading either by someone telling it through a member function.
// Inputs:
//      UPDATE_VALUE  -> rotation angle (deg)
//      UPDATE_VALUE2 -> rotation angle (rad)
//      UPDATE_VALUE3 -> centered radius
//      UPDATE_VALUE4 -> decentered radius
//      UPDATE_VALUE5 -> displacement
//      UPDATE_VALUE6 -> centered or not
//------------------------------------------------------------------------------
class CompassRose : public graphics::Graphic
{
    DECLARE_SUBCLASS(CompassRose, graphics::Graphic)

public:
    CompassRose();

    virtual bool setRotationDeg(const double);
    virtual bool setRotationRad(const double);
    virtual bool setCenteredRadius(const double);
    virtual bool setDeCenteredRadius(const double);
    virtual bool setDisplacement(const double);
    virtual bool setCentered(const bool);

    double getRotationDeg() const      { return rot * static_cast<double>(base::angle::R2DCC); }
    double getRotationRad() const      { return rot; }
    double getCenteredRadius() const   { return cenRadius; }
    double getDeCenteredRadius() const { return decRadius; }
    bool isCentered() const            { return centered; }
    double getDisplacement() const     { return displacement; }

    void draw() override;

    void updateData(const double dt = 0.0) override;
    bool event(const int event, base::IObject* const obj = nullptr) override;

protected:
    // event functions
    bool onUpdateRotDeg(const base::INumber* const);
    bool onUpdateRadius(const base::INumber* const);
    bool onUpdateCenRad(const base::INumber* const);
    bool onUpdateDecRadius(const base::INumber* const);
    bool onUpdateDisplacement(const base::INumber* const);
    bool onUpdateCentered(const base::Boolean* const);

private:
    double rot {};              // rotation angle (rads)
    double cenRadius {0.5};     // our centered radius (inches)
    double decRadius {1.0};     // our decentered radius (inches)
    bool centered {true};       // centered flag
    double displacement {-0.5}; // how far to translate when we go decentered (inches)

    SendData lTicksSD;
    SendData sTicksSD;
    SendData dialSD;

private:
    // slot table helper methods
    bool setSlotCenteredRadius(const base::INumber* const);
    bool setSlotDeCenteredRadius(const base::INumber* const);
    bool setSlotDisplacement(const base::INumber* const);
};

}
}

#endif
