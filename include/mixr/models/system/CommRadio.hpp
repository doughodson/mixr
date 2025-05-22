
#ifndef __mixr_models_common_CommRadio_HPP__
#define __mixr_models_common_CommRadio_HPP__

#include "mixr/models/system/IRadio.hpp"

namespace mixr {
namespace models {
class Datalink;
class RfEmission;

//------------------------------------------------------------------------------
// Class: CommRadio
// Description: Generic class for all communication radio models
//
// Factory name: CommRadio
//------------------------------------------------------------------------------
class CommRadio : public IRadio
{
   DECLARE_SUBCLASS(CommRadio, IRadio)

public:
   CommRadio();

   // The optional datalink system attached to this radio
   Datalink* getDatalink();
   const Datalink* getDatalink() const;
   virtual bool setDatalink(Datalink* const);

   // Transmit a datalink message
   virtual bool transmitDataMessage(base::IObject* const);

protected:
   void receivedEmissionReport(RfEmission* const) override;

private:
   base::safe_ptr<Datalink> datalink;   // Our companion datalink system
};

}
}

#endif
