
#include "mixr/linkage/IoHandler.hpp"

#include "mixr/base/concepts/linkage/AbstractIoData.hpp"
#include "mixr/base/concepts/linkage/AbstractIoDevice.hpp"

#include "IoPeriodicThread.hpp"

#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/Pair.hpp"
#include "mixr/base/PairStream.hpp"
#include "mixr/base/units/Frequencies.hpp"

namespace mixr {
namespace linkage {

IMPLEMENT_ABSTRACT_SUBCLASS(IoHandler, "BaseIoHandler")

BEGIN_SLOTTABLE(IoHandler)
   "ioData",      // 1) Combined input/output data
   "inputData",   // 2) Individual input data (default: none)
   "outputData",  // 3) Individual output data (default: none)
   "devices",     // 4) List of I/O device handlers (IoDevice objects) (default: none)
   "rate",        // 5) Thread rate (default: 50hz)
   "priority"     // 6) Thread priority (zero(0) is lowest, one(1) is highest) (Number, default: 0.5 )
END_SLOTTABLE(IoHandler)

BEGIN_SLOT_MAP(IoHandler)
   ON_SLOT(1, setSlotIoData,     base::AbstractIoData)
   ON_SLOT(2, setSlotInputData,  base::AbstractIoData)
   ON_SLOT(3, setSlotOutputData, base::AbstractIoData)
   ON_SLOT(4, setSlotDevices,    base::PairStream)
   ON_SLOT(5, setSlotRate,       base::Frequency)
   ON_SLOT(6, setSlotPriority,   base::Number)
END_SLOT_MAP()

IoHandler::IoHandler()
{
   STANDARD_CONSTRUCTOR()
}

void IoHandler::copyData(const IoHandler& org, const bool)
{
   BaseClass::copyData(org);

   // clear the I/O buffers and list of devices
   setSlotIoData(nullptr);
   setSlotDevices(nullptr);

   // ---
   // copy the I/O buffers
   // ---
   if (org.inData != nullptr && org.inData == org.outData) {
      // Common input/output buffer
      const auto copy = static_cast<base::AbstractIoData*>(org.inData->clone());
      setSlotIoData(copy);
      copy->unref();
   } else {
      // Separate input/output buffers
      if (org.inData != nullptr) {
         const auto copy = static_cast<base::AbstractIoData*>(org.inData->clone());
         setSlotInputData(copy);
         copy->unref();
      }
      if (org.outData != nullptr) {
         const auto copy = static_cast<base::AbstractIoData*>(org.outData->clone());
         setSlotOutputData(copy);
         copy->unref();
      }
   }

   // ---
   // copy the list of I/O devices
   // ---
   if (org.devices != nullptr) {
      const auto copy = static_cast<base::PairStream*>(org.devices->clone());
      setSlotDevices(copy);
      copy->unref();
   }

   rate = 50;
   pri = 0.0;

   if (periodicThread != nullptr) {
      periodicThread->terminate();
      periodicThread = nullptr;
   }
}

void IoHandler::deleteData()
{
   inData = nullptr;
   outData = nullptr;
   devices = nullptr;

   if (periodicThread != nullptr) {
      periodicThread->terminate();
      periodicThread = nullptr;
   }
}

void IoHandler::reset()
{
   BaseClass::reset();

   // Reset our I/O devices
   if (devices != nullptr) {
      base::List::Item* item{devices->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = static_cast<base::AbstractIoDevice*>(pair->object());
         p->reset();
         item = item->getNext();
      }
   }

   // clear both input and output data
   if (inData != nullptr) inData->clear();
   if (outData != nullptr) outData->clear();
}

bool IoHandler::shutdownNotification()
{
   // Zero (unref) our periodic task thread object (of any).  The thread's function has ref()'d
   // this object, so it won't be deleted until the thread terminates, which it
   // will based on our BaseClass::isShutdown() function.  But at least we won't
   // mistakenly think that it's still around.
   if (periodicThread != nullptr) {
      periodicThread->terminate();
      periodicThread = nullptr;
   }

   return BaseClass::shutdownNotification();
}

void IoHandler::readDeviceInputs(const double dt)
{
   // process our I/O devices
   if (devices != nullptr) {
      base::List::Item* item{devices->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = static_cast<base::AbstractIoDevice*>(pair->object());
         p->processInputs(dt, inData);
         item = item->getNext();
      }
   }
}

void IoHandler::writeDeviceOutputs(const double dt)
{
   // process our I/O devices
   if (devices != nullptr) {
      base::List::Item* item{devices->getFirstItem()};
      while (item != nullptr) {
         const auto pair = static_cast<base::Pair*>(item->getValue());
         const auto p = static_cast<base::AbstractIoDevice*>(pair->object());
         p->processOutputs(dt, outData);
         item = item->getNext();
      }
   }
}

// setup and start asynchronous processing (i.e., create a data acq thread)
void IoHandler::startAsyncProcessingImpl()
{
   if ( periodicThread == nullptr ) {
      periodicThread = new IoPeriodicThread(this, getRate());
      periodicThread->unref(); // 'periodicTask' is a safe_ptr<>

      bool ok{periodicThread->start(getPriority())};
      if (!ok) {
         periodicThread = nullptr;
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "IoHandler::createDataThread(): ERROR, failed to create the thread!" << std::endl;
         }
      }
   }
}

bool IoHandler::setSlotIoData(base::AbstractIoData* const msg)
{
   inData = msg;
   outData = msg;
   return true;
}

bool IoHandler::setSlotInputData(base::AbstractIoData* const msg)
{
   inData = msg;
   return true;
}

bool IoHandler::setSlotOutputData(base::AbstractIoData* const msg)
{
   outData = msg;
   return true;
}

bool IoHandler::setSlotDevices(base::PairStream* const list)
{
   bool ok{true};

   if (list != nullptr) {
      // check to make sure all objects on the list are I/O Devices
      int cnt{};
      base::List::Item* item{list->getFirstItem()};
      while (item != nullptr) {
         cnt++;
         const auto pair = static_cast<base::Pair*>(item->getValue());
         ok = pair->object()->isClassType(typeid(base::AbstractIoDevice));
         if (ok) {
            //static_cast<AbstractIoDevice*>(pair->object())->container(this);
         } else {
            std::cerr << "IoHandler::setSlotDevices(): Item number " << cnt;
            std::cerr << " on the list is a non-IoDevice component!" << std::endl;
         }
         item = item->getNext();
      }
   }

   if (ok) devices = list;

   return ok;
}

bool IoHandler::setSlotRate(const base::Frequency* const msg)
{
    bool ok{};
    if (msg != nullptr) {
        const double x{base::Hertz::convertStatic(*msg)};
        if (x > 0) {
            rate = x;
            ok = true;
        } else {
            std::cerr << "IoHandler::setSlotRate: Rate is invalid; must be greater than zero" << std::endl;
        }
    }
    return ok;
}

bool IoHandler::setSlotPriority(const base::Number* const num)
{
    bool ok{};
    if (num != nullptr) {
        const double x{num->getReal()};
        if (x >= 0 && x <= 1.0) {
            pri = x;
            ok = true;
        } else {
            std::cerr << "IoHandler::setSlotPriority: Priority is invalid, range: [0 .. 1]" << std::endl;
        }
    }
    return ok;
}

}
}
