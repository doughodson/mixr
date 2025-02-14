
#include "mixr/base/Component.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Float.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"

#include "mixr/base/Identifier.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Statistic.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/util/system_utils.hpp"
#include "mixr/base/util/platform_api.hpp"

namespace mixr {
namespace base {

IMPLEMENT_SUBCLASS(Component, "Component")

BEGIN_SLOTTABLE(Component)
    "components",          // 1) Children components                                  (PairStream)
    "select",              // 2) Manage only this child component (component idx)     (Number)
    "enableTimingStats",   // 3) Enable/disable the timing statistics for updateTC()  (Number) (default: false)
    "printTimingStats",    // 4) Enable/disable the printing of the timing statistics (Number) (default: false)
    "freeze",              // 5) Freeze flag: true(1), false(0); default: false       (Number) (default: false)
    "enableMessageType",   // 6) Enable message type { warning info debug user data }
    "disableMessageType"   // 7) Disable message type { warning info debug user data }
END_SLOTTABLE(Component)

BEGIN_SLOT_MAP(Component)
    ON_SLOT( 1, setSlotComponent,         PairStream)
    ON_SLOT( 1, setSlotComponent,         Component)
    ON_SLOT( 2, setSlotSelect,            String)
    ON_SLOT( 2, setSlotSelect,            Integer)
    ON_SLOT( 3, setSlotEnableTimingStats, Boolean)
    ON_SLOT( 4, setSlotPrintTimingStats,  Boolean)
    ON_SLOT( 5, setSlotFreeze,            Boolean)
    ON_SLOT( 6, setSlotEnableMsgType,     Identifier)
    ON_SLOT( 6, setSlotEnableMsgType,     Integer)
    ON_SLOT( 7, setSlotDisableMsgType,    Identifier)
    ON_SLOT( 7, setSlotDisableMsgType,    Integer)
END_SLOT_MAP()

bool Component::event(const int _event, ::mixr::base::Object* const _obj)
{
    bool _used {};

    ON_EVENT_OBJ(SELECT,       select, Integer)
    ON_EVENT_OBJ(SELECT,       select, String)

    ON_EVENT(RESET_EVENT,      onEventReset )
    ON_EVENT(SHUTDOWN_EVENT,   shutdownNotification)

    ON_EVENT_OBJ(FREEZE_EVENT, setSlotFreeze, Boolean )

    // *** Special handling of the end of the EVENT table ***
    // Pass only key events up to our container
    if (_event <= MAX_KEY_EVENT && container() != nullptr) {
        _used = container()->event(_event,_obj);
    }
    return _used;
}

Component::Component()
{
   STANDARD_CONSTRUCTOR()
}

void Component::copyData(const Component& org, const bool)
{
   BaseClass::copyData(org);

   // Copy selection
   setSelectionName(org.selection);
   selected = nullptr;

   // Copy child components
   const PairStream* oc{org.components.getRefPtr()};
   if (oc != nullptr) {
      const auto tmp = static_cast<PairStream*>(oc->clone());
      oc->unref();
      processComponents(tmp, typeid(Component));
      tmp->unref();
   } else {
      components = nullptr;
   }

   // Timing statistics
   if (timingStats != nullptr) timingStats->unref();
   timingStats = nullptr;
   if (org.timingStats != nullptr) {
      timingStats = static_cast<Statistic*>(org.timingStats->clone());
   }
   pts = org.pts;

   // Our container
   containerPtr = nullptr;             // Copied doesn't mean contained in the same container!

   frz = org.frz;
}

void Component::deleteData()
{
    // just in case our components haven't heard, we're shutting down!
    shutdownNotification();

    // Delete component selection
    setSelectionName(nullptr);
    selected = nullptr;

    // Delete list of components
    components = nullptr;

    if (timingStats != nullptr) {
       timingStats->unref();
       timingStats = nullptr;
    }
}

//------------------------------------------------------------------------------
// Message types
//------------------------------------------------------------------------------

bool Component::isMessageEnabled(const unsigned short msgType) const
{
   // Start with our own object's "is enabled" check
   bool enabled{BaseClass::isMessageEnabled(msgType)};

   // If we were not enabled, and we were not explicitly disabled, and if
   // we have a container, then see if our container has this message enabled.
   if ( !enabled && !isMessageDisabled(msgType) && containerPtr != nullptr) {
      enabled = containerPtr->isMessageEnabled(msgType);
   }
   return enabled;
}

//------------------------------------------------------------------------------
// Freeze status and control
//------------------------------------------------------------------------------
bool Component::isFrozen() const
{
   return frz;
}

bool Component::isNotFrozen() const
{
   return !isFrozen();
}

void Component::freeze(const bool fflag)
{
   frz = fflag;
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Component::reset()
{
   PairStream* subcomponents{getComponents()};
   if (subcomponents != nullptr) {
        if (selection != nullptr) {
            // When we've selected only one
            if (selected != nullptr) selected->reset();
        } else {
            // When we should reset them all
            List::Item* item{subcomponents->getFirstItem()};
            while (item != nullptr) {
                const auto pair = static_cast<Pair*>(item->getValue());
                const auto obj = static_cast<Component*>(pair->object());
                obj->reset();
                item = item->getNext();
            }
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }
}

//------------------------------------------------------------------------------
// tcFrame() -- Main time-critical frame
//------------------------------------------------------------------------------
void Component::tcFrame(const double dt)
{
   // ---
   // Collect start time
   // ---
   double tcStartTime{};
   if (isTimingStatsEnabled()) {
      #if defined(WIN32)
         LARGE_INTEGER fcnt;
         QueryPerformanceCounter(&fcnt);
         tcStartTime = static_cast<double>( fcnt.QuadPart );
      #else
         tcStartTime = getComputerTime();
      #endif
   }

   // ---
   // Execute one time-critical frame
   // ---
   this->updateTC(dt);

   // ---
   // Process timing data
   // ---
   if (isTimingStatsEnabled()) {

      double dtime{};    // Delta time in MS
      #if defined(WIN32)
         LARGE_INTEGER cFreq;
         QueryPerformanceFrequency(&cFreq);
         auto freq = static_cast<double>( cFreq.QuadPart );
         LARGE_INTEGER fcnt;
         QueryPerformanceCounter(&fcnt);
         const auto endCnt = static_cast<double>( fcnt.QuadPart );
         double dcnt{endCnt - tcStartTime};
         dtime = (dcnt/freq) * 1000.0;
      #else
         dtime = (getComputerTime() - tcStartTime) * 1000.0;
      #endif
      timingStats->sigma(dtime); // Time in MS

      if (isTimingStatsPrintEnabled()) {
         printTimingStats();
      }

   }
}

//------------------------------------------------------------------------------
// printTimingStats() -- Update time critical stuff here
//------------------------------------------------------------------------------
void Component::printTimingStats()
{
   std::cout << "timing(" << this << "): dt=" << timingStats->value() << ", ave=" << timingStats->mean() << ", max=" << timingStats->maxValue() << std::endl;
}

//------------------------------------------------------------------------------
// updateTC() -- Update time critical stuff here
//------------------------------------------------------------------------------
void Component::updateTC(const double dt)
{
    // Update all my children
    PairStream* subcomponents{getComponents()};
    if (subcomponents != nullptr) {
        if (selection != nullptr) {
            // When we've selected only one
            if (selected != nullptr) selected->tcFrame(dt);
        } else {
            // When we should update them all
            List::Item* item{subcomponents->getFirstItem()};
            while (item != nullptr) {
                const auto pair = static_cast<Pair*>(item->getValue());
                const auto obj = static_cast<Component*>( pair->object() );
                obj->tcFrame(dt);
                item = item->getNext();
            }
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }
}

//------------------------------------------------------------------------------
// updateData() -- Update non-time critical (background) stuff here
//------------------------------------------------------------------------------
void Component::updateData(const double dt)
{
    // Update all my children
    PairStream* subcomponents{getComponents()};
    if (subcomponents != nullptr) {
        if (selection != nullptr) {
            // When we've selected only one
            if (selected != nullptr) selected->updateData(dt);
        } else {
            // When we should update them all
            List::Item* item{subcomponents->getFirstItem()};
            while (item != nullptr) {
                const auto pair = static_cast<Pair*>(item->getValue());
                const auto obj = static_cast<Component*>(pair->object());
                obj->updateData(dt);
                item = item->getNext();
            }
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }
}

//------------------------------------------------------------------------------
// getComponents() -- returns a ref()'d pointer to our list of components;
//                    need to unref() when completed.
//------------------------------------------------------------------------------
PairStream* Component::getComponents()
{
   return components.getRefPtr();
}

const PairStream* Component::getComponents() const
{
   return components.getRefPtr();
}

//------------------------------------------------------------------------------
// getNumberOfComponents() -- returns the number of components
//------------------------------------------------------------------------------
unsigned int Component::getNumberOfComponents() const
{
   unsigned int n{};
   const PairStream* subcomponents{components.getRefPtr()};
   if (subcomponents != nullptr) {
      n = subcomponents->entries();
      subcomponents->unref();
      subcomponents = nullptr;
   }
   return n;
}

//------------------------------------------------------------------------------
// onEventReset() -- Reset event handler
//------------------------------------------------------------------------------
bool Component::onEventReset()
{
   this->reset();
   return true;
}

//------------------------------------------------------------------------------
// shutdownNotification() -- Default shutdown
//------------------------------------------------------------------------------
bool Component::shutdownNotification()
{
   // Tell all of our components
   PairStream* subcomponents{getComponents()};
   if (subcomponents != nullptr) {
      List::Item* item{subcomponents->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<Pair*>(item->getValue());
         const auto p = static_cast<Component*>(pair->object());
         p->event(SHUTDOWN_EVENT);
         item = item->getNext();
      }
      subcomponents->unref();
      subcomponents = nullptr;
   }

   shutdown = true;
   return shutdown;
}


//------------------------------------------------------------------------------
// findContainerByType() -- find a container of ours by type
//------------------------------------------------------------------------------
Component* Component::findContainerByType(const std::type_info& type)
{
   Component* p{};
   if (container() != nullptr) {
      if ( container()->isClassType(type) )
        p = container();
      else
        p = container()->findContainerByType(type);
   }
   return p;
}

const Component* Component::findContainerByType(const std::type_info& type) const
{
   const Component* p{};
   if (container() != nullptr) {
      if ( container()->isClassType(type) )
        p = container();
      else
        p = container()->findContainerByType(type);
   }
   return p;
}


//------------------------------------------------------------------------------
// findByName() -- find one of our components by slotname
//
//  Forms of slotname:
//      xxx     -- simple name, look for 'xxx' as one of our components
//                 or one of our components' components.
//      xxx.yyy -- complex name, look for 'xxx' as one of our
//                 components and '.yyy' as one of our components'
//                 components.
//      .yyy    -- hard name, look for 'yyy' only as one of our
//                 components.
//------------------------------------------------------------------------------
const Pair* Component::findByName(const std::string& slotname) const
{
    const Pair* q{};
    const PairStream* subcomponents{getComponents()};
    if (subcomponents != nullptr) {

        const char* name{slotname.c_str()};
        if (slotname[0] == '.') name++;      // remove '.' from hard names

        // Copy the name up to a possible period.
        char fname[128]{};
        int i {};
        while (name[i] != '\0' && name[i] != '.') {
            fname[i] = name[i];
            i++;
        }
        fname[i] = '\0';

        // Is this a complex name? (xxx.yyy)
        if (name[i] == '.') {
            // When it is a complex name ...

            // Find a component named 'xxx' (we copied 'xxx' to fname above)
            const Pair* q1 {subcomponents->findByName(fname)};

            // Found it?
            if (q1 != nullptr) {
                // Check its components for 'yyy'
                const auto gobj = static_cast<const Component*>(q1->object());
                q = gobj->findByName(&name[i]);
            }

        } else {
            // When it's a simple name ...
            q = subcomponents->findByName(name);
        }

        // Did we find it?
        if (q == nullptr && slotname[0] != '.') {
            // No, its not one of our components and its not a hard name,
            //  so check our components' components
            const List::Item* item {subcomponents->getFirstItem()};
            while (item != nullptr && q == nullptr) {
                const auto p = static_cast<const Pair*>(item->getValue());
                const auto obj = static_cast<const Component*>(p->object());
                q = obj->findByName(slotname);
                item = item->getNext();
            }
        }

        subcomponents->unref();
        subcomponents = nullptr;
    }
    return q;
}

Pair* Component::findByName(const std::string& slotname)
{
   const Component* cThis{this};
   const Pair* p{cThis->findByName(slotname)};
   return const_cast<Pair*>(p);
}

//------------------------------------------------------------------------------
// findByIndex() -- find component one of our components by slot index
//------------------------------------------------------------------------------
const Pair* Component::findByIndex(const int slotindex) const
{
   const Pair* p{};

   const PairStream* subcomponents{getComponents()};
   if (subcomponents != nullptr) {
      p = subcomponents->getPosition(slotindex);
      subcomponents->unref();
      subcomponents = nullptr;
   }

   return p;
}

Pair* Component::findByIndex(const int slotindex)
{
   Pair* p{};

   PairStream* subcomponents{getComponents()};
   if (subcomponents != nullptr) {
      p = subcomponents->getPosition(slotindex);
      subcomponents->unref();
      subcomponents = nullptr;
   }

   return p;
}

//------------------------------------------------------------------------------
// findByType() -- find one of our components by type (our children first
//                 then grandchildren).
//------------------------------------------------------------------------------
const Pair* Component::findByType(const std::type_info& type) const
{
    const Pair* q{};
    const PairStream* subcomponents{getComponents()};
    if (subcomponents != nullptr) {
        q = subcomponents->findByType(type);
        const List::Item* item{subcomponents->getFirstItem()};
        while (item != nullptr && q == nullptr) {
            const auto p = static_cast<const Pair*>(item->getValue());
            const auto obj = static_cast<const Component*>(p->object());
            q = obj->findByType(type);
            item = item->getNext();
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }
    return q;
}

Pair* Component::findByType(const std::type_info& type)
{
   const Component* cThis{this};
   const Pair* p {cThis->findByType(type)};
   return const_cast<Pair*>(p);
}

//------------------------------------------------------------------------------
// findNameOfComponent() --
//   1) Returns a pointer to an Identifier that contains the name of components 'p'
//      (our children first then grandchildren).  For grandchildren, a full name is
//      created to a component (i.e., "xxx.yyy.zzz", see findByName()).
//   2) Unref() the Identifier when finished.
//   3) Zero(0) is returned if 'p' is not found.
//------------------------------------------------------------------------------
const std::string Component::findNameOfComponent(const Component* const p) const
{
    std::string name;
    const PairStream* subcomponents{getComponents()};
    if (subcomponents != nullptr) {

        // First check our component list ..
        name = subcomponents->findName(p);

        if (name.empty()) {

            // Not found, so check our children's components ...

            const List::Item* item{subcomponents->getFirstItem()};
            while (item != nullptr && name.empty()) {
                const auto pair = static_cast<const Pair*>(item->getValue());
                const auto child = static_cast<const Component*>(pair->object());
                const std::string& name0{child->findNameOfComponent(p)};
                if (!name0.empty()) {
                    // Found it, so prefix it with our child's name and
                    // return the full name.
                    std::string fullname{pair->slot()};
                    fullname += ".";
                    fullname += name0;
                    name = fullname;
                }
                item = item->getNext();
            }
        }
        subcomponents->unref();
        subcomponents = nullptr;
    }
    return name;
}

//------------------------------------------------------------------------------
// addComponent() -- Add a new component to our list of components
//------------------------------------------------------------------------------
bool Component::addComponent(Pair* const p)
{
   PairStream* subcomponents{getComponents()};
   processComponents(subcomponents, typeid(Component), p);
   if (subcomponents != nullptr) subcomponents->unref();
   return true;
}


//------------------------------------------------------------------------------
// processComponents() -- process our new components list;
//   -- Add the components from the input list, 'list', to a new list, 'newList'
//      make sure they are all of type Component (or derived from it)
//      tell them that we are their container
//   -- Add an optional component to the end of the new list
//   -- Swap our 'components' list with the new list, newList
//   -- Handle selections.
//------------------------------------------------------------------------------
void Component::processComponents(
      PairStream* const list,
      const std::type_info& filter,
      Pair* const add,
      Component* const remove
   )
{
   PairStream* oldList {components.getRefPtr()};

   // ---
   // Our dynamic_cast (see below) already filters on the Component class
   // ---
   bool skipFilter{};
   if (filter == typeid(Component)) {
      skipFilter = true;
   }

   // ---
   // Create a new list, copy (filter) the component pairs and set their container pointers
   // ---
   const auto newList = new PairStream();
   if (list != nullptr) {

      // Add the (filtered) components to the new list and set their container
      List::Item* item{list->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<Pair*>(item->getValue());
         const auto cp = dynamic_cast<Component*>( pair->object() );
         if ( cp != nullptr && cp != remove && (skipFilter || cp->isClassType(filter)) ) {
            newList->put(pair);
            cp->container(this);
         } else if ( cp != nullptr && cp == remove ) {
            cp->container(nullptr);
         }
         item = item->getNext();
      }

   }

   // ---
   // Add the optional component
   // ---
   if (add != nullptr) {
      const auto cp = dynamic_cast<Component*>( add->object() );
      if ( cp != nullptr && (skipFilter || cp->isClassType(filter)) ) {
         newList->put(add);
         cp->container(this);
      }
   }

   // ---
   // Swap lists
   // ---
   components = newList;
   newList->unref();

   // ---
   // Anything selected?
   // ---
   if (selection != nullptr) {
      if (selection->isClassType(typeid(String))) {
            const auto str = new String(*(static_cast<String*>(selection)));
            select(str);
            str->unref();
      } else {
            const auto num = new Integer( static_cast<int>((static_cast<INumber*>(selection))->asDouble()) );
            select(num);
            num->unref();
      }
   }

   if (oldList != nullptr) {
      oldList->unref();
   }
}

//------------------------------------------------------------------------------
// setSelectionName() -- Name (or number) of component to selected
//------------------------------------------------------------------------------
bool Component::setSelectionName(const Object* const s)
{
   if (selection != nullptr) {
       selection->unref();
   }
   selection = nullptr;
   if (s != nullptr) {
      selection = s->clone();
   }
   return true;
}

//------------------------------------------------------------------------------
// select() -- select one of our components, using String or Number
//------------------------------------------------------------------------------
bool Component::select(const String* const name)
{
    bool ok{true};
    selected = nullptr;
    setSelectionName(nullptr);
    if (name != nullptr) {
        setSelectionName(name);
        Pair* p{findByName((*name).c_str())};
        if (p != nullptr) {
           selected = static_cast<Component*>(p->object());
        } else {
            std::cerr << "Component::select<String>: name not found!"  << std::endl;
            ok = false;
        }
    }
    return ok;
}

bool Component::select(const Integer* const num)
{
    bool ok{true};
    selected = nullptr;
    setSelectionName(nullptr);
    if (num != nullptr) {
        setSelectionName(num);
        Pair* p{findByIndex(num->asInt())};
        if (p != nullptr) {
           selected = static_cast<Component*>(p->object());
        } else {
           std::cerr << "Component::select<Integer>: index out of range; num = " << num->asInt() << std::endl;
           ok = false;
        }
    }
    return ok;
}

//------------------------------------------------------------------------------
// setTimingStatsEnabled() -- enable/disable timing statistics
//------------------------------------------------------------------------------
bool Component::setTimingStatsEnabled(const bool b)
{
   if (b) {
      // Enable timing statistics by creating a statistics object
      if (timingStats != nullptr) {
         // Already have one, just clear it
         timingStats->clear();
      } else {
         timingStats = new Statistic();
      }
   } else {
      // Disable the timing statistics
      if (timingStats != nullptr) {
         // We disable it by getting rid of it.
         timingStats->unref();
         timingStats = nullptr;
      }
   }
   return true;
}

//------------------------------------------------------------------------------
// setPrintTimingStats() -- enable/disable print the timing statistics
//------------------------------------------------------------------------------
bool Component::setPrintTimingStats(const bool b)
{
   pts = b;
   return true;
}

//------------------------------------------------------------------------------
// Slot functions
//------------------------------------------------------------------------------

// setSlotEnableTimingStats() -- slot to enable/disable the timing statistics
bool Component::setSlotEnableTimingStats(const Boolean* const num)
{
   bool ok{};
   if (num != nullptr) {
      ok = setTimingStatsEnabled(num->asBool());
   }
   return ok;
}

// setSlotPrintTimingStats() -- slot to enable/disable printing the timing statistics
bool Component::setSlotPrintTimingStats(const Boolean* const num)
{
   bool ok{};
   if (num != nullptr) {
      ok = setPrintTimingStats(num->asBool());
   }
   return ok;
}

// setSlotFreeze() -- slot to set/clear the freeze flag
bool Component::setSlotFreeze(const Boolean* const num)
{
   bool ok{};
   if (num != nullptr) {
      freeze(num->asBool());
      ok = true;
   }
   return ok;
}

// setSlotComponent() -- Sets a pairstream
bool Component::setSlotComponent(PairStream* const multiple)
{
   // Process the new components list and swap
   processComponents(multiple, typeid(Component));
   return true;
}

// setSlotComponent() -- Sets a single component
bool Component::setSlotComponent(Component* const single)
{
   // When a only one component ... make it a PairStream
   const auto pairStream = new PairStream();
   const auto pair = new Pair("1", single);
   pairStream->put( pair );
   pair->unref();

   // Process the new components list and swap
   processComponents(pairStream, typeid(Component));
   pairStream->unref();

   return true;
}

// enableMessageType --- Enable message type { warning info debug user data }
bool Component::setSlotEnableMsgType(const Identifier* const x)
{
   bool ok{};
   const Identifier* p{x};
   if (*p == "warning")    ok = enableMessageTypes(MSG_WARNING);
   else if (*p == "info")  ok = enableMessageTypes(MSG_INFO);
   else if (*p == "debug") ok = enableMessageTypes(MSG_DEBUG);
   else if (*p == "user")  ok = enableMessageTypes(MSG_USER);
   else if (*p == "data")  ok = enableMessageTypes(MSG_DATA);
   else {
      if (isMessageEnabled(MSG_ERROR)) {
         std::cerr << "Object::setSlotEnableMsgType(): unknown message type: " << *p;
         std::cerr << "; use: { warning info debug user data }" << std::endl;
      }
   }
   return ok;
}

// enableMessageType --- Enable message type by number (e.g., 0x0100)
bool Component::setSlotEnableMsgType(const Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = enableMessageTypes( static_cast<unsigned short>(msg->asInt()) );
   }
   return ok;
}

// disableMessageType --- Disable message type { warning info debug user data }
bool Component::setSlotDisableMsgType(const Identifier* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const Identifier* p{msg};
      if (*p == "warning")    ok = disableMessageTypes(MSG_WARNING);
      else if (*p == "info")  ok = disableMessageTypes(MSG_INFO);
      else if (*p == "debug") ok = disableMessageTypes(MSG_DEBUG);
      else if (*p == "user")  ok = disableMessageTypes(MSG_USER);
      else if (*p == "data")  ok = disableMessageTypes(MSG_DATA);
      else {
         if (isMessageEnabled(MSG_ERROR)) {
             std::cerr << "Object::disableMessageTypes(): unknown message type: " << *p;
             std::cerr << "; use: { warning info debug user data }" << std::endl;
         }
      }
   }
   return ok;
}

// disableMessageType --- Disable message type by number (e.g., 0x0100)
bool Component::setSlotDisableMsgType(const Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = disableMessageTypes( static_cast<unsigned short>(msg->asInt()) );
   }
   return ok;
}

//------------------------------------------------------------------------------
// send() -- various support routines that send event messages to components.
//           Return true of the message was received and used.
//------------------------------------------------------------------------------

// send an event message to component 'id'
bool Component::send(const char* const id, const int event)
{
    bool val{};
    Pair* p{findByName(id)};
    if (p != nullptr) {
        const auto g = static_cast<Component*>(p->object());
        val = g->event(event);
    }
    return val;
}

// send an event message with an int value to component 'id'
bool Component::send(const char* const id, const int event, const int value, SendData& sd)
{
   bool val{};
   Object* vv{sd.getValue(value)};
   if (vv != nullptr) {
      Component* g{sd.getObject(this,id)};
      if (g != nullptr) val = g->event(event,vv);
   }
   return val;
}

// send an event message with a float value to component 'id'
bool Component::send(const char* const id, const int event, const float value, SendData& sd)
{
   bool val{};
   Object* vv{sd.getValue(static_cast<double>(value))};
   if (vv != nullptr) {
      Component* g{sd.getObject(this, id)};
      if (g != nullptr) val = g->event(event,vv);
   }
   return val;
}

// send an event message with a double value to component 'id'
bool Component::send(const char* const id, const int event, const double value, SendData& sd)
{
   bool val{};
   Object* vv{sd.getValue(static_cast<double>(value))};
   if (vv != nullptr) {
      Component* g{sd.getObject(this, id)};
      if (g != nullptr) val = g->event(event,vv);
   }
   return val;
}

// send an event message with a character string to component 'id'
bool Component::send(const char* const id, const int event, const char* const value, SendData& sd)
{
   bool val{};
   Object* vv{sd.getValue(value)};
   if (vv != nullptr) {
      Component* g{sd.getObject(this, id)};
      if (g != nullptr) val = g->event(event,vv);
   }
   return val;
}

// send an event message with a boolean to component 'id'
bool Component::send(const char* const id, const int event, const bool value, SendData& sd)
{
   bool val{};
   Object* vv{sd.getValue(value)};
   if (vv != nullptr) {
      Component* g{sd.getObject(this, id)};
      if (g != nullptr) val = g->event(event,vv);
   }
   return val;
}

// Send an event message with an Object value to component 'id'
bool Component::send(const char* const id, const int event, Object* const value, SendData& sd)
{
    // we don't check past values here, because it would be tedious and more overhead
    // to go through each object and see if any data has changed.  So we take a smaller
    // hit just by sending the data through every time.
    bool val{};
    if (value != nullptr) {
      Component* g{sd.getObject(this, id)};
      if (g != nullptr) val = g->event(event, value);
   }
   return val;
}


// ---
// Send 'n' event messages with int type values to components.  The 'id' is used
// with std::sprintf() to create the component names.  Example: n = 4, id="test%02d"
// will generate component names test01, test02, test03 and test04.
// ---
bool Component::send(const char* const id, const int event, const int value[], SendData sd[], const int n)
{
   bool val{};
   for (int i = 0; i < n; i++) {
      Object* vv{sd[i].getValue(value[i])};
      if (vv != nullptr) {
         Component* g{sd[i].getObject(this, id, (i+1))};
         if (g != nullptr) val = g->event(event,vv);
      }
   }
   return val;
}

// ---
// Send 'n' event messages with float type values to components.  The 'id' is used
// with std::sprintf() to create the component names.  Example: n = 4, id="test%02d"
// will generate component names test01, test02, test03 and test04.
// ---
bool Component::send(const char* const id, const int event, const float value[], SendData sd[], const int n)
{
   bool val{};
   for (int i = 0; i < n; i++) {
      Object* vv{sd[i].getValue(value[i])};
      if (vv != nullptr) {
         Component* g{sd[i].getObject(this,id,(i+1))};
         if (g != nullptr) val = g->event(event,vv);
      }
   }
   return val;
}

// ---
// Send 'n' event messages with char type values to components.  The 'id' is used
// with std::sprintf() to create the component names.  Example: n = 4, id="test%02d"
// will generate component names test01, test02, test03 and test04.
// ---
bool Component::send(const char* const id, const int event, const double value[], SendData sd[], const int n)
{
   bool val{};
   for (int i = 0; i < n; i++) {
      Object* vv{sd[i].getValue(value[i])};
      if (vv != nullptr) {
         Component* g{sd[i].getObject(this,id,(i+1))};
         if (g != nullptr) val = g->event(event,vv);
      }
   }
   return val;
}

bool Component::send(const char* const id, const int event, const bool value[], SendData sd[], const int n)
{
   bool val{};
   for (int i = 0; i < n; i++) {
      Object* vv{sd[i].getValue(value[i])};
      if (vv != nullptr) {
         Component* g{sd[i].getObject(this,id,(i+1))};
         if (g != nullptr) val = g->event(event,vv);
      }
   }
   return val;
}

bool Component::send(const char* const id, const int event, const char* const value[], SendData sd[], const int n)
{
   bool val{};
   for (int i = 0; i < n; i++) {
      Object* vv{sd[i].getValue(value[i])};
      if (vv != nullptr) {
         Component* g{sd[i].getObject(this,id,(i+1))};
         if (g != nullptr) val = g->event(event,vv);
      }
   }
   return val;
}

bool Component::send(const char* const id, const int event, Object* const value[], SendData sd[], const int n)
{
   bool val{};
   for (int i = 0; i < n; i++) {
      if (value != nullptr) {
         Component* g{sd[i].getObject(this,id,(i+1))};
         if (g != nullptr) val = g->event(event,value[i]);
      }
   }
   return val;
}

//==============================================================================
// class Component::SendData
//==============================================================================

// empty() the SendData structure
void Component::SendData::empty()
{
   obj = nullptr;
   if (past != nullptr) past->unref();
   past = nullptr;
}


// setObject() -- Set which object (component) we're sending to
void Component::SendData::setObject(Component* p)
{
   obj = p;
}


// getObject() -- Get/Find the object (component) we're sending to
Component* Component::SendData::getObject(Component* gobj, const char* const id, const int n)
{
    // Did we already find the object?
    if (obj == nullptr) {
        // No, then try to find it among our components ...
        Pair* p{};
        if (n <= 0) {
            // When n is zero (or less) just use 'id' as
            // the name when finding the object.
            p = gobj->findByName(id);
        } else {
            // ---
            // When n is greater than zero, use 'id' as a format
            // with 'n' to get the name.  Great for items in rows
            // or columns.
            //   Example:   n = 5
            //             id = xxx.%d
            //      gives name = xxx.5
            // ---
            char name[128]{};
            std::sprintf(name,id,n);
            p = gobj->findByName(name);
        }
        if (p != nullptr) obj = static_cast<Component*>(p->object());
    }
    return obj;
}

// getValue() -- get an object containing the int value to send
// or null(0) if the value hasn't changed.
Object* Component::SendData::getValue(const int value)
{
    const auto num = dynamic_cast<Integer*>(past);
    if (num == nullptr) {
        if (past != nullptr) past->unref();
        past = new Integer(value);
        return past;
    }
    if (num->asInt() != value) {
        num->setValue(value);
        return num;
    } else {
        return nullptr;
    }
}

// getValue() -- get an object containing the real value to send
// or null(0) if the value hasn't changed.
Object* Component::SendData::getValue(const float value)
{
    const auto num = dynamic_cast<Float*>(past);
    if (num == nullptr) {
        if (past != nullptr) past->unref();
        past = new Float(value);
        return past;
    }
    if (num->asDouble() != static_cast<double>(value)) {
        num->setValue(value);
        return num;
    } else {
        return nullptr;
    }
}

Object* Component::SendData::getValue(const double value)
{
    const auto num = dynamic_cast<Float*>(past);
    if (num == nullptr) {
        if (past != nullptr) past->unref();
        past = new Float(value);
        return past;
    }

    if (num != nullptr && num->asDouble() != value) {
        num->setValue(value);
        return num;
    } else {
        return nullptr;
    }
}

// getValue() -- get an object containing the char string to send
// or null(0) if the value hasn't changed.
Object* Component::SendData::getValue(const char* const value)
{
    // get our past string
    const auto str = dynamic_cast<String*>(past);
    if (str == nullptr) {
        if (past != nullptr) past->unref();
        past = new String(value);
        return past;
    }

    // Compare our new value to our past string.
    if (value != nullptr) {
        if (*str != value) {
            *str = value;
            return str;
        }
        else return nullptr;
    }

    // When our value is a null string, check if the past string was null
    else {
        if ( !str->isEmpty() ) {
            str->clear();
            return str;
        }
        else return nullptr;
    }
}

// getValue() -- get an object containing the boolean value to send
// or null(0) if the value hasn't changed.
Object* Component::SendData::getValue(const bool value)
{
    const auto num = dynamic_cast<Boolean*>(past);
    if (num == nullptr) {
        if (past != nullptr) past->unref();
        past = new Boolean(value);
        return past;
    }
    if (num->asBool() != value) {
        num->setValue(value);
        return num;
    } else {
        return nullptr;
    }
}

}
}
