
#include "mixr/base/IStateMachine.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/IPairStream.hpp"

namespace mixr {
namespace base {

IMPLEMENT_ABSTRACT_SUBCLASS(IStateMachine, "IStateMachine")

BEGIN_SLOTTABLE(IStateMachine)
    "stateMachines",  // 1: List of state machine objects (StateMachine class)
END_SLOTTABLE(IStateMachine)

BEGIN_SLOT_MAP(IStateMachine)
    ON_SLOT(1, setSlotStateMachines, IPairStream)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(IStateMachine)
    ON_EVENT_OBJ(ON_ENTRY, onEntry, IObject)     // always check w/IObject first
    ON_EVENT(ON_ENTRY, onEntry)

    ON_EVENT_OBJ(ON_RETURN, onReturn, IObject)   // always check w/IObject first
    ON_EVENT(ON_RETURN, onReturn)

    ON_EVENT(ON_EXIT, onExit)

   // If our current state is controlled by another StateMachine then
   // see if this StateMachine will handled this event.
   if (stMach != nullptr && !_used) _used = stMach->event(_event,_obj);
END_EVENT_HANDLER()

IStateMachine::IStateMachine()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void IStateMachine::initData()
{
   stateStack.fill(INVALID_STATE);
   substateStack.fill(INVALID_STATE);
}

void IStateMachine::copyData(const IStateMachine& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   setSlotStateMachines(org.stMachList);
}

void IStateMachine::deleteData()
{
   setStMach(nullptr, StateTableCode::CURR_STATE);
   setSlotStateMachines(nullptr);
}

// -----------------------------------------------------------------
// reset() -- Resets the state machine
// -----------------------------------------------------------------
void IStateMachine::reset()
{
   BaseClass::reset();

   // Reset our state machine list
   if (stMachList != nullptr) {
      IList::Item* item{stMachList->getFirstItem()};
      while (item != nullptr) {
         const auto p = static_cast<Pair*>(item->getValue());
         const auto q = static_cast<IComponent*>(p->object());
         q->reset();
         item = item->getNext();
      }
   }

   // Goto our RESET state
   state = INVALID_STATE;
   substate = INVALID_STATE;
   stMach = nullptr;
   stMachName.clear();
   arg = nullptr;

   goTo(INIT_STATE);
}

//------------------------------------------------------------------------------
// updateData() -- update background data here
//------------------------------------------------------------------------------
void IStateMachine::updateData(const double dt)
{
   if (stMach != nullptr) stMach->updateData(dt);
   BaseClass::updateData(dt);
}

// -----------------------------------------------------------------
// updateTC() -- one iteration step of the state machine
// -----------------------------------------------------------------
void IStateMachine::updateTC(const double dt)
{
   // Step state machine
   step(dt);

   // BaseClass
   BaseClass::updateTC(dt);
}

// -----------------------------------------------------------------
// One iteration step of the state machine
// -----------------------------------------------------------------
void IStateMachine::step(const double dt)
{
   // ---
   // 1) If requested, step into a new state
   // ---
   if (nMode == Mode::NEW_STATE) {
      pState = state;
      state = nState;
      arg = nArg;
      mode = Mode::NEW_STATE;
   }
   else if (nMode == Mode::RTN_STATE) {
      pState = state;
      state = nState;
      arg = nArg;
      mode = Mode::RTN_STATE;
   }
   else { // (nMode == HOLD_STATE)
      pState = state;
      arg = nullptr;
      mode = Mode::HOLD_STATE;
   }
   nState = INVALID_STATE;
   nArg = nullptr;
   nMode = Mode::HOLD_STATE;

   // always step the substate
   pSubstate = substate;
   substate = nSubstate;


   // ---
   // 2) common pre-state function
   //
   preStateProc(dt);


   // ---
   // 3) process the state table, which calls state specific function
   //    or sets this state's child state machine, 'stMach'.
   // ---
   {
      stateTable(state, StateTableCode::CURR_STATE, dt);

      // Send state exit events to the previous state's state machine
      if (pStMach != nullptr) {
         pStMach->event(ON_EXIT);
         pStMach = nullptr;
      }

      if (stMach != nullptr) {
         if (mode == Mode::NEW_STATE) {
            // Send state entry events
            stMach->event(ON_ENTRY);
         }
         else if (mode == Mode::RTN_STATE) {
            // Send returning events
            stMach->event(ON_RETURN);
         }

         // Step this state's child state machine
         stMach->tcFrame(dt);
      }
   }

   // ---
   // 4) common post-state function
   // ---
   postStateProc(dt);
}

// -----------------------------------------------------------------
// Transition functions -- these control movement between our states
// -----------------------------------------------------------------

bool IStateMachine::next(IObject* const arg)
{
   bool ok{};
   unsigned short newState{stateTable(state, StateTableCode::FIND_NEXT_STATE)};
   if (newState != INVALID_STATE) {
      nState = newState;
      nSubstate = INIT_STATE;
      nArg = arg;
      nMode = Mode::NEW_STATE;
      ok = true;
   }
   return ok;
}

bool IStateMachine::goTo(const unsigned short newState, IObject* const arg)
{
   bool ok{};
   unsigned short test{stateTable(newState, StateTableCode::TEST_STATE)};
   if (test != INVALID_STATE) {
      nState = newState;
      nSubstate = INIT_STATE;
      nArg = arg;
      nMode = Mode::NEW_STATE;
      ok = true;
   }
   return ok;
}

bool IStateMachine::call(const unsigned short newState, IObject* const arg)
{
   bool ok{};
   if (sp > 0) {
      unsigned short test{stateTable(newState, StateTableCode::TEST_STATE)};
      if (test != INVALID_STATE) {
         stateStack[--sp] = state;
         substateStack[sp] = substate;
         nState = newState;
         nSubstate = INIT_STATE;
         nArg = arg;
         nMode = Mode::NEW_STATE;
         ok = true;
      }
   }
   return ok;
}

bool IStateMachine::rtn(IObject* const arg)
{
   bool ok{};
   if (sp < STACK_SIZE) {
      nSubstate = substateStack[sp];
      nState = stateStack[sp++];
      nArg = arg;
      nMode = Mode::RTN_STATE;
      ok = true;
   }
   return ok;
}


// -----------------------------------------------------------------
// Substate Transition functions -- these control movement between our substates
// -----------------------------------------------------------------

bool IStateMachine::nextSubstate()
{
   nSubstate = (substate+1);
   return true;
}

bool IStateMachine::goToSubstate(const unsigned short newSubstate)
{
   nSubstate = newSubstate;
   return true;
}


// -----------------------------------------------------------------
// Parent State Machine's transition functions -- these control movement
// between our parent state machine's states.
// -----------------------------------------------------------------

bool IStateMachine::nextState(IObject* const arg)
{
   bool ok{};
   const auto parent = dynamic_cast<IStateMachine*>( container() );
   if (parent != nullptr) {
      ok = parent->next(arg);
   }
   return ok;
}

bool IStateMachine::goToState(const unsigned short newState, IObject* const arg)
{
   bool ok{};
   const auto parent = dynamic_cast<IStateMachine*>( container() );
   if (parent != nullptr) {
      ok = parent->goTo(newState,arg);
   }
   return ok;
}

bool IStateMachine::callState(const unsigned short newState, IObject* const arg)
{
   bool ok{};
   const auto parent = dynamic_cast<IStateMachine*>( container() );
   if (parent != nullptr && sp > 0) {
      ok = parent->call(newState,arg);
      if (ok) {
         stateStack[--sp] = state;
         substateStack[sp] = substate;
      }
   }
   return ok;
}

bool IStateMachine::rtnState(IObject* const arg)
{
   bool ok{};
   const auto parent = dynamic_cast<IStateMachine*>( container() );
   if (parent != nullptr) {
      ok = parent->rtn(arg);
   }
   return ok;
}


// -----------------------------------------------------------------
// Default event handlers
// -----------------------------------------------------------------
bool IStateMachine::onEntry(IObject* const msg)
{
   nState = INIT_STATE;
   nArg = msg;
   nMode = Mode::NEW_STATE;
   nSubstate = INIT_STATE;
   return true;
}

bool IStateMachine::onExit()
{
   state = INVALID_STATE;
   stMach = nullptr;
   mode = Mode::HOLD_STATE;
   arg = nullptr;

   pState = INVALID_STATE;
   pStMach = nullptr;

   return true;
}

bool IStateMachine::onReturn(IObject* const msg)
{
   // Try to return to our calling state
   bool ok{rtn(msg)};

   if (!ok) {
      // or just goto the reset state
      nState = INIT_STATE;
      nArg = msg;
      nMode = Mode::RTN_STATE;
      nSubstate = INIT_STATE;
   }
   return true;
}

// -----------------------------------------------------------------
// Default procedure that's called by stepState just before
// the state specific procedure
// -----------------------------------------------------------------
void IStateMachine::preStateProc(const double)
{
}

// -----------------------------------------------------------------
// Default procedure that's called by stepState just after
// the state specific procedure
// -----------------------------------------------------------------
void IStateMachine::postStateProc(const double)
{
}

//------------------------------------------------------------------------------
// findStMachByName(), findStMachByType() -- find a state machine
//------------------------------------------------------------------------------
Pair* IStateMachine::findStMachByName(const char* const name)
{
    Pair* p{};
    if (stMachList != nullptr && name != nullptr) p = stMachList->findByName(name);
    return p;
}


Pair* IStateMachine::findStMachByType(const std::type_info& type)
{
    Pair* p{};
    if (stMachList != nullptr) p = stMachList->findByType(type);
    return p;
}

// -----------------------------------------------------------------
// Sets the state machine 'name' or zero to clear.  Return true
// if successful. If the 'code' is CHK_CUR_STATE or GET_NEXT_STATE
// then we do NOT actually set the stMach, but we only check to
// see if the stMach 'name' exists.
// -----------------------------------------------------------------
bool IStateMachine::setStMach(const char* const name, const StateTableCode code)
{
   bool ok{};

   if (code == StateTableCode::CURR_STATE) {

      // Current state is now also the previous state
      IStateMachine* oldStMach{stMach};

      // First, check to see if they're asking for the same state
      // as our current state.
      ok = (stMachName.empty() && name == nullptr);
      if (!ok && !stMachName.empty() && name != nullptr) {
         ok = (stMachName == name);
      }

      // When they're not the same then we make a switch ...
      if (!ok) {
         if (name != nullptr) {
            Pair* p{findStMachByName(name)};
            if (p != nullptr) {
               stMach = static_cast<IStateMachine*>(p->object());
               stMachName = p->slot();
               ok = true;
            }
         } else {
            // 'name' is null, so set the new state to null.
            stMach = nullptr;
            stMachName = nullptr;
            ok = true;
         }
         if (ok) {
            pStMach = oldStMach;
         }
      }

   }

   else {
      // -- only need to look to see if this named state machine exists.
      ok = (findStMachByName(name) != nullptr);
   }

   return ok;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

bool IStateMachine::setSlotStateMachines(const IPairStream* const msg)
{
   // First remove the old list; and make sure we tell the old stMachList
   // that we're no longer their container.
   if (stMachList != nullptr) {
      IList::Item* item{stMachList->getFirstItem()};
      while (item != nullptr) {
         const auto p = static_cast<Pair*>(item->getValue());
         const auto q = static_cast<IComponent*>(p->object());
         q->container(nullptr);
         item = item->getNext();
      }
      stMachList = nullptr;
   }

   // Build a new list containing only StateMachine class (or derived) objects
   if (msg != nullptr) {
      const auto newList = new IPairStream();

      // For each object in the list; if it's a StateMachine (or derived from) then
      // clone the object and add it to the new list.
      const IList::Item* item{msg->getFirstItem()};
      while (item != nullptr) {
         const auto p = static_cast<const Pair*>(item->getValue());
         const auto q = dynamic_cast<const IStateMachine*>(p->object());

         if (q != nullptr) {
            const auto cp = static_cast<Pair*>(p->clone());
            const auto cq = static_cast<IStateMachine*>(cp->object());
            cq->container(this);
            newList->put(cp);
         } else {
            std::cerr << "StateMachine::setSlotStateMachines(): " << p->slot() << " is not a StateMachine!" << std::endl;
         }

         item = item->getNext();
      }

      // Set the pointer to the new stMach list
      stMachList = newList;
   }

   return true;
}

}
}
