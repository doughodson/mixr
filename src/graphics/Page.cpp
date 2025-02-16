
#include "mixr/graphics/Page.hpp"
#include "mixr/graphics/Display.hpp"

#include "mixr/base/IComponent.hpp"
#include "mixr/base/String.hpp"
#include "mixr/base/Identifier.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace graphics {

IMPLEMENT_SUBCLASS(Page, "Page")

BEGIN_SLOTTABLE(Page)
    "page",                 // 1: initial subpage
    "pages",                // 2: subpages
    "pagingEvent",          // 3: page change event
    "subpagesFirst",        // 4: draw subpages first (default: draw our page graphics first)
    "focusSlavedToSubpage", // 5: slave the focus to the subpage (default: true)
END_SLOTTABLE(Page)

BEGIN_SLOT_MAP(Page)
    ON_SLOT(1, setSlotPage,                 base::Identifier)
    ON_SLOT(2, setSlotSubpageStream,        base::PairStream)
    ON_SLOT(2, setSlotSubpageSingle,        Page)
    ON_SLOT(3, setSlotPagingEvent,          base::PairStream)
    ON_SLOT(4, setSlotDrawSubpageFirst,     base::Boolean)
    ON_SLOT(5, setSlotFocusSlavedToSubpage, base::Boolean)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Page)
    ON_EVENT(ON_ENTRY, onEntry)
    ON_EVENT(ON_EXIT,  onExit)
    ON_EVENT_OBJ(BUTTON_HIT, onButtonHit,   base::String)
    ON_ANYKEY(onKeyHit)
END_EVENT_HANDLER()

void Page::copyData(const Page& org, const bool cc)
{
   BaseClass::copyData(org);

   // copy subpages
   if (!cc && subpages != nullptr) { subpages->unref(); }
   if (org.subpages != nullptr) {
      subpages = org.subpages->clone();
      processSubpages();
   } else {
      subpages = nullptr;
   }

   // copy events
   if (!cc && pageChgEvents != nullptr) {
      pageChgEvents->unref();
   }
   if (org.pageChgEvents != nullptr) {
      pageChgEvents = org.pageChgEvents->clone();
   } else {
      pageChgEvents = nullptr;
   }

   // select the same pages
   cPageName = org.cPageName;
   cPage = nullptr;
   nPage = nullptr;

   // clear the subpage stack
   clearSubpageStack();

   // post draw flag
   postDraw1 = org.postDraw1;

   focusSlavedToSubpage = org.focusSlavedToSubpage;

   // at this time we are not copying the paging arguments.
   pageArg = nullptr;
   caller = nullptr;
}

void Page::deleteData()
{
   // De-select the current pages
   cPage = nullptr;

   // delete list of subpages
   if (subpages != nullptr) {
      subpages->unref();
   }
   subpages = nullptr;

   // delete list of events
   if (pageChgEvents != nullptr) {
      pageChgEvents->unref();
   }
   pageChgEvents = nullptr;
}

//------------------------------------------------------------------------------
// updateTC() -- Update time critical stuff here
//------------------------------------------------------------------------------
void Page::updateTC(const double dt)
{
   // update our subpage and base class
   if (cPage != nullptr) {
      cPage->tcFrame(dt);
   }
   BaseClass::updateTC(dt);
}

//------------------------------------------------------------------------------
// updateData() --
//------------------------------------------------------------------------------
void Page::updateData(const double dt)
{
   // ---
   // Prime the pump -- when we don't have a current subpage (cp) and we do
   // have a subpage name (cPageName) then make it our current subpage
   // ---
   if (cPage == nullptr && (cPageName.length() != 0)) {
      base::Pair* p{};
      if (subpages != nullptr) {
         p = subpages->findByName(cPageName.c_str());
      }
      if (p != nullptr) {
         nPage = static_cast<Page*>(p->object());
         nPage->caller  = nullptr;
         nPage->pageArg = nullptr;
      }
   }

   // ---
   // Check for page changes
   // ---
   if (nPage != nullptr) {
      if (nPage != cPage) {
         // When we have a new page ...
         //  1) inform the old page of EXIT and remove focus
         //  2) set the current page pointer
         //  3) inform the new page of the ENTRY and set focus
         if (cPage != nullptr) {
            cPage->event(ON_EXIT);
            if (isFocusSlavedToSubpage()) {
               getDisplay()->focus(this);
            }
         }
         cPage = nPage;
         if (cPage != nullptr) {
            if (isFocusSlavedToSubpage()) {
               getDisplay()->focus(cPage);
            }
            cPage->event(ON_ENTRY);
         }
      }
   }

   // update our subpage and base class
   if (cPage != nullptr) {
      cPage->updateData(dt);
   }
   BaseClass::updateData(dt);
}

//------------------------------------------------------------------------------
// reset() -- Reset parameters
//------------------------------------------------------------------------------
void Page::reset()
{
   BaseClass::reset();
   if (subpages != nullptr) {
      // reset all of our sub-pages
      base::List::Item* item{subpages->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto obj = static_cast<IComponent*>(pair->object());
         if (obj != nullptr) obj->reset();
         item = item->getNext();
      }
   }
}

//------------------------------------------------------------------------------
// findBySelectName() -- find one of our components by its GL Select (pick) name
//                    (our children first then grandchildren)
//------------------------------------------------------------------------------
base::Pair* Page::findBySelectName(const GLuint name)
{
   // use our base class's functions to check normal components
   base::Pair* q{Graphic::findBySelectName(name)};

   // If still not found, check our subpage ...
   if (q == nullptr && cPage != nullptr) {
      q = cPage->findBySelectName(name);
   }
   return q;
}

//------------------------------------------------------------------------------
// newSubpage() -- change subpages
//------------------------------------------------------------------------------
bool Page::newSubpage(Page* const np1, Page* theCaller, base::Object* theArg)
{
   bool ok{};
   if (np1 != nullptr) {
      cPageName.empty();       // unknown name
      nPage = np1;
      nPage->pageArg = theArg;
      nPage->caller  = theCaller;
      clearSubpageStack();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// newSubpage() -- change subpages by name; returns true of page was found
//------------------------------------------------------------------------------
bool Page::newSubpage(const std::string& name, Page* theCaller, base::Object* theArg)
{
   bool ok{};
   base::Pair* p{};
   if (subpages != nullptr) {
      p = subpages->findByName(name.c_str());
   }
   if (p != nullptr) {
      cPageName = name;            // it's our page
      nPage = static_cast<Page*>(p->object());
      nPage->pageArg = theArg;
      nPage->caller  = theCaller;
      clearSubpageStack();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// clearSubpageStack() -- Clear the subpage stack
//------------------------------------------------------------------------------
bool Page::clearSubpageStack()
{
   subpageSP = SUBPAGE_STACK_SIZE;
   return true;
}

//------------------------------------------------------------------------------
// pushSubpage() -- push the current subpage and change to new subpage 'name'.
//------------------------------------------------------------------------------
bool Page::pushSubpage(const std::string& name, Page* theCaller, base::Object* theArg)
{
   bool ok{};
   if (subpageSP > 0) {
      Page* currPage{cPage};
      base::Pair* p{};
      if (subpages != nullptr) p = subpages->findByName(name.c_str());
      if (p != nullptr) {
         cPageName = name;                   // it's our page
         nPage = static_cast<Page*>(p->object());
         nPage->pageArg = theArg;
         nPage->caller  = theCaller;
         ok = true;
      }
      // ok = newSubpage(name,theCaller,theArg);
      if (ok) subpageStack[--subpageSP] = currPage;
   }
   return ok;
}

//------------------------------------------------------------------------------
// popSubpage() -- pop to the previous subpage on the stack
//------------------------------------------------------------------------------
bool Page::popSubpage(Page* theCaller, base::Object* theArg)
{
   bool ok{};
   if (subpageSP < SUBPAGE_STACK_SIZE) {
      //ok = newSubpage(subpageStack[subpageSP++],theCaller,theArg);
      cPageName.empty();               // unknown name
      nPage = subpageStack[subpageSP++];
      nPage->pageArg = theArg;
      nPage->caller  = theCaller;
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Functions that ask our container to change pages (we're outa here)
//------------------------------------------------------------------------------

// new page by pointer
bool Page::newPage(Page* const newPage, Page* theCaller, base::Object* theArg)
{
   bool ok{};
   const auto cc = dynamic_cast<Page*>(container());
   if (cc != nullptr) ok = cc->newSubpage(newPage,theCaller,theArg);
   return ok;
}

// new page by name
bool Page::newPage(const std::string& name, Page* theCaller, base::Object* theArg)
{
   bool ok{};
   const auto cc = dynamic_cast<Page*>(container());
   if (cc != nullptr) {
      ok = cc->newSubpage(name, theCaller, theArg);
   }
   return ok;
}

// push new page
bool Page::pushPage(const std::string& name, Page* theCaller, base::Object* theArg)
{
   bool ok{};
   const auto cc = dynamic_cast<Page*>(container());
   if (cc != nullptr) ok = cc->pushSubpage(name, theCaller, theArg);
   return ok;
}

// pop back to previous page
bool Page::popPage(Page* theCaller, base::Object* theArg)
{
   bool ok{};
   const auto cc = dynamic_cast<Page*>(container());
   if (cc != nullptr) ok = cc->popSubpage(theCaller, theArg);
   return ok;
}

//------------------------------------------------------------------------------
// findSubpageByName(), findSubpageByType() -- find a member
//------------------------------------------------------------------------------
base::Pair* Page::findSubpageByName(const char* const name)
{
   base::Pair* p{};
   if (subpages != nullptr) {
      p = subpages->findByName(name);
   }
   return p;
}

base::Pair* Page::findSubpageByType(const std::type_info& type)
{
   base::Pair* p{};
   if (subpages != nullptr) {
      p = subpages->findByType(type);
   }
   return p;
}


//------------------------------------------------------------------------------
// onButtonHit() -- handle button hits (with button name) as page change requests
//------------------------------------------------------------------------------
bool Page::onButtonHit(const base::String* const obhobj)
{
   bool used{};
   if (obhobj != nullptr && pageChgEvents != nullptr) {
      used = true;
      base::Pair* pageEvent{pageChgEvents->findByName((*obhobj).c_str())};
      if (pageEvent != nullptr) {
         const auto id = dynamic_cast<base::Identifier*>(pageEvent->object());
         if (id != nullptr) {
            // Find our container and the new page ID, then push
            // current page and go to new page
            newPage((*id).c_str(), this);
         }
      }
   }
   return used;
}


//------------------------------------------------------------------------------
// onKeyHit() -- handle keyboard inputs as page change requests
//------------------------------------------------------------------------------
bool Page::onKeyHit(const int key)
{
   bool used{};
   if (pageChgEvents != nullptr) {
      char keyName[2]{};
      keyName[0] = static_cast<char>(key);
      keyName[1] = '\0';

      // search for a page change event
      base::Pair* pageEvent{pageChgEvents->findByName(keyName)};
      if (pageEvent != nullptr) {
         const auto id = dynamic_cast<base::Identifier*>(pageEvent->object());
         if (id != nullptr) {
            // Find our container and the new page ID, then push
            // current page and go to new page
            newPage((*id).c_str(), this);
            used = true;
          }
      }
   }
   return used;
}

//------------------------------------------------------------------------------
// onEntry() --
//------------------------------------------------------------------------------
bool Page::onEntry()
{
   // send our focus to our subpage, if we have one.
   if (subpage() != nullptr) {
      getDisplay()->focus( subpage() );
      subpage()->event(ON_ENTRY);
   }
   return true;
}

//------------------------------------------------------------------------------
// onExit() --
//------------------------------------------------------------------------------
bool Page::onExit()
{
   if (subpage() != nullptr) {
      subpage()->event(ON_EXIT);
   }
   return true;
}

//------------------------------------------------------------------------------
// draw -- draw this object and its children
//------------------------------------------------------------------------------
void Page::draw()
{
   // only when this object is visible ...
   if ( !isVisible() ) return;

    if (postDraw1) {
        // draw the current subpage and ...
        if (cPage != nullptr) {
           cPage->draw();
        }
        // then draw our own page components
        BaseClass::draw();
    } else {
        // draw our own page components and ...
        BaseClass::draw();
        // then draw the current subpage
        if (cPage != nullptr) {
           cPage->draw();
        }
    }

}

//------------------------------------------------------------------------------
// processSubpages() -- process our subpages; make sure they are all of
// type Page (or derived from it)and tell them that we are their
// container.
//------------------------------------------------------------------------------
bool Page::processSubpages()
{
   bool ok{true};
   if (subpages != nullptr) {
      // Make sure we have only Pages and tell all of the pages
      // that we are their container.
      const base::List::Item* item{subpages->getFirstItem()};
      while (ok && item != nullptr) {
         const auto p = const_cast<base::Pair*>(static_cast<const base::Pair*>(item->getValue()));
         item = item->getNext();
         const auto g = dynamic_cast<Page*>(p->object());
         if (g != nullptr) {
            // It MUST be of type Page
            g->container(this);
         } else {
            // Delete components that are not of Page type
            if (isMessageEnabled(MSG_ERROR)) {
               std::cerr << "Page::processSubpages(): " << p->slot() << " is not a Page!" << std::endl;
            }
            ok = false;
         }
      }
   }
   return ok;
}

//------------------------------------------------------------------------------
// slot methods
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// sets the initial page
//------------------------------------------------------------------------------
bool Page::setSlotPage(const base::Identifier* const x)
{
   cPageName = x->asString();
   return true;
}

//------------------------------------------------------------------------------
//  setSlotSubpageStream() -- it takes a pair stream
//------------------------------------------------------------------------------
bool Page::setSlotSubpageStream (base::PairStream* const x)
{
   // When a PairStream (i.e., more than one, a list) of pages
   if (subpages != nullptr) subpages->unref();
   subpages = x;
   subpages->ref();
   return processSubpages();
}

//------------------------------------------------------------------------------
//  setSlotSubpageSingle() -- it takes an Page
//------------------------------------------------------------------------------
bool Page::setSlotSubpageSingle(Page* const x)
{
   if (subpages != nullptr) subpages->unref();
   subpages = new base::PairStream();
   subpages->put( new base::Pair("1", x) );
   return processSubpages();
}

//------------------------------------------------------------------------------
// setSlotPagingEvent() -- sets the page change event
//------------------------------------------------------------------------------
bool Page::setSlotPagingEvent(base::PairStream* const x)
{
   if (pageChgEvents != nullptr) {
      pageChgEvents->unref();
   }
   pageChgEvents = x;
   pageChgEvents->ref();
   return true;
}

//------------------------------------------------------------------------------
// setSlotDrawSubpageFirst() -- Draw subpages first (default: draw our page graphics first)
//------------------------------------------------------------------------------
bool Page::setSlotDrawSubpageFirst(const base::Boolean* const x)
{
   postDraw1 = x->asBool();
   return true;
}

//------------------------------------------------------------------------------
// drawSubpageFirst() -- Draw subpages first (default: draw our page graphics first)
//------------------------------------------------------------------------------
bool Page::setSlotFocusSlavedToSubpage(const base::Boolean* const x)
{
   setFocusSlavedToSubpage( x->asBool() );
   return true;
}

}
}
