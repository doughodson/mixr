
#include "mixr/models/system/Iff.hpp"

#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/String.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Iff, "Iff")
EMPTY_DELETEDATA(Iff)

BEGIN_SLOTTABLE(Iff)
   "mode1",          //  1) Mode 1 Code   (range: 00 to 073 octal)
                     //                    -- first digit 0 to 7, second digit 0 to 3
   "mode2",          //  2) Mode 2 Code   (range: 0000 to 07777 octal)
   "mode3a",         //  3) Mode 3a Code  (range: 0000 to 07777 octal)
   "mode4a",         //  4) Mode 4a Code
   "mode4b",         //  5) Mode 4b Code
   "enableMode1",    //  6) Mode 1 on
   "enableMode2",    //  7) Mode 2 on
   "enableMode3a",   //  8) Mode 3a on
   "enableMode4",    //  9) Mode 4a on
   "whichMode4",     // 10) Which mode 4 are we using (A == false; B == true)
   "enableModeC",    // 11) Mode C on
END_SLOTTABLE(Iff)

BEGIN_SLOT_MAP(Iff)
   ON_SLOT( 1, setSlotMode1,        base::Integer)
   ON_SLOT( 2, setSlotMode2,        base::Integer)
   ON_SLOT( 3, setSlotMode3a,       base::Integer)
   ON_SLOT( 4, setSlotMode4a,       base::Integer)
   ON_SLOT( 5, setSlotMode4b,       base::Integer)
   ON_SLOT( 6, setSlotEnableMode1,  base::Boolean)
   ON_SLOT( 7, setSlotEnableMode2,  base::Boolean)
   ON_SLOT( 8, setSlotEnableMode3a, base::Boolean)
   ON_SLOT( 9, setSlotEnableMode4,  base::Boolean)
   ON_SLOT(10, setSlotWhichMode4,   base::Boolean)
   ON_SLOT(11, setSlotEnableModeC,  base::Boolean)
END_SLOT_MAP()

Iff::Iff()
{
   STANDARD_CONSTRUCTOR()
}

void Iff::copyData(const Iff& org, const bool)
{
   BaseClass::copyData(org);

   mode1 = org.mode1;
   mode2 = org.mode2;
   mode3a = org.mode3a;
   mode4a = org.mode4a;
   mode4b = org.mode4b;

   enableMode1 = org.enableMode1;
   enableMode2 = org.enableMode2;
   enableMode3a = org.enableMode3a;
   enableMode4 = org.enableMode4;
   enableModeC = org.enableModeC;
   whichMode4 = org.whichMode4;

   icMode1 = org.icMode1;
   icMode2 = org.icMode2;
   icMode3a = org.icMode3a;
   icMode4a = org.icMode4a;
   icMode4b = org.icMode4b;
   icMode1Flg = org.icMode1Flg;
   icMode2Flg = org.icMode2Flg;
   icMode3aFlg = org.icMode3aFlg;
   icMode4Flg = org.icMode4Flg;
   icModeCFlg = org.icModeCFlg;
   icWhichMode4 = org.icWhichMode4;
}

void Iff::reset()
{
   BaseClass::reset();

   setMode1(icMode1);
   setEnabledMode1(icMode1Flg);

   setMode2(icMode2);
   setEnabledMode2(icMode2Flg);

   setMode3a(icMode3a);
   setEnabledMode3a(icMode3aFlg);

   setMode4A(icMode4a);
   setEnabledMode4(icMode4Flg);
   setMode4B(icMode4b);

   setWhichMode4(icWhichMode4);

   setEnabledModeC(icModeCFlg);
}

bool Iff::setMode1(const unsigned short m)
{
   //mode1 = (m && 073);
   mode1 = m;
   return true;
}

bool Iff::setMode2(const unsigned short m)
{
   mode2 = m;
   return true;
}

bool Iff::setMode3a(const unsigned short m)
{
   mode3a = m;
   return true;
}

bool Iff::setMode4A(const unsigned short m)
{
   mode4a = m;
   return true;
}
bool Iff::setMode4B(const unsigned short m)
{
   mode4b = m;
   return true;
}

void Iff::setEnabledMode1(const bool flg)
{
   enableMode1 = flg;
}

void Iff::setEnabledMode2(const bool flg)
{
   enableMode2 = flg;
}

void Iff::setEnabledMode3a(const bool flg)
{
   enableMode3a = flg;
}

void Iff::setEnabledMode4(const bool flg)
{
    enableMode4 = flg;
}
void Iff::setEnabledModeC(const bool flg)
{
   enableModeC = flg;
}
bool Iff::setWhichMode4(const bool newM)
{
    whichMode4 = newM;
    return true;
}

// Slot functions
bool Iff::setSlotEnableMode1(const base::Boolean* const msg)
{
    if (msg != nullptr) setEnabledMode1(msg->asBool());
    // make sure we hold this reset value
    icMode1Flg = enableMode1;
    return true;
}

bool Iff::setSlotEnableMode2(const base::Boolean* const msg)
{
    if (msg != nullptr) setEnabledMode2(msg->asBool());
    icMode2Flg = enableMode2;
    return true;
}

bool Iff::setSlotEnableMode3a(const base::Boolean* const msg)
{
    if (msg != nullptr) setEnabledMode3a(msg->asBool());
    icMode3aFlg = enableMode3a;
    return true;
}

bool Iff::setSlotEnableMode4(const base::Boolean* const msg)
{
    if (msg != nullptr) setEnabledMode4(msg->asBool());
    icMode4Flg = enableMode4;
    return true;
}

bool Iff::setSlotWhichMode4(const base::Boolean* const msg)
{
    if (msg != nullptr) setWhichMode4(msg->asBool());
    icWhichMode4 = whichMode4;
    return true;
}

bool Iff::setSlotEnableModeC(const base::Boolean* const msg)
{
    if (msg != nullptr) setEnabledModeC(msg->asBool());
    icModeCFlg = enableModeC;
    return true;
}

bool Iff::setSlotMode1(const base::Integer* const msg)
{
    bool ok{};
    if (msg != nullptr) ok = setMode1(static_cast<unsigned short>(msg->asInt()));
    if (ok) icMode1 = mode1;
    return ok;
}

bool Iff::setSlotMode2(const base::Integer* const msg)
{
    bool ok{};
    if (msg != nullptr) ok = setMode2(static_cast<unsigned short>(msg->asInt()));
    if (ok) icMode2 = mode2;
    return ok;
}

bool Iff::setSlotMode3a(const base::Integer* const msg)
{
    bool ok{};
    if (msg != nullptr) ok = setMode3a(static_cast<unsigned short>(msg->asInt()));
    if (ok) icMode3a = mode3a;
    return ok;
}

bool Iff::setSlotMode4a(const base::Integer* const msg)
{
    bool ok{};
    if (msg != nullptr) ok = setMode4A(static_cast<unsigned short>(msg->asInt()));
    icMode4a = mode4a;
    return ok;
}

bool Iff::setSlotMode4b(const base::Integer* const msg)
{
    bool ok{};
    if (msg != nullptr) ok = setMode4B(static_cast<unsigned short>(msg->asInt()));
    icMode4b = mode4b;
    return ok;
}

//bool Iff::setSlotPower(const base::String* const msg)
//{
//    if (strcmp(msg->getString(), "OFF") == 0) pwr = OFF;
//    else if (strcmp(msg->getString(), "STANDBY") == 0) pwr = STANDBY;
//    else if (strcmp(msg->getString(), "LOW") == 0) pwr = LOW;
//    else if (strcmp(msg->getString(), "NORMAL") == 0) pwr = NORMAL;
//    else if (strcmp(msg->getString(), "EMERGENCY") == 0) pwr = EMERGENCY;
//    return true;
//}
bool Iff::zeroMode4Codes()
{
    return true;
}

}
}
