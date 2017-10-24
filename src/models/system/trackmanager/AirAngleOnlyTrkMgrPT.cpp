
#include "mixr/models/system/trackmanager/AirAngleOnlyTrkMgrPT.hpp"

#include "mixr/models/player/Player.hpp"
#include "mixr/models/player/weapon/AbstractWeapon.hpp"
#include "mixr/models/IrQueryMsg.hpp"
#include "mixr/models/Track.hpp"
#include "mixr/models/WorldModel.hpp"

#include "mixr/simulation/AbstractDataRecorder.hpp"

#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

#include "mixr/base/units/Times.hpp"
#include "mixr/base/units/Angles.hpp"

#include <cmath>

namespace mixr {
namespace models {

//==============================================================================
// Class: AirAngleOnlyTrkMgrPT - perceived truth version of above
//==============================================================================

// this tm uses merged tracks for its attempts at managing tracks. This means it is
// perceived az and el for managing tracks.

// usePerceivedData determines whether track's potentially merged position and velocity are set in the
// track, which would mean that missile would use perceived truth in its flyout, potentially flying
// towards a merged track centroid instead of toward an actual target.
//
// if !usePerceivedData, it uses track target position, which is a ground truth target position for one
// of the entities in the merged track

IMPLEMENT_SUBCLASS(AirAngleOnlyTrkMgrPT, "AirAngleOnlyTrkMgrPT")
EMPTY_SLOTTABLE(AirAngleOnlyTrkMgrPT)
EMPTY_DELETEDATA(AirAngleOnlyTrkMgrPT)

AirAngleOnlyTrkMgrPT::AirAngleOnlyTrkMgrPT()
{
    STANDARD_CONSTRUCTOR()
}

void AirAngleOnlyTrkMgrPT::copyData(const AirAngleOnlyTrkMgrPT& org, const bool)
{
    BaseClass::copyData(org);
    usePerceivedPosVel = org.usePerceivedPosVel;
}

//------------------------------------------------------------------------------
// Age the tracks by delta time
//------------------------------------------------------------------------------
void AirAngleOnlyTrkMgrPT::updateTrackAges(const double dt)
{
    const base::Vec3d osVel{getOwnship()->getVelocity()};
    const base::Vec3d osAccel{getOwnship()->getAcceleration()};
    const double osGndTrk{getOwnship()->getGroundTrack()};
    base::lock(trkListLock);
    for (unsigned int i = 0; i < nTrks; i++) {
        tracks[i]->ownshipDynamics(osGndTrk, osVel, osAccel, dt);
        tracks[i]->updateTrackAge(dt);
    }
    base::unlock(trkListLock);
}

//------------------------------------------------------------------------------
// Remove tracks that have age beyond the limit.
//------------------------------------------------------------------------------
void AirAngleOnlyTrkMgrPT::removeAgedTracks()
{
    base::lock(trkListLock);
    for (unsigned int it = 0; it < nTrks; /* update 'it' below */ ) {
        IrTrack* const trk{static_cast<IrTrack*>(tracks[it])};  // we produce only IrTracks
        if (trk->getTrackAge() >= getMaxTrackAge()) {
            //if (isMessageEnabled(MSG_INFO)) {
            //   std::cout << "Removed Aged AIR track[it] = [" << it << "] id = " << trk->getTrackID() << std::endl;
            //}
            // Object 1: player, Object 2: Track Data
           Player* ownship{getOwnship()};
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
}

//------------------------------------------------------------------------------
// processTrackList() -- process the track list
//------------------------------------------------------------------------------
void AirAngleOnlyTrkMgrPT::processTrackList(const double dt)
{
    // Make sure we have an ownship to work with
    Player* ownship{getOwnship()};
    if (ownship == nullptr) return;

    IrQueryMsg* queryMessages[MAX_REPORTS]{};
    unsigned int reportNumMatches[MAX_REPORTS]{};               // Number of matches for each report
    unsigned int nReports{};

    // ---
    // 1)Age existing tracks by delta time
    // ---
    updateTrackAges(dt);


    // ---
    // 2) Process new reports
    // ---
    // Get each new IR query message report from the queue
    double tmp{};
    for (IrQueryMsg* q = getQuery(&tmp); q != nullptr && nReports < MAX_REPORTS; q = getQuery(&tmp)) {
        Player* tgt{q->getTarget()};

        bool dummy{};
        if (tgt->isMajorType(Player::WEAPON)) {
            dummy = (static_cast<const AbstractWeapon*>(tgt))->isDummy();
        }

        if ( tgt->isMajorType(Player::AIR_VEHICLE) ||
            tgt->isMajorType(Player::SHIP) ||
            (tgt->isMajorType(Player::WEAPON) && !dummy)
            ) {
                // Using only air vehicles
                queryMessages[nReports] = q;
                reportNumMatches[nReports] = 0;
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


    if (nTrks > 0) {
        // Report to Track matching matrix
        unsigned int report2TrackMatch[MAX_REPORTS][MAX_TRKS]{};    // Report/Track matched matrix
        unsigned int trackNumMatches[MAX_TRKS]{};                   // Number of matcher for each track

        if (nReports > 0) {

            // ---
            // 3) Match new reports (observations) to all potential track matches
            // ---
            base::lock(trkListLock);
            for (unsigned int it = 0; it < nTrks; it++) {
                trackNumMatches[it] = 0;
                const IrTrack* const trk{static_cast<const IrTrack*>(tracks[it])};  // we produce only IrTracks
                for (unsigned int ir = 0; ir < nReports; ir++) {
                    double azDiff{queryMessages[ir]->getRelativeAzimuth() - trk->getPredictedAzimuth()};
                    if (azDiff < 0.0f) azDiff = 0.0f - azDiff;
                    double elDiff{queryMessages[ir]->getRelativeElevation() - trk->getPredictedElevation()};
                    if (elDiff < 0.0f) elDiff = 0.0f - elDiff;
                    if ((azDiff < azimuthBin) && (elDiff < elevationBin)) {
                        // We have a new report for the same target as this track ...
                        report2TrackMatch[ir][it] = 1;
                        trackNumMatches[it]++;
                        reportNumMatches[ir]++;
                    } else
                        report2TrackMatch[ir][it] = 0;
                }
            }
            base::unlock(trkListLock);


            // ---
            // 4) Apply rules to associate the proper report to track.
            // ---

            // prevent one report assigned to multiple tracks
            // and attempt to distribute reports so that one report per track
            // and not two reports in one track and none in another
            // algorithm does not otherwise prevent multiple reports to one track
            // and note that 5) below only uses the last of multiple reports matched to a track

            base::lock(trkListLock);

            // using two passes so that we prioritize assigning reports to later tracks for both situations
            // avoid assigning multiple reports to one track
            // attempt to distribute reports to tracks 1->1
            // with priority to later tracks

            // this pass deals with case if one report matches to multiple tracks,
            // and also one track matches to multiple reports
            // by prioritizing assignment of reports to newest tracks
            // this does no assignment of reports to tracks, it removes prospective assignments
            for (unsigned int it = 0; it < nTrks; it++) {
                for (unsigned int ir = 0; ir < nReports; ir++) {
                    if (report2TrackMatch[ir][it]==1 && reportNumMatches[ir]>1 && trackNumMatches[it]>1) {
                        // there are multiple matches for this report and for this track
                        // disable this match, since report and track each have a subsequent match to use
                        // iterating up, prioritizes assignment of reports to later tracks
                        report2TrackMatch[ir][it] = 0;
                        trackNumMatches[it]--;
                        reportNumMatches[ir]--;
                    }
                }
            }

            // prevent matching one report to multiple tracks;
            // prioritize matching to the last of the matching tracks
            for (int it = (nTrks-1); it >= 0; it--) {
                for (unsigned int ir = 0; ir < nReports; ir++) {
                    if (report2TrackMatch[ir][it] == 1) {
                        if (reportNumMatches[ir]>1 && trackNumMatches[it] == 1) {
                            // assign this report to this track but prevent assigning to any others
                            reportNumMatches[ir]=0;
                        } else if (reportNumMatches[ir]==0) {
                            // this report previously assigned to another track - disable this match
                            report2TrackMatch[ir][it] = 0;
                        }
                    }
                }
            }
            base::unlock(trkListLock);

        } // endif nReports > 0

        // ---
        // 5) Create inputs for current tracks
        //
        // note that 5) and 6) are now processed together in one loop on track[]
        // ---
        base::lock(trkListLock);
        for (unsigned int it = 0; it < nTrks; it++) {
            base::Vec3d uPosition;
            base::Vec3d uVelocity;
            double uAzimuth{};
            double uElevation{};
            //double age = tracks[it]->getTrackAge();
            bool haveU{};

            // ---
            // Process new input data for current tracks
            // ---
            if (trackNumMatches[it] > 0) {
                for (unsigned int ir = 0; ir < nReports; ir++) {
                    if (report2TrackMatch[ir][it] > 0) {
                        IrTrack* const irTrk{static_cast<IrTrack*>(tracks[it])};  // we produce only IrTracks

                        // Update the track's signal
                        irTrk->setSignal(queryMessages[ir]->getSignalToNoiseRatio(), queryMessages[ir]);

                        if (isMessageEnabled(MSG_DEBUG)) {
                            if (irTrk->getLastQuery()->getQueryMergeStatus() == IrQueryMsg::MERGED)
                                std::cout << it << " snDbl-merged " << queryMessages[ir]->getSignalToNoiseRatio() << std::endl;
                            else
                                std::cout << it << " snDbl " << queryMessages[ir]->getSignalToNoiseRatio() << std::endl;
                        }

                        uAzimuth = queryMessages[ir]->getRelativeAzimuth() - irTrk->getRelAzimuth();
                        uElevation = queryMessages[ir]->getRelativeElevation() - irTrk->getElevation();

                        if (usePerceivedPosVel) {
                            // to use PT data, need to store PT pos and vel from query for use in track prediction (next)
                            uPosition = queryMessages[ir]->getPosVec();
                            uVelocity = queryMessages[ir]->getVelocityVec();
                        }

                        // Track age and flags
                        //if (age[it]>0.03 && (!haveU[it]) && (dt > 0.0f)) { // force gaps in data to test prediction
                        if ((!haveU) && (dt > 0.0)) {
                            tracks[it]->resetTrackAge();
                            haveU = true;
                        }
                    }
                }
            }

            // finished processing new inputs for this track.
            // but note that we only look at the last report that is matched to this track...


            // ---
            // 6) Smooth and predict position for the next frame
            //
            //   X(k+1) = A X(k) + (1-A)pX(k) for az and el
            //   X(k+1) = B X(k) + (1-B)pX(k) for azrate and elrate

            // Save X(k)
            double taz{tracks[it]->getRelAzimuth()};
            double tazRate{tracks[it]->getRelAzimuthRate()};
            double tazAccel{tracks[it]->getRelAzimuthAcceleration()};
            double tel{tracks[it]->getElevation()};
            double telRate{tracks[it]->getElevationRate()};
            double telAccel{tracks[it]->getElevationAcceleration()};

            double ageAtNextTimeStep = dt;

            if (haveU) {
                // missiles need pos + vel for guidance; update pos and vel for missile dynamics, using reported/perceived position and velocity
                if (usePerceivedPosVel) {
                    tracks[it]->setPosition(uPosition - getOwnship()->getPosition());
                    tracks[it]->setVelocity(uVelocity - getOwnship()->getVelocity());
                } else {
                    // using ground truth to calc position and velocity
                    tracks[it]->setPosition(tracks[it]->getTarget()->getPosition() - getOwnship()->getPosition());
                    tracks[it]->setVelocity(tracks[it]->getTarget()->getVelocity() - getOwnship()->getVelocity());
                }

                // a track's target can change w/o track changing - sync track target with target from most recent query return
                const auto trk = dynamic_cast<IrTrack*>(tracks[it]);
                if ( trk && (trk->getLastQuery()->getTarget() != trk->getTarget()) ) {
                    const Player* tgt{trk->getLastQuery()->getTarget()};
                    const auto ttgt = const_cast<Player*>(tgt);
                    // if track contained merged targets, a track's target may change
                    tracks[it]->setTarget( ttgt );
                    if (isMessageEnabled(MSG_INFO))
                        std::cout << "AirAngleOnlyTrkMgrPT: track " << it << " changed target " << tracks[it]->getTarget()->getID() << std::endl;
                }
            } else if (dt > 0.0) {
                // update az/el for ownship dynamics only, since no new report
                // do not update for ownship dynamics if sim is frozen
                // assume same los, but use GT rotmat to generate new rel az and rel el
                const base::Vec3d los_vec = getOwnship()->getRotMat() * tracks[it]->getLosVec();
                const double ra = std::sqrt(los_vec.x() * los_vec.x() + los_vec.y()*los_vec.y());
                const double az = std::atan2(los_vec.y(), los_vec.x());
                const double el = std::atan2(-los_vec.z(), ra);
                uAzimuth = az - taz;
                uElevation = el - tel;
                haveU = true;
            }

            if (haveU) {
                const double pTaz{tracks[it]->getPredictedAzimuth()};
                const double pTazRate{tracks[it]->getPredictedAzimuthRate()};
                const double pTel{tracks[it]->getPredictedElevation()};
                const double pTelRate{tracks[it]->getPredictedElevationRate()};

                // If the track is updated set the new azimuth and elevation data based on
                // a weighted average of the updated value and the predicted value from the
                // previous time step
                tracks[it]->setRelAzimuth((pTaz * oneMinusAlpha) + ((taz + uAzimuth) * alpha));
                tracks[it]->setRelAzimuthRate((pTazRate * oneMinusBeta) + ((uAzimuth / dt) * beta ));
                tracks[it]->setRelAzimuthAcceleration((tracks[it]->getRelAzimuthRate() - tazRate) / dt);
                tracks[it]->setElevation((pTel * oneMinusAlpha) + ((tel + uElevation) * alpha));
                tracks[it]->setElevationRate((pTelRate * oneMinusBeta) + ((uElevation / dt) * beta ));
                tracks[it]->setElevationAcceleration((tracks[it]->getElevationRate() - telRate) / dt);

                taz = tracks[it]->getRelAzimuth();
                tazRate = tracks[it]->getRelAzimuthRate();
                tazAccel = tracks[it]->getRelAzimuthAcceleration();
                tel = tracks[it]->getElevation();
                telRate = tracks[it]->getElevationRate();
                telAccel = tracks[it]->getElevationAcceleration();
            }

            if (ageAtNextTimeStep > 0.0) {
                // Project the values for the next time step
                const double deltaTimeSquaredOverTwo{(ageAtNextTimeStep * ageAtNextTimeStep) / 2.0};

                tracks[it]->setPredictedAzimuth(taz + tazRate*ageAtNextTimeStep
                    + tazAccel*deltaTimeSquaredOverTwo);
                tracks[it]->setPredictedAzimuthRate(tazRate + tazAccel*ageAtNextTimeStep);
                tracks[it]->setPredictedElevation(tel + telRate*ageAtNextTimeStep
                    + telAccel*deltaTimeSquaredOverTwo);
                tracks[it]->setPredictedElevationRate(telRate + telAccel*ageAtNextTimeStep);
            }
        } // end for (unsigned int it = 0; it < nTrks; it++) {
        base::unlock(trkListLock);


        // ---
        // 7) Remove tracks that have age beyond the limit.
        // ---
        removeAgedTracks();

    } // end if (nTrks > 0)


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
            newTrk->setType(Track::AIR_TRACK_BIT | Track::ONBOARD_SENSOR_BIT);

            if (usePerceivedPosVel) {
                // using reported/perceived position and velocity
                newTrk->setPosition(queryMessages[i]->getPosVec() - getOwnship()->getPosition());
                newTrk->setVelocity(queryMessages[i]->getVelocityVec() - getOwnship()->getVelocity());
            } else {
                // missile tracking requires position and velocity, using GT data
                newTrk->setPosition(queryMessages[i]->getTarget()->getPosition() - getOwnship()->getPosition());
                newTrk->setVelocity(queryMessages[i]->getTarget()->getVelocity() - getOwnship()->getVelocity());
            }

            newTrk->setRelAzimuth(queryMessages[i]->getRelativeAzimuth());
            newTrk->setElevation(queryMessages[i]->getRelativeElevation());
            newTrk->setPredictedAzimuth(queryMessages[i]->getRelativeAzimuth());
            newTrk->setPredictedElevation(queryMessages[i]->getRelativeElevation());
            newTrk->setSignal(queryMessages[i]->getSignalToNoiseRatio(), queryMessages[i]);

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
