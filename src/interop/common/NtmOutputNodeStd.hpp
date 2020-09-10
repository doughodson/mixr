

#include "mixr/interop/common/NtmOutputNode.hpp"

namespace mixr {
namespace base { class List; }
namespace models { class Player; }
namespace interop {
class Ntm;

//------------------------------------------------------------------------------
// Class: NtmOutputStd
// Description: Standard outgoing NTM node
//------------------------------------------------------------------------------
class NtmOutputNodeStd : public NtmOutputNode
{
   DECLARE_SUBCLASS(NtmOutputNodeStd, NtmOutputNode)

public:
   NtmOutputNodeStd(const models::Player* const, const char* const factoryName);

   const Ntm* findNetworkTypeMapper(const models::Player* const) const override;
   bool add2OurLists(Ntm* const) override;

private:
   bool checkAndAddNtm(Ntm* const);
   bool addNtmSorted(Ntm* const);

   char* nodeFactoryName{};     // Factory name for this node
   const models::Player* tp{};  // Template player for this node
   base::List* ntmList{};       // List of Ntm objects at this level
   base::List* subnodeList{};   // List of NtmOutputNode nodes for players derived this level
};

}
}
