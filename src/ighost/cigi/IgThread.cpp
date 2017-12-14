
#include "IgThread.hpp"

#include "mixr/ighost/cigi/CigiHost.hpp"
#include "mixr/ighost/cigi/session/HostSession.hpp"

namespace mixr {
namespace cigi {

IMPLEMENT_SUBCLASS(IgThread, "IgThread")
EMPTY_SLOTTABLE(IgThread)
EMPTY_COPYDATA(IgThread)
EMPTY_DELETEDATA(IgThread)

IgThread::IgThread(base::Component* const parent, const double priority): base::SingleTask(parent, priority)
{
   STANDARD_CONSTRUCTOR()
}

unsigned long IgThread::userFunc()
{
   const auto cigi = dynamic_cast<CigiHost*>(getParent());
   const auto session = cigi->getHostSession();
   if (session != nullptr) {
      if (session->isMessageEnabled(MSG_INFO)) {
         std::cout << "IgThread::userFunc(): session = " << session << std::endl;
      }
      session->processIgMessages();
   }
   return 0;
}

}
}
