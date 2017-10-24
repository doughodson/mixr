
#include "mixr/models/system/trackmanager/TrackManager.hpp"

#include "mixr/models/Emission.hpp"
#include "mixr/models/Track.hpp"
#include "mixr/models/player/Player.hpp"
#include "mixr/models/player/weapon/AbstractWeapon.hpp"

#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/units/Times.hpp"

#include "mixr/base/List.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"

namespace mixr {
namespace models {

IMPLEMENT_PARTIAL_SUBCLASS(TrackManager, "TrackManager")

BEGIN_SLOTTABLE(TrackManager)
   "maxTracks",        // 1: Maximum number of tracks
   "maxTrackAge",      // 2: Maximum track age (time: sec)
   "firstTrackId",     // 3: First track ID
   "alpha",            // 4: Alpha
   "beta",             // 5: Beta
   "gamma",            // 6: Gamma
   "logTrackUpdates",  // 7: whether to log all updates to tracks (default: true)
END_SLOTTABLE(TrackManager)

BEGIN_SLOT_MAP(TrackManager)
   ON_SLOT(1, setSlotMaxTracks,       base::Number)
   ON_SLOT(2, setSlotMaxTrackAge,     base::Number)
   ON_SLOT(3, setSlotFirstTrackId,    base::Number)
   ON_SLOT(4, setSlotAlpha,           base::Number)
   ON_SLOT(5, setSlotBeta,            base::Number)
   ON_SLOT(6, setSlotGamma,           base::Number)
   ON_SLOT(7, setSlotLogTrackUpdates, base::Number)
END_SLOT_MAP()

TrackManager::TrackManager()
{
   STANDARD_CONSTRUCTOR()
}

TrackManager::TrackManager(const TrackManager& org)
{
   STANDARD_CONSTRUCTOR()
   copyData(org, true);
}

TrackManager::~TrackManager()
{
   STANDARD_DESTRUCTOR()
}

TrackManager& TrackManager::operator=(const TrackManager& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

TrackManager* TrackManager::clone() const
{
   return nullptr;
}

void TrackManager::copyData(const TrackManager& org, const bool)
{
   BaseClass::copyData(org);

   logTrackUpdates = org.logTrackUpdates;

   maxTrks = org.maxTrks;
   maxTrackAge = org.maxTrackAge;
   clearTracksAndQueues();

   type = org.type;
   firstTrkId = org.firstTrkId;
   nextTrkId = org.nextTrkId;

   // Copy Matrix A
   haveMatrixA = org.haveMatrixA;
   for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
         A[i][j] = org.A[i][j];
      }
   }
   // Parameters
   alpha   = org.alpha;
   beta    = org.beta;
   gamma   = org.gamma;
}

void TrackManager::deleteData()
{
   clearTracksAndQueues();
}

//------------------------------------------------------------------------------
// clearTracksAndQueues() -- Clear all tracks and queues
//------------------------------------------------------------------------------
void TrackManager::clearTracksAndQueues()
{
   // ---
   // Clear out the queue(s)
   // ---
   base::lock(queueLock);
   for (Emission* em = emQueue.get(); em != nullptr; em = emQueue.get()) {
      em->unref();    // unref() the emission
      snQueue.get();  // and every emission had a S/N value
   }
   base::unlock(queueLock);

   // ---
   // Clear the track list
   // ---
   base::lock(trkListLock);
   const unsigned int n{nTrks};
   nTrks = 0;
   for (unsigned int i = 0; i < n; i++) {
      if (tracks[i] != nullptr) {
         tracks[i]->clear();
         tracks[i]->unref();
         tracks[i] = nullptr;
      }
   }
   base::unlock(trkListLock);
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void TrackManager::reset()
{
   BaseClass::reset();

   clearTracksAndQueues();
   nextTrkId = firstTrkId;
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool TrackManager::shutdownNotification()
{
   clearTracksAndQueues();

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void TrackManager::process(const double dt)
{
   processTrackList(dt);
   BaseClass::process(dt);
}

//------------------------------------------------------------------------------
// Get track manager attributes
//------------------------------------------------------------------------------
double TrackManager::getMaxTrackAge() const
{
   return maxTrackAge;
}

unsigned int TrackManager::getMaxTracks() const
{
   return maxTrks;
}

unsigned int TrackManager::getNumTracks() const
{
   return nTrks;
}

bool TrackManager::isType(const short t) const
{
   return ((type & t) != 0);
}

short TrackManager::getType() const
{
   return type;
}

void TrackManager::setType(const short t)
{
   type = t;
}

void TrackManager::setSubtype(const short t)
{
   type = (t | type);
}

bool TrackManager::getLogTrackUpdates() const
{
   return logTrackUpdates;
}

bool TrackManager::setMaxTrackAge(const double s)
{
   bool ok{};
   if (s > 0) {
      maxTrackAge = static_cast<double>(s);
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// getTrackList() -- Sets entries in 'tlist' to a maximum of 'max' target
//                  tracks and returns the actual number of tracks.
//------------------------------------------------------------------------------
int TrackManager::getTrackList(base::safe_ptr<Track>* const tlist, const unsigned int max) const
{
   int n{};

   if (tlist != nullptr) {
      base::lock(trkListLock);
      for (unsigned int i = 0; i < nTrks && i < max; i++) {
         tlist[n++] = tracks[i];
      }
      base::unlock(trkListLock);
   }

   return n;
}

//------------------------------------------------------------------------------
// getTrackList() -- Sets entries in 'tlist' to a maximum of 'max' target
//                  tracks and returns the actual number of tracks.
//------------------------------------------------------------------------------
int TrackManager::getTrackList(base::safe_ptr<const Track>* const tlist, const unsigned int max) const
{
   int n{};

   if (tlist != nullptr) {
      base::lock(trkListLock);
      for (unsigned int i = 0; i < nTrks && i < max; i++) {
         tlist[n++] = tracks[i];
      }
      base::unlock(trkListLock);
   }

   return n;
}

//------------------------------------------------------------------------------
// getTrackList() -- returns the track list as an array of pointers
//------------------------------------------------------------------------------
int TrackManager::getTrackList(Track* tlist[], const unsigned int max)
{
   int n{};

   if (tlist != nullptr) {
      base::lock(trkListLock);
      for (unsigned int i = 0; i < nTrks && i < max; i++) {
         tlist[n] = tracks[i];
         tlist[n]->ref();
         n++;
      }
      base::unlock(trkListLock);
   }

   return n;
}

int TrackManager::getTrackList(const Track* tlist[], const unsigned int max) const
{
   int n{};

   if (tlist != nullptr) {
      base::lock(trkListLock);
      for (unsigned int i = 0; i < nTrks && i < max; i++) {
         tlist[n] = tracks[i];
         tlist[n]->ref();
         n++;
      }
      base::unlock(trkListLock);
   }

   return n;
}

//------------------------------------------------------------------------------
// killedNotification() -- We were just killed by player 'p'
//------------------------------------------------------------------------------
bool TrackManager::killedNotification(Player* const p)
{
   clearTracksAndQueues();
   return BaseClass::killedNotification(p);
}

//------------------------------------------------------------------------------
// newReport() -- Accept a new emission report
//------------------------------------------------------------------------------
void TrackManager::newReport(Emission* em, double sn)
{
   // Queue up emissions reports
   if (em != nullptr) {
      base::lock(queueLock);
      if (emQueue.isNotFull()) {
      em->ref();
      emQueue.put(em);
      snQueue.put(sn);
      }
      base::unlock(queueLock);
   }
}

//------------------------------------------------------------------------------
// getReport() -- Get the next 'new' report of the queue
//------------------------------------------------------------------------------
Emission* TrackManager::getReport(double* const sn)
{
   Emission* em{};

   base::lock(queueLock);
   em = emQueue.get();
   if (em != nullptr) {
      *sn = snQueue.get();
   }
   base::unlock(queueLock);

   return em;
}

//------------------------------------------------------------------------------
// addTrack() -- Add a track to the list
//------------------------------------------------------------------------------
bool TrackManager::addTrack(Track* const t)
{
   bool ok{};

   base::lock(trkListLock);
   if (nTrks < maxTrks) {
      t->ref();
      tracks[nTrks++] = t;
      ok = true;
   }
   base::unlock(trkListLock);

   return ok;
}

//------------------------------------------------------------------------------
// makeMatrixA() -- make standard A matrix
//------------------------------------------------------------------------------
void TrackManager::makeMatrixA(double dt)
{
   // Delta time (default: 50 hz)
   double t{dt};
   if (t == 0) t = 1.0 / 50.0;

   A[0][0] = 1;
   A[0][1] = t;
   A[0][2] = (t*t)/2.0;

   A[1][0] = 0;
   A[1][1] = 1;
   A[1][2] = t;

   A[2][0] = 0;
   A[2][1] = 0;
   A[2][2] = 1;

   haveMatrixA = true;
}

//------------------------------------------------------------------------------
// setMaxTracks() -- Sets the maximum number of active tracks
//------------------------------------------------------------------------------
bool TrackManager::setSlotMaxTracks(const base::Number* const num)
{
   bool ok{};
   if (num != nullptr) {
      const int max{num->getInt()};
      if (max > 0 && max <= static_cast<int>(MAX_TRKS)) {
         maxTrks = static_cast<unsigned int>(max);
         ok = true;
      } else {
         std::cerr << "TrackManager::setMaxTracks: maxTracks is invalid, range: [1 .. " << MAX_TRKS << "]" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotMaxTrackAge() -- Sets the maximum age of tracks
//------------------------------------------------------------------------------
bool TrackManager::setSlotMaxTrackAge(const base::Number* const num)
{
   double age{};
   const auto p = dynamic_cast<const base::Time*>(num);
   if (p != nullptr) {
      // We have a time and we want it in seconds ...
      base::Seconds seconds;
      age = seconds.convert(*p);
   } else if (num != nullptr) {
      // We have only a number, assume it's in seconds ...
      age = num->getReal();
   }

   // Set the value if it's valid
   bool ok{true};
   if (age > 0.0) {
      ok = setMaxTrackAge(age);
   } else {
      std::cerr << "TrackManager::setMaxTrackAge: invalid age, must be greater than zero." << std::endl;
      ok = false;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotFirstTrackId() -- Sets the first (starting) track id number
//------------------------------------------------------------------------------
bool TrackManager::setSlotFirstTrackId(const base::Number* const num)
{
   bool ok{};
   if (num != nullptr) {
      int first{num->getInt()};
      if (first >= 0) {
         firstTrkId = static_cast<unsigned int>(first);
         nextTrkId = firstTrkId;
         ok = true;
      } else {
         std::cerr << "TrackManager::setSlotFirstTrackId: firstTrackId is invalid: must be >= 0" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets alpha
//------------------------------------------------------------------------------
bool TrackManager::setSlotAlpha(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      alpha = msg->getReal();
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets beta
//------------------------------------------------------------------------------
bool TrackManager::setSlotBeta(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      beta = msg->getReal();
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets gamma
//------------------------------------------------------------------------------
bool TrackManager::setSlotGamma(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      gamma = msg->getReal();
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets logTrackUpdates
//------------------------------------------------------------------------------
bool TrackManager::setSlotLogTrackUpdates(const base::Number* const num)
{
   bool ok{};
   if (num != nullptr) {
      ok = setLogTrackUpdates( num->getBoolean() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets logTrackUpdates; controls output
//------------------------------------------------------------------------------
bool TrackManager::setLogTrackUpdates (const bool b)
{
   logTrackUpdates = b;
   return true;
}

}
}
