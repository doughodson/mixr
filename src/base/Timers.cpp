
#include "mixr/base/Timers.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/units/times.hpp"
#include "mixr/base/util/atomics.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Timer, "Timer")

bool Timer::frz{};                    // Freeze flag
Timer* Timer::timers[MAX_TIMERS]{};   // List of timers
int Timer::nTimers{};                 // Number of timers in the list
long Timer::semaphore{};              // Semaphore for the timer list

BEGIN_SLOTTABLE(Timer)
   "timerValue",         // 1: Timer interval (default: 0)
   "alarmTime",          // 2: Alarm time (default: 0)
   "active",             // 3: Sets timer active (running) flag (default: false)
END_SLOTTABLE(Timer)

BEGIN_SLOT_MAP(Timer)
   ON_SLOT(1, setSlotTimerValue,  ITime)
   ON_SLOT(2, setSlotAlarmTime,   ITime)
   ON_SLOT(3, setSlotTimerActive, Boolean)
END_SLOT_MAP()

Timer::Timer()
{
   STANDARD_CONSTRUCTOR()
   addToTimerList(this);
}

Timer::Timer(const Type direction, const double rtime)
{
    STANDARD_CONSTRUCTOR()

    timerValue = rtime;
    ctime  = rtime;
    dir = direction;

    addToTimerList(this);
}

void Timer::copyData(const Timer& org, const bool cc)
{
    active = false;

    BaseClass::copyData(org);
    if (cc) { addToTimerList(this); }

    timerValue = org.timerValue;
    ctime  = org.ctime;
    alarmTime = org.alarmTime;
    dir = org.dir;
    active = org.active;
}

void Timer::deleteData()
{
   removeFromTimerList(this);
}

// -----------------------------------------------------------------
// Support functions
// -----------------------------------------------------------------
void Timer::start()                          { active = true; }
void Timer::stop()                           { active = false; }
void Timer::reset()                          { stop(); ctime = timerValue; }
void Timer::reset(const double rtime)        { stop(); timerValue = rtime; reset(); }
void Timer::restart()                        { reset(); start(); }
void Timer::restart(const double rtime)      { reset(rtime); start(); }
void Timer::update(const double dt)          { if (active && !frz) { ctime += (dir == Type::UP ? dt : -dt); } }
bool Timer::alarm(const double atime)        { alarmTime = atime; return alarm(); }
bool Timer::setCurrentTime(const double sec) { ctime = sec; return true; }
bool Timer::setAlarmTime(const double sec)   { alarmTime = sec; return true; }
bool Timer::setTimerValue(const double sec)  { timerValue = sec; return true; }

bool Timer::freeze(const bool ff)
{
    bool f{frz};
    frz = ff;
    return f;
}

bool Timer::alarm() const
{
    if (active) return dir == Type::UP ? (ctime >= alarmTime) : (ctime <= alarmTime);
    else return false;
}


// -----------------------------------------------------------------
// Update all timers in the list
// -----------------------------------------------------------------
void Timer::updateTimers(const double dt)
{
    if (!frz) {
      lock( semaphore );
      for (int i = 0; i < nTimers; i++) {
         timers[i]->update(dt);
      }
      unlock( semaphore );
    }
}


// -----------------------------------------------------------------
// Add a new timer to the list
// -----------------------------------------------------------------
void Timer::addToTimerList(Timer* timer)
{
   bool ok{};
   lock( semaphore );
   if (nTimers < MAX_TIMERS) {
      timers[nTimers++] = timer;
      ok = true;
   }
   unlock( semaphore );

   if (!ok) {
      std::cerr << "Timer::addToTimerList() ERROR failed to add a new timer to static timer list" << std::endl;
   }
}


// -----------------------------------------------------------------
// Remove a timer from the list
// -----------------------------------------------------------------
void Timer::removeFromTimerList(Timer* timer)
{
   lock( semaphore );

   // Find this timer in the list
   int found{MAX_TIMERS};
   for (int i = 0; i < nTimers && found == MAX_TIMERS; i++) {
      if (timers[i] == timer) found = i;
   }

   // If found then remove it by moving all timer in the list that are
   // beyond this timer down position
   if (found != MAX_TIMERS) {
      --nTimers;  // One less timer
      for (int i = found; i < nTimers; i++) {
         timers[i] = timers[i+1];
      }
   }

   unlock( semaphore );
}

// -----------------------------------------------------------------
// Slot functions
// -----------------------------------------------------------------

// Sets the timer value
bool Timer::setSlotTimerValue(const ITime* const x)
{
   bool ok{};
   if (x != nullptr) {
      double sec{x->getValueInSeconds()};
      ok = setTimerValue(sec);
      if (ok) setCurrentTime(sec);
   }
   return ok;
}

// Sets the alarm value
bool Timer::setSlotAlarmTime(const ITime* const x)
{
   bool ok{};
   if (x != nullptr) {
      ok = setAlarmTime(x->getValueInSeconds());
    }
    return ok;
}

// Sets the timer active (running) flag
bool Timer::setSlotTimerActive(const Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      active = msg->asBool();
      ok = true;
   }
   return ok;
}

//==============================================================================
// Class UpTimer
//==============================================================================
IMPLEMENT_SUBCLASS(UpTimer, "UpTimer")
EMPTY_SLOTTABLE(UpTimer)
EMPTY_COPYDATA(UpTimer)
EMPTY_DELETEDATA(UpTimer)

UpTimer::UpTimer(const double rtime) : Timer(Type::UP, rtime)
{
}


//==============================================================================
// Class UpTimer
//==============================================================================
IMPLEMENT_SUBCLASS(DownTimer, "DownTimer")
EMPTY_SLOTTABLE(DownTimer)
EMPTY_COPYDATA(DownTimer)
EMPTY_DELETEDATA(DownTimer)

DownTimer::DownTimer(const double rtime) : Timer(Type::DOWN, rtime)
{
}


}
}

