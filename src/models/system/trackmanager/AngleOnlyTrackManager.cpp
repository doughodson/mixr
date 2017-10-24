
#include "mixr/models/system/trackmanager/AngleOnlyTrackManager.hpp"

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

IMPLEMENT_PARTIAL_SUBCLASS(AngleOnlyTrackManager, "AngleOnlyTrackManager")

BEGIN_SLOTTABLE(AngleOnlyTrackManager)
    "azimuthBin",       // 1: Azimuth Bin
    "elevationBin",     // 2: Elevation Bin
END_SLOTTABLE(AngleOnlyTrackManager)

BEGIN_SLOT_MAP(AngleOnlyTrackManager)
    ON_SLOT(1, setSlotAzimuthBin,   base::Number)
    ON_SLOT(2, setSlotElevationBin, base::Number)
END_SLOT_MAP()

AngleOnlyTrackManager::AngleOnlyTrackManager() : queryQueue(MAX_TRKS)
{
    STANDARD_CONSTRUCTOR()
}

AngleOnlyTrackManager::AngleOnlyTrackManager(const AngleOnlyTrackManager& org) : queryQueue(MAX_TRKS)
{
    STANDARD_CONSTRUCTOR()
    copyData(org, true);
}

AngleOnlyTrackManager::~AngleOnlyTrackManager()
{
    STANDARD_DESTRUCTOR()
}

AngleOnlyTrackManager& AngleOnlyTrackManager::operator=(const AngleOnlyTrackManager& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

AngleOnlyTrackManager* AngleOnlyTrackManager::clone() const
{
    return nullptr;
}

void AngleOnlyTrackManager::copyData(const AngleOnlyTrackManager& org, const bool)
{
    BaseClass::copyData(org);

    azimuthBin = org.azimuthBin;
    elevationBin = org.elevationBin;
    oneMinusAlpha = org.oneMinusAlpha;
    oneMinusBeta = org.oneMinusBeta;
}

void AngleOnlyTrackManager::deleteData()
{
    clearTracksAndQueues();
}

//------------------------------------------------------------------------------
// shutdownNotification()
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::shutdownNotification()
{
   clearTracksAndQueues();
   return BaseClass::shutdownNotification();
}

//------------------------------------------------------------------------------
// clearTracksAndQueues() -- Clear all tracks and queues
//------------------------------------------------------------------------------
void AngleOnlyTrackManager::clearTracksAndQueues()
{
    // ---
    // Clear out the queue(s)
    // ---
    base::lock(queueLock);
    for (IrQueryMsg* q = queryQueue.get(); q != nullptr; q = queryQueue.get()) {
        q->unref();     // unref() the IR query message
        snQueue.get();  // and every IR query message had a S/N value
    }
    base::unlock(queueLock);

    // ---
    // Clear the track list
    // ---
    base::lock(trkListLock);
    unsigned int n{nTrks};
    nTrks = 0;
    for (unsigned int i = 0; i < n; i++) {
        tracks[i]->clear();
        tracks[i]->unref();
        tracks[i] = nullptr;
    }
    base::unlock(trkListLock);
}

//------------------------------------------------------------------------------
// newReport() -- Accept a new IR Query Message report
//------------------------------------------------------------------------------
void AngleOnlyTrackManager::newReport(IrQueryMsg* q, double sn)
{
    // Queue up IR query messages reports
    if (q != nullptr) {
        q->ref();
        base::lock(queueLock);
        queryQueue.put(q);
        snQueue.put(sn);
        base::unlock(queueLock);
    }
}

//------------------------------------------------------------------------------
// getQuery() -- Get the next 'new' report of the queue
//------------------------------------------------------------------------------
IrQueryMsg* AngleOnlyTrackManager::getQuery(double* const sn)
{
    IrQueryMsg* q{};

    base::lock(queueLock);
    q = queryQueue.get();
    if (q != nullptr) {
        *sn = snQueue.get();
    }
    base::unlock(queueLock);

    return q;
}

//------------------------------------------------------------------------------
// addTrack() -- Add a track to the list
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::addTrack(Track* const t)
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
// Sets azimuth bin
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::setSlotAzimuthBin(const base::Number* const msg)
{
    double value{};

    const auto a = dynamic_cast<const base::Angle*>(msg);
    if (a != nullptr) {
        base::Radians r;
        value = static_cast<double>(r.convert(*a));
    } else if (msg != nullptr) {
        value = msg->getReal();
    }

    azimuthBin = value;

    return true;
}

//------------------------------------------------------------------------------
// Sets azimuth bin
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::setSlotElevationBin(const base::Number* const msg)
{
    double value{};

    const auto a = dynamic_cast<const base::Angle*>(msg);
    if (a != nullptr) {
        base::Radians r;
        value = static_cast<double>(r.convert(*a));
    } else if (msg != nullptr) {
        value = msg->getReal();
    }

    elevationBin = value;

    return true;
}

//------------------------------------------------------------------------------
// Sets alpha
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::setSlotAlpha(const base::Number* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = true;
        alpha = msg->getReal();
        oneMinusAlpha = 1.0 - alpha;
    }
    return ok;
}

//------------------------------------------------------------------------------
// Sets beta
//------------------------------------------------------------------------------
bool AngleOnlyTrackManager::setSlotBeta(const base::Number* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        ok = true;
        beta = msg->getReal();
        oneMinusBeta = 1.0 - beta;
    }
    return ok;
}

}
}
