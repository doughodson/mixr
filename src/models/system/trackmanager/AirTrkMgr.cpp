
#include "mixr/models/system/trackmanager/AirTrkMgr.hpp"

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

IMPLEMENT_SUBCLASS(AirTrkMgr, "AirTrkMgr")

BEGIN_SLOTTABLE(AirTrkMgr)
   "positionGate",     // 1: Position Gate (meters)
   "rangeGate",        // 2: Range Gate (meters)
   "velocityGate",     // 3: Velocity Gate (m/s)
END_SLOTTABLE(AirTrkMgr)

BEGIN_SLOT_MAP(AirTrkMgr)
   ON_SLOT(1, setSlotPositionGate, base::Number)
   ON_SLOT(2, setSlotRangeGate,    base::Number)
   ON_SLOT(3, setSlotVelocityGate, base::Number)
END_SLOT_MAP()

AirTrkMgr::AirTrkMgr()
{
   STANDARD_CONSTRUCTOR()

   initData();
}

void AirTrkMgr::initData()
{
   setType( Track::ONBOARD_SENSOR_BIT | Track::AIR_TRACK_BIT );

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

void AirTrkMgr::copyData(const AirTrkMgr& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   posGate = org.posGate;
   rngGate = org.rngGate;
   velGate = org.velGate;

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

void AirTrkMgr::deleteData()
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
//
//  (Based on Hovanessian, "Radar Detection & Tracking Systems")
//
//------------------------------------------------------------------------------
void AirTrkMgr::processTrackList(const double dt)
{
   // Make sure we have an ownship to work with
   const auto ownship = dynamic_cast<Player*>( findContainerByType(typeid(Player)) );
   if (ownship == nullptr || dt == 0) return;

   // Make sure we have the A and B matrix
   if (!haveMatrixA) makeMatrixA(dt);

   // ---
   // 1) Apply ownship dynamics to current track positions and age the tracks by delta time
   // ---
   base::Vec3d osVel{ownship->getVelocity()};
   base::Vec3d osAccel{ownship->getAcceleration()};
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
   base::Vec3d tgtPos[MAX_REPORTS]{};
   double tmp{};
   for (Emission* em = getReport(&tmp); em != nullptr; em = getReport(&tmp)) {

      if (nReports < MAX_REPORTS) {

      Player* tgt{em->getTarget()};

      bool dummy{};
      if (tgt->isMajorType(Player::WEAPON)) {
         dummy = (static_cast<const AbstractWeapon*>(tgt))->isDummy();
      }

      if ( tgt->isMajorType(Player::AIR_VEHICLE) ||
         tgt->isMajorType(Player::SHIP) ||
         (tgt->isMajorType(Player::WEAPON) && !dummy)
         ) {
            // Using only air vehicles
            emissions[nReports] = em;
            newSignal[nReports] = tmp;
            newRdot[nReports] = emissions[nReports]->getRangeRate();
            reportNumMatches[nReports] = 0;
            tgtPos[nReports] = tgt->getPosition() - ownship->getPosition();
            nReports++;
      }
      else {
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
   const double d2{posGate * posGate};    // position gate squared
   base::lock(trkListLock);
   for (unsigned int i = 0; i < nTrks; i++) {
      // Save X(k)
      base::Vec3d tpos{tracks[i]->getPosition()};
      base::Vec3d tvel{tracks[i]->getVelocity()};
      base::Vec3d tacc{tracks[i]->getAcceleration()};

      if (haveU[i]) {
         // Have Input vector U, use ...
         // where B is ...
         double b0{alpha};
         double b1{};
         if (age[i] != 0) b1 = beta / age[i];
         double b2{};
         //double b2 = gamma * 2.0f / (age[i]*age[i]);
         if (u[i].length2() > d2) {
            // Large position change: just set position
            b0 = 1.0;
            b1 = 0.0;
         }

         // X(k+1) = A*X(k) + B*U(k)
         tracks[i]->setPosition(     (tpos*A[0][0] + tvel*A[0][1] + tacc*A[0][2]) + (u[i]*b0) );
         tracks[i]->setVelocity(     (tpos*A[1][0] + tvel*A[1][1] + tacc*A[1][2]) + (u[i]*b1) );
         tracks[i]->setAcceleration( (tpos*A[2][0] + tvel*A[2][1] + tacc*A[2][2]) + (u[i]*b2) );

         // Object 1: player, Object 2: Track Data
         if (getLogTrackUpdates()) {
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
      RfTrack* const trk{static_cast<RfTrack*>(tracks[it])};  // we produce only RfTracks
      if (trk->getTrackAge() >= getMaxTrackAge()) {

         // Object 1: player, Object 2: Track Data
         BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_TRACK_REMOVED )
            SAMPLE_2_OBJECTS( ownship, tracks[it] )
         END_RECORD_DATA_SAMPLE()

         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "Removed Aged AIR track: player: " << trk->getLastEmission()->getTarget() << ", refCnt = " << trk->getLastEmission()->getTarget()->getRefCount() << std::endl;
         }

         // Track has timed out -- delete the track and ...
         trk->setType(0);
         trk->unref();
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
         const auto newTrk = new RfTrack();
         newTrk->setTrackID( getNewTrackID() );
         newTrk->setTarget( emissions[i]->getTarget() );
         newTrk->setType(Track::AIR_TRACK_BIT | Track::ONBOARD_SENSOR_BIT);
         newTrk->setPosition(tgtPos[i]);
         newTrk->ownshipDynamics(osGndTrk, osVel, osAccel, 0.0);
         newTrk->setRangeRate(newRdot[i]);
         newTrk->setSignal(newSignal[i],emissions[i]);

         if (isMessageEnabled(MSG_INFO)) {
            std::cout << "New AIR track[it] = [" << nTrks << "] id = " << newTrk->getTrackID() << std::endl;
         }

         BEGIN_RECORD_DATA_SAMPLE( getWorldModel()->getDataRecorder(), REID_NEW_TRACK )
            SAMPLE_2_OBJECTS( ownship, tracks[i] )
         END_RECORD_DATA_SAMPLE()

         tracks[nTrks++] = newTrk;
      }
      // Free the emission report
      emissions[i]->unref();
   }
   base::unlock(trkListLock);
}

//------------------------------------------------------------------------------
// setPositionGate() -- Sets the size of the position gate
//------------------------------------------------------------------------------
bool AirTrkMgr::setSlotPositionGate(const base::Number* const num)
{
   double value{};
   const auto p = dynamic_cast<const base::Distance*>(num);
   if (p != nullptr) {
      // We have a distance and we want it in meters ...
      base::Meters meters;
      value = meters.convert(*p);
   }
   else if (num != nullptr) {
      // We have only a number, assume it's in meters ...
      value = num->getReal();
   }

   // Set the value if it's valid
   bool ok{true};
   if (value > 0.0) {
      posGate = value;
   }
   else {
      std::cerr << "TrackManager::setPositionGate: invalid gate, must be greater than zero." << std::endl;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setRangeGate() -- Sets the size of the range gate
//------------------------------------------------------------------------------
bool AirTrkMgr::setSlotRangeGate(const base::Number* const num)
{
   double value{};
   const auto p = dynamic_cast<const base::Distance*>(num);
   if (p != nullptr) {
      // We have a distance and we want it in meters ...
      base::Meters meters;
      value = meters.convert(*p);
   }
   else if (num != nullptr) {
      // We have only a number, assume it's in meters ...
      value = num->getReal();
   }

   // Set the value if it's valid
   bool ok{true};
   if (value > 0.0) {
      rngGate = value;
   }
   else {
      std::cerr << "TrackManager::setRangeGate: invalid gate, must be greater than zero." << std::endl;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setVelocityGate() -- Sets the size of the velocity gate
//------------------------------------------------------------------------------
bool AirTrkMgr::setSlotVelocityGate(const base::Number* const num)
{
   double value{};
   if (num != nullptr) {
      // We have only a number, assume it's in meters ...
      value = num->getReal();
   }

   // Set the value if it's valid
   bool ok{true};
   if (value > 0.0) {
      velGate = value;
   }
   else {
      std::cerr << "TrackManager::setVelocityGate: invalid gate, must be greater than zero." << std::endl;
      ok = true;
   }
   return ok;
}

}
}
