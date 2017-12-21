
#include "IgThread.hpp"

#include "mixr/ighost/cigi/CigiHost.hpp"
#include "mixr/ighost/cigi/session/HostSession.hpp"

namespace mixr {
namespace cigi {

IgThread::IgThread(base::Component* const parent): base::OneShotThread(parent)
{
}

unsigned long IgThread::userFunc()
{
   const auto cigi = dynamic_cast<CigiHost*>(getParent());
   const auto session = cigi->getHostSession();
   if (session != nullptr) {
//      if (session->isMessageEnabled(MSG_INFO)) {
         std::cout << "IgThread::userFunc(): session = " << session << std::endl;
//      }
      session->processIgMessages();
   }
   return 0;
}

}
}
