
#ifndef __mixr_interop_common_INtm_HPP__
#define __mixr_interop_common_INtm_HPP__

#include "mixr/base/IObject.hpp"
#include "mixr/base/safe_ptr.hpp"

namespace mixr {
namespace models { class Player; }
namespace interop {
class INib;

//------------------------------------------------------------------------------
// Class: INtm
// Description: Interface Network Type Mapper (NTM)
//
//    Interface to map mixr player types to the entity type codes
//    that are unique to an interoperability network (i.e., DIS, HLA/FOM, etc).
//
//    The mixr player type is defined by providing an example Player using
//    the 'template' slot.  On incoming entities, the 'template' player is cloned
//    to create a proxy player that is added to the simulation players list.
//
//    Network specific classes, which are derived from this class, are used to
//    define the network specific entity types.
//
//    The INetIO interface will manage two lists of these Ntm objects; one to map
//    incoming entity type codes to player types, and a second to map outgoing
//    player types to network entity codes.  These lists are provided as inputs
//    to the INetIO object using the 'inputEntityTypes' and 'outputEntityTypes'
//    slots.  For examples of these lists, see  the files 'DisIncomingEntityTypes.epp'
//    and 'DisOutgoingEntityTypes.epp' in the "mixr-data" directory.
//
//
// Slots:
//     template   <Player>   ! Template player (default: nullptr)
//
//
// Notes:
//    1) When matching a target player's type to the Ntm template players ...
//       a) Using factory names, the target player's class is matched with the
//          template player's class.
//       b) Given a template player with a matching class (or base class), the
//          template player's type string (see function Player::getType) is
//          compared to the target player's type string and the template
//          player's string will match as a substring of the target player.
//
//       Example:  Give an Aircraft player with type string "F-16C"
//
//          template: ( Aircraft type: "F-16C" )   /* #1, match: exact */
//          template: ( Aircraft type: "F-16" )    /* #2, match: as substring */
//          template: ( Aircraft type: "F-16A" )   /* #3, no match: wrong type string */
//          template: ( Aircraft type: "F-16C1" )  /* #4, no match: template type string is too long. */
//          template: ( AirVehicle type: "F-16C" ) /* #5, match: matching subclass */
//          template: ( AirVehicle type: "F-16" )  /* #6, match: matching subclass; type as substring */
//          template: ( Ship type: "F-16C" )       /* #7, no match: wrong class type */
//
//          Best match is returned: #1 (best), #2, #5 and #6 (worst)
//
//------------------------------------------------------------------------------
class INtm : public base::IObject
{
    DECLARE_SUBCLASS(INtm, base::IObject)

public:
   INtm();

   // Template player
   const models::Player* getTemplatePlayer() const    { return tPlayer; }

   // This network specific function is used to copy the entity
   // type codes to a target NIB object.
   virtual bool copyEntityType(INib* const targetNib) const =0;

private:
   base::safe_ptr<const models::Player> tPlayer; // Template player

private:
   // slot table helper methods
   virtual bool setSlotTemplatePlayer(const models::Player* const);     // Sets the template player
};

}
}

#endif
