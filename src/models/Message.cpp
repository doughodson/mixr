
#include "mixr/models/Message.hpp"

#include "mixr/base/String.hpp"
#include "mixr/base/List.hpp"

namespace mixr {
namespace models {

IMPLEMENT_SUBCLASS(Message, "Message")
EMPTY_SLOTTABLE(Message)

Message::Message()
{
   initData();
}

void Message::initData()
{
   senderName = new base::String();
}

void Message::deleteData()
{
   if (senderName != nullptr) { senderName->unref();  senderName = nullptr; }
   if (destNames  != nullptr) { destNames->unref();   destNames  = nullptr; }
}

void Message::copyData(const Message& org, const bool cc)
{
   BaseClass::copyData(org);
   if (cc) initData();

   timeStamp = org.timeStamp;
   lifeSpan = org.lifeSpan;
   ack = org.ack;

   if (senderName != nullptr) {
      senderName->unref();
      senderName = nullptr;
   }
   if (org.senderName != nullptr)
      senderName = org.senderName->clone();

   if (destNames != nullptr) {
      destNames->unref();
      destNames = nullptr;
   }
   if (org.destNames != nullptr)
      destNames = org.destNames->clone();
}

//-------------------------------------------------------------------------
// set sender information
//-------------------------------------------------------------------------
void Message::setSenderName(const base::String* const sName)
{
   if(sName != nullptr) {
      *senderName = *sName;
   }
}

void Message::setSenderName(const char* const sName)
{
   if(sName != nullptr) {
      *senderName = sName;
   }
}

//-------------------------------------------------------------------------
// add / remove / clear destination methods
//-------------------------------------------------------------------------
bool Message::addDestName(base::String* const name)
{
   bool ret{};
   if(name != nullptr) {
      if (destNames == nullptr)
         destNames = new base::List();
      destNames->addTail(name);
      ret = true;
   }
   return ret;
}

bool Message::addDestName(const char* const name)
{
   bool ret{};
   if (name != nullptr) {
      const auto p = new base::String(name);
      ret = addDestName(p);
   }
   return ret;
}

void Message::removeDestName(base::String* const name)
{
   if (destNames != nullptr) {
      destNames->remove(name);
      if (destNames->entries() == 0) {
         destNames->unref();
         destNames = nullptr;
      }
   }
}

void Message::clearDestNames()
{
   if (destNames != nullptr) {
      destNames->unref();
      destNames = nullptr;
   }
}

}
}
