
#ifndef __mixr_recorder_PrintSelected_H__
#define __mixr_recorder_PrintSelected_H__

#include "mixr/recorder/OutputHandler.hpp"
#include "mixr/recorder/PrintHandler.hpp"

#include <string>
#include <sstream>
#include <fstream>

#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"

namespace mixr {
namespace base { class Float; class Integer; }
namespace recorder {
namespace pb {
class Time; class FileIdMsg; class NewPlayerEventMsg; class PlayerRemovedEventMsg; class PlayerDataMsg;
class PlayerDamagedEventMsg; class PlayerCollisionEventMsg; class PlayerCrashEventMsg;
class PlayerKilledEventMsg; class WeaponReleaseEventMsg; class WeaponHungEventMsg;
class WeaponDetonationEventMsg; class GunFiredEventMsg; class NewTrackEventMsg;
class TrackRemovedEventMsg; class TrackDataMsg; class PlayerId; class PlayerState;
class TrackData; class EmissionData;
}

//------------------------------------------------------------------------------
// Class: PrintSelected
// Description: Print selected data record message data
//
// Factory name: PrintSelected
// Slots:
//   messageToken    <base::Number>   ! Message ID (token)
//   fieldName       <base::String>   ! Full field name (e.g., mixr.Recorder.Pb.PlayerId.name)
//   compareToValS   <base::String>   ! value to compare (string)
//   compareToValI   <base::Number>   ! value to compare (num)
//   compareToValD   <base::Number>   ! value to compare (dbl)
//   condition       <base::String>   ! EQ, LT, or GT (ignored for bool and strings)
//   timeOnly        <base::Number>   ! match time conditions only. Print ALL messages that match
//------------------------------------------------------------------------------
class PrintSelected : public PrintHandler
{
    DECLARE_SUBCLASS(PrintSelected, PrintHandler)

public:

   PrintSelected();

   enum class Condition { EQ, LT, GT };

   unsigned int getMsgToken() const;
   std::string getFieldName() const;
   std::string getCompareToStr() const;
   double getCompareToDbl() const;
   int getCompareToNum() const;
   bool getCompareToBool() const;

   // Set comparison criteria:
   bool setMsgToken(const unsigned int);
   bool setFieldOfInterest(const std::string& name);
   bool setCompareToValue(const std::string&);
   bool setCompareToValue(const int);
   bool setCompareToValue(const double);
   bool setCompareCondition(const Condition);
   bool setTimeOnly(const bool);

protected:

   void processRecordImp(const DataRecordHandle* const handle) override;

   // Recursive function to look at each embedded message
   void processMessage(const google::protobuf::Message* const);

   // Recursive function to print all messages within a top-level message
   void printMessage(std::ostream& soutFields, std::ostream& soutVals, const google::protobuf::Message* const msg);

   // Return the event message name
   std::string getEventMsgName(const google::protobuf::Message* const msg);

   std::string printTimeMsg(double time);

private:
   unsigned int msgToken {};
   double compareValD {};
   int compareValI {};
   Condition condition {Condition::EQ};

   std::string fieldNameStr;
   std::string compareStr;
   const google::protobuf::Message* recMsg {};
   const google::protobuf::Message* eventMsg {};
   bool foundSelected {};
   bool printHeader {};
   bool timeOnly {};

private:
   // slot table helper methods
   bool setSlotMsgToken(const base::Number* const);
   bool setSlotFieldName(const base::String* const);
   bool setSlotCompareToStr(const base::String* const);
   bool setSlotCompareToNum(const base::Number* const);
   bool setSlotCompareToDbl(const base::Number* const);
   bool setSlotCondition(const base::String* const);
   bool setSlotTimeOnly(const base::Number* const);
};

inline unsigned int PrintSelected::getMsgToken() const { return msgToken; }
inline std::string PrintSelected::getFieldName() const { return fieldNameStr; }
inline std::string PrintSelected::getCompareToStr() const { return compareStr; }
inline double PrintSelected::getCompareToDbl() const { return compareValD; }
inline int PrintSelected::getCompareToNum() const { return compareValI; }
inline bool PrintSelected::getCompareToBool() const
{
   if (compareValI == 0) return false;
   else return true;
}

}
}

#endif
