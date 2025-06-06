
#include "mixr/models/system/trackmanager/AirAngleOnlyTrkMgr.hpp"

#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/player/weapon/IWeapon.hpp"
#include "mixr/models/IrQueryMsg.hpp"
#include "mixr/models/track/ITrack.hpp"
#include "mixr/models/track/RfTrack.hpp"
#include "mixr/models/track/IrTrack.hpp"
#include "mixr/models/WorldModel.hpp"

#include "mixr/simulation/IDataRecorder.hpp"

#include "mixr/base/IList.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

#include "mixr/base/units/times.hpp"
#include "mixr/base/units/angles.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(AirAngleOnlyTrkMgr, "AirAngleOnlyTrkMgr")
EMPTY_SLOTTABLE(AirAngleOnlyTrkMgr)
EMPTY_DELETEDATA(AirAngleOnlyTrkMgr)

AirAngleOnlyTrkMgr::AirAngleOnlyTrkMgr()
{
    STANDARD_CONSTRUCTOR()
    setType( ITrack::ONBOARD_SENSOR_BIT | ITrack::AIR_TRACK_BIT );
}

void AirAngleOnlyTrkMgr::copyData(const AirAngleOnlyTrkMgr& org, const bool)
{
    BaseClass::copyData(org);
}

//------------------------------------------------------------------------------
// processTrackList() -- process the track list
//------------------------------------------------------------------------------
void AirAngleOnlyTrkMgr::processTrackList(const double dt)
{
    // Make sure we have an ownship to work with
    const auto ownship = dynamic_cast<IPlayer*>( findContainerByType(typeid(IPlayer)) );
    if (ownship == nullptr || dt == 0.0) return;

    // Make sure we have the A and B matrix
    //if (!haveMatrixA) makeMatrixA(dt);   // not used

    // Report to Track matching matrix
    unsigned int report2TrackMatch[MAX_REPORTS][MAX_TRKS]{};    // Report/Track matched matrix
    unsigned int reportNumMatches[MAX_REPORTS]{};               // Number of matches for each report
    unsigned int trackNumMatches[MAX_TRKS]{};                   // Number of matcher for each track

    // ---
    // 1)Age the tracks by delta time
    // ---
    const base::Vec3d osVel{ownship->getVelocity()};
    const base::Vec3d osAccel{ownship->getAcceleration()};
    const double osGndTrk{ownship->getGroundTrack()};
    base::lock(trkListLock);
    for (unsigned int i = 0; i < nTrks; i++) {
        tracks[i]->ownshipDynamics(osGndTrk, osVel, osAccel, dt);
        tracks[i]->updateTrackAge(dt);
    }
    base::unlock(trkListLock);

    // ---
    // 2) Process new reports
    // ---

    // Get each new IR query message report from the queue
    unsigned int nReports{};
    IrQueryMsg* queryMessages[MAX_REPORTS]{};
    double newSignal[MAX_REPORTS]{};
    double newElevation[MAX_REPORTS]{};
    double newAzimuth[MAX_REPORTS]{};
    double tmp{};
    for (IrQueryMsg* q = getQuery(&tmp); q != nullptr && nReports < MAX_REPORTS; q = getQuery(&tmp)) {
        IPlayer* tgt{q->getTarget()};

        bool dummy{};
        if (tgt->isMajorType(IPlayer::WEAPON)) {
            dummy = (static_cast<const IWeapon*>(tgt))->isDummy();
        }

        if ( tgt->isMajorType(IPlayer::AIR_VEHICLE) ||
            tgt->isMajorType(IPlayer::SHIP) ||
            (tgt->isMajorType(IPlayer::WEAPON) && !dummy)
            ) {
                // Using only air vehicles
                queryMessages[nReports] = q;
                newSignal[nReports] = tmp;
                //           newAzimuth[nReports] = queryMessages[nReports]->getAzimuthAoi();
                reportNumMatches[nReports] = 0;

                newAzimuth[nReports] = queryMessages[nReports]->getRelativeAzimuth();
                newElevation[nReports] = queryMessages[nReports]->getRelativeElevation();
                nReports++;
        } else {
            // Free up IR query messages from other types of players
            q->unref();
        }
    }

    // Clean out the rest of the queue, if we had more than the maximum IR query messages
    for (IrQueryMsg* q = getQuery(&tmp); q != nullptr; q = getQuery(&tmp)) {
        q->unref();
    }

    // ---
    // 3) Match current tracks to new reports (observations)
    // ---
    base::lock(trkListLock);
    for (unsigned int it = 0; it < nTrks; it++) {
        trackNumMatches[it] = 0;
        const IrTrack* const trk{static_cast<const IrTrack*>(tracks[it])};  // we produce only IrTracks

        for (unsigned int ir = 0; ir < nReports; ir++) {
            // use ground truth to match new reports to existing tracks
            if (trk->getTarget() == queryMessages[ir]->getTarget()) {
                // We have a new report for the same target as this track ...
                report2TrackMatch[ir][it] = 1;
                trackNumMatches[it]++;
                reportNumMatches[ir]++;
            } else {
               report2TrackMatch[ir][it] = 0;
            }
        }
    }
    base::unlock(trkListLock);

    // ---
    // 4) Apply rules to associate the proper report to track.
    // ---

    // No Rules -- Just Right

    // ---
    // 5) Create inputs for current tracks
    // ---
    double uAzimuth[MAX_TRKS]{};
    double uElevation[MAX_TRKS]{};
    double age[MAX_TRKS]{};
    bool haveU[MAX_TRKS]{};
    base::lock(trkListLock);
    for (unsigned int it = 0; it < nTrks; it++) {
        haveU[it] = false;
        age[it] = tracks[it]->getTrackAge();
        if (trackNumMatches[it] > 0) {
            for (unsigned int ir = 0; ir < nReports; ir++) {
                if (report2TrackMatch[ir][it] > 0) {
                    IrTrack* const trk{static_cast<IrTrack*>(tracks[it])};  // we produce only IrTracks

                    // Update the track's signal
                    trk->setSignal(newSignal[ir],queryMessages[ir]);

                    uAzimuth[it] = newAzimuth[ir] - trk->getRelAzimuth();
                    uElevation[it] = newElevation[ir] - trk->getElevation();

                    // Track age and flags
                    if ((!haveU[it]) && (dt > 0.0f)) {
                        tracks[it]->resetTrackAge();
                        haveU[it] = true;
                    }
                }
            }
        }
    }
    base::unlock(trkListLock);

    // ---
    // 6) Smooth and predict position for the next frame
    //
    //    X(k+1) = A*X(k) + B*U(k)
    //    where:
    //      X(k) is the state vector [ pos vel accel ]
    //      U(k) is the difference between the observed & predicted positions
    // ---
    base::lock(trkListLock);
    for (unsigned int i = 0; i < nTrks; i++) {
        // Save X(k)
        double taz{tracks[i]->getRelAzimuth()};
        double tazRate{tracks[i]->getRelAzimuthRate()};
        double tazAccel{tracks[i]->getRelAzimuthAcceleration()};
        double tel{tracks[i]->getElevation()};
        double telRate{tracks[i]->getElevationRate()};
        double telAccel{tracks[i]->getElevationAcceleration()};

        double ageAtNextTimeStep{dt};

        if (haveU[i]) {
            tracks[i]->setPosition(tracks[i]->getTarget()->getPosition() - ownship->getPosition());

            // If the track is updated set the new azimuth and elevation data based on
            // a weighted average of the updated value and the predicted value from the
            // previous time step
            const double pTaz{tracks[i]->getPredictedAzimuth()};
            const double pTazRate{tracks[i]->getPredictedAzimuthRate()};
            const double pTel{tracks[i]->getPredictedElevation()};
            const double pTelRate{tracks[i]->getPredictedElevationRate()};

            tracks[i]->setRelAzimuth((pTaz * oneMinusAlpha) + ((taz + uAzimuth[i]) * alpha));
            tracks[i]->setRelAzimuthRate((pTazRate * oneMinusBeta) + ((uAzimuth[i] / age[i]) * beta ));
            tracks[i]->setRelAzimuthAcceleration((tracks[i]->getRelAzimuthRate() - tazRate) / age[i]);
            tracks[i]->setElevation((pTel * oneMinusAlpha) + ((tel + uElevation[i]) * alpha));
            tracks[i]->setElevationRate((pTelRate * oneMinusBeta) + ((uElevation[i] / age[i]) * beta ));
            tracks[i]->setElevationAcceleration((tracks[i]->getElevationRate() - telRate) / age[i]);

            if (getLogTrackUpdates()) {
               // Object 1: player, Object 2: Track Data
               BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_TRACK_DATA )
                  SAMPLE_2_OBJECTS( ownship, tracks[i] )
               END_RECORD_DATA_SAMPLE()
            }

            taz = tracks[i]->getRelAzimuth();
            tazRate = tracks[i]->getRelAzimuthRate();
            tazAccel = tracks[i]->getRelAzimuthAcceleration();
            tel = tracks[i]->getElevation();
            telRate = tracks[i]->getElevationRate();
            telAccel = tracks[i]->getElevationAcceleration();

        } else {
            ageAtNextTimeStep += age[i];
        }

        // Project the values for the next time step

        const double deltaTimeSquaredOverTwo{(ageAtNextTimeStep* ageAtNextTimeStep) / 2.0};

        tracks[i]->setPredictedAzimuth(taz + tazRate*ageAtNextTimeStep
            + tazAccel*deltaTimeSquaredOverTwo);
        tracks[i]->setPredictedAzimuthRate(tazRate + tazAccel*ageAtNextTimeStep);
        tracks[i]->setPredictedElevation(tel + telRate*ageAtNextTimeStep
            + telAccel*deltaTimeSquaredOverTwo);
        tracks[i]->setPredictedElevationRate(telRate + telAccel*ageAtNextTimeStep);

    }
    base::unlock(trkListLock);

    // ---
    // 7) For tracks with new observation reports, reset their age.
    //    Remove tracks that have age beyond the limit.
    // ---
    base::lock(trkListLock);
    for (unsigned int it = 0; it < nTrks; /* update 'it' below */ ) {
        IrTrack* const trk{static_cast<IrTrack*>(tracks[it])};  // we produce only IrTracks
        if (trk->getTrackAge() >= getMaxTrackAge()) {
            //if (isMessageEnabled(MSG_INFO)) {
            //   std::cout << "Removed Aged AIR track[it] = [" << it << "] id = " << trk->getTrackID() << std::endl;
            //}

             // Object 1: player, Object 2: Track Data
            IPlayer* ownship{getOwnship()};
            BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_TRACK_REMOVED )
               SAMPLE_2_OBJECTS( ownship, trk )
            END_RECORD_DATA_SAMPLE()

            // Track has timed out -- delete the track and ...
            if (isMessageEnabled(MSG_INFO)) {
                std::cout << "Removed Aged AIR track: player: " << trk->getLastQuery()->getTarget();
                std::cout << ", refCnt = " << trk->getLastQuery()->getTarget()->getRefCount() << std::endl;
            }

            trk->setType(0);
            trk->unref();
            nTrks--;
            // move all other tracks down in the list.
            for (unsigned int it2 = it; it2 < nTrks; it2++) {
                tracks[it2] = tracks[it2+1];
            }
        } else {
            // update track index, it
            it++;
        }
    }
    base::unlock(trkListLock);

    // ---
    // 8) Create new tracks from unmatched reports and free up IR query messages
    // ---
    base::lock(trkListLock);
    for (unsigned int i = 0; i < nReports; i++) {
        if ((reportNumMatches[i] == 0) && (nTrks < maxTrks)) {
            // This is a new report, so create a new track for it
            IrTrack* newTrk{new IrTrack()};
            newTrk->setTrackID( getNewTrackID() );
            newTrk->setTarget( queryMessages[i]->getTarget() );
            newTrk->setType(ITrack::AIR_TRACK_BIT | ITrack::ONBOARD_SENSOR_BIT);
            newTrk->setPosition(queryMessages[i]->getTarget()->getPosition() - ownship->getPosition());
            newTrk->setVelocity(queryMessages[i]->getTarget()->getVelocity() - ownship->getVelocity());
            newTrk->setRelAzimuth(queryMessages[i]->getRelativeAzimuth());
            newTrk->setElevation(queryMessages[i]->getRelativeElevation());
            newTrk->setPredictedAzimuth(queryMessages[i]->getRelativeAzimuth());
            newTrk->setPredictedElevation(queryMessages[i]->getRelativeElevation());
            newTrk->setSignal(newSignal[i],queryMessages[i]);

            if (isMessageEnabled(MSG_INFO)) {
                std::cout << "New AIR track[it] = [" << nTrks << "] id = " << newTrk->getTrackID() << std::endl;
            }

            // Object 1: player, Object 2: Track Data
            BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_NEW_TRACK )
               SAMPLE_2_OBJECTS( ownship, newTrk )
            END_RECORD_DATA_SAMPLE()

            tracks[nTrks++] = newTrk;
        }
        // Free the IR query message report
        queryMessages[i]->unref();
    }
    base::unlock(trkListLock);

}

}
}
