
#ifndef __mixr_graphics_Page_HPP__
#define __mixr_graphics_Page_HPP__

#include "Graphic.hpp"
#include "mixr/base/Identifier.hpp"

#include <array>
#include <string>

namespace mixr {
namespace base { class Boolean; class Pair; class PairStream; class String; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: Page
// Description: Page manages a list of graphic objects and methods to manage
//              them - this includes subpages and a background page
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: Page
// Slots:
//  page                 <Identifier>   ! initial subpage (default: 0)
//  pages                <PairStream>   ! subpages (default: 0)
//  pages                <Page>
//  pagingEvent          <PairStream>
//  subpagesFirst        <Boolean>      ! draw subpages first (default: draw our page graphics first)
//  focusSlavedToSubpage <Boolean>      ! slave the focus to the subpage (default: true)
//------------------------------------------------------------------------------
// Events:
//  ON_ENTRY      <>        ! sets focus to our subpage, if we have one
//  ON_EXIT       <>        !
//  BUTTON_HIT    <String>  ! handle button hits (with button name) as page change requests
//  ON_ANYKEY     <int>     ! handle keyboard inputs as page change requests
//------------------------------------------------------------------------------
class Page : public Graphic
{
   DECLARE_SUBCLASS(Page, Graphic)

public:
   Page()                                       { STANDARD_CONSTRUCTOR() }

   const char* subpageName() const              { return cPageName.c_str(); }
   Page* subpage() const                        { return cPage; }

   bool isPostDrawSubpage() const               { return postDraw1; }

   // returns a member from the name given, else nullptr
   virtual base::Pair* findSubpageByName(const char* const);
   // returns a member of the given type, else nullptr
   virtual base::Pair* findSubpageByType(const std::type_info&);

   // returns true if the focus is slaved to a subpage
   bool isFocusSlavedToSubpage() const          { return focusSlavedToSubpage; }
   void setFocusSlavedToSubpage(const bool x)   { focusSlavedToSubpage = x; }

   // call new sub-page
   // changes subpages by page, returns true if page was found
   bool newSubpage(Page* const newPage, Page* theCaller, base::IObject* theArg = nullptr);
   // changes subpages by name, returns true if the page was found
   bool newSubpage(const std::string& name, Page* theCaller, base::IObject* theArg = nullptr);

   // event handlers
   virtual bool onEntry();
   virtual bool onExit();
   // handles the button hit as a page change event
   virtual bool onButtonHit(const base::String* const);
   // handles the keyboard hit as a page change event
   virtual bool onKeyHit(const int key);

   void draw() override;
   base::Pair* findBySelectName(const GLuint name) override;
   bool event(const int event, base::IObject* const obj = nullptr) override;

   void updateTC(const double dt = 0.0) override;
   void updateData(const double dt = 0.0) override;
   void reset() override;

protected:
   // returns our paging arguments
   base::IObject* getArgument()              { return pageArg; }
   const Page* getCaller()                   { return caller; }

   // returns our subpages
   base::PairStream* subPages()              { return subpages; }

   // manage our (sub)page stack
   bool clearSubpageStack();
   bool pushSubpage(const std::string& name, Page* theCaller, base::IObject* theArg = nullptr);
   bool popSubpage(Page* theCaller, base::IObject* theArg = nullptr);

   // call/push/pop major pages (our container's pages, which we are a member of)
   bool newPage(Page* const newPage, Page* theCaller, base::IObject* theArg = nullptr);
   bool newPage(const std::string& name, Page* theCaller, base::IObject* theArg = nullptr);
   bool pushPage(const std::string& name, Page* theCaller, base::IObject* theArg = nullptr);
   bool popPage(Page* theCaller, base::IObject* theArg = nullptr);

private:
   bool processSubpages();

   Page* cPage{};                       // current subpage
   std::string cPageName;               // current subpage name
   Page* nPage{};                       // new subpage (requesting a page change)

   base::PairStream* subpages {};       // subpages
   base::PairStream* pageChgEvents {};  // page change events

   bool postDraw1 {};                   // post draw component (child) graphic
   bool focusSlavedToSubpage {true};    // input event focus should follow subpage changes

   // passed by calling page
   base::safe_ptr<base::IObject> pageArg;     // paging argument
   const Page* caller{};                     // calling page

   // subpage stack
   static const std::size_t SUBPAGE_STACK_SIZE {50};
   std::array<Page*, SUBPAGE_STACK_SIZE> subpageStack {};
   std::size_t subpageSP {SUBPAGE_STACK_SIZE};           // stack pointer

private:
   // slot table helper methods
   bool setSlotPage(const base::Identifier* const);
   bool setSlotSubpageStream(base::PairStream* const);
   bool setSlotSubpageSingle(Page* const);
   bool setSlotPagingEvent(base::PairStream* const);
   bool setSlotDrawSubpageFirst(const base::Boolean* const);
   bool setSlotFocusSlavedToSubpage(const base::Boolean* const);
};

}
}

#endif
