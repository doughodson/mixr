
#ifndef __mixr_graphics_AbstractReadout_HPP__
#define __mixr_graphics_AbstractReadout_HPP__

#include "mixr/graphics/Graphic.hpp"
#include "mixr/base/String.hpp"

#include "mixr/base/util/str_utils.hpp"
#include <string>

namespace mixr {
namespace base { class Identifier; class Integer; class List; }
namespace graphics {

//------------------------------------------------------------------------------
// Class: AbstractReadout
// Description: Abstract class for the readout classes
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: AbstractReadout
// Slots:
//      position       <List>        ! starting position ( Line Column ) (default: 0)
//      width          <Integer>     ! field width (default: 0)
//      highLight      <Boolean>     ! highlight text flag  (display mode = 0x1001, default: false)
//      underline      <Boolean>     ! underlined text flag  (display mode = 0x1002, default: false)
//      reversed       <Boolean>     ! reversed video flag  (display mode = 0x1004, default: false)
//      justification  <Identifier>  ! { none, left, center, right } (default: "none")
//      vertical       <Boolean>     ! vertical field flag (text displayed vertically) (display mode = 0x1010, default: false)
//      brackets       <Boolean>     ! bracket the field with a character just left
//                                   ! and right of this field. (display mode = 0x1020, default: false)
//      linked         <Boolean>     ! linked flag (auto step to next input field) (default: false)
//      inheritColor   <Boolean>     ! inherit color of our container (default: false)
//      font           <Identifier>  ! specific font name we wish to use (default: 0)
//      startCharPos   <Integer>     ! our starting character position (we may want to skip!) (default: 0)
//------------------------------------------------------------------------------
class AbstractReadout : public Graphic
{
   DECLARE_SUBCLASS(AbstractReadout, Graphic)

public:
   enum class Mode { Display, Input };

   enum {
          // display modes
          highlight  = 0x0001,
          underline  = 0x0002,
          reversed   = 0x0004,
          special    = 0x0008,
          vertical   = 0x0010,
          brackets   = 0x0020,
        };

public:
   AbstractReadout();

   virtual void line(const int x)            { ln = x; return; }             // set line number
   virtual int line() const                  { return ln;      }             // return line number

   virtual void column(const int x)          { cp = x; return; }             // set column number
   virtual int column() const                { return cp;      }             // return column number

   void width(const std::size_t x)           { w = x; adjust(); return; }    // set width
   std::size_t width() const                 { return w;                }    // return width

   Mode setMode(const Mode);
   Mode getMode() const                      { return mode; }                // return mode of field

   bool isHighLighted() const                { return (dmode & highlight) != 0; }  // highlighted?
   bool isUnderlined() const                 { return (dmode & underline) != 0; }  // underlined?
   bool isReversed() const                   { return (dmode & reversed)  != 0; }  // reversed?
   bool isSpecial() const                    { return (dmode & special)   != 0; }  // special?
   bool isVertical() const                   { return (dmode & vertical)  != 0; }  // vertical?
   bool areBracketsOn() const                { return (dmode & brackets)  != 0; }  // brackets on?

   void setDisplayMode(const int x)          { dmode |= x;   }                     // set display mode
   bool isDisplayMode(const int x) const     { return (dmode & x) != 0; }          // display mode?
   void clearDisplayMode(const int x)        { dmode &= ~x;  }                     // clear display mode
   void clearAllDisplayModes()               { dmode = 0;    }                     // clear all modes

   base::Justify justification() const;                                // returns current justification mode
   base::Justify justification(const base::Justify);                   // sets justification

   virtual bool setPosition(const base::List* const);                  // set position to [Line Column]

   // returns true if location specified is within the current field.
   virtual bool withinField(const int ln, const int cp) const;
   virtual void setText(const char s[]);                               // sets text string

   // sets the input example to example, and returns the length of example
   virtual std::size_t setExample(const char* const);

   virtual void setChar(const char);                 // set the character if in input mode
   virtual char getChar();                           // get character if in input mode
   virtual void backSpace(const int ns = 1);         // backs up one space if in input mode
   virtual void advanceSpace(const int ns = 1);      // advance one space is in input mode
   virtual bool onForwardSpace();                    // calls advanceSpace() and returns true
   virtual bool onBackSpace();                       // calls backspace() and returns true

   // filters input event 'event' using template character 'x'
   virtual char filterInputEvent(const int event, const char x);
   // makes sure the current input position is valid
   virtual bool isValidInputPosition(const int);

   // returns 0.0 - this will need revision once the function does something
   virtual double getInputValue() const;
   // returns true - this will need revision once the function does something
   virtual bool isInputValueValid() const;
   // returns the current input character position
   int getInputCharacterPosition() const                 { return icp; }

   bool setInputCharacterPosition(const unsigned int);

   void setLinked(const bool f)                          { linked = f; }      // sets linked on/off
   bool isLinked() const                                 { return linked; }   // is linked?

   // returns true if inheritColor is on
   bool isInheritColor() const                           { return inheritColor; }
   // sets the inheritColor to i, then returns true
   bool setInheritColor(const bool i)                    { inheritColor = i; return true; }

   // sets the line number and returns true.
   virtual bool onSetLine(const base::Integer* const);
   // sets the column number and returns true.
   virtual bool onSetColumn(const base::Integer* const);

   const std::string getInputExample() const             { return inputExample; }
   int getCharacterPos() const                           { return icp; }

   // returns true if text cursor should be seen within this
   // object and the position of the cursor
   bool cursor(int* ln, int* cp) const override;

   void drawFunc() override;
   bool event(const int event, Object* const obj = nullptr) override;

   void updateData(const double dt = 0.0) override;

protected:
   std::string origStr;               // original text saved by setText
   std::string inputExample;          // input Template String
   base::String str;                  // text stored in field
   Mode mode {Mode::Display};         // current mode
   int icp{};                         // input character pointer
   int inpDspMode{};                  // auto switches to this display mode during input mode
   bool inpModeHold{};                // hold input mode until after first input character

private:
   void adjust()                      { str.setString(origStr.c_str(), w, jmode); }

   int ln{};                          // line this field is on
   int cp{};                          // starting character position of field
   std::size_t w{};                   // width of the field
   bool linked{};                     // linked field (on input)
   unsigned int startCP{};            // our starting character position that we "write" from

   int dmode{};                       // display mode flags
   base::Justify jmode{};             // justification mode
   bool inheritColor{};               // inherit color instead of using a default color
   base::Identifier* fontName{};      // name of the font we want our display to use (if overridden)

private:
   // slot table helper methods
   bool setSlotPosition(const base::List* const);
   bool setSlotWidth(const base::Integer* const);
   bool setSlotHighlight(const base::Boolean* const);
   bool setSlotUnderline(const base::Boolean* const);
   bool setSlotReversed(const base::Boolean* const);
   bool setSlotJustification(const base::Identifier* const);
   bool setSlotVertical(const base::Boolean* const);
   bool setSlotBrackets(const base::Boolean* const);
   bool setSlotLinked(const base::Boolean* const);
   bool setSlotInheritColor(const base::Boolean* const);
   bool setSlotFont(const base::Identifier* const);
   bool setSlotStartCharPos(const base::Integer* const);
};

}
}

#endif
