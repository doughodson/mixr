
#ifndef __mixr_recorder_TabPrinter_HPP__
#define __mixr_recorder_TabPrinter_HPP__

#include "mixr/recorder/protobuf_v2/OutputHandler.hpp"
#include "mixr/recorder/protobuf_v2/PrintHandler.hpp"
#include <string>
#include <sstream>
#include <fstream>

namespace mixr {
namespace base { class Identifier; class String; }
namespace recorder {
namespace protobuf_v2 {
namespace proto {
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
   virtual void printFileIdMsg(const proto::Time* const, const proto::FileIdMsg* const);
   virtual void printNewPlayerEventMsg(const proto::Time* const, const proto::NewPlayerEventMsg* const);
   virtual void printPlayerRemovedEventMsg(const proto::Time* const, const proto::PlayerRemovedEventMsg* const);
   virtual void printPlayerDataMsg(const proto::Time* const, const proto::PlayerDataMsg* const);
   virtual void printPlayerDamagedEventMsg(const proto::Time* const, const proto::PlayerDamagedEventMsg* const);
   virtual void printPlayerCollisionEventMsg(const proto::Time* const, const proto::PlayerCollisionEventMsg* const);
   virtual void printPlayerCrashEventMsg(const proto::Time* const, const proto::PlayerCrashEventMsg* const);
   virtual void printPlayerKilledEventMsg(const proto::Time* const, const proto::PlayerKilledEventMsg* const);
   virtual void printWeaponReleaseEventMsg(const proto::Time* const, const proto::WeaponReleaseEventMsg* const);
   virtual void printWeaponHungEventMsg(const proto::Time* const, const proto::WeaponHungEventMsg* const);
   virtual void printWeaponDetonationEventMsg(const proto::Time* const, const proto::WeaponDetonationEventMsg* const);
   virtual void printGunFiredEventMsg(const proto::Time* const, const proto::GunFiredEventMsg* const);
   virtual void printNewTrackEventMsg(const proto::Time* const, const proto::NewTrackEventMsg* const);
   virtual void printTrackRemovedEventMsg(const proto::Time* const, const proto::TrackRemovedEventMsg* const);
   virtual void printTrackDataMsg(const proto::Time* const, const proto::TrackDataMsg* const);
   virtual void printMarkerMsg(const proto::Time* const, const proto::MarkerMsg* const);
   virtual void printInputDeviceMsg(const proto::Time* const, const proto::InputDeviceMsg* const, const  unsigned int msgId);

   // Events without messages
   virtual void printUnhandledIdToken(const proto::Time* const);
   virtual void printEndOfData(const proto::Time* const);
   virtual void printResetEvent(const proto::Time* const);

   // Common Data Messages
   virtual void printTimeMsg(std::ostream& sout, const proto::Time* const);
   virtual void printPlayerIdMsg(std::ostream& sout, const proto::PlayerId* const);
   virtual void printPlayerStateMsg(std::ostream& sout, const proto::PlayerState* const);
   virtual void printCommonTrackDataMsg(std::ostream& sout, const proto::TrackData* const);
   virtual void printEmissionDataMsg(std::ostream& sout, const proto::EmissionData* const);

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
}

#endif
