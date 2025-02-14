
#ifndef __mixr_simulation_Station_HPP__
#define __mixr_simulation_Station_HPP__

#include "mixr/base/Component.hpp"

#include <string>

namespace mixr {
namespace base { class IIoHandler; class Boolean; class Identifier; class Integer;
                 class INumber; class PairStream; class String; class ITime; }
namespace simulation {
class IDataRecorder;
class Simulation;
class IPlayer;
class IIgHost;
class StationBgPeriodicThread;
class StationTcPeriodicThread;
class StationNetPeriodicThread;

//------------------------------------------------------------------------------
// Class: Station
//
// Description:  Application's top level container class with support for the
//               simulation model, controls & displays, network interfaces
//               and image generator systems.  Also, contains default
//               support for the time-critical and network threads.
//
// Factory name: Station
// Slots --
//    simulation         <Simulation>               ! Executable simulation model (default: nullptr)
//
//    networks           <base::PairStream>         ! List of interoperability network models (DIS, HLA, TENA) (default: nullptr)
//
//    igHosts            <base::PairStream>         ! List of Image Generator (IG) host interfaces
//
//    ioHandler          <base::AbstractIoHandler>  ! Input/Output (IO) data handler
//
//    ownship            <base::String>             ! Player name of our ownship (primary) player (default: nullptr)
//
//    tcRate             <base::INumber>            ! Time-critical thread rate (Hz) (default: 50hz)
//    tcPriority         <base::INumber>            ! Time-critical thread priority  (default: DEFAULT_TC_THREAD_PRI)
//    tcStackSize        <base::Integer>            ! Time-critical thread stack size (default: <system default size>)
//
//    fastForwardRate    <base::Integer>            ! Fast forward rate for time critical functions
//                                                  ! (i.e., the number of times updateTC() is called per frame).
//                                                  ! (default: DEFAULT_FAST_FORWARD_RATE)
//
//    netRate            <base::INumber>            ! Network thread rate (Hz) (default: 0hz)
//    netPriority        <base::INumber>            ! Network thread priority (default: DEFAULT_NET_THREAD_PRI )
//    netStackSize       <base::Integer>            ! Network thread stack size (default: <system default size>)
//
//    bgRate             <base::INumber>            ! Background thread rate (Hz) (default: 0 -- no thread)
//    bgPriority         <base::INumber>            ! Background thread priority (default: DEFAULT_BG_THREAD_PRI )
//    bgStackSize        <base::Integer>            ! Background thread stack size (default: <system default size>)
//
//    startupResetTime   <base::ITime>              ! Startup (initial) RESET event timer value (default: no reset event)
//                                                  !  (some simulations may need this -- let it run a few initial frames then reset)
//
//    enableUpdateTimers <base::Boolean>            ! Enable calling base::Timers::updateTimers() from updateTC() (default: false)
//
//    dataRecorder       <AbstractDataRecorder>     ! Our Data Recorder
//
//
// Ownship player:
//
//    The ownship player is the primary focus of our controls & displays (C&D).
//    It is set by the 'ownship' slot, but it can be changed using the
//    setOwnshipByName() function.  For example, a cockpit's C&D can be
//    focused on one local player then it can 'jump' (i.e., switched) to a
//    different local player.  If you're planning to jump between ownships then
//    ensure that your C&D software is written accordingly.
//
//    When the ownship player is changed, an ON_OWNSHIP_DISCONNECT event
//       is sent to the previous ownship player and then an ON_OWNSHIP_CONNECT
//       event is sent to the new ownship player.
//
//
// Threads and main work functions:
//
//    1) Currently supports the creation of three optional threads for (a) time-critical,
//       (b) interoperabilty networks and (c) background tasks.  These are created by
//       the virtual functions createTimeCriticalProcess(), createNetworkProcess()
//       and createBackgroundProcess(), respectively.  Users can derive from Station Class
//       and create their own threads (e.g., to sync any of the threads with an
//       external interrupt).
//
//    2) Thread priorities are from zero (lowest) to one (highest).
//       (see base/concurrent/Thread.hpp)
//
//    3) updateTC() -- The main application can use createTimeCriticalProcess()
//       to create a thread, which will run at 'tcRate' Hz and 'tcPriority'
//       priority, that will call our updateTC(); or the application can call
//       our updateTC() function directly.
//
//       a: The updateTC() function calls the updateTC() for the SimExec class,
//          IG interfaces, and updates the I/O handlers;
//
//       b: And updateTC() calls the static function base::Timer::updateTimers()
//          if isUpdateTimersEnabled() is true (i.e., slot 'enableUpdateTimers');
//
//       c: To sync hardware I/O with the simulation exec, the two functions,
//          inputDevices() and outputDevices(), are called from our updateTC()
//          just before and just after, respectively, the call to the simulation's
//          updateTC().  Any slow I/O (e.g., RS-232) or blocked I/O should not be
//          in these functions.
//
//    4) updateData() -- The main application will need to call updateData()
//       directly at its required (i.e., as needed) rate.  By default, this
//       function will call the simulation exec and interoperability network
//       updateData() functions.  However, ...
//
//       a: If the background thread rate, 'bgRate', is set greater than zero,
//          then our updateData() will create a separate thread to call the
//          simulation and IG models' updateData() functions, otherwise
//          they're called from our updateData().
//
//       b: The interoperabilty networks (e.g., DIS, HLA, TENA, etc) are updated
//          using the two functions, inputFrame() and outputFrame(), which are
//          called from the processNetworkInputTasks() and processNetworkOutputTasks()
//          functions, respectively.
//
//          If the network thread rate, 'netRate', is set greater than zero,
//          then our updateData() will create a separate thread to call the
//          interoperability network functions, otherwise they're called from
//          our updateData().
//
//    5) The time-critical and background functions for the IG host are
//       called from the same thread, or threads, as the simulation model's
//       functions are called from.
//
//    6) When subclassing off of this class for your application, the
//       convention is that the updateData() function for any graphic
//       component (see graphics::Graphic) is called by their display
//       manager (see graphics::GlutDisplay) and therefore from the
//       display manager's thread.
//
//
// Shutdown:
//
//    At shutdown, the user application must send a SHUTDOWN_EVENT event
//    to this object.
//
//------------------------------------------------------------------------------
class Station : public base::Component
{
   DECLARE_SUBCLASS(Station, base::Component)

public:
   // Default priorities and rates
   static const double DEFAULT_TC_THREAD_PRI;
   static const double DEFAULT_BG_THREAD_PRI;
   static const double DEFAULT_NET_THREAD_PRI;
   static const unsigned int DEFAULT_FAST_FORWARD_RATE{1};

public:
   Station();

   Simulation* getSimulation();                                     // Simulation executive
   const Simulation* getSimulation() const;                         // Simulation executive (const version)

   base::PairStream* getPlayers();                                  // Player list; pre-ref()'d
   const base::PairStream* getPlayers() const;                      // Player list; pre-ref()'d (const version)

   IPlayer* getOwnship();                                           // The ownship (primary) player
   const IPlayer* getOwnship() const;                               // The ownship (primary) player (const version)

   const std::string& getOwnshipName() const;                       // The ownship's name
   virtual bool setOwnshipPlayer(IPlayer* const newOS);             // Sets the ownship player
   virtual bool setOwnshipByName(const char* const newOS);          // Selects the ownship player by name

   base::PairStream* getIgHostList();                               // Image generator host interfaces
   const base::PairStream* getIgHostList() const;                   // Image generator host interfaces (const version)

   base::PairStream* getNetworks();                                 // Interoperability network handlers
   const base::PairStream* getNetworks() const;                     // Interoperability network handlers (const version)

   base::IIoHandler* getIoHandler();                                // I/O handler
   const base::IIoHandler* getIoHandler() const;                    // I/O handler (const version)

   IDataRecorder* getDataRecorder();                                // Returns the data recorder
   const IDataRecorder* getDataRecorder() const;                    // Returns the data recorder (const version)
   virtual bool setDataRecorder(IDataRecorder* const p);            // Sets the data recorder

   // Is Timer::updateTimers() being called from our updateTC()
   bool isUpdateTimersEnabled() const;
   virtual bool setUpdateTimersEnable(const bool enb);

   // ---
   // Use these functions to process the time-critical, background and network
   // tasks if you're managing your own thread(s) from your main application
   // or a derived Station class.
   // ---
   virtual void processTimeCriticalTasks(const double dt);
   virtual void processBackgroundTasks(const double dt);
   virtual void processNetworkInputTasks(const double dt);
   virtual void processNetworkOutputTasks(const double dt);

   // ---
   // Time-critical thread support
   // ---
   double getTimeCriticalRate() const;                       // Time-critical thread rate (Hz)
   double getTimeCriticalPriority() const;                   // Time-critical thread priority
   unsigned int getTimeCriticalStackSize() const;            // Time-critical thread stack size
   bool setTimeCriticalStackSize(const unsigned int bytes);  // Set Time-critical thread stack size  (bytes or zero for default)

   // Optionally called by the main application  to create a thread
   // that will call 'updateTC()' at 'getTimeCriticalRate()' Hz
   virtual void createTimeCriticalProcess();
   bool doWeHaveTheTcThread() const;                         // Do we have a T/C thread?

   // Fast forward rates used by processTimeCriticalTasks().
   //   (i.e., number of times Station::tcFrame() is called per frame)
   unsigned int getFastForwardRate() const { return fastForwardRate; } // Hz
   virtual bool setFastForwardRate(const unsigned int r);              // Hz

   // ---
   // Interoperability network(s) thread support
   // ---
   double getNetworkRate() const;                            // Network thread rate (Hz)
   double getNetworkPriority() const;                        // Network thread priority
   unsigned int getNetworkStackSize() const;                 // Network thread stack size
   bool setNetworkStackSize(const unsigned int bytes);       // Network thread stack size (bytes or zero for default)
   virtual void createNetworkProcess();                      // Creates a network thread
   bool doWeHaveTheNetThread() const;                        // Do we have a network thread?

   // ---
   // Background thread support.
   // ---
   double getBackgroundRate() const;                         // Background thread rate (Hz)
   double getBackgroundPriority() const;                     // Background thread priority
   unsigned int getBackgroundStackSize() const;              // Background thread stack size
   bool setBackgroundStackSize(const unsigned int bytes);    // Background thread stack size (bytes or zero for default)
   virtual void createBackgroundProcess();                   // Creates a B/G thread
   bool doWeHaveTheBgThread() const;                         // Do we have a background thread?

   void updateTC(const double dt = 0.0) override;
   void updateData(const double dt = 0.0) override;
   void reset() override;

protected:
   virtual void inputDevices(const double dt);    // Handle device inputs
   virtual void outputDevices(const double dt);   // Handle device output

   // base::Component protected functions
   bool shutdownNotification() override;

private:
   void setTcThread(StationTcPeriodicThread*);
   void setNetThread(StationNetPeriodicThread*);
   void setBgThread(StationBgPeriodicThread*);

   Simulation* sim{};                                        // Executable simulation model
   base::safe_ptr<base::PairStream> networks;                // List of networks
   base::safe_ptr<base::PairStream> igHosts;                 // List of Image generator (IG) host interfaces
   base::safe_ptr<base::IIoHandler> ioHandler;               // Input/Output (IO) data handler
   IPlayer* ownship{};                                       // Ownship (primary) player
   std::string ownshipName;                                  // Name of our ownship player
   bool tmrUpdateEnbl{};                                     // Enable base::Timers::updateTimers() call from updateTC()
   IDataRecorder* dataRecorder{};                            // Data Recorder

   double tcRate{50.0};                                      // Time-critical thread Rate (hz)
   double tcPri{DEFAULT_TC_THREAD_PRI};                      // Priority of the time-critical thread (0->lowest, 1->highest)
   unsigned int tcStackSize{};                               // Time-critical thread stack size (bytes or zero for system default size)
   base::safe_ptr<StationTcPeriodicThread> tcThread;         // The Time-critical thread
   unsigned int fastForwardRate{DEFAULT_FAST_FORWARD_RATE};  // Time-critical thread fast forward rate

   double netRate{};                                         // Network thread Rate (hz)
   double netPri{DEFAULT_NET_THREAD_PRI};                    // Priority of the Network thread (0->lowest, 1->highest)
   unsigned int netStackSize{};                              // Network thread stack size (bytes or zero for system default size)
   base::safe_ptr<StationNetPeriodicThread> netThread;       // The optional network thread

   double bgRate{};                                          // Background thread Rate (hz)
   double bgPri{DEFAULT_BG_THREAD_PRI};                      // Priority of the Background thread (0->lowest, 1->highest)
   unsigned int bgStackSize{};                               // Background thread stack size (bytes or zero for system default size)
   base::safe_ptr<StationBgPeriodicThread> bgThread;         // The optional background thread

   double startupResetTimer{-1.0};                           // Startup RESET timer (sends a RESET_EVENT after timeout)
   const base::ITime* startupResetTimer0{};                  // Init value of the startup RESET timer

private:
   // slot table helper methods
   bool setSlotSimulation(Simulation* const);

   bool setSlotNetworks(base::PairStream* const);

   bool setSlotIgHosts(base::PairStream* const);

   bool setSlotIoHandler(base::IIoHandler* const);

   bool setSlotOwnshipName(const base::Identifier* const);

   bool setSlotTimeCriticalRate(const base::INumber* const hz);
   bool setSlotTimeCriticalPri(const base::INumber* const);
   bool setSlotTimeCriticalStackSize(const base::Integer* const);
   bool setSlotFastForwardRate(const base::Integer* const);

   bool setSlotNetworkRate(const base::INumber* const hz);
   bool setSlotNetworkPri(const base::INumber* const);
   bool setSlotNetworkStackSize(const base::Integer* const);

   bool setSlotBackgroundRate(const base::INumber* const hz);
   bool setSlotBackgroundPri(const base::INumber* const);
   bool setSlotBackgroundStackSize(const base::Integer* const);

   bool setSlotStartupResetTime(const base::ITime* const);
   bool setSlotEnableUpdateTimers(const base::Boolean* const);

   bool setSlotDataRecorder(IDataRecorder* const x)                     { return setDataRecorder(x); }
};

}
}

#endif
