
#ifndef __mixr_recorder_TabPrinter_HPP__
#define __mixr_recorder_TabPrinter_HPP__

#include "mixr/recorder/OutputHandler.hpp"
#include "mixr/recorder/PrintHandler.hpp"
#include <string>
#include <sstream>
#include <fstream>

namespace mixr {
namespace base { class Identifier; class String; }
namespace recorder {
namespace pb {
class Time; class FileIdMsg; class NewPlayerEventMsg; class PlayerRemovedEventMsg; class PlayerDataMsg;
class PlayerDamagedEventMsg; class PlayerCollisionEventMsg; class PlayerCrashEventMsg;
class PlayerKilledEventMsg; class WeaponReleaseEventMsg; class WeaponHungEventMsg;
class WeaponDetonationEventMsg; class GunFiredEventMsg; class NewTrackEventMsg;
class TrackRemovedEventMsg; class TrackDataMsg; class PlayerId; class PlayerState;
class TrackData; class EmissionData; class MarkerMsg; class InputDeviceMsg;
}

//------------------------------------------------------------------------------
// Class: TabPrinter
// Description: Print the data from a protocol buffer DataRecord
//
// Factory name: TabPrinter
// Slots:
//    msgHdrOptn   <String>  ! Msg Header options { no_hdr, all_msgs, new_msg, on_change }
//    divider      <String>  ! Field divider (default: tab)
//------------------------------------------------------------------------------
class TabPrinter : public PrintHandler
{
   DECLARE_SUBCLASS(TabPrinter, PrintHandler)

public:
   enum class MsgHdrOptions { NO_HDR, ALL_MSGS, NEW_MSG, ON_CHANGE };

public:
   TabPrinter();

   // set message header options
   void setMsgHdrOptions(MsgHdrOptions);
   bool isSimReset() const;                   // True if reset msg recorded

protected:
   // Event Messages
   virtual void printFileIdMsg(const pb::Time* const, const pb::FileIdMsg* const);
   virtual void printNewPlayerEventMsg(const pb::Time* const, const pb::NewPlayerEventMsg* const);
   virtual void printPlayerRemovedEventMsg(const pb::Time* const, const pb::PlayerRemovedEventMsg* const);
   virtual void printPlayerDataMsg(const pb::Time* const, const pb::PlayerDataMsg* const);
   virtual void printPlayerDamagedEventMsg(const pb::Time* const, const pb::PlayerDamagedEventMsg* const);
   virtual void printPlayerCollisionEventMsg(const pb::Time* const, const pb::PlayerCollisionEventMsg* const);
   virtual void printPlayerCrashEventMsg(const pb::Time* const, const pb::PlayerCrashEventMsg* const);
   virtual void printPlayerKilledEventMsg(const pb::Time* const, const pb::PlayerKilledEventMsg* const);
   virtual void printWeaponReleaseEventMsg(const pb::Time* const, const pb::WeaponReleaseEventMsg* const);
   virtual void printWeaponHungEventMsg(const pb::Time* const, const pb::WeaponHungEventMsg* const);
   virtual void printWeaponDetonationEventMsg(const pb::Time* const, const pb::WeaponDetonationEventMsg* const);
   virtual void printGunFiredEventMsg(const pb::Time* const, const pb::GunFiredEventMsg* const);
   virtual void printNewTrackEventMsg(const pb::Time* const, const pb::NewTrackEventMsg* const);
   virtual void printTrackRemovedEventMsg(const pb::Time* const, const pb::TrackRemovedEventMsg* const);
   virtual void printTrackDataMsg(const pb::Time* const, const pb::TrackDataMsg* const);
   virtual void printMarkerMsg(const pb::Time* const, const pb::MarkerMsg* const);
   virtual void printInputDeviceMsg(const pb::Time* const, const pb::InputDeviceMsg* const, const  unsigned int msgId);

   // Events without messages
   virtual void printUnhandledIdToken(const pb::Time* const);
   virtual void printEndOfData(const pb::Time* const);
   virtual void printResetEvent(const pb::Time* const);

   // Common Data Messages
   virtual void printTimeMsg(std::ostream& sout, const pb::Time* const);
   virtual void printPlayerIdMsg(std::ostream& sout, const pb::PlayerId* const);
   virtual void printPlayerStateMsg(std::ostream& sout, const pb::PlayerState* const);
   virtual void printCommonTrackDataMsg(std::ostream& sout, const pb::TrackData* const);
   virtual void printEmissionDataMsg(std::ostream& sout, const pb::EmissionData* const);

   // Message Field Header functions
   virtual void printTimeMsgHdr(std::ostream&);
   virtual void printPlayerIdMsgHdr(std::ostream&);
   virtual void printPlayerStateMsgHdr(std::ostream&);
   virtual void printTrackDataHdr(std::ostream&);
   virtual void printEmissionDataMsgHdr(std::ostream&);
   virtual void printWeaponMsgHdr(std::ostream&);
   virtual void printTrackMsgHdr(std::ostream&);

   virtual void printEmissionDataSpacer(std::ostream&);
   virtual void printPlayerIdSpacer(std::ostream&);
   virtual void printPlayerDataSpacer(std::ostream&);
   virtual void printTrackDataSpacer(std::ostream&);

   void printExecTimeMsg(std::ostream& sout, double execTime);
   void printSimTimeMsg(std::ostream& sout, double simTime);
   void printUtcTimeMsg(std::ostream& sout, double utcTime);

   void processRecordImp(const DataRecordHandle* const) override;

   virtual void setMsgHeaders(const bool);
private:
   // print options
   bool printHeader{};
   bool simReset{true};

   // individual headers
   bool fileIdHdr{true};
   bool playerNewHdr{true};
   bool playerRemovedHdr{true};
   bool playerDataHdr{true};
   bool playerDamagedHdr{true};
   bool playerCollisionHdr{true};
   bool playerCrashHdr{true};
   bool playerKilledHdr{true};
   bool weaponReleaseHdr{true};
   bool weaponHungHdr{true};
   bool weaponDetonateHdr{true};
   bool gunFiredHdr{true};
   bool trackNewHdr{true};
   bool trackRemovedHdr{true};
   bool trackDataHdr{true};
   bool markerHdr{true};
   bool inputDeviceHdr{true};

   // Group headers
   bool playerHeader{true};
   bool weaponHeader{true};
   bool trackHeader{true};

   MsgHdrOptions option{MsgHdrOptions::NO_HDR};        // options for printing field names
   unsigned int lastMessage{REID_UNHANDLED_ID_TOKEN};  // previous message printed
   std::string divider{"\t"};                          // divider between message fields

private:
   // slot table helper methods
   bool setSlotMsgHdr(const base::Identifier* const);
   bool setSlotDivider(const base::String* const);

};

inline bool TabPrinter::isSimReset() const { return simReset; }

}
}

#endif
