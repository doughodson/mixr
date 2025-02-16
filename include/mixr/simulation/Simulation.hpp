
#ifndef __mixr_simulation_Simulation_HPP__
#define __mixr_simulation_Simulation_HPP__

#include "mixr/base/IComponent.hpp"
#include "mixr/base/safe_queue.hpp"
#include "mixr/base/osg/Matrixd"
#include <array>

namespace mixr {
namespace base { class EarthModel; class Integer; class LatLon; class Pair; class ITime; }
namespace simulation {
class IDataRecorder;
class SimulationBgSyncThread;
class SimulationTcSyncThread;
class Station;
class Statistic;
class IPlayer;

//------------------------------------------------------------------------------
// Class: Simulation
//
// Description: Simulation executive, a class to manage the execution of a world
//              model that contains a list of players
//
//
// Factory name: Simulation
//
// Slots --
//    players        <base::PairStream>       ! Local player list (base::PairStream of IPlayer) (default: nullptr)
//
//    simulationTime <base::Time>             ! Initial simulated time since midnight (UTC) (second),
//                                            ! or -1 to use current time of day (default: -1)
//
//    day            <base::Integer>          ! Initial simulated day of month [ 1 .. 31 ],
//                                            ! or zero to use current day (default: 0)
//
//    month          <base::Integer>          ! Initial simulated month [ 1 .. 12 ],
//                                            ! or zero to use current month (default: 0)
//
//    year           <base::Integer>          ! Initial simulated year [ 1970 .. 2100 ],
//                                            ! or zero to use current year (default: 0)
//
//    firstWeaponId  <base::Integer>          ! First Released Weapon ID; [ 10001 ... 65535 ] (default: 10001)
//
//    numTcThreads   <base::Integer>          ! Number of T/C threads to use with the player list
//                                            !   default: 1 -- no additional threads)
//                                            !   range: [ 1 .. (#CPUs-1) ]; minimum of one
//
//    numBgThreads   <base::Integer>          ! Number of background threads to use with the player list
//                                            !   default: 1 -- no additional threads)
//                                            !   range: [ 1 .. (#CPUs-1) ]; minimum of one
//
//    enableFrameTiming <base:Boolean>        ! Enable/disable the frame timing statistics (default: false)
//
//    printFrameTimingStats <base:Boolean>    ! Enable/disable the printing of the frame timing statistics (default: false)
//
// The player list
//
//    a) is managed by the background thread
//
//    b) You can request a new player to be added to the player list by
//       using addNewPlayer(), however the player will not be added until
//       updatePlayerList() is run in the background thread.
//
//    c) You can request a player to be removed from the list by setting
//       the player's mode to DELETE_REQUEST.  Again, the player will not
//       actually be removed until updatePlayerList() is run in the
//       background thread.
//
//    d) During reset(), the player list is restored to the initial players
//       that were defined by the 'players' slot, plus any active networked
//       players.
//
//    e) The Player list is sorted by networdID (zero being local) and then by
//       player ID, therefore all local players are located at the beginning of
//       the list.
//
//    f) To ensure a stable player list throughout the time-critical and background
//       frames, the player list is ref() before and unref() after the player list
//       is traversed in both the updateTC() and updateData() functions.
//
//    g) You can find players on the list by Player ID [plus Net ID], findPlayer(),
//       or by name using findPlayerByName().
//
//
// Cycles, frames and phases:
//
//    Each call to updateTC() by its manager (e.g., a Station class) is one
//    frame, and there are 16 frames, which are numbered 0 to 15, to one cycle.
//
//    Each frame is broken into 4 phases, which are numbered 0 to 3, and are used
//    to process dynamics, transmit sensor queries (e.g., emissions), receive sensor
//    queries, and information or control processing, respectively.  These phases
//    allow for the synchronized flow of data between players within a given frame.
//    The player list is traversed for each phase, therefore the player list is
//    traversed 4 times the core frame rate (e.g., 50 Hz frame rate is 200Hz phase
//    rate).
//
//    Use cycle(), frame() and phase() to get the current values, and use getExecCounter()
//    to get the total number of phases since the start of the exec.
//
//
// Multiple time critical and background threads:
//
//    Use the 'numTcThreads' and 'numBgThreads' slots, you can request the number of
//    threads to traverse the player list.  These threads will each process a subset
//    of players.  The T/C threads rejoin at the end of each phase (see phases above).
//
//    There is overhead with managing threads, so this is effective only with
//    a larger number of players.  The trade off point is dependent on the
//    complexity of the players and the speed of your computer system, so you
//    may need to do a little experimenting on your system.
//
//    These threads will be very CPU bound, so having more threads than CPUs is
//    very ineffective.  And to be nice, ...
//
//       Rule: Can not request more threads than the number of CPUs minus one.
//             This leaves a CPU for the operating system, other applications
//             and our other threads.
//
//
// Time and Date:
//
//    All time and date values are updated only at the start of each frame.
//
//    1) Computer System Time of Day:
//       -- Use getSysTimeOfDay() to get the current computer system's time of
//       day in seconds since midnight, UTC.
//
//    2) Executive time:
//       -- Use getExecTimeSec() to get the time in seconds since the start of
//       the application.
//
//    3) Simulated Time and Date:
//       a) Use getSimTimeOfDay() to get the simulated time of day in seconds
//       since midnight, UTC.
//
//       b) Use getSimTimeValues() to get the two time values ...
//          1) The whole seconds since midnight (00:00:00), January 1, 1970
//          2) The number of microseconds in the current second.
//
//       By default, the simulated time and date are slaved to the computer
//       system's time and date.  Use the slots 'simulationTime', 'day',
//       'month' and 'year' to override the computer system's time and/or
//       date with your own simulated time and/or date.
//
//    4) See "mixr/base/util/system.hpp" for additional time related functions.
//
//
// Event IDs:
//
//    There are several player events that need to be identified uniquely
//    within the simulation -- usually because of interoperability networks.
//       a) To identify major simulation events, use getNewEventID()
//       b) To identify weapon release/detonation events, use getNewWeaponEventID()
//       c) To uniquely set player IDs for newly released weapons, use getNewReleasedWeaponID()
//
//
// Shutdown:
//
//    At shutdown, the parent object must send a SHUTDOWN_EVENT event to
//    this object, which will send it to all players, and other components.
//
//------------------------------------------------------------------------------
class Simulation : public base::IComponent
{
    DECLARE_SUBCLASS(Simulation, base::IComponent)

public:
   // Minimum released weapon ID
   static const unsigned short MIN_WPN_ID{10001};

   // Size of the new player queue; ie, the max number
   // of new players accepted per background frame
   static const int MAX_NEW_PLAYERS{1000};

public:
    Simulation();

    base::PairStream* getPlayers();                // Returns the player list; pre-ref()'d
    const base::PairStream* getPlayers() const;    // Returns the player list; pre-ref()'d (const version)

    unsigned int cycle() const;                    // Cycle counter; each cycle represents 16 frames.
    unsigned int frame() const;                    // Frame counter [0 .. 15]; each frame represents a call to our updateTC()
    unsigned int phase() const;                    // Phase counter [0 .. 3]; frames are divide into 4 phases to help
                                                   // the sequencing of data within a  frame.
                                                   //    { 0::dynamics, 1::transmit, 2::receive, 3::process }

    unsigned int getExecCounter() const;           // Executive counter (R/T phases since start)
    double getExecTimeSec() const;                 // Executive time; time since start of simulation (seconds)
    double getSysTimeOfDay() const;                // Computer system's time of day; UTC (seconds since midnight)
    double getSimTimeOfDay() const;                // Simulated run time of day; UTC (seconds since midnight);

    void getSimTimeValues(                         // Simulated time (UTC) values, where ...
       unsigned long* const simSec,                //    The whole seconds since midnight (00:00:00), January 1, 1970
       unsigned long* const simUSec) const;        //    The number of microseconds in the current second.

    // Unique event and weapon IDs
    unsigned short getNewEventID();                // Generates an unique major simulation event ID [1 .. 65535]
    unsigned short getNewWeaponEventID();          // Generates a unique weapon event ID [1 .. 65535]
    unsigned short getNewReleasedWeaponID();       // Generates a unique ID number for released weapons

    IDataRecorder* getDataRecorder();              // Returns the data recorder

    Station* getStation();                         // Returns our Station
    const Station* getStation() const;             // Returns our Station (const version)

    IPlayer* findPlayer(const short id, const int netID = 0);                       // Find a player by player (and network) ID
    const IPlayer* findPlayer(const short id, const int netID = 0) const;           // Find a player by player (and network) ID (const version)

    IPlayer* findPlayerByName(const char* const playerName);                        // Find a player by name
    const IPlayer* findPlayerByName(const char* const playerName) const;            // Find a player by name (const version)

    virtual bool addNewPlayer(const char* const playerName, IPlayer* const player); // Add a new player
    virtual bool addNewPlayer(base::Pair* const player);                                   // Add a new player (pair: name, player)

    virtual bool setInitialSimulationTime(const long time);    // Sets the initial simulated time (sec; or less than zero to slave to UTC)

    // frame timing statistics
    const base::Statistic* getFrameTimingStats() const;        // Returns the timing statistics for the frames
    bool isFrameTimingEabled() const;                          // True if we're collecting frame timing
    bool isPrintFrameTimingEnabled() const;                    // True if we're printing the frame timing statistics

    void updateTC(const double dt = 0.0) override;
    void updateData(const double dt = 0.0) override;
    void reset() override;

public:
    void updateTcPlayerList(
       base::PairStream* const playerList,
       const double dt,
       const unsigned int idx,
       const unsigned int n
    );

    void updateBgPlayerList(
       base::PairStream* const playerList,
       const double dt,
       const unsigned int idx,
       const unsigned int n
    );

protected:
    virtual void updatePlayerList();                  // Updates the current player list

    virtual void incCycle();                          // Increments the cycle counter
    virtual void setCycle(const unsigned int c);      // Sets the cycle counter
    virtual void setFrame(const unsigned int f);      // Sets the frame counter
    virtual void setPhase(const unsigned int c);      // Sets the phase counter

    virtual void setEventID(unsigned short id);       // Sets the simulation event ID counter
    virtual void setWeaponEventID(unsigned short id); // Sets the weapon ID event counter

    virtual bool setFrameTimingEnabled(const bool);
    virtual bool setPrintFrameTimingStats(const bool);
    virtual void printFrameTimingStats();

    void printTimingStats() override;
    bool shutdownNotification() override;

private:
   Station* getStationImp();

   bool insertPlayerSort(base::Pair* const newPlayer, base::PairStream* const newList);
   IPlayer* findPlayerPrivate(const short id, const int netID) const;
   IPlayer* findPlayerByNamePrivate(const char* const playerName) const;

   base::safe_ptr<base::PairStream> players;     // Main player list (sorted by network and player IDs)
   base::safe_ptr<base::PairStream> origPlayers; // Original player list

   unsigned int cycleCnt{};      // Real-Time Cycle Counter (Cycles consist of Frames)
   unsigned int frameCnt{};      // Real-Time Frame Counter (Frames consist of Phases)
   unsigned int phaseCnt{};      // Real-Time Phase Counter

   double execTime{};            // Executive time (seconds since start of application )

   double pcTime{};              // Computer system UTC time of day (seconds since midnight)
   unsigned long pcTvSec{};      // Computer system UTC time value: Whole seconds since midnight (00:00:00), January 1, 1970
   unsigned long pcTvUSec{};     // Computer system UTC time value: Microseconds

   double simTime{};             // Simulated UTC time of day (seconds since midnight)
   unsigned long simTvSec{};     // Simulated UTC time value: Whole seconds since midnight (00:00:00), January 1, 1970
   unsigned long simTvUSec{};    // Simulated UTC time value: Microseconds
   bool simTimeSlaved{true};     // Simulated time is slaved to the computer time

   long simTime0{-1};            // Initial time of day since midnight (default: -1.0, which slaves to UTC time)
   unsigned short simDay0{};     // Initial day of the month [ 1 .. 31 ] (default: 0, which slaves to UTC time)
   unsigned short simMonth0{};   // Initial month [ 1 .. 12 ] (default: 0, which slaves to UTC time)
   unsigned short simYear0{};    // Initial year YYYY (default: 0, which slaves to UTC time)

   unsigned short eventID{};              // Major event ID
   unsigned short eventWpnID{};           // Weapon event ID
   unsigned short relWpnId{MIN_WPN_ID};   // Current released weapon ID

   base::safe_queue<base::Pair*> newPlayerQueue;   // Queue of new players

   Station* station{};                    // The Station that owns us (not ref()'d)

   // Time critical thread pool
   static const int MAX_TC_THREADS{32};
   std::array<SimulationTcSyncThread*, MAX_TC_THREADS> tcThreads{};   // Thread pool; 'numTcThreads' threads
   int reqTcThreads{1};                                               // Requested number of threads
   int numTcThreads{};                                                // Number of threads in pool; should be (reqTcThreads - 1)
   bool tcThreadsFailed{};                                            // Failed to create threads.

   // Background thread pool
   static const int MAX_BG_THREADS{32};
   std::array<SimulationBgSyncThread*, MAX_BG_THREADS> bgThreads{};   // Thread pool; 'reqBgThreads' threads
   int reqBgThreads{1};                                               // Requested number of threads
   int numBgThreads{};                                                // Number of threads in pool; should be (reqBgThreads - 1)
   bool bgThreadsFailed{};                                            // Failed to create threads.

   base::Statistic* frameTimingStats{};                               // Frame timing statistics
   double tcLastFrameTime{0.0};                                       // Previous frame time
   bool pfts{};                                                       // Print frame timing statistics

private:
   // slot table helper methods
   bool setSlotPlayers(base::PairStream* const);

   bool setSlotSimulationTime(const base::ITime* const);
   bool setSlotDay(const base::Integer* const);
   bool setSlotMonth(const base::Integer* const);
   bool setSlotYear(const base::Integer* const);

   bool setSlotFirstWeaponId(const base::Integer* const);

   bool setSlotNumTcThreads(const base::Integer* const);
   bool setSlotNumBgThreads(const base::Integer* const);
   bool setSlotEnableFrameTiming(const base::Boolean* const);
   bool setSlotPrintFrameTimingStats(const base::Boolean* const);
};

// Returns the timing statistics for the frames
inline const base::Statistic* Simulation::getFrameTimingStats() const
{
   return frameTimingStats;
}

// True if we're collecting frame timing
inline bool Simulation::isFrameTimingEabled() const
{
   return (frameTimingStats != nullptr);
}

// True if we're printing the frame timing statistics
inline bool Simulation::isPrintFrameTimingEnabled() const
{
   return pfts;
}

}
}

#endif
