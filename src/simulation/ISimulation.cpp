
#include "mixr/simulation/ISimulation.hpp"

#include "mixr/simulation/IPlayer.hpp"

#include "SimulationTcSyncThread.hpp"
#include "SimulationBgSyncThread.hpp"

#include "mixr/simulation/IDataRecorder.hpp"
#include "mixr/simulation/INib.hpp"
#include "mixr/simulation/Station.hpp"

#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/Statistic.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/units/times.hpp"
#include "mixr/base/util/system_utils.hpp"

#include <cstring>
#include <cmath>

namespace mixr {
namespace simulation {

IMPLEMENT_PARTIAL_SUBCLASS(ISimulation, "ISimulation")

BEGIN_SLOTTABLE(ISimulation)
   "players",        // 1) All players
   "simulationTime", // 2) Simulation time
   "day",            // 3) Initial simulated day of month [ 1 .. 31 ]
   "month",          // 4) Initial simulated month [ 1 .. 12 ]
   "year",           // 5) Initial simulated year [ 1970 .. 2100 ]

   "firstWeaponId",  // 6) First Released Weapon ID (default: 10001)

   "numTcThreads",   // 7) Number of T/C threads to use with the player list
   "numBgThreads",   // 8) Number of background threads to use with the player list
   "enableFrameTiming",     // 9) Enable/disable the frame timing
   "printFrameTimingStats"  //10) Enable/disable the printing of the frame timing statistics
   END_SLOTTABLE(ISimulation)

BEGIN_SLOT_MAP(ISimulation)
    ON_SLOT( 1, setSlotPlayers,         base::PairStream)

    ON_SLOT( 2, setSlotSimulationTime,  base::ITime)
    ON_SLOT( 3, setSlotDay,             base::Integer)
    ON_SLOT( 4, setSlotMonth,           base::Integer)
    ON_SLOT( 5, setSlotYear,            base::Integer)

    ON_SLOT( 6, setSlotFirstWeaponId,   base::Integer)

    ON_SLOT( 7, setSlotNumTcThreads,    base::Integer)
    ON_SLOT( 8, setSlotNumBgThreads,    base::Integer)
    ON_SLOT( 9, setSlotEnableFrameTiming,     base::Boolean)
    ON_SLOT(10, setSlotPrintFrameTimingStats, base::Boolean)
END_SLOT_MAP()

ISimulation::ISimulation() : newPlayerQueue(MAX_NEW_PLAYERS)
{
   STANDARD_CONSTRUCTOR()
}

ISimulation::ISimulation(const ISimulation& org) : newPlayerQueue(MAX_NEW_PLAYERS)
{
   STANDARD_CONSTRUCTOR()
   copyData(org, true);
}

ISimulation::~ISimulation()
{
   STANDARD_DESTRUCTOR()
}

ISimulation& ISimulation::operator=(const ISimulation& org)
{
    if (this != &org) copyData(org,false);
    return *this;
}

ISimulation* ISimulation::clone() const
{
   return new ISimulation(*this);
}

void ISimulation::copyData(const ISimulation& org, const bool)
{
   BaseClass::copyData(org);

   // Find our own Station
   station = nullptr;

   // Unref our old stuff (if any)

   // Copy original players -- DPG need proper method to copy original player list
   if (origPlayers != nullptr) { origPlayers = nullptr; }
   if (org.origPlayers != nullptr) {
      origPlayers = org.origPlayers->clone();
      origPlayers->unref();  // safe_ptr<> has it
   }

   // Copy active players
   if (players != nullptr)     { players = nullptr; }
   if (org.players != nullptr) {
      players = org.players->clone();
      players->unref();  // safe_ptr<> has it
   }

   // Timing
   cycleCnt = org.cycleCnt;
   frameCnt = org.frameCnt;
   phaseCnt = org.phaseCnt;

   execTime = org.execTime;

   pcTime = org.pcTime;
   pcTvSec = org.pcTvSec;
   pcTvUSec = org.pcTvUSec;

   simTime = org.simTime;
   simTvSec = org.simTvSec;
   simTvUSec = org.simTvUSec;
   simTimeSlaved = org.simTimeSlaved;

   simTime0 = org.simTime0;
   simDay0 = org.simDay0;
   simMonth0 = org.simMonth0;
   simYear0 = org.simYear0;

   eventID = org.eventID;
   eventWpnID = org.eventWpnID;
   relWpnId = org.relWpnId;

   // ---
   // Terminate our threads and only copy the required number of threads;
   // reset() will create new ones.
   // ---

   for (int i = 0; i < numTcThreads; i++) {
      tcThreads[i]->terminate();
      tcThreads[i]->unref();
      tcThreads[i] = nullptr;
   }
   numTcThreads = 0;
   tcThreadsFailed = false;
   reqTcThreads = org.reqTcThreads;

   for (int i = 0; i < numBgThreads; i++) {
      bgThreads[i]->terminate();
      bgThreads[i]->unref();
      bgThreads[i] = nullptr;
   }
   numBgThreads = 0;
   bgThreadsFailed = false;
   reqBgThreads = org.reqBgThreads;

   // Timing statistics
   if (frameTimingStats != nullptr) {
      frameTimingStats->unref();
      frameTimingStats = nullptr;
   }
   if (org.frameTimingStats != nullptr) {
      frameTimingStats = org.frameTimingStats->clone();
   }
   pfts = org.pfts;
   tcLastFrameTime = 0.0;
}

void ISimulation::deleteData()
{
   if (origPlayers != nullptr) { origPlayers = nullptr; }
   if (players != nullptr)     { players = nullptr; }

   base::Pair* newPlayer{newPlayerQueue.get()};
   while (newPlayer != nullptr) {
      newPlayer->unref();
      newPlayer = newPlayerQueue.get();
   }

   for (int i = 0; i < numTcThreads; i++) {
      tcThreads[i]->terminate();
      tcThreads[i]->unref();
      tcThreads[i] = nullptr;
    }
   numTcThreads = 0;
   tcThreadsFailed = false;

   for (int i = 0; i < numBgThreads; i++) {
      bgThreads[i]->terminate();
      bgThreads[i]->unref();
      bgThreads[i] = nullptr;
   }
   numBgThreads = 0;
   bgThreadsFailed = false;

   station = nullptr;

   if (frameTimingStats != nullptr) {
      frameTimingStats->unref();
      frameTimingStats = nullptr;
   }
}

//------------------------------------------------------------------------------
// reset() -- Reset the simulation & players
//------------------------------------------------------------------------------
void ISimulation::reset()
{
   // ---
   // Something old and something new ...
   // ... We're going to create a new player list.
   // ---
   base::safe_ptr<base::PairStream> newList( new base::PairStream() );
   newList->unref();  // 'newList' has it, so unref() from the 'new'

   // ---
   // Copy original players to the new list
   // ---
   {
      if (origPlayers != nullptr) {
         base::safe_ptr<base::PairStream> origPlayerList = origPlayers;
         base::IList::Item* item{origPlayerList->getFirstItem()};
         while (item != nullptr) {
            base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
            IPlayer* ip{static_cast<IPlayer*>(pair->object())};

            // reinstated the container pointer and player name
            ip->container(this);
            ip->setName((pair->slot()));

            // Insert the player into the new list in sorted order
            insertPlayerSort(pair, newList);
            item = item->getNext();
         }
      }
   }

   // ---
   // Copy the old proxy players to the new list
   // ---
   {
      if (players != nullptr) {
         base::safe_ptr<base::PairStream> origPlayerList = players;
         base::IList::Item* item{origPlayerList->getFirstItem()};
         while (item != nullptr) {
            base::Pair* pair {static_cast<base::Pair*>(item->getValue())};
            IPlayer* ip {static_cast<IPlayer*>(pair->object())};
            if (ip->isProxyPlayer()) {

               // reinstated the container pointer and player name
               ip->container(this);
               ip->setName(pair->slot());

               // Insert the proxy player into the new list in sorted order
               insertPlayerSort(pair, newList);
            }
            item = item->getNext();
         }
      }
   }

   // ---
   // Swap the lists
   // ---
   players = newList;

   // ---
   // Create the T/C thread pool
   // ---
   if (reqTcThreads > 1 && numTcThreads == 0 && !tcThreadsFailed) {

      // Use the T/C priority from our container Station.
      double priority{Station::DEFAULT_TC_THREAD_PRI};
      const Station* sta{static_cast<const Station*>(findContainerByType( typeid(Station) ))};
      if (sta != nullptr) {
         priority = sta->getTimeCriticalPriority();
      }

      for (int i = 0; i < (reqTcThreads-1); i++) {
         tcThreads[numTcThreads] = new SimulationTcSyncThread(this);
         bool ok{tcThreads[numTcThreads]->start(priority)};
         if (ok) {
            std::cout << "Created T/C pool thread[" << i << "] = " << tcThreads[i] << std::endl;
            numTcThreads++;
         } else {
            tcThreads[numTcThreads]->unref();
            tcThreads[numTcThreads] = nullptr;
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "Simulation::reset(): ERROR, failed to create a T/C pool thread!" << std::endl;
            }
         }
      }

      // If we still don't have any threads then something failed
      // and we don't want to try again.
      tcThreadsFailed = (reqTcThreads > 1 && numTcThreads == 0);

   }

   // ---
   // Create the background thread pool
   // ---
   if (reqBgThreads > 1 && numBgThreads == 0 && !bgThreadsFailed) {

      // Use the background priority from our container Station.
      double priority{Station::DEFAULT_BG_THREAD_PRI};
      const Station* sta{static_cast<const Station*>(findContainerByType( typeid(Station) ))};
      if (sta != nullptr) {
         priority = sta->getBackgroundPriority();
      }

      for (int i = 0; i < (reqBgThreads-1); i++) {
         bgThreads[numBgThreads] = new SimulationBgSyncThread(this);
         bool ok {bgThreads[numBgThreads]->start(priority)};
         if (ok) {
            std::cout << "Created background pool thread[" << i << "] = " << bgThreads[i] << std::endl;
            numBgThreads++;
         } else {
            bgThreads[numBgThreads]->unref();
            bgThreads[numBgThreads] = nullptr;
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "Simulation::reset(): ERROR, failed to create a background pool thread!" << std::endl;
            }
         }
      }

      // If we still don't have any threads then something failed
      // and we don't want to try again.
      bgThreadsFailed = (reqBgThreads > 1 && numBgThreads == 0);

   }

   // ---
   // Reset simulated time (if not slaved to UTC)
   // ---

   // Start with the current computer system date and time as seconds
   // since midnight, January 1, 1970 (UTC)
   base::getTime(&pcTvSec, &pcTvUSec);

   // Decompose into year, month, etc
   unsigned int cYear{};
   unsigned int cMonth{};
   unsigned int cDay{};
   unsigned int cHour{};
   unsigned int cMin{};
   unsigned int cSec{};
   base::convertSec2Ymdhms(pcTvSec, &cYear, &cMonth, &cDay, &cHour, &cMin, &cSec);
      //std::printf("RESET PC Times: y=%d; m=%d; d=%d; h=%d; m=%d; s=%d\n", cYear, cMonth, cDay, cHour, cMin, cSec);
      //std::printf("RESET PC sec = %d, uSec=%d\n", pcTvSec, pcTvUSec);

   // Computer time of day (seconds since midnight)
   pcTime = ( cHour * 3600.0 + cMin * 60.0 + cSec );

   // Simulated uSec same as the computer systems
   simTvUSec = pcTvUSec;

   simTimeSlaved = (simTime0 < 0) && (simDay0 == 0) && (simMonth0 == 0) && (simYear0 == 0);
   if (!simTimeSlaved) {
      // Replace year, month or date, as required
      if (simYear0 > 0) cYear = simYear0;
      if (simMonth0 > 0) cMonth = simMonth0;
      if (simDay0 > 0) cDay = simDay0;

      // Replace the time of day, as required
      if (simTime0 >= 0) {
         unsigned int t{static_cast<unsigned int>(simTime0)};
         // Compute simulated hour of the date
         cHour = (t / 3600);
         t -= (cHour * 3600);
         // Compute simulated minute past the hour
         cMin  = t/60;
         t -= (cMin * 60);
         // Compute simulated second past the minute
         cSec = t;

         // Set simulated uSec set to 0; using pc microseconds is meaningless and introduces randomness where none is desired.
         simTvUSec = 0;
      }

      // Recompute simulated whole seconds since midnight, January 1, 1970
      base::convertYmdhms2Sec(cYear, cMonth, cDay, cHour, cMin, cSec, &simTvSec);
      //std::printf("RESET sim Times: y=%d; m=%d; d=%d; h=%d; m=%d; s=%d\n", cYear, cMonth, cDay, cHour, cMin, cSec);
      //std::printf("RESET SIM sec = %d, uSec=%d\n", simTvSec, simTvUSec);

      // simulated time of day (seconds since midnight)
      simTime = ( cHour * 3600.0 + cMin * 60.0 + cSec );
   } else {
      simTvSec = pcTvSec;
      simTime = pcTime;
   }

   // ---
   // Now reset the new player list
   // ---
   if (players != nullptr) {
      base::safe_ptr<base::PairStream> pl = players;
      base::IList::Item* item{pl->getFirstItem()};
      while (item != nullptr) {
         base::Pair* pair {static_cast<base::Pair*>(item->getValue())};
         if (pair != nullptr) {
            IPlayer* ip {static_cast<IPlayer*>(pair->object())};
            if (ip != nullptr) ip->event(RESET_EVENT);
         }
         item = item->getNext();
      }
   }

   BaseClass::reset();
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Shutdown the simulation
//------------------------------------------------------------------------------
bool ISimulation::shutdownNotification()
{
   // ---
   // Shutdown our baseclass, which will notify our components
   // ---
   BaseClass::shutdownNotification();

   // ---
   // Tell everyone on our player list
   // ---
   base::PairStream* plist{getPlayers()};
   if (plist != nullptr) {

      // Send shutdown to all players
      base::IList::Item* item{plist->getFirstItem()};
      while (item != nullptr) {
         base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
         base::IComponent* p{static_cast<base::IComponent*>(pair->object())};
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }

      // cleanup
      plist->unref();
      plist = nullptr;
   }

   // ---
   // Tell everyone on our 'new' player list, and remove
   // them from the list
   // ---
   base::Pair* newPlayer{newPlayerQueue.get()};
   while (newPlayer != nullptr) {
      base::IComponent* p{static_cast<base::IComponent*>(newPlayer->object())};
      p->event(SHUTDOWN_EVENT);
      newPlayer->unref();
      newPlayer = newPlayerQueue.get();
   }

   // ---
   // Shut down the thread pools
   // ---
   if (numTcThreads > 0) {
      for (int i = 0; i < numTcThreads; i++) {
         // We're just going to make sure the threads not suspended,
         // and they'll check our shutdown flag.
         tcThreads[i]->signalStart();
      }
   }
   if (numBgThreads > 0) {
      for (int i = 0; i < numBgThreads; i++) {
         // We're just going to make sure the threads not suspended,
         // and they'll check our shutdown flag.
         bgThreads[i]->signalStart();
      }
   }

   return true;
}

//------------------------------------------------------------------------------
// updateTC() -- update time critical stuff here
//------------------------------------------------------------------------------
void ISimulation::updateTC(const double dt)
{
   // ---
   // Process frame timing
   // ---
   if (isFrameTimingEabled()) {

      bool printIt{ false };

#if defined(WIN32)
      LARGE_INTEGER cFreq;
      QueryPerformanceFrequency(&cFreq);
      auto freq = static_cast<double>(cFreq.QuadPart);
      LARGE_INTEGER fcnt;
      QueryPerformanceCounter(&fcnt);
      const auto endCnt = static_cast<double>(fcnt.QuadPart);
      if (tcLastFrameTime > 0.0 && cycle() > 0) {
         double dcnt(endCnt - tcLastFrameTime);
         const auto dtime = (dcnt / freq) * 1000.0; // Delta time in MS
         frameTimingStats->sigma(dtime);
         printIt = isPrintFrameTimingEnabled();
      }
      tcLastFrameTime = endCnt;
#else
      double curTime{base::getComputerTime()};
      if (tcLastFrameTime > 0.0 && cycle() > 0) {
         const auto dtime =(curTime - tcLastFrameTime) * 1000.0; // delta time in ms
         frameTimingStats->sigma(dtime);
         printIt = isPrintFrameTimingEnabled();
      }
      tcLastFrameTime = curTime;
#endif

      if (printIt) printFrameTimingStats();
   }

   // ---
   // Update the executive time
   // ---
   execTime += dt;

   // ---
   // Update the computer's UTC time and date,
   // and set the current time of day
   // ---
   {
      unsigned long newPcTvUSec {pcTvUSec + static_cast<unsigned long>(dt * 1000000.0 + 0.5)};
      // while loop is used instead of if statement in case dt > 1 sec (i.e., tcRate < 1 Hz)
      while (newPcTvUSec >= 1000000) {
         newPcTvUSec = newPcTvUSec - 1000000;
         pcTvSec++;
      }
      pcTvUSec = newPcTvUSec;
      //getTime(&pcTvSec, &pcTvUSec); /* test */

      unsigned int cYear{};
      unsigned int cMonth{};
      unsigned int cDay{};
      unsigned int cHour{};
      unsigned int cMin{};
      unsigned int cSec{};
      base::convertSec2Ymdhms(pcTvSec, &cYear, &cMonth, &cDay, &cHour, &cMin, &cSec);

      // Computer time of day (seconds since midnight)
      pcTime = ( cHour * 3600.0 + cMin * 60.0 + cSec + static_cast<double>(pcTvUSec) / 1000000.0 );

      //std::printf("ComputerTimes: y=%d; m=%d; d=%d; h=%d; m=%d; s=%d, us=%06d\n", cYear, cMonth, cDay, cHour, cMin, cSec, pcTvUSec);
      //std::printf("PC sec = %d\n", pcTvSec);
      //std::printf("PC Time = %f\n", pcTime);
   }

   // ---
   // Delta-Time (Frozen?)
   // ---
   double dt0 {dt};
   if (isFrozen()) dt0 = 0.0;

   // ---
   // Update simulated time
   // ---
   {
      // If slaved then the simulated time can not be frozen
      double deltaTime{dt0};
      if (simTimeSlaved) deltaTime = dt;

      unsigned long newSimTvUSec{simTvUSec + static_cast<unsigned long>(deltaTime * 1000000.0 + 0.5)};
      // while loop is used instead of if statement in case dt > 1 sec (i.e., tcRate < 1 Hz)
      while (newSimTvUSec >= 1000000) {
         newSimTvUSec = newSimTvUSec - 1000000;
         simTvSec++;
      }

      simTvUSec = newSimTvUSec;
   }

   // compute the simulated time of day
   {
      unsigned int cYear{};
      unsigned int cMonth{};
      unsigned int cDay{};
      unsigned int cHour{};
      unsigned int cMin{};
      unsigned int cSec{};
      base::convertSec2Ymdhms(simTvSec, &cYear, &cMonth, &cDay, &cHour, &cMin, &cSec);

      // Computer time of day (seconds since midnight)
      simTime = ( cHour * 3600.0 + cMin * 60.0 + cSec + static_cast<double>(simTvUSec) / 1000000.0 );

      //std::printf("SimTimes: y=%d; m=%d; d=%d; h=%d; m=%d; s=%d, us=%06d\n", cYear, cMonth, cDay, cHour, cMin, cSec, simTvUSec);
      //std::printf("Sim sec = %d\n", simTvSec);
      //std::printf("simTime = %f\n", simTime);
   }

   // ---
   // Update base classes stuff
   // ---
   BaseClass::updateTC(dt0);

   // ---
   // Called once per frame -- Process 4 phases per frame
   // ---
   {
      // This locks the current player list for this time-critical frame
      base::safe_ptr<base::PairStream> currentPlayerList = players;

      for (unsigned int f = 0; f < 4; f++) {

         // Set the current phase
         setPhase(f);

         if (reqTcThreads == 1) {
            // Our single TC thread
            updateTcPlayerList(currentPlayerList, (dt0/4.0), 1, 1);
         } else if (numTcThreads > 0) {
            // multiple threads
            for (unsigned short i = 0; i < numTcThreads; i++) {

               // assign the threads from the pool
               unsigned int idx {static_cast<unsigned int>(i+1)};
               tcThreads[i]->start0(currentPlayerList, (dt0/4.0), idx, reqTcThreads);
            }

            // we're the last thread
            updateTcPlayerList(currentPlayerList, (dt0/4.0), reqTcThreads, reqTcThreads);

            // Now wait for the other thread(s) to complete
            base::ISyncThread** pp {reinterpret_cast<base::ISyncThread**>(&tcThreads[0])};
            base::ISyncThread::waitForAllCompleted(pp, numTcThreads);

         } else if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "simulation::updateTC() ERROR, invalid T/C thread setup";
            std::cerr << "; reqTcThreads = " << reqTcThreads;
            std::cerr << "; numTcThreads = " << numTcThreads;
            std::cerr << std::endl;
         }
      }
   }

   // Update frame & cycle counts
   int cframe{static_cast<int>(frame() + 1)};
   if (cframe >= 16) {
      incCycle();
      cframe = 0;
   }
   setFrame(cframe);
   setPhase(0);
}

//------------------------------------------------------------------------------
// Time critical thread processing for every n'th player starting
// with the idx'th player
//------------------------------------------------------------------------------
void ISimulation::updateTcPlayerList(
   base::PairStream* const playerList,
   const double dt,
   const unsigned int idx,
   const unsigned int n)
{
   if (playerList != nullptr) {
      unsigned int index{idx};
      unsigned int count{};
      base::IList::Item* item {playerList->getFirstItem()};
      while (item != nullptr) {
         count++;
         if (count == index) {
            base::Pair* pair {static_cast<base::Pair*>(item->getValue())};
            IPlayer* ip {static_cast<IPlayer*>(pair->object())};
            ip->tcFrame(dt);
            index += n;
         }
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// updateData() -- update non-time critical stuff here
//------------------------------------------------------------------------------
void ISimulation::updateData(const double dt)
{
    // Delta-Time (Frozen?)
    double dt0{dt};
    if (isFrozen()) dt0 = 0.0;

    // Update base classes stuff
    BaseClass::updateData(dt0);

    // Update the player list
    updatePlayerList();

    // Update all players
    if (players != nullptr) {
         base::safe_ptr<base::PairStream> currentPlayerList = players;

         if (reqBgThreads == 1) {
            // Our single thread
            updateBgPlayerList(currentPlayerList, dt0, 1, 1);
         } else if (numBgThreads > 0) {
            // multiple threads
            for (int i = 0; i < numBgThreads; i++) {
               // assign the threads from the pool
               unsigned int idx {static_cast<unsigned int>(i+1)};
               bgThreads[i]->start0(currentPlayerList, dt0, idx, reqBgThreads);
            }

            // we're the last thread
            updateBgPlayerList(currentPlayerList, dt0, reqBgThreads, reqBgThreads);

            // Now wait for the other thread(s) to complete
            base::ISyncThread** pp = reinterpret_cast<base::ISyncThread**>(&bgThreads[0]);
            base::ISyncThread::waitForAllCompleted(pp, numBgThreads);

         } else if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "Simulation::updateData() ERROR, invalid background thread setup";
            std::cerr << "; reqBgThreads = " << reqBgThreads;
            std::cerr << "; numBgThreads = " << numBgThreads;
            std::cerr << std::endl;
        }
    }
}

//------------------------------------------------------------------------------
// Background thread processing for every n'th player starting
// with the idx'th player
//------------------------------------------------------------------------------
void ISimulation::updateBgPlayerList(
         base::PairStream* const playerList,
         const double dt,
         const unsigned int idx,
         const unsigned int n)
{
   if (playerList != nullptr) {
      unsigned int index{idx};
      unsigned int count{};
      base::IList::Item* item{playerList->getFirstItem()};
      while (item != nullptr) {
         count++;
         if (count == index) {
         base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
            IPlayer* ip{static_cast<IPlayer*>(pair->object())};
            ip->updateData(dt);
            index += n;
         }
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// printFrameTimingStats() --Print the time critical frame timing statistics
//------------------------------------------------------------------------------
void ISimulation::printFrameTimingStats()
{
   const base::Statistic* ts{getFrameTimingStats()};
   int c{static_cast<int>(cycle())};
   int f{static_cast<int>(frame() - 1)};
   if (f < 0) {
      c--;
      f = 15;
   }
   std::cout << "frame timing(" << c << "," << f << "): dt=" << ts->value() << ", ave=" << ts->mean() << ", max=" << ts->maxValue() << ", min=" << ts->minValue() << std::endl;
}

//------------------------------------------------------------------------------
// printTimingStats() -- Update time critical stuff here
//------------------------------------------------------------------------------
void ISimulation::printTimingStats()
{
   const base::Statistic* ts{getTimingStats()};
   int c{static_cast<int>(cycle())};
   int f{static_cast<int>(frame() - 1)};
   if (f < 0) {
      c--;
      f = 15;
   }
   std::cout << "Simulation(" << c << "," << f << "): dt=" << ts->value() << ", ave=" << ts->mean() << ", max=" << ts->maxValue() << std::endl;
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Returns the player list
base::PairStream* ISimulation::getPlayers()
{
   return players.getRefPtr();
}

// Returns the player list (const version)
const base::PairStream* ISimulation::getPlayers() const
{
   return players.getRefPtr();
}

// Real-time cycle counter
unsigned int ISimulation::cycle() const
{
   return cycleCnt;
}

// Real-time frame counter [0 .. 15]
unsigned int ISimulation::frame() const
{
   return frameCnt;
}

// Real-time phase counter [0 .. 3]
unsigned int ISimulation::phase() const
{
   return phaseCnt;
}

// Returns the exec counter (R/T phases since start)
unsigned int ISimulation::getExecCounter() const
{
   return ((cycleCnt << 6) + (frameCnt << 2) + phaseCnt);
}

// Returns executive time, which is time since start (sec)
double ISimulation::getExecTimeSec() const
{
   return execTime;
}

// Returns computer systems time of day (UTC -- seconds since midnight)
double ISimulation::getSysTimeOfDay() const
{
   return pcTime;
}

// Returns the simulated time of day (UTC -- seconds since midnight)
double ISimulation::getSimTimeOfDay() const
{
   return simTime;
}

// Simulated time (UTC) values, where ...
void ISimulation::getSimTimeValues(
      unsigned long* const simSec,  // (OUT) The whole seconds since midnight (00:00:00), January 1, 1970
      unsigned long* const simUSec  // (OUT) The number of microseconds in the current second.
   ) const
{
   if (simSec != nullptr) *simSec = simTvSec;
   if (simUSec != nullptr) *simUSec = simTvUSec;
}

// Generates an unique major simulation event ID [1 .. 65535]
unsigned short ISimulation::getNewEventID()
{
   return ++eventID;
}

// Generates a unique weapon event ID [1 .. 65535]
unsigned short ISimulation::getNewWeaponEventID()
{
   return ++eventWpnID;
}

// Generates a unique ID number for released weapons
unsigned short ISimulation::getNewReleasedWeaponID()
{
   return relWpnId++;
};

// Returns the data recorder
IDataRecorder* ISimulation::getDataRecorder()
{
   IDataRecorder* p{};
   Station* sta{getStation()};
   if (sta != nullptr) p = sta->getDataRecorder();
   return p;
}

// Our Station
Station* ISimulation::getStation()
{
   if (station == nullptr) {
      getStationImp();
   }
   return station;
}

// Our Station (const version)
const Station* ISimulation::getStation() const
{
   if (station == nullptr) {
      (const_cast<ISimulation*>(this))->getStationImp();
   }
   return station;
}

Station* ISimulation::getStationImp()
{
   if (station == nullptr) {
      station = static_cast<Station*>(findContainerByType(typeid(Station)));
      if (station == nullptr && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "ISimulation::getStationImp(): ERROR, unable to locate the Station class!" << std::endl;
      }
   }
   return station;
}

//------------------------------------------------------------------------------
// setSlotPlayers() -- set the original player list (make sure we have only
// player type objects with unique names and IDs)
//------------------------------------------------------------------------------
bool ISimulation::setSlotPlayers(base::PairStream* const pl)
{
   // Early out if we're just zeroing the player lists
   if (pl == nullptr) {
      origPlayers = nullptr;
      players = nullptr;
      return true;
   }

   bool ok{true};
   unsigned short maxID{};

   // First, make sure they are all Players.
   {
      base::IList::Item* item{pl->getFirstItem()};
      while (item != nullptr && ok) {
         base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
         item = item->getNext();
         const auto ip = dynamic_cast<IPlayer*>( pair->object() );
         if (ip == nullptr) {
            // Item is NOT a Player
            std::cerr << "Simulation::setSlotPlayers: slot: " << pair->slot() << " is NOT of a Player type!" << std::endl;
            ok = false;
         }
         else {
            // find the max ID# of players with preassigned IDs
            if (ip->getID() > maxID)
               maxID = ip->getID();
         }
      }
   }

   // increment to next available ID
   maxID++;

   // Next, make sure we have unique player names and IDs
   if (ok) {
      // For all players ...
      base::IList::Item* item1{pl->getFirstItem()};
      while (item1 != nullptr) {
         base::Pair* pair1{static_cast<base::Pair*>(item1->getValue())};
         item1 = item1->getNext();
         IPlayer* ip1{static_cast<IPlayer*>(pair1->object())};

         // unassigned ID
         if ( (ip1->getID() == 0) && (maxID < 65535) ) {
            ip1->setID(maxID);
            ++maxID;
         }

         base::IList::Item* item2{item1};
         while (item2 != nullptr) {
            base::Pair* pair2{static_cast<base::Pair*>(item2->getValue())};
            IPlayer* ip2{static_cast<IPlayer*>(pair2->object())};

            // unassigned ID
            if ( (ip2->getID() == 0)  && (maxID < 65535) ) {
               ip2->setID(maxID);
               ++maxID;
            }

            if (ip1->getID() == ip2->getID()) {
               std::cerr << "Simulation::setSlotPlayers: duplicate player ID: " << ip1->getID() << std::endl;
               ok = false;
            }

            if (pair1->slot() == pair2->slot()) {
               std::cerr << "Simulation::setSlotPlayers: duplicate player name: " << pair1->slot() << std::endl;
               ok = false;
            }

            item2 = item2->getNext();
         }

      }
   }

   // Next, set the container pointer, set the player's name
   // and setup the player lists.
   if (ok) {
      base::IList::Item* item{pl->getFirstItem()};
      while (item != nullptr) {
         base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
         item = item->getNext();
         const auto ip = static_cast<IPlayer*>(pair->object());
         ip->container(this);
         ip->setName(pair->slot());
      }

      // Set the original player list pointer
      origPlayers = pl;

      // Create the new active player list
      base::PairStream* newList{new base::PairStream()};

      // Copy original players to the new list
      if (origPlayers != nullptr) {
         base::safe_ptr<base::PairStream> origPlayerList = origPlayers;
         base::IList::Item* item {origPlayerList->getFirstItem()};
         while (item != nullptr) {
            base::Pair* pair {static_cast<base::Pair*>(item->getValue())};
            insertPlayerSort(pair, newList);
            item = item->getNext();
         }
      }

      // Set the active player list pointer
      players = newList;
      newList->unref();
   }

   return ok;
}

//------------------------------------------------------------------------------
// updatePlayerList() -- update the player list ...
//                       1) remove 'deleteRequest' mode players
//                       2) add new players
//------------------------------------------------------------------------------
void ISimulation::updatePlayerList()
{
    // ---
    // Do we need to swap player lists?  Only if a player
    // needs to be added or removed ---
    // ---

    // First check for new players ...
    bool yes{newPlayerQueue.isNotEmpty()};

    // Second, check for delete requests
    if (!yes) {
        base::safe_ptr<base::PairStream> pl = players;
        base::IList::Item* item{pl->getFirstItem()};
        while (!yes && item != nullptr) {
            base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
            const auto p = static_cast<IPlayer*>(pair->object());
            yes = p->isMode(IPlayer::Mode::DELETE_REQUEST);
            item = item->getNext();
        }
    }

    if (yes) {

        // Update Required!

        // ---
        // Something old and something new ...
        // ---
        base::safe_ptr<base::PairStream> newList( new base::PairStream() );
        newList->unref();  // 'newList' has it, so unref() from the 'new'

        // ---
        // Copy players to the new list; except 'deleteRequest' mode players
        // ---
        base::safe_ptr<base::PairStream> oldList = players;
        base::IList::Item* item{oldList->getFirstItem()};
        while (item != nullptr) {
            base::Pair* pair{static_cast<base::Pair*>(item->getValue())};
            item = item->getNext();
            const auto p = static_cast<IPlayer*>(pair->object());
            if (p->isNotMode(IPlayer::Mode::DELETE_REQUEST)) {
                // Add the player to the new list
                newList->put(pair);
            } else {
                // Deleting this player: remove us as its container
                // and don't add to the new player list
                p->container(nullptr);

                BEGIN_RECORD_DATA_SAMPLE( getDataRecorder(), REID_PLAYER_REMOVED )
                   SAMPLE_1_OBJECT( p )
                END_RECORD_DATA_SAMPLE()
            }
        }

        // ---
        // Add any new players
        // ---
        base::Pair* newPlayer{newPlayerQueue.get()};
        while (newPlayer != nullptr) {
            // get the player
            const auto ip = static_cast<IPlayer*>(newPlayer->object());

            BEGIN_RECORD_DATA_SAMPLE( getDataRecorder(), REID_NEW_PLAYER )
               SAMPLE_1_OBJECT( ip )
            END_RECORD_DATA_SAMPLE()

            // Set container and name
            ip->container(this);
            ip->setName(newPlayer->slot());

            // Insert the new player into the new list in sorted order
            insertPlayerSort(newPlayer, newList);

            newPlayer->unref();

         newPlayer = newPlayerQueue.get();
        }

        // ---
        // Swap the lists
        // ---
        players = newList;
    }
}

//------------------------------------------------------------------------------
// addNewPlayer() -- add a new player by name and player object; the new
//                   player is added to the player list at the start of
//                   the next frame.  Returns true of player will be added
//                   or false if there is an error.
//------------------------------------------------------------------------------
bool ISimulation::addNewPlayer(base::Pair* const player)
{
    if (player == nullptr) return false;
    player->ref();

    newPlayerQueue.put(player);

    return true;
}

//------------------------------------------------------------------------------
// addNewPlayer() -- add a new player by name and player object; the new
//                   player is added to the player list at the start of
//                   the next frame.  Returns true of player will be added
//                   or false if there is an error.
//------------------------------------------------------------------------------
bool ISimulation::addNewPlayer(const char* const playerName, IPlayer* const player)
{
    if (playerName == nullptr || player == nullptr) return false;

    const auto pair = new base::Pair(playerName, player);
    bool ok{addNewPlayer(pair)};
    pair->unref();
    return ok;
}

//------------------------------------------------------------------------------
// insertPlayerSort() -- Insert the new player into the new list in sorted order
//------------------------------------------------------------------------------
bool ISimulation::insertPlayerSort(base::Pair* const newPlayerPair, base::PairStream* const newList)
{
    newList->ref();

    // create a new base::List::Item to hold the player
    base::IList::Item* newItem{new base::IList::Item};
    newPlayerPair->ref();
    newItem->value = newPlayerPair;

    // Get the player
    const auto newPlayer = static_cast<IPlayer*>(newPlayerPair->object());

    // Search the new player list and insert into the correct position --
    //  -- sorted by network ID and player ID
    bool inserted{};
    base::IList::Item* refItem{newList->getFirstItem()};
    while (refItem != nullptr && !inserted) {
        base::Pair* refPair{static_cast<base::Pair*>(refItem->getValue())};
        const auto refPlayer = static_cast<IPlayer*>(refPair->object());

        bool insert{};
        if (newPlayer->isProxyPlayer()) {

            // *** Insert proxy player -- after local players and lower NIB IDs first
            if (refPlayer->isProxyPlayer()) {

               // Get the NIBs
               const INib* nNib{newPlayer->getNib()};
               const INib* rNib{refPlayer->getNib()};

               // Compare federate names
               int result{nNib->getFederateName() == rNib->getFederateName()};
               if (result == 0) {
                  // Same federate name; compare player IDs
                  if (nNib->getPlayerID() > rNib->getPlayerID()) result = +1;
                  else if (nNib->getPlayerID() < rNib->getPlayerID()) result = -1;
               }

               // Insert if the new NIB's IDs are less than the ref NIB's IDs
               insert = (result < 0);
            }
        } else {

            // *** Insert a local player -- by player ID and before any proxy player
            insert = ( (newPlayer->getID() < refPlayer->getID()) || refPlayer->isProxyPlayer() );

        }

        if (insert) {
            newList->insert(newItem, refItem);
            inserted = true;
        }

        refItem = refItem->getNext();
    }

    // Hasn't been inserted yet, so add to the tail
    if (!inserted) {
        newList->insert(newItem, nullptr);
    }

    newList->unref();
    return true;
}


//------------------------------------------------------------------------------
// findPlayer() -- Find a player that matches 'id' and 'networkID'
//------------------------------------------------------------------------------
IPlayer* ISimulation::findPlayer(const short id, const int netID)
{
   return findPlayerPrivate(id, netID);
}

const IPlayer* ISimulation::findPlayer(const short id, const int netID) const
{
   return findPlayerPrivate(id, netID);
}

IPlayer* ISimulation::findPlayerPrivate(const short id, const int netID) const
{
    // Quick out
    if (players == nullptr) return nullptr;

    // Find a Player that matches player ID and Sources
    IPlayer* player{};
    const base::IList::Item* item{players->getFirstItem()};
    while (player == nullptr && item != nullptr) {
        const auto pair = static_cast<const base::Pair*>(item->getValue());
        if (pair != nullptr) {
            const auto ip = const_cast<IPlayer*>(static_cast<const IPlayer*>(pair->object()));
            if (ip != nullptr) {
                if (netID > 0) {
                    if ((ip->getID() == id) && (ip->getNetworkID() == netID)) {
                        player = ip;
                    }
                } else {
                    if (ip->getID() == id) {
                        player = ip;
                    }
                }
            }
        }
        item = item->getNext();
    }

    return player;
}

//------------------------------------------------------------------------------
// findPlayerByName() -- Find a player by name
//------------------------------------------------------------------------------
IPlayer* ISimulation::findPlayerByName(const char* const playerName)
{
   return findPlayerByNamePrivate(playerName);
}

const IPlayer* ISimulation::findPlayerByName(const char* const playerName) const
{
   return findPlayerByNamePrivate(playerName);
}

IPlayer* ISimulation::findPlayerByNamePrivate(const char* const playerName) const
{
    // Quick out
    if (players == nullptr || playerName == nullptr) return nullptr;

    // Find a Player named 'playerName'
    IPlayer* player{};
    const base::IList::Item* item{players->getFirstItem()};
    while (player == nullptr && item != nullptr) {
        const auto pair = static_cast<const base::Pair*>(item->getValue());
        if (pair != nullptr) {
            const auto ip = const_cast<IPlayer*>(static_cast<const IPlayer*>(pair->object()));
            if (ip != nullptr && ip->isName(playerName)) {
               player = ip;
            }
        }
        item = item->getNext();
    }

    return player;
}

//------------------------------------------------------------------------------
// Data set routines
//------------------------------------------------------------------------------

// Sets the initial simulation time (sec; or less than zero to slave to UTC)
bool ISimulation::setInitialSimulationTime(const long time)
{
   simTime0 = time;
   return true;
}

// Increment the cycle counter
void ISimulation::incCycle()
{
   cycleCnt++;
}

// Sets the cycle counter
void ISimulation::setCycle(const unsigned int c)
{
   cycleCnt = c;
}

// Sets the frame counter
void ISimulation::setFrame(const unsigned int f)
{
   frameCnt = f;
}

// Sets the phase counter
void ISimulation::setPhase(const unsigned int c)
{
   phaseCnt = c;
}

// Sets the simulation event ID counter
void ISimulation::setEventID(unsigned short id)
{
   eventID = id;
}

// Sets the weapon ID event counter
void ISimulation::setWeaponEventID(unsigned short id)
{
   eventWpnID = id;
}

bool ISimulation::setFrameTimingEnabled(const bool b)
{
   if (b) {
      // enable frame timing by creating a statistics object
      if (frameTimingStats != nullptr) {
         // already have one, just clear it
         frameTimingStats->clear();
      } else {
         frameTimingStats = new base::Statistic();
      }
   } else {
      // disable the frame timing
      if (frameTimingStats != nullptr) {
         frameTimingStats->unref();
         frameTimingStats = nullptr;
      }
   }
   tcLastFrameTime = 0.0;
   return true;
}

bool ISimulation::setPrintFrameTimingStats(const bool b)
{
   pfts = b;
   return true;
}

//------------------------------------------------------------------------------
// Set Slot routines
//------------------------------------------------------------------------------

bool ISimulation::setSlotSimulationTime(const base::ITime* const msg)
{
    bool ok{};
    if (msg != nullptr) {
       const long t{static_cast<long>(base::osg::round(msg->getValueInSeconds()))};
       if (t >= -1 && t < (60*60*24)) {
          ok = setInitialSimulationTime(t);
       } else {
         std::cerr << "Simulation::setSlotSimulationTime(): invalid time of day: " << t << " seconds; use [ 0 .. 86400 ] seconds or -1" << std::endl;
       }
    }
    return ok;
}

bool ISimulation::setSlotDay(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v {msg->asInt()};
      if (v >= 0 && v <= 31) {
         simDay0 = static_cast<unsigned short>(v);
         ok = true;
      } else {
         std::cerr << "Simulation::setSlotDay(): invalid day: " << v << "; use [ 1 .. 31 ] or zero" << std::endl;
      }
   }
   return ok;
}

bool ISimulation::setSlotMonth(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v{msg->asInt()};
      if (v >= 0 && v <= 12) {
         simMonth0 = static_cast<unsigned short>(v);
         ok = true;
      } else {
         std::cerr << "Simulation::setSlotMonth(): invalid month: " << v << "; use [ 1 .. 12 ] or zero" << std::endl;
      }
   }
   return ok;
}

bool ISimulation::setSlotYear(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v{msg->asInt()};
      if ((v >= 1970 && v <= 2099) || v == 0) {
         simYear0 = static_cast<unsigned short>(v);
         ok = true;
      } else {
         std::cerr << "Simulation::setSlotYear(): invalid year: " << v << "; use [ 1970 .. 2100 ] or zero" << std::endl;
      }
   }
   return ok;
}

bool ISimulation::setSlotFirstWeaponId(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v{msg->asInt()};
      if (v >= MIN_WPN_ID && v <= 0xffff) {
         relWpnId = static_cast<unsigned short>(v);
         ok = true;
      } else {
         std::cerr << "Simulation::setSlotFirstWeaponId(): invalid first weapon ID " << v;
         std::cerr << ", use [ " << MIN_WPN_ID << " ... 65535 ]." << std::endl;
      }
   }
   return ok;
}

bool ISimulation::setSlotNumTcThreads(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {

      // Max threads is the number of processors assigned to this
      // process minus one, or minimum of one.
      const int np{base::IThread::getNumProcessors()};
      int maxT{1};
      if (np > 1) maxT = np - 1;

      const int v{msg->asInt()};
      if (v >= 1 && v <= maxT) {
         reqTcThreads = v;
         ok = true;
      } else {
         std::cerr << "Simulation::setSlotNumTcThreads(): invalid number of threads: " << v;
         std::cerr << "; number of processors = " << np;
         std::cerr << "; use [ 1 ... " << maxT << " ];" << std::endl;
      }
   }
   return ok;
}

bool ISimulation::setSlotNumBgThreads(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {

      // Max threads is the number of processors assigned to this
      // process minus one, or minimum of one.
      const int np{base::IThread::getNumProcessors()};
      int maxT{1};
      if (np > 1) maxT = np - 1;

      const int v{msg->asInt()};
      if (v >= 1 && v <= maxT) {
         reqBgThreads = v;
         ok = true;
      } else {
         std::cerr << "Simulation::setSlotNumBgThreads(): invalid number of threads: " << v;
         std::cerr << "; number of processors = " << np;
         std::cerr << "; use [ 1 ... " << maxT << " ];" << std::endl;
      }
   }
   return ok;
}

bool ISimulation::setSlotEnableFrameTiming(const base::Boolean* const num)
{
   bool ok{};
   if (num != nullptr) {
      ok = setFrameTimingEnabled(num->asBool());
   }
   return ok;
}

bool ISimulation::setSlotPrintFrameTimingStats(const base::Boolean* const num)
{
   bool ok{};
   if (num != nullptr) {
      ok = setPrintFrameTimingStats(num->asBool());
   }
   return ok;
}

}
}

