
#ifndef __mixr_models_common_Iff_HPP__
#define __mixr_models_common_Iff_HPP__

#include "mixr/models/system/Radio.hpp"

namespace mixr {
namespace base { class Boolean; class Integer; class Number; }
namespace models {

//------------------------------------------------------------------------------
// Class: Iff
//
// Description: Generic class for all IFF systems (a.k.a. the SQUAWK box)
//
// Factory name: Iff
// Slots:
//   mode1         <Integer>   ! Mode 1 Code   (range: 00 to 073 octal) (default: 0)
//                             ! -- first digit 0 to 7, second digit 0 to 3 (default: 0)
//   mode2         <Integer>   ! Mode 2 Code   (range: 0000 to 07777 octal) (default: 0)
//   mode3a        <Integer>   ! Mode 3a Code  (range: 0000 to 07777 octal) (default: 0)
//   mode4a        <Integer>   ! Mode 4a Code (default: 0)
//   mode4b        <Integer>   ! Mode 4b Code (default: 0)
//   enableMode1   <Boolean>   ! Mode 1 on (default: false)
//   enableMode2   <Boolean>   ! Mode 2 on (default: false)
//   enableMode3a  <Boolean>   ! Mode 3a on (default: false)
//   enableMode4   <Boolean>   ! Mode 4a on (default: false)
//   whichMode4    <Boolean>   ! Which mode 4 are we using (A == false; B == true)
//                             ! (default: false)
//   enableModeC   <Boolean>   ! Mode C on (default: false)
//
//------------------------------------------------------------------------------
class Iff : public Radio
{
   DECLARE_SUBCLASS(Iff, Radio)

public:
   enum {
      PWR_LOW = BaseClass::PWR_LAST,
      PWR_EMERGENCY,
      PWR_LAST       // Hook for subclasses to expand
   };

public:
   Iff();

   bool isEnabledMode1() const                     { return enableMode1; }
   unsigned short getMode1() const                 { return mode1; }
   virtual bool setMode1(const unsigned short m);
   virtual void setEnabledMode1(const bool flg);

   bool isEnabledMode2() const                     { return enableMode2; }
   unsigned short getMode2() const                 { return mode2; }
   virtual bool setMode2(const unsigned short m);
   virtual void setEnabledMode2(const bool flg);

   bool isEnabledMode3a() const                    { return enableMode3a; }
   unsigned short getMode3a() const                { return mode3a; }
   virtual bool setMode3a(const unsigned short m);
   virtual void setEnabledMode3a(const bool flg);

   bool isEnabledMode4() const                     { return enableMode4; }
   unsigned short getMode4A() const                { return mode4a; }
   unsigned short getMode4B() const                { return mode4b; }
   bool getWhichMode4() const                      { return whichMode4; }  // False (4A), True(4B)
   virtual bool setMode4A(const unsigned short m);
   virtual bool setMode4B(const unsigned short m);
   virtual void setEnabledMode4(const bool flg);
   virtual bool setWhichMode4(const bool newM);
   virtual bool zeroMode4Codes();

   bool isEnabledModeC() const                     { return enableModeC; }
   virtual void setEnabledModeC(const bool flg);

   void reset() override;

private:
   // Codes
   unsigned short mode1 {};      // Mode 1 Code
   unsigned short mode2 {};      // Mode 2 Code
   unsigned short mode3a {};     // Mode 3a Code
   unsigned short mode4a {};     // Mode 4a Codes
   unsigned short mode4b {};     // Mode 4b Codes

   // Enabled flags
   bool enableMode1 {};          // Mode 1 on
   bool enableMode2 {};          // Mode 2 on
   bool enableMode3a {};         // Mode 3a on
   bool enableMode4 {};          // Mode 4 on
   bool enableModeC {};          // Mode C on

   // Initial (reset) conditions (set by slot functions)
   unsigned short icMode1 {};    // Initial Mode 1 Code
   unsigned short icMode2 {};    // Initial Mode 2 Code
   unsigned short icMode3a {};   // Initial Mode 3a Code
   unsigned short icMode4a {};   // Initial Mode 4a Code
   unsigned short icMode4b {};   // Initial Mode 4b Code
   bool icMode1Flg {};           // Initial Mode 1 flag
   bool icMode2Flg {};           // Initial Mode 2 flag
   bool icMode3aFlg {};          // Initial Mode 3a flag
   bool icMode4Flg {};           // Initial Mode 4 flag
   bool icModeCFlg {};           // Initial Mode C flag
   bool whichMode4 {};           // Tells us which mode 4 we are using (A/B)
   bool icWhichMode4 {};         // Initial Mode 4 mode

private:
   // slot table helper methods
   bool setSlotMode1(const base::Integer* const);
   bool setSlotMode2(const base::Integer* const);
   bool setSlotMode3a(const base::Integer* const);
   bool setSlotMode4a(const base::Integer* const);
   bool setSlotMode4b(const base::Integer* const);
   bool setSlotEnableMode1(const base::Boolean* const);
   bool setSlotEnableMode2(const base::Boolean* const);
   bool setSlotEnableMode3a(const base::Boolean* const);
   bool setSlotWhichMode4(const base::Boolean* const);
   bool setSlotEnableMode4(const base::Boolean* const);
   bool setSlotEnableModeC(const base::Boolean* const);
};

}
}

#endif
