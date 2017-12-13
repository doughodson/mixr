//==============================================================================
// UsbJoystick -- Linux implementation
//==============================================================================
#include <errno.h>

#include <iostream>
#include <cstdio>

#include <malloc.h>
#include <fcntl.h>
#include <unistd.h>

#include <linux/joystick.h>

#include "UsbJoystick_linux.hpp"
#include "mixr/base/util/str_utils.hpp"
#include "mixr/base/util/system_utils.hpp"

#include "mixr/base/concepts/linkage/AbstractIoData.hpp"

#include "mixr/base/numeric/Number.hpp"

namespace mixr {
namespace linkage {

IMPLEMENT_SUBCLASS(UsbJoystick, "UsbJoystick")
EMPTY_DELETEDATA(UsbJoystick)

BEGIN_SLOTTABLE(UsbJoystick)
   "deviceIndex"           // 1) Device Index (default: 0)
END_SLOTTABLE(UsbJoystick)

BEGIN_SLOT_MAP(UsbJoystick)
   ON_SLOT(1, setSlotDeviceIndex, base::Number)
END_SLOT_MAP()

UsbJoystick::UsbJoystick()
{
   STANDARD_CONSTRUCTOR()
}

void UsbJoystick::copyData(const UsbJoystick& org, const bool)
{
   BaseClass::copyData(org);
   deviceIndex = org.deviceIndex;

   ai = org.ai;
   di = org.di;

}

//------------------------------------------------------------------------------
// reset() -- reset (open) device
//------------------------------------------------------------------------------
void UsbJoystick::reset()
{
   if (deviceName.empty()) {

      // create the device name based on the device index
      {
         char cbuff[128]{};
         // search for device at "/dev/jsX" first
         std::sprintf(cbuff, "/dev/js%d", deviceIndex);
         if (base::doesFileExist(cbuff)) {
            deviceName = cbuff;
         } else {
            // if not found, search for device at "/dev/input/jsX"
            std::sprintf(cbuff, "/dev/input/js%d", deviceIndex);
            if (base::doesFileExist(cbuff)) {
               deviceName = cbuff;
            }
         }
      }

      // if no device found, print message and leave
      if (deviceName.empty()) {
         stream = -1;
         if (isMessageEnabled(MSG_ERROR)) {
            std::cerr << "UsbJoystick::reset(): Joystick device not found" << std::endl;
         }
         return;
      }

      // try opening device
      stream = open(deviceName.c_str(), O_RDONLY | O_NONBLOCK );
      if ((stream == -1) && isMessageEnabled(MSG_ERROR)) {
         std::cerr << "UsbJoystick::reset(): Error opening device at : " << deviceName << std::endl;
      }

      // determine characteristics and print out info message
      if (stream != -1) {

         int driverVersion {};
         ioctl(stream, JSIOCGVERSION, &driverVersion);

         char modelName[128] {};
         ioctl(stream, JSIOCGNAME(sizeof(modelName)), modelName);

         {
            unsigned char numOfAxes {};
            ioctl(stream, JSIOCGAXES, &numOfAxes);
            ai.resize(numOfAxes);
         }

         {
            unsigned char numOfBtns {};
            ioctl(stream, JSIOCGBUTTONS, &numOfBtns);
            di.resize(numOfBtns);
         }

         const int major_version{driverVersion >> 16};
         const int minor_version{(driverVersion >> 8) & 0xff};
         const int bug_fix{driverVersion & 0xff};

         std::cout << std::endl;
         std::cout << "---------------------------" << std::endl;
         std::cout << "USB Joystick Configuration:" << std::endl;
         std::cout << "---------------------------" << std::endl;
         std::cout << "  Port          : " << deviceName << std::endl;
         std::cout << "  Name          : " << modelName  << std::endl;
         std::cout << "  #AIs          : " << static_cast<int>(ai.size()) << std::endl;
         std::cout << "  #DIs          : " << static_cast<int>(di.size()) << std::endl;
         std::cout << "  Driver version: " << major_version << "." << minor_version << "." << bug_fix;
         std::cout  << std::endl << std::endl;
      }
   }
}

bool UsbJoystick::getDiscreteInput(bool* const value, const int bit, const int) const
{
   bool ok{};
   if ( value != nullptr && bit < static_cast<int>(di.size()) ) {
      *value = di[bit];
      ok = true;
   }
   return ok;
}

bool UsbJoystick::getAnalogInput(double* const value, const int channel) const
{
   bool ok{};
   if (value != nullptr && channel < static_cast<int>(ai.size())) {
      *value = ai[channel];
      ok = true;
   }
   return ok;
}

//------------------------------------------------------------------------------
// Go get our AIs and DIs here
//------------------------------------------------------------------------------
void UsbJoystick::readInputs()
{
   js_event js; // joystick event structure

   // Loop reading joystick events ..
   while (true) {

      // read the next joystick event (if any)
      const int status {static_cast<int>(read(stream, &js, sizeof(js)))};

      // Break out of the loop when there are no more events
      if (status != sizeof(js)) break;

      // decode the event
      switch ( js.type & ~JS_EVENT_INIT ) {

         // button event
         case JS_EVENT_BUTTON:
            {
            const int n {js.number};
            if (n < static_cast<int>(di.size()))
                di[n] = static_cast<bool>(js.value);
            }
         break;

         // axis event
         case JS_EVENT_AXIS:
            {
            const int n {js.number};
            if (n < static_cast<int>(ai.size()))
                ai[n] = (js.value / 32767.0);
            }
            break;

      }
   }
}

bool UsbJoystick::setSlotDeviceIndex(const base::Number* const msg)
{
   bool ok{};
   if (msg != nullptr) {
      deviceIndex = msg->getInt();
      ok = true;
   }
   return ok;
}

}
}

