
#include "mixr/models/system/CommRadio.hpp"

#include "mixr/models/system/Datalink.hpp"
#include "mixr/models/Emission.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(CommRadio, "CommRadio")
EMPTY_SLOTTABLE(CommRadio)

CommRadio::CommRadio()
{
   STANDARD_CONSTRUCTOR()
}

void CommRadio::copyData(const CommRadio& org, const bool)
{
   BaseClass::copyData(org);

   // No datalink yet
   setDatalink(nullptr);
}

void CommRadio::deleteData()
{
   setDatalink(nullptr);
}

//------------------------------------------------------------------------------
// get functions
//------------------------------------------------------------------------------

Datalink* CommRadio::getDatalink()
{
   return datalink;
}

const Datalink* CommRadio::getDatalink() const
{
   return datalink;
}

//------------------------------------------------------------------------------
// Set functions
//------------------------------------------------------------------------------

bool CommRadio::setDatalink(Datalink* const p)
{
   datalink = p;
   return true;
}

//------------------------------------------------------------------------------
// transmitDataMessage() -- send a data message emission;
// returns true if the data emission was sent.
//------------------------------------------------------------------------------
bool CommRadio::transmitDataMessage(base::Object* const msg)
{
   bool sent{};
   // Transmitting, scanning and have an antenna?
   if (getOwnship() == nullptr) {
      if (isMessageEnabled(MSG_DEBUG)) {
         std::cout << "CommRadio ownship == nullptr!" << std::endl;
      }
      return sent;
   }

   if (msg != nullptr && isTransmitterEnabled() && getAntenna() != nullptr) {
      // Send the emission to the other player
      const auto em = new Emission();
      em->setDataMessage(msg);
      em->setFrequency(getFrequency());
      em->setBandwidth(getBandwidth());
      em->setPower(getPeakPower());
      em->setTransmitLoss(getRfTransmitLoss());
      em->setMaxRangeNM(getMaxDetectRange());
      em->setTransmitter(this);
      em->setReturnRequest(false);
      getAntenna()->rfTransmit(em);
      em->unref();
      sent = true;
   }
   return sent;
}

//------------------------------------------------------------------------------
// receivedEmissionReport() -- Datalink messages --
//  Handle reports of valid emission reports (signal/noise ratio above threshold).
//------------------------------------------------------------------------------
void CommRadio::receivedEmissionReport(Emission* const em)
{
   if (em != nullptr && datalink != nullptr) {
      // If we have a datalink and this emission contains a message, then it
      // must be a datalink message.
      base::Object* msg{em->getDataMessage()};
      if (msg != nullptr) datalink->event(DATALINK_MESSAGE, msg);
   }
}

}
}
