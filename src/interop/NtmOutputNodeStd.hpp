

#include "mixr/interop/INtmOutputNode.hpp"

namespace mixr {
namespace base { class List; }
namespace models { class Player; }
namespace interop {
class INtm;

//------------------------------------------------------------------------------
// Class: NtmOutputStd
// Description: Standard outgoing NTM node
//------------------------------------------------------------------------------
class NtmOutputNodeStd : public INtmOutputNode
{
   DECLARE_SUBCLASS(NtmOutputNodeStd, INtmOutputNode)

public:
   NtmOutputNodeStd(const models::Player* const, const char* const factoryName);

   const INtm* findNetworkTypeMapper(const models::Player* const) const override;
   bool add2OurLists(INtm* const) override;

private:
   bool checkAndAddNtm(INtm* const);
   bool addNtmSorted(INtm* const);

   char* nodeFactoryName{};     // Factory name for this node
   const models::Player* tp{};  // Template player for this node
   base::List* ntmList{};       // List of Ntm objects at this level
   base::List* subnodeList{};   // List of NtmOutputNode nodes for players derived this level
};

}
}
