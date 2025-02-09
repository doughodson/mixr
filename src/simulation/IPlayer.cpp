
#include "mixr/simulation/IPlayer.hpp"

#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/Identifier.hpp"

#include "mixr/simulation/INetIO.hpp"
#include "mixr/simulation/INib.hpp"

namespace mixr {
namespace simulation {

IMPLEMENT_ABSTRACT_SUBCLASS(IPlayer, "IPlayer")

BEGIN_SLOTTABLE(IPlayer)
   "id",                // 01) Player id   [ 1 .. 65535 ]
   "mode",              // 02) Initial player mode ( INACTIVE, ACTIVE, DEAD )
END_SLOTTABLE(IPlayer)

BEGIN_SLOT_MAP(IPlayer)
   ON_SLOT(01, setSlotID,       base::Integer)
   ON_SLOT(02, setSlotInitMode, base::Identifier)
END_SLOT_MAP()

IPlayer::IPlayer()
{
   STANDARD_CONSTRUCTOR()
   initData();
}

void IPlayer::initData()
{
   nibList = new INib*[INetIO::MAX_NETWORK_ID];
   for (int i{}; i < INetIO::MAX_NETWORK_ID; i++) {
      nibList[i] = nullptr;
   }
}

void IPlayer::copyData(const IPlayer& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   id = org.id;
   name = org.name;

   mode = org.mode;
   initMode = org.initMode;

   netID = org.netID;
   enableNetOutput = org.enableNetOutput;

   // NIB pointers are not copied!
   setNib( nullptr );
   for (int i{}; i < simulation::INetIO::MAX_NETWORK_ID; i++) {
      setOutgoingNib(nullptr, i);
   }
}

void IPlayer::deleteData()
{
   setNib(nullptr);
   if (nibList != nullptr) {
      for (int i{}; i < simulation::INetIO::MAX_NETWORK_ID; i++) {
         setOutgoingNib(nullptr, i);
      }
      delete[] nibList;
      nibList = nullptr;
   }
}

bool IPlayer::shutdownNotification()
{
   if (nib != nullptr) nib->event(SHUTDOWN_EVENT);
   if (nibList != nullptr) {
      for (int i{}; i < simulation::INetIO::MAX_NETWORK_ID; i++) {
         if (nibList[i] != nullptr) nibList[i]->event(SHUTDOWN_EVENT);
      }
   }
   return BaseClass::shutdownNotification();
}

// reset parameters
void IPlayer::reset()
{
   if (isLocalPlayer()) {
      setMode(initMode);
   }

   // ---
   // Reset our base class
   // -- Do this last because it sends reset pulses to our components and
   //    we want to make sure our data is initialized first.
   // ---
   BaseClass::reset();
}

// Player's outgoing NIB(s)
INib* IPlayer::getLocalNib(const int netId)
{
   INib* p{};
   if (nibList != nullptr && netId >= 1 && netId <= INetIO::MAX_NETWORK_ID) {
      p = nibList[netId-1];
   }
   return p;
}

// Player's outgoing NIB(s)  (const version)
const INib* IPlayer::getLocalNib(const int netId) const
{
   const INib* p{};
   if (nibList != nullptr && netId >= 1 && netId <= INetIO::MAX_NETWORK_ID) {
      p = nibList[netId-1];
   }
   return p;
}

//-----------------------------------------------------------------------------

// Sets a pointer to the Network Interface Block (NIB)
bool IPlayer::setNib(INib* const n)
{
   if (nib != nullptr) nib->unref();
   nib = n;
   if (nib != nullptr) {
      // Ref() the new NIB and get the network ID
      nib->ref();
      INetIO* netIO{nib->getNetIO()};
      if (netIO != nullptr) netID = netIO->getNetworkID();
   } else {
      netID = 0;
   }
   return true;
}

// Sets the network output enabled flag
bool IPlayer::setEnableNetOutput(const bool x)
{
   enableNetOutput = x;
   return true;
}

// Sets the outgoing NIB for network 'id'
bool IPlayer::setOutgoingNib(INib* const p, const int id)
{
   bool ok{};
   if (nibList != nullptr && id >= 1 && id <= INetIO::MAX_NETWORK_ID) {
      int idx{id - 1};
      if (nibList[idx] != nullptr) nibList[idx]->unref();
      nibList[idx] = p;
      if (nibList[idx] != nullptr) nibList[idx]->ref();
   }
   return ok;
}

//-----------------------------------------------------------------------------

// id: Player id  [ 1 .. 65535 ]
bool IPlayer::setSlotID(const base::Integer* const num)
{
   bool ok{};
   const int newID{num->asInt()};
   if (newID > 0 && newID <= 65535) {
      setID( static_cast<unsigned short>(newID)  );
      ok = true;
   } else {
      std::cerr << "AbstractPlayer::setSlotID(): Invalid ID number: range 1 .. 65535" << std::endl;
   }
   return ok;
}

// initial player mode { inactive, active, killed, crashed, detonated, launched }
bool IPlayer::setSlotInitMode(base::Identifier* const x)
{
   bool ok{};
   if (*x == "inactive" || *x == "INACTIVE")        { setInitMode(Mode::INACTIVE);  ok = true; }
   else if (*x == "active" || *x == "ACTIVE")       { setInitMode(Mode::ACTIVE);    ok = true; }
   else if (*x == "killed" || *x == "KILLED")       { setInitMode(Mode::KILLED);    ok = true; }
   else if (*x == "crashed" || *x == "CRASHED")     { setInitMode(Mode::CRASHED);   ok = true; }
   else if (*x == "detonated" || *x == "DETONATED") { setInitMode(Mode::DETONATED); ok = true; }
   else if (*x == "launched" || *x == "LAUNCHED")   { setInitMode(Mode::LAUNCHED);  ok = true; }
   return ok;
}

}
}
