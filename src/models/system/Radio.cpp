
#include "mixr/models/system/Radio.hpp"

#include "mixr/models/system/Datalink.hpp"
#include "mixr/models/Emission.hpp"

#include "mixr/base/PairStream.hpp"
#include "mixr/base/numeric/Number.hpp"

#include "mixr/base/units/Frequencies.hpp"

#include <cmath>

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Radio, "Radio")

BEGIN_SLOTTABLE(Radio)
   "numChannels",       // 1: Number of channels (less than or equal MAX_CHANNELS)
   "channels",          // 2: Our channels (list of base::Frequency objects)
   "channel",           // 3: Channel number [ 1 .. numChanels ]
   "maxDetectRange",    // 4: maximum detection capability (NM) (def: 120NM)
   "radioID",           // 5: radioID used by DIS
END_SLOTTABLE(Radio)

BEGIN_SLOT_MAP(Radio)
    ON_SLOT(1, setSlotNumChannels,    base::Number)
    ON_SLOT(2, setSlotChannels,       base::PairStream)
    ON_SLOT(3, setSlotChannel,        base::Number)
    ON_SLOT(4, setSlotMaxDetectRange, base::Number)
    ON_SLOT(5, setSlotRadioId,        base::Number)
END_SLOT_MAP()

Radio::Radio()
{
   STANDARD_CONSTRUCTOR()
}

void Radio::copyData(const Radio& org, const bool)
{
   BaseClass::copyData(org);

   // (Re)set the number of channels
   setNumberOfChannels(org.numChan);

   // Copy the channel frequencies
   const unsigned short numChannels{getNumberOfChannels()};
   for (unsigned short chan = 1; chan <= numChannels; chan++) {
      setChannelFrequency( chan, org.getChannelFrequency(chan) );
   }

   // Set the channel number
   setChannel(org.channel);

   radioId = org.radioId;
   maxDetectRange = org.maxDetectRange;
}

void Radio::deleteData()
{
   setNumberOfChannels(0);
}

//------------------------------------------------------------------------------
// Get functions
//------------------------------------------------------------------------------

// Is the radio tuned to its current channel number
bool Radio::isChannelTuned() const
{
   return getChannelFrequency(channel) == getFrequency();
}

// Is the radio manually tuned (i.e., the current frequency
// is not the same as the current channel's frequency)
bool Radio::isManualTuned() const
{
   return getChannelFrequency(channel) != getFrequency();
}

// Returns the radio's channel number
unsigned short Radio::getChannel() const
{
   return channel;
}

// Number of channels
unsigned short Radio::getNumberOfChannels() const
{
   return (chanFreqTbl != nullptr) ? numChan : 0;
}

// Get a channel's frequency (Hz)
// Returns -1 if the channel is invalid
double Radio::getChannelFrequency(const unsigned short chan) const
{
   const unsigned short nc{getNumberOfChannels()};
   double freq{-1.0};
   if (chanFreqTbl != nullptr && nc > 0 && chan > 0 && chan <= nc) {
      freq = chanFreqTbl[chan-1];
   }
   return freq;
}

// Returns the radio's maximum detection range (NM)
double Radio::getMaxDetectRange() const
{
   return maxDetectRange;
}

// DIS radio ID
unsigned short Radio::getRadioId() const
{
   return radioId;
}


//------------------------------------------------------------------------------
// Set Functions
//------------------------------------------------------------------------------

// Sets the radio's channel number and changes the radio frequency
bool Radio::setChannel(const unsigned short chan)
{
   bool ok{};

   unsigned short nc{getNumberOfChannels()};
   if (nc > 0 && chan <= nc) {

      // Set a temporary channel number, c1, which defaults to the
      // current channel number
      unsigned short c1{chan};
      if (chan == 0) c1 = channel;

      // If the temp channel number is greater than zero then try
      // to set the channel's frequency
      if (c1 > 0) {
         ok = setFrequency( getChannelFrequency(c1) );
      }

      // If we were able to tune to this channel then make it the current channel
      if (ok) channel = c1;

   }
   return ok;
}

// Set a channel's frequency; channel numbers [ 1 .. getNumberOfChannels() ]
bool Radio::setChannelFrequency(const unsigned short chan, const double freq)
{
   bool ok{};

   const unsigned short nc{getNumberOfChannels()};
   if (chanFreqTbl != nullptr && nc > 0 && chan > 0 && chan <= nc) {
      chanFreqTbl[chan-1] = freq;
      ok = true;
   }

   return ok;
}

// Sets the number of channels; previous channels are lost!
bool Radio::setNumberOfChannels(const unsigned short n)
{
   bool ok{true};

   // When 'n' is zero
   if (n == 0) {
      // delete the old table
      numChan = 0;
      if (chanFreqTbl != nullptr) {
         delete[] chanFreqTbl;
         chanFreqTbl = nullptr;
      }
   }

   // When 'n' is less than or equal to the max
   else if (n <= MAX_CHANNELS) {
      // delete the old table and create a new one.
      if (chanFreqTbl != nullptr) delete[] chanFreqTbl;
      chanFreqTbl = new double[n];
      numChan = n;
   }

   // default:
   else {
      // nothing done
      ok = false;
   }

   return ok;
}

// setMaxDetectRange() -- set the max range (NM)
bool Radio::setMaxDetectRange(const double num)
{
   maxDetectRange = num;
   return true;
}

// setRadioID() -- set Radio ID
bool Radio::setRadioId(const unsigned short num)
{
   radioId = num;
   return true;
}

//------------------------------------------------------------------------------
// receive() -- process received emissions
//------------------------------------------------------------------------------
void Radio::receive(const double dt)
{
   BaseClass::receive(dt);

   // Receiver losses
   const double noise{getRfRecvNoise()};

   // ---
   // Process Emissions
   // ---

   Emission* em = nullptr;
   double signal = 0;

   // Get an emission from the queue
   base::lock(packetLock);
   if (np > 0) {
      np--; // Decrement 'np', now the array index
      em = packets[np];
      signal = signals[np];
   }
   base::unlock(packetLock);

   while (em != nullptr) {

      // Signal/Noise  (Equation 2-9)
      double sn{signal / noise};
      double snDbl{10.0 * std::log10(sn)};

      // Is S/N above receiver threshold?
      if ( snDbl >= getRfThreshold() ) {
         // Report this valid emission to the radio model ...
         receivedEmissionReport(em);
      }

      em->unref();
      em = nullptr;

      // Get another emission from the queue
      base::lock(packetLock);
      if (np > 0) {
         np--;
         em = packets[np];
         signal = signals[np];
      }
      base::unlock(packetLock);
   }
}

//------------------------------------------------------------------------------
// receivedEmissionReport() -- default (nothing to do)
//  Handle reports of valid emission reports (signal/noise ratio above threshold).
//------------------------------------------------------------------------------
void Radio::receivedEmissionReport(Emission* const)
{
}

//------------------------------------------------------------------------------
// Slot Functions  (return 'true' if the slot was set, else 'false')
//------------------------------------------------------------------------------

bool Radio::setSlotNumChannels(base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v{msg->getInt()};
      if (v >= 0 && v <= 0xFFFF) {
         ok = setNumberOfChannels( static_cast<unsigned short>(v) );
      }
   }
   return ok;
}

bool Radio::setSlotChannels(const base::PairStream* const msg)
{
   // ---
   // Quick out if the number of channels hasn't been set.
   // ---
   const unsigned short nc{getNumberOfChannels()};
   if (nc == 0 || msg == nullptr) {
      std::cerr << "Radio::setSlotChannels() Number of channels is not set!" << std::endl;
      return false;
   }

   {
      unsigned short chan{1};
      const base::List::Item* item{msg->getFirstItem()};
      while (chan <= nc && item != nullptr) {

         const base::Pair* pair{static_cast<const base::Pair*>(item->getValue())};
         const base::Frequency* p{static_cast<const base::Frequency*>(pair->object())};
         if (p != nullptr) {
            const double freq{base::Hertz::convertStatic( *p )};
            const bool ok{setChannelFrequency(chan, freq)};
            if (!ok) {
               std::cerr << "Radio::setSlotChannels() Error setting frequency for channel " << chan << "; with freq = " << *p << std::endl;
            }
         } else {
            std::cerr << "Radio::setSlotChannels() channel# " << chan << " is not of type Frequency" << std::endl;
         }

         chan++;
         item = item->getNext();
      }
   }

   return true;
}

// channel: Channel the radio is set to
bool Radio::setSlotChannel(base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      const int v{msg->getInt()};
      if (v >= 0 && v <= 0xFFFF) {
         ok = setChannel( static_cast<unsigned short>(v) );
      }
   }
   return ok;
}

// maxDetectRange: maximum detection capability (NM)
bool Radio::setSlotMaxDetectRange(base::Number* const num)
{
   bool ok{};
   if (num != nullptr) {
      maxDetectRange = num->getReal();
      ok = true;
   }
   return ok;
}

// radio ID: the radio id used for DIS
bool Radio::setSlotRadioId(base::Number* const num)
{
   bool ok{};
   if (num != nullptr) {
      const unsigned short num2{static_cast<unsigned short>(num->getInt())};
      ok = setRadioId(num2);
   }
   return ok;
}

}
}
