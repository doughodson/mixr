
#include "SignalProcessor.hpp"

#include "cigicl/CigiBaseSignalProcessing.h"
#include "mixr/ighost/cigi/CigiHost.hpp"

#include "cigicl/CigiSOFV3.h"
#include "cigicl/CigiCollDetSegRespV3.h"
#include "cigicl/CigiCollDetVolRespV3.h"
#include "cigicl/CigiHatHotRespV3.h"
#include "cigicl/CigiHatHotRespV3_2.h"
#include "cigicl/CigiHatHotXRespV3_2.h"
#include "cigicl/CigiIGMsgV3.h"
#include "cigicl/CigiLosRespV3.h"
#include "cigicl/CigiSensorRespV3.h"

namespace mixr {
namespace cigi {

//------------------------------------------------------------------------------
// callbacks -- Pass control to the correct object (session's) member function
//------------------------------------------------------------------------------

void SignalProcessor::OnSOF(CigiBasePacket* packet)
{
   if (p != nullptr) p->startOfFrame( dynamic_cast<CigiSOFV3*> (packet) );
}

void SignalProcessor::OnCollDetSegResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->collisionSegmentResp( dynamic_cast<CigiCollDetSegRespV3*> (packet) );
}

void SignalProcessor::OnCollDetVolResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->collisionVolumeResp( dynamic_cast<CigiCollDetVolRespV3*> (packet) );
}

void SignalProcessor::OnHotResp(CigiBasePacket* packet)
{
   if (p != nullptr) {

      const auto m3 = dynamic_cast<CigiHatHotRespV3*> (packet);
      if (m3 != nullptr) {
         p->hatHotResp( m3 );
      } else {

         const auto m3_2 = dynamic_cast<CigiHatHotRespV3_2*> (packet);
         if (m3_2 != nullptr) {
            CigiHatHotRespV3 resp;
            resp.SetHatHotID(m3_2->GetHatHotID());
            resp.SetHot(m3_2->GetHot());
            resp.SetValid(m3_2->GetValid());
            p->hatHotResp( &resp );
         } else {
            const auto mx3_2 = dynamic_cast<CigiHatHotXRespV3_2*> (packet);
            if (mx3_2 != nullptr) {
               CigiHatHotRespV3 resp;
               resp.SetHatHotID(mx3_2->GetHatHotID());
               resp.SetHot(mx3_2->GetHot());
               resp.SetValid(mx3_2->GetValid());
               p->hatHotResp( &resp );
            }
         }

      }
   }
}

void SignalProcessor::OnHatHotResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->hatHotResp( dynamic_cast<CigiHatHotRespV3*> (packet) );
}

void SignalProcessor::OnIGMsg(CigiBasePacket* packet)
{
   if (p != nullptr) p->igResponse( dynamic_cast<CigiIGMsgV3*> (packet) );
}

void SignalProcessor::OnLosResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->losResp( dynamic_cast<CigiLosRespV3*> (packet) );
}

void SignalProcessor::OnSensorResp(CigiBasePacket* packet)
{
   if (p != nullptr) p->sensorResp( dynamic_cast<CigiSensorRespV3*> (packet) );
}

}
}
