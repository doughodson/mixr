
#ifndef __mixr_models_CommRadio_H__
#define __mixr_models_CommRadio_H__

#include "mixr/models/system/Radio.hpp"

namespace mixr {
namespace models {
class Datalink;
class Emission;

//------------------------------------------------------------------------------
// Class: CommRadio
// Description: Generic class for all communication radio models
//
// Factory name: CommRadio
//------------------------------------------------------------------------------
class CommRadio : public Radio
{
   DECLARE_SUBCLASS(CommRadio, Radio)

public:
   CommRadio();

   // The optional datalink system attached to this radio
   Datalink* getDatalink();
   const Datalink* getDatalink() const;
   virtual bool setDatalink(Datalink* const);

   // Transmit a datalink message
   virtual bool transmitDataMessage(base::Object* const);

protected:
   void receivedEmissionReport(Emission* const) override;

private:
   base::safe_ptr<Datalink> datalink;   // Our companion datalink system
};

}
}

#endif
