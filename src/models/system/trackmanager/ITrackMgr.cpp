
#include "mixr/models/system/trackmanager/ITrackMgr.hpp"

#include "mixr/models/RfEmission.hpp"
#include "mixr/models/track/ITrack.hpp"
#include "mixr/models/player/IPlayer.hpp"
#include "mixr/models/player/weapon/IWeapon.hpp"

#include "mixr/base/IList.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/IPairStream.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/qty/times.hpp"

namespace mixr {
namespace models {

IMPLEMENT_PARTIAL_SUBCLASS(ITrackMgr, "ITrackMgr")

BEGIN_SLOTTABLE(ITrackMgr)
   "maxTracks",        // 1: Maximum number of tracks
   "maxTrackAge",      // 2: Maximum track age (time: sec)
   "firstTrackId",     // 3: First track ID
   "alpha",            // 4: Alpha
   "beta",             // 5: Beta
   "gamma",            // 6: Gamma
   "logTrackUpdates",  // 7: whether to log all updates to tracks (default: true)
END_SLOTTABLE(ITrackMgr)

BEGIN_SLOT_MAP(ITrackMgr)
   ON_SLOT(1, setSlotMaxTracks,       base::Integer)
   ON_SLOT(2, setSlotMaxTrackAge,     base::INumber)
   ON_SLOT(2, setSlotMaxTrackAge,     base::ITime)
   ON_SLOT(3, setSlotFirstTrackId,    base::Integer)
   ON_SLOT(4, setSlotAlpha,           base::INumber)
   ON_SLOT(5, setSlotBeta,            base::INumber)
   ON_SLOT(6, setSlotGamma,           base::INumber)
   ON_SLOT(7, setSlotLogTrackUpdates, base::Boolean)
END_SLOT_MAP()

ITrackMgr::ITrackMgr()
{
   STANDARD_CONSTRUCTOR()
}

ITrackMgr::ITrackMgr(const ITrackMgr& org)
{
   STANDARD_CONSTRUCTOR()
   copyData(org, true);
}

ITrackMgr::~ITrackMgr()
{
   STANDARD_DESTRUCTOR()
}

ITrackMgr& ITrackMgr::operator=(const ITrackMgr& org)
{
   if (this != &org) copyData(org,false);
   return *this;
}

ITrackMgr* ITrackMgr::clone() const
{
   return nullptr;
}

void ITrackMgr::copyData(const ITrackMgr& org, const bool)
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

void ITrackMgr::deleteData()
{
   clearTracksAndQueues();
}

//------------------------------------------------------------------------------
// clearTracksAndQueues() -- Clear all tracks and queues
//------------------------------------------------------------------------------
void ITrackMgr::clearTracksAndQueues()
{
   // ---
   // Clear out the queue(s)
   // ---
   base::lock(queueLock);
   for (RfEmission* em = emQueue.get(); em != nullptr; em = emQueue.get()) {
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
void ITrackMgr::reset()
{
   BaseClass::reset();

   clearTracksAndQueues();
   nextTrkId = firstTrkId;
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool ITrackMgr::shutdownNotification()
{
   clearTracksAndQueues();

   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// process() -- Process phase
//------------------------------------------------------------------------------
void ITrackMgr::process(const double dt)
{
   processTrackList(dt);
   BaseClass::process(dt);
}

//------------------------------------------------------------------------------
// Get track manager attributes
//------------------------------------------------------------------------------
double ITrackMgr::getMaxTrackAge() const
{
   return maxTrackAge;
}

unsigned int ITrackMgr::getMaxTracks() const
{
   return maxTrks;
}

unsigned int ITrackMgr::getNumTracks() const
{
   return nTrks;
}

bool ITrackMgr::isType(const short t) const
{
   return ((type & t) != 0);
}

short ITrackMgr::getType() const
{
   return type;
}

void ITrackMgr::setType(const short t)
{
   type = t;
}

void ITrackMgr::setSubtype(const short t)
{
   type = (t | type);
}

bool ITrackMgr::getLogTrackUpdates() const
{
   return logTrackUpdates;
}

bool ITrackMgr::setMaxTrackAge(const double s)
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
int ITrackMgr::getTrackList(base::safe_ptr<ITrack>* const tlist, const unsigned int max) const
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
int ITrackMgr::getTrackList(base::safe_ptr<const ITrack>* const tlist, const unsigned int max) const
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
int ITrackMgr::getTrackList(ITrack* tlist[], const unsigned int max)
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

int ITrackMgr::getTrackList(const ITrack* tlist[], const unsigned int max) const
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
bool ITrackMgr::killedNotification(IPlayer* const p)
{
   clearTracksAndQueues();
   return BaseClass::killedNotification(p);
}

//------------------------------------------------------------------------------
// newReport() -- Accept a new emission report
//------------------------------------------------------------------------------
void ITrackMgr::newReport(RfEmission* em, double sn)
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
RfEmission* ITrackMgr::getReport(double* const sn)
{
   RfEmission* em{};

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
bool ITrackMgr::addTrack(ITrack* const t)
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
void ITrackMgr::makeMatrixA(double dt)
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
bool ITrackMgr::setSlotMaxTracks(const base::Integer* const num)
{
   bool ok{};
   if (num != nullptr) {
      const int max{num->asInt()};
      if (max > 0 && max <= static_cast<int>(MAX_TRKS)) {
         maxTrks = static_cast<unsigned int>(max);
         ok = true;
      } else {
         std::cerr << "ITrackMgr::setMaxTracks: maxTracks is invalid, range: [1 .. " << MAX_TRKS << "]" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotMaxTrackAge() -- Sets the maximum age of tracks
//------------------------------------------------------------------------------
bool ITrackMgr::setSlotMaxTrackAge(const base::INumber* const x)
{
   double age{};
   if (x != nullptr) {
      // We have only a number, assume it's in seconds ...
      age = x->asDouble();
   }

   // Set the value if it's valid
   bool ok{true};
   if (age > 0.0) {
      ok = setMaxTrackAge(age);
   } else {
      std::cerr << "ITrackMgr::setMaxTrackAge: invalid age, must be greater than zero." << std::endl;
      ok = false;
   }
   return ok;
}

bool ITrackMgr::setSlotMaxTrackAge(const base::ITime* const x)
{
   double age{};
   if (x != nullptr) {
      age = x->getValueInSeconds();
   }

   // Set the value if it's valid
   bool ok{ true };
   if (age > 0.0) {
      ok = setMaxTrackAge(age);
   } else {
      std::cerr << "ITrackMgr::setMaxTrackAge: invalid age, must be greater than zero." << std::endl;
      ok = false;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotFirstTrackId() -- Sets the first (starting) track id number
//------------------------------------------------------------------------------
bool ITrackMgr::setSlotFirstTrackId(const base::Integer* const num)
{
   bool ok{};
   if (num != nullptr) {
      int first{num->asInt()};
      if (first >= 0) {
         firstTrkId = static_cast<unsigned int>(first);
         nextTrkId = firstTrkId;
         ok = true;
      } else {
         std::cerr << "ITrackMgr::setSlotFirstTrackId: firstTrackId is invalid: must be >= 0" << std::endl;
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets alpha
//------------------------------------------------------------------------------
bool ITrackMgr::setSlotAlpha(const base::INumber* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      alpha = msg->asDouble();
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets beta
//------------------------------------------------------------------------------
bool ITrackMgr::setSlotBeta(const base::INumber* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      beta = msg->asDouble();
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets gamma
//------------------------------------------------------------------------------
bool ITrackMgr::setSlotGamma(const base::INumber* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      gamma = msg->asDouble();
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets logTrackUpdates
//------------------------------------------------------------------------------
bool ITrackMgr::setSlotLogTrackUpdates(const base::Boolean* const num)
{
   bool ok{};
   if (num != nullptr) {
      ok = setLogTrackUpdates( num->asBool() );
   }
   return ok;
}

//------------------------------------------------------------------------------
// Sets logTrackUpdates; controls output
//------------------------------------------------------------------------------
bool ITrackMgr::setLogTrackUpdates (const bool b)
{
   logTrackUpdates = b;
   return true;
}

}
}
