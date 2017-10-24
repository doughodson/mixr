
#include "mixr/models/system/trackmanager/GmtiTrkMgr.hpp"

#include "mixr/models/Emission.hpp"
#include "mixr/models/Track.hpp"
#include "mixr/models/player/Player.hpp"
#include "mixr/models/player/weapon/AbstractWeapon.hpp"

#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/units/Times.hpp"

#include "mixr/base/units/Distances.hpp"

#include "mixr/simulation/AbstractDataRecorder.hpp"
#include "mixr/models/WorldModel.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(GmtiTrkMgr, "GmtiTrkMgr")
EMPTY_SLOTTABLE(GmtiTrkMgr)

GmtiTrkMgr::GmtiTrkMgr()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void GmtiTrkMgr::initData()
{
   setType( Track::ONBOARD_SENSOR_BIT | Track::GND_TRACK_BIT );

   reportNumMatches = new unsigned int[MAX_REPORTS];
   trackNumMatches = new unsigned int[MAX_TRKS];
   report2TrackMatch = new bool*[MAX_REPORTS];
   for (unsigned int i = 0; i < MAX_REPORTS; i++) {
      reportNumMatches[i] = 0;
      report2TrackMatch[i] = new bool[MAX_TRKS];
      for (unsigned int j = 0; j < MAX_TRKS; j++) {
         report2TrackMatch[i][j] = false;
      }
   }
   for (unsigned int i = 0; i < MAX_TRKS; i++) {
      trackNumMatches[i] = 0;
   }
}

void GmtiTrkMgr::copyData(const GmtiTrkMgr& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   for (unsigned int i = 0; i < MAX_REPORTS; i++) {
      reportNumMatches[i] = org.reportNumMatches[i];
      for (unsigned int j = 0; j < MAX_TRKS; j++) {
         report2TrackMatch[i][j] = org.report2TrackMatch[i][j];
      }
   }
   for (unsigned int i = 0; i < MAX_TRKS; i++) {
      trackNumMatches[i] = org.trackNumMatches[i];
   }
}

void GmtiTrkMgr::deleteData()
{
   if (report2TrackMatch != nullptr) {
      for (unsigned int i = 0; i < MAX_REPORTS; i++) {
         if (report2TrackMatch[i] != nullptr) {
            delete[] report2TrackMatch[i];
            report2TrackMatch[i] = nullptr;
         }
      }
      delete[] report2TrackMatch;
      report2TrackMatch = nullptr;
   }

   if (reportNumMatches != nullptr) {
      delete[] reportNumMatches;
      reportNumMatches = nullptr;
   }

   if (trackNumMatches != nullptr) {
      delete[] trackNumMatches;
      trackNumMatches = nullptr;
   }
}

//------------------------------------------------------------------------------
// processTrackList() -- process the track list
//------------------------------------------------------------------------------
void GmtiTrkMgr::processTrackList(const double dt)
{
   // Make sure we have an ownship to work with
   const auto ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
   if (ownship == nullptr || dt == 0) return;

   // Make sure we have the A and B matrix
   if (!haveMatrixA) makeMatrixA(dt);

   // ---
   // 1) Apply ownship dynamics to current track positions and age the tracks by delta time
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

   // Get each new emission report from the queue
   unsigned int nReports{};
   Emission* emissions[MAX_REPORTS]{};
   double newSignal[MAX_REPORTS]{};
   double newRdot[MAX_REPORTS]{};
   base::Vec3d tgtPos[MAX_REPORTS];
   double tmp{};
   for (Emission* em = getReport(&tmp); em != nullptr; em = getReport(&tmp)) {
      if (nReports < MAX_REPORTS) {
      Player* tgt{em->getTarget()};
      if (tgt->isMajorType(Player::GROUND_VEHICLE)) {
         // Using only Ground vehicles
         emissions[nReports] = em;
         newSignal[nReports] = tmp;
         newRdot[nReports] = emissions[nReports]->getRangeRate();
         reportNumMatches[nReports] = 0;
         tgtPos[nReports] = tgt->getPosition() - ownship->getPosition();
         nReports++;
      } else {
         // Free up emissions from other types of players
         em->unref();
      }
   }
      else {
         // ignore -- too many reports
      em->unref();
   }
   }

   // ---
   // 3) Match current tracks to new reports (observations)
   // ---
   base::lock(trkListLock);
   for (unsigned int it = 0; it < nTrks; it++) {
      trackNumMatches[it] = 0;
      const RfTrack* const trk{static_cast<const RfTrack*>(tracks[it])};  // we produce only RfTracks
      const Player* const tgt{trk->getLastEmission()->getTarget()};
      for (unsigned int ir = 0; ir < nReports; ir++) {
         if (emissions[ir]->getTarget() == tgt) {
            // We have a new report for the same target as this track ...
            report2TrackMatch[ir][it] = true;
            trackNumMatches[it]++;
            reportNumMatches[ir]++;
         }
         else report2TrackMatch[ir][it] = false;
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
   base::Vec3d u[MAX_TRKS];
   double age[MAX_TRKS]{};
   bool haveU[MAX_TRKS]{};
   base::lock(trkListLock);
   for (unsigned int it = 0; it < nTrks; it++) {
      u[it].set(0,0,0);
      haveU[it] = false;
      if (trackNumMatches[it] > 0) {
         for (unsigned int ir = 0; ir < nReports; ir++) {
            if (report2TrackMatch[ir][it]) {

               RfTrack* const trk{static_cast<RfTrack*>(tracks[it])};  // we produce only RfTracks

               // Update the track's signal
               trk->setSignal(newSignal[ir],emissions[ir]);

               // Create a track input vector
               u[it] = (tgtPos[ir] - trk->getPosition());

               // Track age and flags
               if (!haveU[it]) {
                  age[it] = trk->getTrackAge();
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
      const base::Vec3d tpos{tracks[i]->getPosition()};
      const base::Vec3d tvel{tracks[i]->getVelocity()};
      const base::Vec3d tacc{tracks[i]->getAcceleration()};

      if (haveU[i]) {
         // Have Input vector U, use ...
         // X(k+1) = A*X(k) + B*U(k)
         // where B is ...
         double b0{alpha};
         double b1{};
         if (age[i] != 0) b1 = beta / age[i];
         double b2{};
         //double b2 = gamma * 2.0 / (age[i]*age[i]);
         tracks[i]->setPosition(     (tpos*A[0][0] + tvel*A[0][1] + tacc*A[0][2]) + (u[i]*b0) );
         tracks[i]->setVelocity(     (tpos*A[1][0] + tvel*A[1][1] + tacc*A[1][2]) + (u[i]*b1) );
         tracks[i]->setAcceleration( (tpos*A[2][0] + tvel*A[2][1] + tacc*A[2][2]) + (u[i]*b2) );

         if (getLogTrackUpdates()) {
            // Object 1: player, Object 2: Track Data
            BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_TRACK_DATA )
               SAMPLE_2_OBJECTS( ownship, tracks[i] )
            END_RECORD_DATA_SAMPLE()
         }

      }
      else {
         // Do not have Input vector U, use ...
         // X(k+1) = A*X(k)
         tracks[i]->setPosition(     (tpos*A[0][0] + tvel*A[0][1] + tacc*A[0][2]));
         tracks[i]->setVelocity(     (tpos*A[1][0] + tvel*A[1][1] + tacc*A[1][2]));
         tracks[i]->setAcceleration( (tpos*A[2][0] + tvel*A[2][1] + tacc*A[2][2]));
      }
   }
   base::unlock(trkListLock);

   // ---
   // 7) For tracks with new observation reports, reset their age.
   //    Remove tracks that have aged beyond the limit.
   // ---
   base::lock(trkListLock);
   for (unsigned int it = 0; it < nTrks; /* update 'it' below */ ) {
      if (tracks[it]->getTrackAge() >= getMaxTrackAge()) {
         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Removed Aged GND track[it] = [" << it << "] id = " << tracks[it]->getTrackID() << std::endl;
         }

         // Object 1: player, Object 2: Track Data
         BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_TRACK_REMOVED )
            SAMPLE_2_OBJECTS( ownship, tracks[it] )
         END_RECORD_DATA_SAMPLE()

         // Track has timed out -- delete the track and ...
         tracks[it]->unref();
         nTrks--;
         // move all other tracks down in the list.
         for (unsigned int it2 = it; it2 < nTrks; it2++) {
            tracks[it2] = tracks[it2+1];
         }
      }
      else {
         // update track index, it
         it++;
      }
   }
   base::unlock(trkListLock);

   // ---
   // 8) Create new tracks from unmatched reports and free up emissions
   // ---
   base::lock(trkListLock);
   for (unsigned int i = 0; i < nReports; i++) {
      if ((reportNumMatches[i] == 0) && (nTrks < maxTrks)) {
         // This is a new report, so create a new track for it
         RfTrack* newTrk{new RfTrack()};
         newTrk->setTrackID( getNewTrackID() );
         newTrk->setTarget( emissions[i]->getTarget() );
         newTrk->setType(Track::GND_TRACK_BIT | Track::ONBOARD_SENSOR_BIT);
         newTrk->setPosition(tgtPos[i]);
         newTrk->ownshipDynamics(osGndTrk, osVel, osAccel, 0.0);
         newTrk->setRangeRate(newRdot[i]);
         newTrk->setSignal(newSignal[i], emissions[i]);

         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "New GND track[it] = [" << nTrks << "] id = " << newTrk->getTrackID() << std::endl;
         }

         // Object 1: player, Object 2: Track Data
         BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_NEW_TRACK )
            SAMPLE_2_OBJECTS( ownship, newTrk )
         END_RECORD_DATA_SAMPLE()

         tracks[nTrks++] = newTrk;
      }
      // Free the emission report
      emissions[i]->unref();
   }
   base::unlock(trkListLock);
}

}
}
