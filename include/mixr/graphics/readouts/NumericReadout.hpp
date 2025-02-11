
#ifndef __mixr_graphics_NumericReadout_HPP__
#define __mixr_graphics_NumericReadout_HPP__

#include "mixr/graphics/readouts/IReadout.hpp"

#include "mixr/base/util/constants.hpp"

namespace mixr {
namespace base { class Boolean; class Object; class String; class Float; class Integer; }
namespace graphics {
class ReformatScanner;

//------------------------------------------------------------------------------
// Class: NumericReadout
//------------------------------------------------------------------------------
// EDL Interface:
//
// Factory name: NumericReadout
// Slots:
//   value           <Float>     ! Value to be displayed (default: 0.0)
//   value           <Integer>   ! Value to be displayed
//   maxValue        <Float>     ! Maximum floating point value that can be displayed (default: UNDEFINED_VALUE)
//   maxValue        <Integer>   ! Maximum integer value that can be displayed
//   format          <String>    ! Example format text (default: '\0')
//   plusChar        <String>    ! Positive value character (default: '\0')
//   minusChar       <String>    ! Negative value character (default: '\0')
//   dpChar          <String>    ! Decimal Point character (default: '\0')
//   undefinedChar   <String>    ! Undefined value character (default: '-')
//   overflowChar    <String>    ! Overflow character (default: '+')
//   maxValid        <Number>    ! Maximum valid input (default: UNDEFINED_VALUE)
//   minValid        <Number>    ! Minimum valid input (default: UNDEFINED_VALUE)
//   blankIfZero     <Boolean>   ! Display blanks if value is zero
//------------------------------------------------------------------------------
// Example formats:
//
//    ######    // Integer
//    +#####    // Integer w/plus sign
//    00000#    // Integer w/leading zeros
//    +0000#    // Integer w/plus sign & leading zeros
//    ###.##    // Float w/2 right of decimal point
//    +##.##    // Float w/plus sign, 2 right of decimal point
//    00#.##    // Float w/2 right of decimal point & leading zeros
//    +0#.##    // Float w/plus sign, 2 right of decimal point, & leading zeros
//------------------------------------------------------------------------------
class NumericReadout : public IReadout
{
   DECLARE_SUBCLASS(NumericReadout, IReadout)

public:
   NumericReadout();

   // get num as an int
   int getInt() const                           { return static_cast<int>(num); }
   // get num as a float
   double getFloat() const                      { return num; }

   // sets num to v as an double (in both cases) then redisplays the value
   void setValue(const int v)                   { num = static_cast<double>(v); redisplay(); }
   void setValue(const double v)                { num = v; redisplay(); }

   // sets maxNum to v as an double (in both cases) then redisplays the value
   void setMaxValue(const int v)                { maxNum = static_cast<double>(v); redisplay(); }
   void setMaxValue(const double v)             { maxNum = v; redisplay(); }

   // gets the input value
   double getInputValue() const override;
   // checks the input value and returns true if it is valid
   bool isInputValueValid() const override;
   char filterInputEvent(const int event, const char tc) override;

   bool event(const int key, base::Object* const obj = nullptr) override;
   void updateData(const double dt = 0.0) override;

   // event handler methods
   virtual bool onUpdateValue(const base::Float* const);
   virtual bool onUpdateValue(const base::Integer* const);
   virtual bool onUpdateValue(const base::Number* const);

protected:
   virtual void makeText();
   virtual void redisplay();
   virtual void reformat(const char* const example);

   static const std::size_t CBUF_LENGTH{32};    // Max length of cbuf
   static const std::size_t FORMAT_LENGTH{32};  // Max length of format

   char cbuf[CBUF_LENGTH]{};       // buffer
   char format[FORMAT_LENGTH]{};   // Current format string

   char plusChar{};                // Positive value character
   char minusChar{};               // Negative value character
   char dpChar{};                  // Decimal point character
   char undefinedChar{'-'};        // Undefined value character
   char overflowChar{'*'};         // Overflow character
   bool postSign{};                // If true, sign char is at end of string

   static ReformatScanner* reformatter;      // Generates format statements by example

private:
   double num{};                             // Value as double
   double maxNum{base::UNDEFINED_VALUE};     // Maximum value
   double maxValid{base::UNDEFINED_VALUE};   // Maximum valid input value
   double minValid{base::UNDEFINED_VALUE};   // Minimum valid input value
   bool blankZero{};                         // Display blank instead of zero value

private:
   // slot table helper methods
   bool setSlotFloatToBeDisplayed(const base::Float* const);
   bool setSlotIntegerToBeDisplayed(const base::Integer* const);
   bool setSlotFloatMaxValue(const base::Float* const);
   bool setSlotIntegerMaxValue(const base::Integer* const);
   bool setSlotExampleFormatText(const base::String* const);
   bool setSlotPlusChar(const base::String* const);
   bool setSlotMinusChar(const base::String* const);
   bool setSlotDecimalPointChar(const base::String* const);
   bool setSlotUndefinedChar(const base::String* const);
   bool setSlotOverflowChar(const base::String* const);
   bool setSlotMaxValid(const base::Number* const);
   bool setSlotMinValid(const base::Number* const);
   bool setSlotBlankZero(const base::Boolean* const);
};

}
}

#endif
