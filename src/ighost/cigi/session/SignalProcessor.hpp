
#ifndef __mixr_ighost_cigi_session_SignalProcessor_HPP__
#define __mixr_ighost_cigi_session_SignalProcessor_HPP__

#include "cigicl/CigiBaseSignalProcessing.h"

namespace mixr {
namespace cigi {
class CigiHost;

class SignalProcessor : public CigiBaseSignalProcessing
{
public:
   SignalProcessor(CigiHost* parent) : p(parent) { }
   virtual ~SignalProcessor() = default;

   virtual void OnSOF(CigiBasePacket*);
   virtual void OnCollDetSegResp(CigiBasePacket*);
   virtual void OnCollDetVolResp(CigiBasePacket*);
   virtual void OnHatHotResp(CigiBasePacket*);
   virtual void OnHotResp(CigiBasePacket*);
   virtual void OnIGMsg(CigiBasePacket*);
   virtual void OnLosResp(CigiBasePacket*);
   virtual void OnSensorResp(CigiBasePacket*);

private:
   CigiHost* p{};
};

}
}

#endif
