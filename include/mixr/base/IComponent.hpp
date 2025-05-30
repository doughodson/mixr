
#ifndef __mixr_base_IComponent_HPP__
#define __mixr_base_IComponent_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/base/safe_ptr.hpp"

#include <string>

namespace mixr {
namespace base {
class Boolean;
class Identifier;
class Integer;
class INumber;
class Pair;
class PairStream;
class Statistic;
class String;

//------------------------------------------------------------------------------
// Class: IComponent
// Description: Interface that defines time critical and non-time critical tasks,
//              and for passing event messages.  Any component can be a container
//              for a list of components.
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: IComponent
// Slots:
//    components           <IComponent> ! Single child component (default: 0)
//                         <PairStream> ! List of child components.
//
//    select               <String>     ! Selects a child component by name (default: 0)
//                         <Integer>    ! Selects a child component by index (default: 0)
//
//    enableTimingStats    <Boolean>    ! Enable/disable the timing statistics for updateTC() (default: false)
//
//    printTimingStats     <Boolean>    ! Enable/disable the printing of the timing statistics (default: false)
//
//    freeze               <Boolean>    ! Freeze flag: true/frozen, false/unfrozen; (default: false)
//
//    enableMessageType    <Identifier> ! Enable message type { warning info debug user data }
//                         <Integer>    ! Enable message type by number (e.g., 0x0100)
//
//    disableMessageType   <Identifier> ! Disable message type { warning, info, debug, data, user }
//                         <Integer>    ! Disable message type by number (e.g., 0x0100) (default: 0)
//
//------------------------------------------------------------------------------
// Events:
//    SELECT          <String>    ! Select a child component by name
//    SELECT          <Integer>   ! Select a child component by index
//    RESET_EVENT     <>          ! Reset the component
//    FREEZE_EVENT    <>          ! Freeze this component
//    FREEZE_EVENT    <Boolean>   ! Freeze this component if arg is true
//    SHUTDOWN_EVENT  <>          ! Shutdown this component
//
//    Components can send an event token, along with an optional Object based argument,
//    to any other component.  Events are passed by calling the receiving component's
//    event() function.  Event() can be called from an updateTC() routines, so the
//    event tokens must be processed quickly, similar to interrupt handlers.
//
//    Standard event tokens are defined in the file, eventTokens.hpp, which is included
//    by this Component class and are therefore within the mixr::base::Component
//    namespace.  There is a companion file, eventTokens.epp, which can be included
//    as part of the mixr Description Language (EDL) input files.
//
//    The event() function can be implemented using the BEGIN_EVENT_HANDLER()
//    and END_EVENT_HANDLER() macros.  Along with the macros ON_EVENT(), ON_EVENT_OBJ(),
//    ON_ANYKEY() and ON_ANYKEY_OBJ(), these macros are used to build an event dispatch
//    table.  Components will typically provide functions, "event handlers", that will
//    process the individual event tokens.
//
//          BEGIN_EVENT_HANDLER(Foo)
//
//             // The function onSelect() handles the 'SELECT' token if passed with
//             // a Number type argument
//             ON_EVENT_OBJ( SELECT, onSelect, Number)
//
//             // The function onUpdateValue() handles the 'UPDATE_VALUE' token if
//             // passed with a String type argument
//             ON_EVENT_OBJ( UPDATE_VALUE, onUpdateValue, String)
//
//             // The function onF1Key() handles the 'RESET_EVENT' token.  Any argument
//             // that may have been passed is ignored.
//             ON_EVENT( F1_KEY, onF1Key )
//
//          END_EVENT_HANDLER()
//
//    The event handlers are usually 'virtual' functions that can be overridden and
//    handled by a derived class.
//
//    The event handlers will take either a single argument, which matches the type
//    in the ON_EVENT_OBJ macro, or no argument if using just ON_EVENT.  The event
//    handlers will return a boolean which will be 'true' if the event is processed,
//    or 'false' if the event was not processed.
//
//    Events that are not processed are passed up to the 'BaseClass' class until reaching
//    this Component class.  'Key' events (see eventTokens.hpp) that are not processed by
//    this Component class are passed up to the container object.
//------------------------------------------------------------------------------
class IComponent : public IObject
{
   DECLARE_SUBCLASS(IComponent, IObject)

public:
   IComponent();

   //-----------------------------------------------------------------------------
   // Container interface
   //
   // Using a PairStream (see PairStream.hpp), a component can contain a list of
   // named components, and can be contained in a container's component list,
   // therefore creating a component tree.  All functions return zero if the
   // container or component is not found.  The components are initially set
   // using the 'components' slot, and the list can be modified with the
   // addComponent() and processComponents() functions.
   //-----------------------------------------------------------------------------

   // pointer to our container (i.e., we are a component of our container)
   IComponent* container()                                                   { return containerPtr; }
   const IComponent* container() const                                       { return containerPtr; }

   // going up the component tree, finds the first of our containers that
   // is of class type 'foo'
   IComponent* findContainerByType(const std::type_info&);
   const IComponent* findContainerByType(const std::type_info&) const;

   // sets our container pointer; returns 'p'
   IComponent* container(IComponent* const p)                                { return (containerPtr = p); }

   // returns the number of child components
   unsigned int getNumberOfComponents() const;

   // returns a ref()'d pointer to our list of components
   PairStream* getComponents();
   const PairStream* getComponents() const;

   virtual bool addComponent(Pair* const);

   //-----------------------------------------------------------------------------
   // Subcomponent search capabilities & interface
   //-----------------------------------------------------------------------------

   // finds one of our components by 'name'; returns a pointer to the
   // component Pair.  The following are rules for locating components
   // by name.
   //
   //       xxx      -- Look for 'xxx' as the name of one of our children
   //                   components then our grandchildren components, etc.
   //
   //       .xxx     -- Only look for 'xxx' as the name of one of our
   //                   children components.
   //
   //       xxx.yyy  -- complex name; look for 'xxx' as the name of one
   //                   of our children components then our grandchildren
   //                   components, etc. and if found, do a look for the
   //                   name '.yyy' as one of 'xxx's components.
   virtual Pair* findByName(const std::string&);
   virtual const Pair* findByName(const std::string&) const;

   // finds a child component by list 'index' number; returns a pointer
   // to the child component Pair.
   // -- Component index number is one based
   virtual Pair* findByIndex(const int);
   virtual const Pair* findByIndex(const int) const;

   // going down the component tree, finds one of our components by type;
   // returns a pointer to the component Pair.
   virtual Pair* findByType(const std::type_info& type);
   virtual const Pair* findByType(const std::type_info& type) const;

   // returns a ref()'d pointer to an Identifier that contains the name of
   // components 'p'; checking our children first then the grandchildren, etc.
   //
   // -- For children components, a clone of the name (Identifier) is returned.
   // -- For grandchildren, a new Identifier is created containing the full name
   //    of the component (i.e., "xxx.yyy.zzz")
   virtual const std::string findNameOfComponent(const IComponent* const) const;

   //-----------------------------------------------------------------------------
   // Component code execution interface
   //-----------------------------------------------------------------------------

   // time-critical update -- This routine will be called by our tcFrame()
   // at a steady rate of 1/dt, where 'dt' is the  delta time in seconds
   // between calls.  Derived classes will provide updateTC() routines,
   // as needed
   virtual void updateTC(const double dt = 0.0);

   // non-time-critical (i.e., background) update of the component, where
   // 'dt' is the delta time in seconds between calls.  Derived classes
   // will provide updateData() routines, as needed
   virtual void updateData(const double dt = 0.0);

   // time-critical Frame -- This routine will be called by our container
   // at a steady rate of 1/dt, where 'dt' is the  delta time in seconds
   // between calls.  The component time statistics are computed by this
   // function (see slots 'enableTimingStats' and 'printTimingStats')
   void tcFrame(const double dt = 0.0);

   // gets the status of freeze flag.  When the freeze flag is set, delta time is
   // typically set to zero in updateTC() and updateData().  The freeze flag
   // can be preset using the 'freeze' slot.
   virtual bool isFrozen() const;
   virtual bool isNotFrozen() const;
   // sets the freeze flag
   virtual void freeze(const bool);

   // sets this component to its initial conditions
   virtual void reset();

   // true if the component is shutting down or already shutdown. (e.g., received
   // a SHUTDOWN_EVENT event)
   bool isShutdown() const                    { return shutdown; }
   // true if the component is not shutting down, or shut down
   bool isNotShutdown() const                 { return !shutdown; }

   //-----------------------------------------------------------------------------
   // Event processing capabilities (SendData and associated send() methods)
   //
   // Using the send() methods, a container component can send an event token,
   // along with an optional argument, to one of its components named 'id'.
   // The send() methods pass the token and argument to 'id' using the event()
   // function.  As with event(), send() will return true if the event was
   // processed.
   //
   // send() locates the receiving component, 'id', from our components list,
   // and it uses the SendData structure to save the pointer to the receiving
   // component.
   //
   // For 'int', 'float', 'double', 'bool' and 'char*' type arguments, send()
   // will create the proper type Object to pass to the event() function.
   // The SendData structure is used to save the previous argument value.
   // With all subsequent send()'s, the previous argument value is compared to
   // the current argument value, and the event() is sent only if the values
   // have changed.
   //-----------------------------------------------------------------------------

   // event tokens
   #include "mixr/base/eventTokens.hpp"

   // SendData class - used by the send() methods
   class SendData {
      public:  SendData() = default;
      public:  SendData(const SendData&) = delete;
      public:  SendData& operator=(const SendData&) = delete;
      public:  ~SendData()                                         { empty(); }
      public:  void empty();
      public:  IComponent* getObject(IComponent* p, const char* const id, const int n = 0);
      public:  void setObject(IComponent* p);
      public:  IObject* getValue(const int);
      public:  IObject* getValue(const float);
      public:  IObject* getValue(const double);
      public:  IObject* getValue(const char* const);
      public:  IObject* getValue(const bool);
      private: IComponent* obj{};   // Object to send to
      private: IObject* past{};     // Old value
   };

   // sends the 'event' token with an optional argument 'obj' to this component.
   // Returns true if the 'event' has received and processed this component.
   // Typically implemented using the event macros (see above).
   virtual bool event(const int event, IObject* const obj = nullptr);

   // send the 'event' message to our component named 'id' with an optional
   // argument, 'value',  The SendData structure maintains the n-1 value
   // and the pointer to our component.  Send() returns true if the 'event'
   // has been received and used.
   bool send(const char* const id, const int event);
   bool send(const char* const id, const int event, const int value, SendData&);
   bool send(const char* const id, const int event, const float value, SendData&);
   bool send(const char* const id, const int event, const double value, SendData&);
   bool send(const char* const id, const int event, const char* const value, SendData&);
   bool send(const char* const id, const int event, const bool value, SendData&);
   bool send(const char* const id, const int event, IObject* const value, SendData&);

   // sends the 'event' message to 'n' components with 'n' arguments from the
   // 'value' arrays.  This requires an array of 'n' SendData structures to
   // maintain the n-1 values and component pointers.  The component 'id' is
   // used in a format for sprintf() to determine the 'n' individual components.
   //    Example:
   //       n = 4 and id = 'test%02d'
   //    gives component id's = { test01 test02 test03 test04 }
   bool send(const char* const id, const int event, const int value[], SendData sd[], const int n);
   bool send(const char* const id, const int event, const float value[], SendData sd[], const int n);
   bool send(const char* const id, const int event, const double value[], SendData sd[], const int n);
   bool send(const char* const id, const int event, const char* const value[], SendData sd[], const int n);
   bool send(const char* const id, const int event, const bool value[], SendData sd[], const int n);
   bool send(const char* const id, const int event, IObject* const value[], SendData sd[], const int n);

   //-----------------------------------------------------------------------------
   // time-critical statistics (managed by the tcFrame() function)
   //-----------------------------------------------------------------------------

   const Statistic* getTimingStats() const                                   { return timingStats; }
   bool isTimingStatsEnabled() const                                         { return (timingStats != nullptr); }
   bool isTimingStatsPrintEnabled() const                                    { return (pts && isTimingStatsEnabled()); }
   virtual bool setTimingStatsEnabled(const bool);
   virtual bool setPrintTimingStats(const bool);

   //-----------------------------------------------------------------------------
   // Message interface
   //
   // The Object class isMessageEnabled() function has been extended to search
   // up the component tree for explicitly enabled or disabled messages.  Using
   // the 'enableMessageType' and 'disableMessageType' slots, or the functions
   // below, a container component can affect all of its child component's
   // messages, unless a child explicitly enables or disables their own message
   // bits.
   //-----------------------------------------------------------------------------

   // same as Object, plus if the message bit hasn't been explicitly
   // enabled or disabled in this component, it'll check our container's
   // bits as well.
   bool isMessageEnabled(const unsigned short msgType) const override;

protected:
   virtual void printTimingStats();         // Print statistics on component timing
   virtual bool shutdownNotification();     // We're shutting down
   virtual bool onEventReset();             // Reset event handler

   //-----------------------------------------------------------------------------
   // Component selection interface
   //-----------------------------------------------------------------------------

   virtual bool setSelectionName(const IObject* const);   // name (or number) of component to select

   // selects a component for processing.  By default, all of our components
   // are called by updateTC() and updateData().  If a component is selected,
   // only the selected component will be updated.  A component can be pre-
   // selected by name or index number using the 'select' slot.
   virtual bool select(const String* const name);        // select component by name

   // see "select(String* name)" description
   virtual bool select(const Integer* const num);        // select component by Integer value

public:
   // returns true if a subcomponent was selected by index or name using
   // the 'select()' function above (even if the selected index or name
   // wasn't found).
   bool isComponentSelected() const                                          { return (selection != nullptr); }
   IComponent* getSelectedComponent()                                        { return selected; }

   // returns a pointer to the selected component, or zero if no component
   // is selected or the selected component wasn't found.
   const IComponent* getSelectedComponent() const                            { return selected; }

protected:
   // process our new components list
   //   -- add the components from the input list, 'list', to a new list
   //      make sure they are all of class Component (or derived from it)
   //      tell them that we are their container
   //   -- add an optional component to the end of the new list
   //   -- swap our 'components' list with this new list
   //   -- handle component selection
   virtual void processComponents(
         PairStream* const list,             // source list of components
         const std::type_info& filter,       // type filter
         Pair* const add = nullptr,          // optional pair to add
         IComponent* const remove = nullptr  // optional component to remove
      );

private:
   safe_ptr<PairStream> components;    // Child components
   IComponent* containerPtr{};         // We are a component of this container

   IComponent* selected{};             // Selected child (process only this one)
   IObject* selection{};               // Name of selected child

   Statistic* timingStats{};           // Timing statistics
   bool pts{};                         // Print timing statistics
   bool frz{};                         // Freeze flag -- true if this component is frozen
   bool shutdown{};                    // True if this component is being (or has been) shutdown

private:
   // slot table helper methods
   bool setSlotComponent(PairStream* const multiple);        // sets the components list
   bool setSlotComponent(IComponent* const single);          // sets a single component
   bool setSlotSelect(const String* const name)              { return select(name); }
   bool setSlotSelect(const Integer* const num)              { return select(num);  }
   bool setSlotEnableTimingStats(const Boolean* const);      // sets the timing enabled flag
   bool setSlotPrintTimingStats(const Boolean* const);       // sets the print timing stats flag
   bool setSlotFreeze(const Boolean* const);                 // sets the freeze flag
   bool setSlotEnableMsgType(const Identifier* const);       // enables message types by name
   bool setSlotEnableMsgType(const Integer* const);          // enables message types by bit
   bool setSlotDisableMsgType(const Identifier* const);      // disables message types by name
   bool setSlotDisableMsgType(const Integer* const);         // disables message types by bit
};

}
}

#endif
