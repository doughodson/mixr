
#include "mixr/models/sensor/Gmti.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/Antenna.hpp"
#include "mixr/models/system/trackmanager/TrackManager.hpp"
#include "mixr/models/Emission.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Gmti, "Gmti")
EMPTY_DELETEDATA(Gmti)

BEGIN_SLOTTABLE(Gmti)
    "poi",            // 1: Point-Of-Interest (POI): meters [ north east down ]
END_SLOTTABLE(Gmti)

BEGIN_SLOT_MAP(Gmti)
    ON_SLOT(1, setSlotPoi, base::List)
END_SLOT_MAP()

Gmti::Gmti()
{
    STANDARD_CONSTRUCTOR()
}

void Gmti::copyData(const Gmti& org, const bool)
{
    BaseClass::copyData(org);
    poiVec = org.poiVec;
}

//------------------------------------------------------------------------------
// dynamics() --  Update dynamics
//------------------------------------------------------------------------------
void Gmti::dynamics(const double dt)
{
    BaseClass::dynamics(dt);

    // ---
    // Update the antenna's Reference position
    // ---
    if (getAntenna() != nullptr && getOwnship() != nullptr) {
        // Compute relative vector to POI
        base::Vec3d dpoi {getPoi() - getOwnship()->getPosition()};

        // rotate to ownship heading
        double sinHdg{getOwnship()->getSinHeading()};
        double cosHdg{getOwnship()->getCosHeading()};
        double x{dpoi[models::Player::INORTH] * cosHdg + dpoi[models::Player::IEAST] * sinHdg};
        double y{-dpoi[models::Player::INORTH] * sinHdg + dpoi[models::Player::IEAST] * cosHdg};
        double z{dpoi[models::Player::IDOWN]};

        // Compute az & el to POI
        double grng{std::sqrt(x*x + y*y)};
        double az{std::atan2(y,x)};
        double el{std::atan2(-z,grng)};

        // Get current antenna limits and search volume
        double leftLim{}, rightLim{};
        double lowerLim{}, upperLim{};
        double width{}, height{};
        getAntenna()->getAzimuthLimits(&leftLim, &rightLim);
        getAntenna()->getElevationLimits(&lowerLim, &upperLim);
        getAntenna()->getScanVolume(&width, &height);

        // Limit to within search scan limits of antenna
        if (az < base::angle::aepcdRad(leftLim + width/2.0))
            az = base::angle::aepcdRad(leftLim + width/2.0);
        else if (az > base::angle::aepcdRad(rightLim - width/2.0))
            az = base::angle::aepcdRad(rightLim - width/2.0);

        if (el < base::angle::aepcdRad(lowerLim + height/2.0))
            el = base::angle::aepcdRad(lowerLim + height/2.0);
        else if (el > base::angle::aepcdRad(upperLim - height/2.0))
            el = base::angle::aepcdRad(upperLim - height/2.0);

        // Set the reference 'look' angles
        getAntenna()->setRefAzimuth(az);
        getAntenna()->setRefElevation(el);
    }

    BaseClass::dynamics(dt);
}

//------------------------------------------------------------------------------
// setPoi() -- Gmti Point Of Interest
//------------------------------------------------------------------------------
void Gmti::setPoi(const double x, const double y, const double z)
{
    poiVec.set(x, y, z);
}

void Gmti::setPoi(const base::Vec3d& newPoi)
{
    poiVec = newPoi;
}

//------------------------------------------------------------------------------
// setSlotPoi:  Set Slot POI Vector [ north east down ]
//------------------------------------------------------------------------------
bool Gmti::setSlotPoi(base::List* const numList)
{
    bool ok{};
    double values[3]{};
    const std::size_t n{numList->getNumberList(values, 3)};
    if (n == 3) {
        setPoi(values[0], values[1], values[2]);
        ok = true;
    }
    return ok;
}

}
}
