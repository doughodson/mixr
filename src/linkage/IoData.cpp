
#include "mixr/linkage/IoData.hpp"

#include "mixr/base/numeric/Number.hpp"

#include <iostream>
#include <vector>
#include <algorithm>

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(IoData, "IoData")
EMPTY_DELETEDATA(IoData)

BEGIN_SLOTTABLE(IoData)
    "numAI",     // 1) Number of analog inputs (AIs)
    "numAO",     // 2) Number of analog outputs (AOs)
    "numDI",     // 3) Number of discrete inputs (DIs)
    "numDO",     // 4) Number of discrete outputs (DOs)
END_SLOTTABLE(IoData)

BEGIN_SLOT_MAP(IoData)
   ON_SLOT( 1, setSlotNumAI,    base::Number)
   ON_SLOT( 2, setSlotNumAO,    base::Number)
   ON_SLOT( 3, setSlotNumDI,    base::Number)
   ON_SLOT( 4, setSlotNumDO,    base::Number)
END_SLOT_MAP()

IoData::IoData()
{
   STANDARD_CONSTRUCTOR()
}

void IoData::copyData(const IoData& org, const bool)
{
   BaseClass::copyData(org);

   ai_table = org.ai_table;
   ao_table = org.ao_table;
   di_table = org.di_table;
   do_table = org.do_table;
}

// -----------------------------------------------------------------------------
// Default quantity functions
// -----------------------------------------------------------------------------
int IoData::getNumAnalogInputChannels() const     { return static_cast<int>(ai_table.size()); }
int IoData::getNumAnalogOutputChannels() const    { return static_cast<int>(ao_table.size()); }
int IoData::getNumDiscreteInputChannels() const   { return static_cast<int>(di_table.size()); }
int IoData::getNumDiscreteOutputChannels() const  { return static_cast<int>(do_table.size()); }

bool IoData::getAnalogInput(const int channel, double* const value) const
{
   bool ok {};
   if (value != nullptr && channel > 0 && channel <= static_cast<int>(ai_table.size())) {
      *value = ai_table[channel-1];
      ok = true;
   }
   return ok;
}

bool IoData::getAnalogOutput(const int channel, double* const value) const
{
   bool ok {};
   if (value != nullptr && channel > 0 && channel <= static_cast<int>(ao_table.size())) {
      *value = ao_table[channel-1];
      ok = true;
   }
   return ok;
}

bool IoData::getDiscreteInput(const int channel, bool* const value) const
{
   bool ok {};
   if (value != nullptr && channel > 0 && channel <= static_cast<int>(di_table.size())) {
      *value = di_table[channel-1];
      ok = true;
   }
   return ok;
}

bool IoData::getDiscreteOutput(const int channel, bool* const value) const
{
   bool ok {};
   if (value != nullptr && channel > 0 && channel <= static_cast<int>(do_table.size())) {
      *value = do_table[channel-1];
      ok = true;
   }
   return ok;
}

bool IoData::setAnalogInput(const int channel, const double value)
{
   bool ok {};
   if (channel > 0 && channel <= static_cast<int>(ai_table.size())) {
      ai_table[channel-1] = value;
      ok = true;
   }
   return ok;
}

bool IoData::setAnalogOutput(const int channel, const double value)
{
   bool ok {};
   if (channel > 0 && channel <= static_cast<int>(ao_table.size())) {
      ao_table[channel-1] = value;
      ok = true;
   }
   return ok;
}

bool IoData::setDiscreteInput(const int channel, const bool value)
{
   bool ok {};
   if (channel > 0 && channel <= static_cast<int>(di_table.size())) {
      di_table[channel-1] = value;
      ok = true;
   }
   return ok;
}

bool IoData::setDiscreteOutput(const int channel, const bool value)
{
   bool ok {};
   if (channel > 0 && channel <= static_cast<int>(do_table.size())) {
      do_table[channel-1] = value;
      ok = true;
   }
   return ok;
}

// -----------------------------------------------------------------------------
// sets the data buffer (AI's and DI's) to zero/false
// -----------------------------------------------------------------------------
void IoData::clear()
{
   std::fill(ai_table.begin(), ai_table.end(), 0.0);
   std::fill(ao_table.begin(), ao_table.end(), 0.0);
   std::fill(di_table.begin(), di_table.end(), false);
   std::fill(do_table.begin(), do_table.end(), false);
}

bool IoData::setNumAI(const int num)
{
   ai_table.resize(num);
   return true;
}

bool IoData::setNumAO(const int num)
{
   ao_table.resize(num);
   return true;
}

bool IoData::setNumDI(const int num)
{
   di_table.resize(num);
   return true;
}

bool IoData::setNumDO(const int num)
{
   do_table.resize(num);
   return true;
}

// define the number of analog inputs (AIs) in the data block
bool IoData::setSlotNumAI(const base::Number* const msg)
{
   bool ok {true};
   if (msg != nullptr) {
      const int num {msg->getInt()};
      if (num >= 0) setNumAI(num);
      else ok = false;
   }
   return ok;
}

// define the number of analog outputs (AOs) in the data block
bool IoData::setSlotNumAO(const base::Number* const msg)
{
   bool ok {true};
   if (msg != nullptr) {
      const int num {msg->getInt()};
      if (num >= 0) setNumAO(num);
      else ok = false;
   }
   return ok;
}

// define the number of discrete inputs (DIs) in the data block
bool IoData::setSlotNumDI(const base::Number* const msg)
{
   bool ok {true};
   if (msg != nullptr) {
      const int num {msg->getInt()};
      if (num >= 0) setNumDI(num);
      else ok = false;
   }
   return ok;
}

// define the number of discrete outputs (DOs) in the data block
bool IoData::setSlotNumDO(const base::Number* const msg)
{
   bool ok {true};
   if (msg != nullptr) {
      const int num {msg->getInt()};
      if (num >= 0) setNumDO(num);
      else ok = false;
   }
   return ok;
}

}
}
