
#include "mixr/recorder/protobuf_v2/PrintSelected.hpp"
#include "mixr/recorder/protobuf_v2/proto/DataRecord.pb.h"
#include "mixr/recorder/protobuf_v2/DataRecordHandle.hpp"

#include "mixr/base/String.hpp"
#include "mixr/base/numeric/Boolean.hpp"
#include "mixr/base/numeric/Float.hpp"
#include "mixr/base/numeric/Integer.hpp"
#include "mixr/base/numeric/INumber.hpp"
#include "mixr/base/units/times.hpp"

#include "google/protobuf/descriptor.h"
#include "google/protobuf/message.h"

#include <string>
#include <iostream>
#include <iomanip>

namespace mixr {
namespace recorder {

IMPLEMENT_SUBCLASS(PrintSelected,"PrintSelected")

BEGIN_SLOTTABLE(PrintSelected)
   "messageToken",   // 1) Message ID (token)
   "fieldName",      // 2) Full field name (e.g., mixr.Recorder.Pb.PlayerId.name)
   "compareToValS",  // 3) value to compare (string)
   "compareToValI",  // 4) value to compare (num)
   "compareToValD",  // 5) value to compare (dbl)
   "condition",      // 6) EQ, LT, or GT (ignored for bool and strings)
   "timeOnly",       // 7) match time conditions only. Print ALL messages that match
END_SLOTTABLE(PrintSelected)

BEGIN_SLOT_MAP(PrintSelected)
   ON_SLOT( 1, setSlotMsgToken,        base::Integer)
   ON_SLOT( 2, setSlotFieldName,       base::String)
   ON_SLOT( 3, setSlotCompareToStr,    base::String)
   ON_SLOT( 4, setSlotCompareToNum,    base::Integer)
   ON_SLOT( 5, setSlotCompareToDbl,    base::INumber)
   ON_SLOT( 6, setSlotCondition,       base::String)
   ON_SLOT( 7, setSlotTimeOnly,        base::Boolean)
END_SLOT_MAP()

EMPTY_DELETEDATA(PrintSelected)

PrintSelected::PrintSelected()
{
   STANDARD_CONSTRUCTOR()
}

void PrintSelected::copyData(const PrintSelected& org, const bool)
{
   BaseClass::copyData(org);

   msgToken = org.msgToken;
   compareValD = org.compareValD;
   compareValI = org.compareValI;
   condition = org.condition;

   fieldNameStr = org.fieldNameStr;
   compareStr = org.compareStr;
   foundSelected = org.foundSelected;
   printHeader = org.printHeader;
   timeOnly = org.timeOnly;
}

// Slots

bool PrintSelected::setSlotMsgToken(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      msgToken = msg->asInt();
   }
   return ok;
}

bool PrintSelected::setSlotFieldName(const base::String* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      fieldNameStr.assign( msg->c_str() );
   }
   return ok;

}

bool PrintSelected::setSlotCompareToStr(const base::String* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      compareStr.assign( msg->c_str() );
   }
   return ok;
}

bool PrintSelected::setSlotCompareToNum(const base::Integer* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      compareValI = msg->asInt();
   }
   return ok;
}

bool PrintSelected::setSlotCompareToDbl(const base::INumber* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      compareValD = msg->asDouble();
   }
   return ok;
}


bool PrintSelected::setSlotCondition(const base::String* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;

      if ((*msg == "EQ") || (*msg == "eq")) {
         condition = Condition::EQ;
      }
      else if ((*msg == "LT") || (*msg == "lt")) {
         condition = Condition::LT;
      }
      else if ((*msg == "GT") || (*msg == "gt")) {
         condition = Condition::GT;
      }
      else {
         // error (or set default)
         condition = Condition::EQ;
      }
   }

   return ok;
}

bool PrintSelected::setSlotTimeOnly(const base::Boolean* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      ok = true;
      timeOnly = msg->asBool();
   }
   return ok;
}

//------------------------------------------------------------------------------
// processRecordImp
//------------------------------------------------------------------------------
void PrintSelected::processRecordImp(const DataRecordHandle* const handle)
{
   if (handle == nullptr) return;  // cannot continue
   const pb::DataRecord* dataRecord{handle->getRecord()};
   if (dataRecord == nullptr) return;  // cannot continue

   // using reflection:
   // save to: const google::protobuf::Message* recMsg
   recMsg = dataRecord;
   //const google::protobuf::Message& root = *recMsg;
   const google::protobuf::Descriptor* descriptor{recMsg->GetDescriptor()};
   const google::protobuf::Reflection* reflection{recMsg->GetReflection()};

   const google::protobuf::FieldDescriptor* idField{descriptor->FindFieldByName("id")};
   unsigned int id{reflection->GetUInt32(*recMsg, idField)};

   foundSelected = false;

   // Process time message
   processMessage( &dataRecord->time());

   // Process the event message
   const google::protobuf::Message* processMsg{};
   std::string msgCat{""};
   std::string msgType{""};

   switch (msgToken) {
      case REID_FILE_ID: {
         processMsg = &dataRecord->file_id_msg();
         msgCat =  "FILE     ";
         msgType = "ID       ";
         static bool firstTime{true};
         if ((firstTime) || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_NEW_PLAYER: {
         processMsg = &dataRecord->new_player_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "NEW      ";
         static bool firstTime{true};
         if ((firstTime) || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_PLAYER_REMOVED: {
         processMsg = &dataRecord->player_removed_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "REMOVED  ";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_PLAYER_DATA: {
         processMsg = &dataRecord->player_data_msg();
         msgCat =  "PLAYER   ";
         msgType = "DATA     ";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) {
            printHeader = true;
         }
         firstTime = false;
         break;
      }
      case REID_PLAYER_DAMAGED: {
         processMsg = &dataRecord->player_damaged_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "DAMAGED  ";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_PLAYER_COLLISION: {
         processMsg = &dataRecord->player_collision_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "COLLISION";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_PLAYER_CRASH: {
         processMsg = &dataRecord->player_crash_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "CRASH    ";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_PLAYER_KILLED: {
         processMsg = &dataRecord->player_killed_event_msg();
         msgCat =  "PLAYER   ";
         msgType = "KILLED   ";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_WEAPON_RELEASED: {
         processMsg = &dataRecord->weapon_release_event_msg();
         msgCat =  "WEAPON   ";
         msgType = "RELEASED ";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_WEAPON_HUNG: {
         processMsg = &dataRecord->weapon_hung_event_msg();
         msgCat =  "WEAPON   ";
         msgType = "HUNG     ";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_WEAPON_DETONATION: {
         processMsg = &dataRecord->weapon_detonation_event_msg();
         msgCat =  "WEAPON  ";
         msgType = "DETONATE";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_GUN_FIRED: {
         processMsg = &dataRecord->gun_fired_event_msg();
         msgCat =  "GUN     ";
         msgType = "FIRED   ";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_NEW_TRACK: {
         processMsg = &dataRecord->new_track_event_msg();
         msgCat =  "TRACK   ";
         msgType = "NEW     ";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_TRACK_REMOVED: {
         processMsg = &dataRecord->track_removed_event_msg();
         msgCat =  "TRACK   ";
         msgType = "REMOVED ";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      case REID_TRACK_DATA: {
         processMsg = &dataRecord->track_data_msg();
         msgCat =  "TRACK   ";
         msgType = "DATA    ";
         static bool firstTime{true};
         if (firstTime || isFileEmpty()) printHeader = true;
         firstTime = false;
         break;
      }
      default: break;
   }

   // Process the message and see if the conditions are found
   if (!timeOnly && (processMsg != nullptr)) {
      processMessage(processMsg);
   }

   // If the condition has been found and this is the message we want, print it
   std::stringstream soutFields;
   std::stringstream soutVals;
   if (foundSelected &&  timeOnly) {
      // print the message, whatever it is, because it matches the time criteria
      std::string msgName{getEventMsgName(recMsg)};  // This also sets the event message
      if (eventMsg != nullptr) {
         soutFields << std::left << std::setw(32) << msgName << "\t" << "HEADER" << "\t";
         soutVals << std::left << std::setw(32) << msgName << "\t" "VALUES" << "\t";

         soutFields <<  std::left << std::setw(12)<< "exec time " << "\t";
         soutFields <<  std::left << std::setw(12)<< "sim time " << "\t";
         soutFields <<  std::left << std::setw(12)<< "utc time  "<< "\t" ;

         std::string timeStr = "";

         timeStr = printTimeMsg(dataRecord->time().exec_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

         timeStr = printTimeMsg(dataRecord->time().sim_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

         timeStr = printTimeMsg(dataRecord->time().utc_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

     //    soutVals <<  std::left << std::setw(12) << dataRecord->time().sim_time() << "\t";
     //    soutVals <<  std::left << std::setw(12) << dataRecord->time().exec_time() << "\t";
     //    soutVals <<  std::left << std::setw(12) << dataRecord->time().utc_time() << "\t";

    //     printMessage(soutFields, soutVals, recMsg);  // Use this to include time and message ID
         printMessage(soutFields, soutVals, eventMsg);
         printToOutput( soutFields.str().c_str() );
         printToOutput( soutVals.str().c_str() );
      }
   }
   else if (foundSelected && (id == msgToken)) {

      // Print the message name

      soutFields << msgCat << "\t" << "HEADER" << "\t";
      soutVals << msgCat << "\t" << msgType << "\t";

      // Print the time
  //    const pb::Time* timeMsg  = &dataRecord->time();
  //    printMessage(soutFields, soutVals, timeMsg);  // print out the time message

         soutFields <<  std::left << std::setw(12)<< "exec time " << "\t";
         soutFields <<  std::left << std::setw(12)<< "sim time " << "\t";
         soutFields <<  std::left << std::setw(12)<< "utc time  "<< "\t" ;

         std::string timeStr = "";

         timeStr = printTimeMsg(dataRecord->time().exec_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

         timeStr = printTimeMsg(dataRecord->time().sim_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

         timeStr = printTimeMsg(dataRecord->time().utc_time());
         soutVals <<  std::left << std::setw(12) << timeStr << "\t";

      // Print out the event message
      // print the message, whatever it is, because it matches the time criteria
      std::string msgName = getEventMsgName(recMsg);  // This also sets event message
      if (eventMsg != nullptr) {
         printMessage(soutFields, soutVals, eventMsg);
      }

//      if (processMsg != 0) printMessage(soutFields, soutVals, processMsg);  // print out the event message
      // Print to output:
      if (printHeader) {
         printHeader = false;
         printToOutput( soutFields.str().c_str() );
      }
      printToOutput( soutVals.str().c_str() );
   }
}

//------------------------------------------------------------------------------
// processMessage(): Recursive function to go through all messages
//---------------------------------------------------------------------------
void PrintSelected::processMessage(const google::protobuf::Message* const msg)
{
   const google::protobuf::Message& root = *msg;
   const google::protobuf::Descriptor* descriptor{msg->GetDescriptor()};
   const google::protobuf::Reflection* reflection{msg->GetReflection()};

   int fieldCount{descriptor->field_count()};
   const google::protobuf::FieldDescriptor* fieldDescriptor{};

   // look at fields to find a match with the slot condition
   if (fieldCount > 0) {
      // Look at each field in this message
      for (int i = 0; i < fieldCount; i++) {

         // Get field descriptor (includes messages)
         fieldDescriptor = descriptor->field(i);

         // what type is this field?
         google::protobuf::FieldDescriptor::CppType cppType{fieldDescriptor->cpp_type()};

         // If this field is a message, then call this again
         if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) {
            // Get sub message
            const google::protobuf::Message& sub_message = reflection->GetMessage(root, fieldDescriptor);
            processMessage(&sub_message);
         }
         else {
            // field is not a message. Check File name for match
            if (fieldDescriptor->full_name() == fieldNameStr) {

               // This is the right field. Now check the value, based on type
               if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_STRING) {
                  std::string str{reflection->GetString(root, fieldDescriptor)};
                  std::string matchStr{getCompareToStr()};
                  if (str == matchStr) {
                     // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_INT32) {
                  const int num{reflection->GetInt32(root, fieldDescriptor)};
                  if (((condition == Condition::EQ) && (num == getCompareToNum())) ||
                     ((condition == Condition::GT) && (num > getCompareToNum())) ||
                     ((condition == Condition::LT) && (num < getCompareToNum()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_INT64) {
                  const long long num{reflection->GetInt64(root, fieldDescriptor)};
                  if (((condition == Condition::EQ) && (num == getCompareToNum())) ||
                     ((condition == Condition::GT) && (num > getCompareToNum())) ||
                     ((condition == Condition::LT) && (num < getCompareToNum()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_UINT32) {
                  const int num{static_cast<int>(reflection->GetUInt32(root, fieldDescriptor))};
                  if (((condition == Condition::EQ) && (num == getCompareToNum())) ||
                     ((condition == Condition::GT) && (static_cast<int>(num) > getCompareToNum())) ||
                     ((condition == Condition::LT) && (static_cast<int>(num) < getCompareToNum()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_FLOAT) {
                  const double num{static_cast<double>(reflection->GetFloat(root, fieldDescriptor))};
                  if ( ((condition == Condition::EQ) && base::equal(num, getCompareToDbl())) ||
                     ((condition == Condition::GT) && (num > getCompareToDbl())) ||
                     ((condition == Condition::LT) && (num < getCompareToDbl()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE) {
                  const double num{reflection->GetDouble(root, fieldDescriptor)};
                  if ( ((condition == Condition::EQ) && base::equal(num, getCompareToDbl())) ||
                     ((condition == Condition::GT) && (num > getCompareToDbl())) ||
                     ((condition == Condition::LT) && (num < getCompareToDbl()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_BOOL) {
                  const bool num{reflection->GetBool(root, fieldDescriptor)};
                  if (num == getCompareToBool()) {
                      // Found it!
                     foundSelected = true;
                  }
               }
               else if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_ENUM) {
                  const google::protobuf::EnumValueDescriptor* enumVal{reflection->GetEnum(root, fieldDescriptor)};
                  const int enumIndex{enumVal->index()};
                  if (((condition == Condition::EQ) && (enumIndex == getCompareToNum())) ||
                     ((condition == Condition::GT) && (enumIndex > getCompareToNum())) ||
                     ((condition == Condition::LT) && (enumIndex < getCompareToNum()))) {
                      // Found it!
                     foundSelected = true;
                  }
               }
            }
         }
      }
   }
}

//------------------------------------------------------------------------------
// printMessage(): Recursive function to print all fields in this message
//---------------------------------------------------------------------------
void PrintSelected::printMessage(std::ostream& soutFields, std::ostream& soutVals, const google::protobuf::Message* const msg)
{
   std::streamsize oldWidth{soutFields.width()};
   std::ostream::fmtflags oldFlags{soutFields.flags()};

   const google::protobuf::Descriptor* descriptor{msg->GetDescriptor()};
   const google::protobuf::Reflection* reflection{msg->GetReflection()};

   const google::protobuf::Message& root = *msg;
   std::string msgName{descriptor->name()};

   int fieldCount{descriptor->field_count()};

   const google::protobuf::FieldDescriptor* fieldDescriptor{};

   std::string msgDivider{";   "};
   std::string fieldDivider{", "};

   bool lastField{};

   if (fieldCount > 0) {
      for (int i = 0; i < fieldCount; i++) {
         if ((i+1) == fieldCount) lastField = true;

         // Get field descriptor (includes messages)
         fieldDescriptor = descriptor->field(i);

         // what type is this field?
         google::protobuf::FieldDescriptor::CppType cppType{fieldDescriptor->cpp_type()};

         if (cppType == google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) {

            // do the same again for this message, etc.
            const google::protobuf::Message& sub_message = reflection->GetMessage(root, fieldDescriptor);
            printMessage(soutFields, soutVals, &sub_message);
         }
         else {
            // not a message: Print the field
            if (reflection->HasField(root, fieldDescriptor)) {
               soutFields <<  std::left << std::setw(12) << fieldDescriptor->name(); // Field name

               // get the value
               switch (cppType) {
                  case google::protobuf::FieldDescriptor::CPPTYPE_STRING: {
                     soutVals <<  std::left << std::setw(12)<<  reflection->GetString(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_INT32: {
                     soutVals <<  std::left << std::setw(12)<<  reflection->GetInt32(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_INT64: {
                     soutVals <<  std::left << std::setw(12)<< reflection->GetInt64(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_UINT32: {
                     soutVals <<  std::left << std::setw(12)<< reflection->GetUInt32(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_UINT64: {
                     soutVals <<  std::left << std::setw(12)<< reflection->GetUInt64(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_DOUBLE: {
                     soutVals  <<  std::left << std::setw(12)<< reflection->GetDouble(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_FLOAT: {
                     soutVals  <<  std::left << std::setw(12)<< reflection->GetFloat(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_BOOL: {
                     soutVals <<  std::left << std::setw(12)<< reflection->GetBool(root, fieldDescriptor);
                     break;
                  }
                  case google::protobuf::FieldDescriptor::CPPTYPE_ENUM: {
                     const google::protobuf::EnumValueDescriptor* enumVal = reflection->GetEnum(root, fieldDescriptor);
                     int enumIndex = enumVal->index();
                     soutVals <<  std::left << std::setw(12)<<  enumIndex;
                     break;
                  }
                  default: {
                     soutVals << "   \t";
                     break;
                  }
               }
            }  // end if has field
            else {
               // This field had no data. We should skip it
            }
            if (lastField) {
               // print message divider
               soutFields << msgDivider;
               soutVals << msgDivider;
            }
            else {
               // print field Divider
               soutFields << fieldDivider;
               soutVals << fieldDivider;
            }
         }
      }
   }

   soutFields.width( oldWidth );
   soutFields.setf( oldFlags );
}

// Set selection data by function call:
//------------------------------------------------------------------------------
// setMsgToken(): Set Event Token of interest
//---------------------------------------------------------------------------
bool PrintSelected::setMsgToken(const unsigned int token)
{
   msgToken = token;
   return true;
}

//------------------------------------------------------------------------------
// setFieldOfInterest(): Set field of interest
//---------------------------------------------------------------------------
bool PrintSelected::setFieldOfInterest(const std::string& field )
{
   fieldNameStr = field;
   return true;
}

//------------------------------------------------------------------------------
// setCompareToValue(): Set string to compare to field
//---------------------------------------------------------------------------
bool PrintSelected::setCompareToValue(const std::string& strVal)
{
   compareStr = strVal;
   return true;
}

//------------------------------------------------------------------------------
// setCompareToValue(): Set integer value to compare to field
//---------------------------------------------------------------------------
bool PrintSelected::setCompareToValue(const int numVal )
{
   compareValI = numVal;
   return true;
}

//------------------------------------------------------------------------------
// setCompareToValue(): Set double value to compare to field
//---------------------------------------------------------------------------
bool PrintSelected::setCompareToValue(const double dblVal )
{
   compareValD = dblVal;
   return true;
}

//------------------------------------------------------------------------------
// setCompareCondition(): Set comparison condition
//---------------------------------------------------------------------------
bool PrintSelected::setCompareCondition(const Condition cc )
{
   condition = cc;
   return true;
}

//------------------------------------------------------------------------------
// setTimeOnly(): Set time only flag
//---------------------------------------------------------------------------
bool PrintSelected::setTimeOnly(const bool flg)
{
   timeOnly = flg;
   return true;
}

//------------------------------------------------------------------------------
// getEventMsgName(): Return the name of the event message in a data record
//---------------------------------------------------------------------------
std::string PrintSelected::getEventMsgName(const google::protobuf::Message* const msg)
{
   std::string name{""};

   const google::protobuf::Descriptor* descriptor{msg->GetDescriptor()};
   const google::protobuf::Reflection* reflection{msg->GetReflection()};

   const google::protobuf::Message& root = *msg;

   int fieldCount{descriptor->field_count()};
   const google::protobuf::FieldDescriptor* fieldDescriptor{};
   eventMsg = nullptr;

   if (descriptor->field_count() > 0) {
      for (int i = 0; i < fieldCount; i++) {

         // Get field descriptor (includes messages)
         fieldDescriptor = descriptor->field(i);

         // Is this a message?
         if (fieldDescriptor->cpp_type() == google::protobuf::FieldDescriptor::CPPTYPE_MESSAGE) {
            // Yes, check has field, then save the name
            if (reflection->HasField(root, fieldDescriptor)) {
               if (fieldDescriptor->name() != "time") {
                  name = fieldDescriptor->name();
                  eventMsg = &(reflection->GetMessage(root, fieldDescriptor));
               }
            }
         }
      }
   }

   return name;
}

//------------------------------------------------------------------------------
// printTimeMsg() -- format a time string for printing
//------------------------------------------------------------------------------
std::string PrintSelected::printTimeMsg(double time)
{
    char cbuf[16]{};
    int hh{};        // Hours
    int mm{};        // Min
    double ss{};     // Sec

    // utc time
    base::time::getHHMMSS(static_cast<double>(time), &hh, &mm, &ss);
    std::sprintf(cbuf, "%02d:%02d:%06.3f", hh, mm, ss);
    std::string timeStr;
    timeStr.assign(cbuf);
    return timeStr;
}

}
}
