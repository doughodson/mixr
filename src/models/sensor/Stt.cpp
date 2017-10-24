
#include "mixr/models/sensor/Stt.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/system/Antenna.hpp"
#include "mixr/models/system/trackmanager/TrackManager.hpp"
#include "mixr/models/Track.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Stt, "Stt")
EMPTY_SLOTTABLE(Stt)
EMPTY_DELETEDATA(Stt)

Stt::Stt()
{
    STANDARD_CONSTRUCTOR()
}

void Stt::copyData(const Stt& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// dynamics() --  Update dynamics
//------------------------------------------------------------------------------
void Stt::dynamics(const double dt)
{
    BaseClass::dynamics(dt);

    // ---
    // Update the antenna's Reference position
    // ---
    TrackManager* tm{getTrackManager()};
    if (getAntenna() != nullptr && getOwnship() != nullptr && tm != nullptr) {

        base::safe_ptr<Track> trackList[2];
        int n = tm->getTrackList(trackList,2);

        if (n > 0) {
            // ---
            // Point the antenna at the first track
            // ---

            // Relative position vector to track
            base::Vec3d dpoi{trackList[0]->getPosition()};

            // rotate to ownship heading
            double sinHdg{getOwnship()->getSinHeading()};
            double cosHdg{getOwnship()->getCosHeading()};
            double x{dpoi[Player::INORTH] * cosHdg + dpoi[Player::IEAST] * sinHdg};
            double y{-dpoi[Player::INORTH] * sinHdg + dpoi[Player::IEAST] * cosHdg};
            double z{dpoi[Player::IDOWN]};

            // Compute az & el to track
            double grng{std::sqrt(x*x + y*y)};
            double az{std::atan2(y,x)};
            double el{std::atan2(-z,grng)};

            // Get current antenna limits
            double leftLim{}, rightLim{};
            double lowerLim{}, upperLim{};
            getAntenna()->getAzimuthLimits(&leftLim, &rightLim);
            getAntenna()->getElevationLimits(&lowerLim, &upperLim);

            // Limit to antenna limits
            if (az < leftLim) az = leftLim;
            else if (az > rightLim) az = rightLim;
            if (el < lowerLim) el = lowerLim;
            else if (el > upperLim) el = upperLim;

            // Set the reference 'look' angles and conical scan mode
            getAntenna()->setRefAzimuth(az);
            getAntenna()->setRefElevation(el);
            getAntenna()->setScanMode(Antenna::CONICAL_SCAN);
        } else {
            // ---
            // when we don't have any tracks, enter the default (from input file) search mode
            // ---
            getAntenna()->setRefAzimuth(0.0);
            getAntenna()->setRefElevation(0.0);
            getAntenna()->setScanMode(Antenna::HORIZONTAL_BAR_SCAN);
        }
    }
}

}
}
