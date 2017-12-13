
#include "UsbJoystick_msvc.hpp"

#include "mixr/base/numeric/Number.hpp"
#include "mixr/base/util/platform_api.hpp"

#include <iostream>

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
   cmin = org.cmin;
   cmax = org.cmax;
}

void UsbJoystick::reset()
{
   //queryDeviceSpecs();

   std::cout << "Number of joysticks supported: " << joyGetNumDevs() << std::endl;
   std::cout << "Device selected: " << deviceIndex << std::endl;

   JOYCAPS jc;
   if (joyGetDevCaps(deviceIndex, &jc, sizeof(jc))!=JOYERR_NOERROR) {
      std::cerr << "Please connect a compatible joystick" << std::endl;
   }

   const int naxes = static_cast<int>(jc.wNumAxes);
   const int nbuttons = static_cast<int>(jc.wNumButtons);
   const int MID_VALUE {static_cast<int>(USHRT_MAX/2)};

   // ---
   // when the number if AIs and DIs are both zero, try to get the
   // capabilities of the joystick
   // ---
   if (static_cast<int>(ai.size()) == 0 && static_cast<int>(di.size()) == 0) {

      JOYCAPS jsCaps;  // joystick capabilities

      // query windows for capabilities
      if (joyGetDevCaps(deviceIndex, &jsCaps, sizeof(jsCaps)) == JOYERR_NOERROR) {

         // number of analogs (MS Windows have 8 channels)
         ai.resize(8);
         cmin.resize(8);
         cmax.resize(8);

         // Set channel max/min values
         setMaxMin( 0, static_cast<double>(jsCaps.wXmax), static_cast<double>(jsCaps.wXmin) );
         setMaxMin( 1, static_cast<double>(jsCaps.wYmax), static_cast<double>(jsCaps.wYmin) );
         setMaxMin( 2, static_cast<double>(jsCaps.wZmax), static_cast<double>(jsCaps.wZmin) );
         setMaxMin( 3, static_cast<double>(jsCaps.wRmax), static_cast<double>(jsCaps.wRmin) );
         setMaxMin( 4, static_cast<double>(jsCaps.wUmax), static_cast<double>(jsCaps.wUmin) );
         setMaxMin( 5, static_cast<double>(jsCaps.wVmax), static_cast<double>(jsCaps.wVmin) );

         // resize di array
         di.resize(nbuttons);
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

void UsbJoystick::queryDeviceSpecs()
{
   JOYINFOEX  b={0};
   b.dwSize=sizeof(JOYINFOEX );
   b.dwFlags=JOY_RETURNALL;

   // query all possible joysticks
   for (int i=0; i<=static_cast<int>(joyGetNumDevs()-1); i++) {

      // if any errors exist, continue to next joystick
      if (joyGetPosEx(i,&b)!= JOYERR_NOERROR || joyGetPosEx(i,&b)== JOYERR_UNPLUGGED) {
         b={0};
         break;
      }

      long h = b.dwButtons;

      // test the directions
      // will be 8 directions(inclued diagonals)
      int direction{};
      if (b.dwXpos == 0 && b.dwYpos ==65535)           { direction=5; }
      else if (b.dwXpos == 65535 && b.dwYpos == 0)     { direction=6; }
      else if (b.dwXpos == 65535 && b.dwYpos == 65535) { direction=7; }
      else if(b.dwXpos == 0 && b.dwYpos == 0)          { direction=8; }
      else if (b.dwXpos == 0)                          { direction=1; }
      else if (b.dwXpos == 65535)                      { direction=2; }
      else if (b.dwYpos == 0)                          { direction=3; }
      else if (b.dwYpos == 65535)                      { direction=4; }
   }
}

//------------------------------------------------------------------------------
// Go get our AIs and DIs here
//------------------------------------------------------------------------------
void UsbJoystick::readInputs()
{
   JOYINFOEX js;
   js.dwFlags = JOY_RETURNALL;   // return all joystick information
   js.dwSize  = sizeof(js);      // set the size of the structure / effectively sets version

   // query joystick for its position and button status
   MMRESULT status {joyGetPosEx(deviceIndex, &js)};
   if (status == JOYERR_NOERROR) {

      // First 6 channels are X, Y, Z, R, U, V, and they need to be scaled.
      setInputScaled(0, static_cast<double>(js.dwXpos));
      setInputScaled(1, static_cast<double>(js.dwYpos));
      setInputScaled(2, static_cast<double>(js.dwZpos));
      setInputScaled(3, static_cast<double>(js.dwRpos));
      setInputScaled(4, static_cast<double>(js.dwUpos));
      setInputScaled(5, static_cast<double>(js.dwVpos));

      // Last two channels are set using the POV angle
      {
         double povLR {};
         double povFB {};
         if (js.dwPOV != JOY_POVCENTERED) {

            // right/left
            if (js.dwPOV >= (45*100) && js.dwPOV <= (135*100)) {
               povLR = 1.0;  //right
            } else if (js.dwPOV >= (225*100) && js.dwPOV <= (315*100)) {
               povLR = -1.0; // left
            }

            // back/forward
            if (js.dwPOV >= (135*100) && js.dwPOV <= (225*100)) {
               povFB = 1.0;  // back
            } else if (js.dwPOV >= (315*100) || js.dwPOV <= (45*100)) {
               povFB = -1.0; // forward
            }

         }
         ai[6] = povLR;
         ai[7] = povFB;
      }

      // update all individual button states
      {
         const unsigned long buttons {js.dwButtons};
         for (int i = 0; i < static_cast<int>(di.size()); i++) {
            di[i] = (buttons & (1 << i)) != 0;
         }
      }
   }
}

// Set an analog input channels min/max values
bool UsbJoystick::setMaxMin(const int channel, const double max, const double min)
{
   bool ok {};
   if (channel < static_cast<int>(ai.size())) {
      cmax[channel] = max;
      cmin[channel] = min;
      ok = true;
   }
   return ok;
}

// Set an analog input channel values using a raw input and the max/min values
bool UsbJoystick::setInputScaled(const int cn, const double raw)
{
   bool ok {};
   if (cn < static_cast<int>(ai.size())) {
      const double normalized {(raw - cmin[cn])/(cmax[cn] - cmin[cn])};  // range: [ 0 ... 1 ]
      const double v11 {(normalized * 2.0) - 1.0};                       // range: [ -1 ... 1 ]
      ai[cn] = v11;
      ok = true;
   }
   return ok;
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
