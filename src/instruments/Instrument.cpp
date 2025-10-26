
#include "mixr/instruments/Instrument.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/numeric/Float.hpp"
#include "mixr/graphics/ColorRotary.hpp"
#include "mixr/base/relations/Table1.hpp"
#include "mixr/base/IPairStream.hpp"
#include "mixr/base/Pair.hpp"

namespace mixr {
namespace instruments {

IMPLEMENT_SUBCLASS(Instrument, "Instrument")

BEGIN_SLOTTABLE(Instrument)
    "scalingTable",         // 1) table for figuring linear interpolation (if not a linear scale)
    "instVal",              // 2) our instrument value
    "allowComponentPass",   // 3) if this is true, we will send all instrument values we receive down to our components.
END_SLOTTABLE(Instrument)

BEGIN_SLOT_MAP(Instrument)
    ON_SLOT(1, setSlotScalingTable, base::Table1)
    ON_SLOT(2, setSlotInstVal,      base::INumber)
    ON_SLOT(3, setSlotAllowValPass, base::Boolean)
END_SLOT_MAP()

BEGIN_EVENT_HANDLER(Instrument)
    ON_EVENT_OBJ(UPDATE_INSTRUMENTS, onUpdateInstVal, base::INumber)
END_EVENT_HANDLER()

Instrument::Instrument()
{
    STANDARD_CONSTRUCTOR()
}

void Instrument::copyData(const Instrument& org, const bool)
{
    BaseClass::copyData(org);

    if (org.myTable != nullptr) {
        base::Table1* copy = org.myTable->clone();
        setSlotScalingTable( copy );
        copy->unref();
    } else {
        setSlotScalingTable(nullptr);
    }

    instVal = org.instVal;
    allowPassing = org.allowPassing;
    preScaleInstVal = org.preScaleInstVal;
}

void Instrument::deleteData()
{
   if (myTable != nullptr) {
      myTable->unref();
      myTable = nullptr;
   }
}

// SLOT functions
//------------------------------------------------------------------------------
// setSlotScalingTable() --
//------------------------------------------------------------------------------
bool Instrument::setSlotScalingTable(const base::Table1* const newTable)
{
   bool ok{};
   if (newTable != nullptr) {
      if (myTable != nullptr) myTable->unref();
      myTable = newTable;
      if (myTable != nullptr) myTable->ref();
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// setSlotInstVal() -- sets our instrument value slot
//------------------------------------------------------------------------------
bool Instrument::setSlotInstVal(const base::INumber* const newVal)
{
    bool ok{};
    if (newVal != nullptr) ok = setInstVal(newVal->asDouble());
    return ok;
}

//------------------------------------------------------------------------------
// setSlotAllowValPass() --
//------------------------------------------------------------------------------
bool Instrument::setSlotAllowValPass(const base::Boolean* const newAVP)
{
    bool ok{};
    if (newAVP != nullptr) ok = setAllowValPass(newAVP->asBool());
    return ok;
}

// SET Functions
//------------------------------------------------------------------------------
// setAllowValPass() -- allow our values to be passed down
//------------------------------------------------------------------------------
bool Instrument::setAllowValPass(const bool newVP)
{
    allowPassing = newVP;
    return true;
}

// EVENT
// Update our gauge if necessary, then send the event down to all of our graphic components who need it
//------------------------------------------------------------------------------
// update our instrument value
//------------------------------------------------------------------------------
bool Instrument::onUpdateInstVal(const base::INumber* const newPos)
{
    bool ok{};
    // now call our set function
    ok = setInstVal(newPos->asDouble());
    return ok;
}

// sets our instrument value
bool Instrument::setInstVal(const double newPos)
{
    // store our raw instrument value, in case some instruments need them
    preScaleInstVal = newPos;

    // do we have a table to use?
    if (myTable != nullptr) instVal = myTable->lfi(newPos);
    else instVal = newPos;
    return true;
}

void Instrument::updateData(const double dt)
{
   // update our base class
   BaseClass::updateData(dt);

   // check for a color rotary, just in case we need one
   const auto cr = dynamic_cast<graphics::ColorRotary*>(getColor());
   if (cr != nullptr) cr->determineColor(preScaleInstVal);

   // only tell the rest of our instruments our value if we want them to know it
   if (allowPassing) {
      // sort out the instruments from our components
      base::IPairStream* ps = getComponents();
      if (ps != nullptr) {
         base::IList::Item* item = ps->getFirstItem();
         while(item != nullptr) {
            const auto pair = dynamic_cast<base::Pair*>(item->getValue());
            if (pair != nullptr) {
               // send the value down to all of our instrument components
               const auto myInst = dynamic_cast<Instrument*>(pair->object());
               base::Float n(preScaleInstVal);
               if (myInst != nullptr) myInst->event(UPDATE_INSTRUMENTS, &n);
            }
            item = item->getNext();
         }
         ps->unref();
         ps = nullptr;
      }
   }
}

}
}
