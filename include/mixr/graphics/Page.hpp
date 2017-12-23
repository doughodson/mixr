
#ifndef __mixr_graphics_Page_H__
#define __mixr_graphics_Page_H__

#include "Graphic.hpp"
#include "mixr/base/Identifier.hpp"

#include <array>

namespace mixr {
namespace graphics {

//------------------------------------------------------------------------------
// Class: Page
//
// Description: Page format. A list of graphic objects and methods to manage
//              them as a page, also contains subpages and a background page.
// Factory name: Page
//
// Slots:
//  page                 <Identifier>   ! Initial subpage (default: 0)
//  pages                <PairStream>   ! Subpages (default: 0)
//  pagingEvent          <Page>         ! Page change event (default: 0)
//  subpagesFirst        <PairStream>   ! Draw subpages first (default: draw our page graphics first)
//  focusSlavedToSubpage <Number>       ! Slave the focus to the subpage (default: true)
//
// Events
//  1. ON_ENTRY
//  2. ON_EXIT
//  3. BUTTON_HIT
//  4. ON_ANYKEY
//
// Public member functions:
//  char* subpageName()
//    Returns cpName.
//
//  Page* subpage()
//    Returns cp.
//
//  bool isPostDrawSubpage()
//    Returns postDraw1
//
//  Pair* findSubpageByName(char* slotname)
//    Returns a member from the slot name given, else 0.
//
//  Pair* findSubpageByType(std::type_info& type)
//    Returns a member of the given type, else 0.
//
//  bool isFocusSlavedToSubpage()
//    Returns true if the focus is slaved to a subpage.
//
//  void setFocusSlavedToSubpage(bool f)
//    Sets focusSlavedToSubpage to f.
//
//  bool onButtonHit(String* obhobj)
//    Handles the button hit as a page change event.
//
//  bool onKeyHit(int key)
//    Handles the keyboard hit as a page change event.
//
// Call new sub-page
//  bool newSubpage(Page* newPage, Page* theCaller, Object* theArg)
//    Changes subpages by page. Returns true if page was found.
//
//  bool newSubpage(char* name, Page* theCaller, Object* theArg)
//    Changes subpages by name. Returns true if the page was found.
//------------------------------------------------------------------------------
class Page : public Graphic
{
   DECLARE_SUBCLASS(Page, Graphic)

public:
   Page();

   const char* subpageName() const              { return cpName; }
   Page* subpage() const                        { return cp; }

   bool isPostDrawSubpage() const               { return postDraw1; }

   virtual base::Pair* findSubpageByName(const char* const slotname);
   virtual base::Pair* findSubpageByType(const std::type_info& type);

   bool isFocusSlavedToSubpage() const          { return focusSlavedToSubpage; }
   void setFocusSlavedToSubpage(const bool f)   { focusSlavedToSubpage = f; }

   // call new sub-page
   bool newSubpage(Page* const newPage, Page* theCaller, base::Object* theArg = nullptr);
   bool newSubpage(const char* const name, Page* theCaller, base::Object* theArg = nullptr);

   // event handler functions
   virtual bool onEntry();
   virtual bool onExit();
   virtual bool onButtonHit(const base::String* const obhobj);
   virtual bool onKeyHit(const int key);

   void draw() override;
   base::Pair* findBySelectName(const GLuint name) override;
   bool event(const int event, base::Object* const obj = nullptr) override;

   void updateTC(const double dt = 0.0) override;
   void updateData(const double dt = 0.0) override;
   void reset() override;

protected:
   // Return our paging arguments
   base::Object* getArgument()               { return pageArg; }
   const Page* getCaller()                   { return caller; }

   // Return our subpages
   base::PairStream* subPages()              { return subpages; }

   // Manage our (sub)page stack
   bool clearSubpageStack();
   bool pushSubpage(const char* const name, Page* theCaller, base::Object* theArg = nullptr);
   bool popSubpage(Page* theCaller, base::Object* theArg = nullptr);

   // Call/push/pop major pages (our container's pages, which we are a member of)
   bool newPage(Page* const newPage, Page* theCaller, base::Object* theArg = nullptr);
   bool newPage(const char* const name, Page* theCaller, base::Object* theArg = nullptr);
   bool pushPage(const char* const name, Page* theCaller, base::Object* theArg = nullptr);
   bool popPage(Page* theCaller, base::Object* theArg = nullptr);

private:
   bool processSubpages();

   Page* cp {};                         // Current Subpage
   base::Identifier cpName;             // Current Subpage Name
   Page* np {};                         // New subpage (requesting a page change)

   base::PairStream* subpages {};       // Subpages
   base::PairStream* pageChgEvents {};  // Page change events

   bool postDraw1 {};                   // Post draw component (child) graphic
   bool focusSlavedToSubpage {true};    // Input event focus should follow subpage changes

   // Passed by calling page
   base::safe_ptr<base::Object> pageArg;     // Paging argument
   const Page* caller {};                    // Calling page

   // Subpage Stack
   static const int SUBPAGE_STACK_SIZE {50};
   std::array<Page*, SUBPAGE_STACK_SIZE> subpageStack {};
   int subpageSP {SUBPAGE_STACK_SIZE};       // Stack pointer

private:
   // slot table helper methods
   bool setSlotPage(const base::Identifier* const);
   bool setSlotSubpageStream(base::PairStream* const);
   bool setSlotSubpageSingle(Page* const);
   bool setSlotPagingEvent(base::PairStream* const);
   bool setSlotDrawSubpageFirst(const base::Number* const);
   bool setSlotFocusSlavedToSubpage(const base::Number* const);
};

}
}

#endif
