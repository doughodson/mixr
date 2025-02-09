
#ifndef __mixr_simulation_IPlayer_HPP__
#define __mixr_simulation_IPlayer_HPP__

#include "mixr/base/Component.hpp"

#include <string>

namespace mixr {
namespace base { class Identifier; class Integer; }
namespace simulation {
class INib;

//------------------------------------------------------------------------------
// Class: IPlayer
// Description: Interface for all players
//
// Factory name: IPlayer
//
// Slots --
//
//    ! ---
//    ! Player's id and mode parameters
//    ! ---
//    id             <base::Integer>     ! id  [ 1 .. 65535 ] (default: 0)
//    mode           <base::Identifier>  ! initial mode { inactive, active, killed, crashed, detonated, launched }
//                                       !   (default: active)
//------------------------------------------------------------------------------
class IPlayer : public base::Component
{
   DECLARE_SUBCLASS(IPlayer, base::Component)

public:
   IPlayer();

   // player mode
   enum class Mode {
      INACTIVE,         // Player is not being updated and is not being sent to the networks
      ACTIVE,           // Player is being updated and is being sent to the networks
      KILLED,           // Player was killed   (One of the dead conditions)
      CRASHED,          // Player crashed      (One of the dead conditions)
      DETONATED,        // Weapon player has detonated (One of the dead conditions) (Original & flyout weapons)
      PRE_RELEASE,      // Weapon player is created but not released (Flyout weapons only)
      LAUNCHED,         // Weapon player has been launched (Original weapons only)
      DELETE_REQUEST    // Request player removal from the active player list
   };

   // player id
   void setID(const int x)                  { id = x;         }        // sets the player's ID
   bool isID(const int x) const             { return x == id; }        // true if player's ID matches
   int getID() const                        { return id;      }        // returns ID

   // player name
   void setName(const std::string& x)       { name = x;         }      // set players name
   bool isName(const std::string& x) const  { return x == name; }      // true if the players name matches
   const std::string& getName() const       { return name;      }      // return players name

   // player mode
   void setMode(const Mode x)               { mode = x;     }                    // sets the player's current mode
   void setInitMode(const Mode x)           { initMode = x; }                    // sets the player's initial mode (after reset)
   Mode getMode() const                     { return mode;  }                    // current mode ( INACTIVE, ACTIVE, DETONATED ... )
   bool isActive() const                    { return mode == Mode::ACTIVE;    }  // true if player's mode is active
   bool isKilled() const                    { return mode == Mode::KILLED;    }  // true if player's mode is killed
   bool isCrashed() const                   { return mode == Mode::CRASHED;   }  // true if player's mode is crashed
   bool isDetonated() const                 { return mode == Mode::DETONATED; }  // true if player has detonated (weapons only)
   bool isInactive() const                  { return mode == Mode::INACTIVE;  }  // true if player's mode is inactive
   bool isMode(const Mode x) const          { return mode == x;               }  // true if player is currently this mode
   bool isNotMode(const Mode x) const       { return mode != x;               }  // true if player is not currently this mode
   bool isDead() const                      { return isKilled() || isCrashed() || isDetonated(); }  // true if player's mode is dead (Killed, Crashed, Detonated, etc....)

   // proxy related data
   bool isProxyPlayer() const               { return nib != nullptr; }    // true if this is a proxy player
   bool isLocalPlayer() const               { return nib == nullptr; }    // true if this is a local player

   int getNetworkID() const                 { return netID; }             // id of a proxy player's controlling network model
   INib* getNib()                           { return nib;   }             // proxy player's Nib object
   const INib* getNib() const               { return nib;   }             // proxy player's Nib object  (const version)

   bool isNetOutputEnabled() const          { return enableNetOutput; }   // is player output to the network enabled?
   INib* getLocalNib(const int netId);                                    // player's outgoing NIB(s)
   const INib* getLocalNib(const int netId) const;                        // player's outgoing NIB(s)  (const version)

   // helper methods
   virtual bool setNib(INib* const);                                  // Sets the proxy player's Nib object
   virtual bool setEnableNetOutput(const bool);                       // Sets the network output enabled flag
   virtual bool setOutgoingNib(INib* const, const int id);            // Sets the outgoing NIB for network 'id'

   void reset() override;

protected:
   bool shutdownNotification() override;

   Mode mode{Mode::ACTIVE};      // Player mode (see above)
   INib* nib{};                  // Network Interface Block (ref()'d)

private:
   void initData();

   // player identity
   int id{};
   std::string name;

   Mode initMode{Mode::ACTIVE};   // initial mode

   // incoming network support
   int netID{};                   // network id

   // outgoing network support data
   INib** nibList{};              // pointer to a list of outgoing NIBs
   bool enableNetOutput{true};    // enable output to the network

private:
   // slot table helper methods
   bool setSlotID(const base::Integer* const);
   bool setSlotInitMode(base::Identifier* const);
};

}
}

#endif
