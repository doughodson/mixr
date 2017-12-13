
#ifndef __mixr_linkage_windows_UsbJoystick_msvc_H__
#define __mixr_linkage_windows_UsbJoystick_msvc_H__

#include "mixr/linkage/IoDevice.hpp"

#include <vector>

namespace mixr {
namespace base { class Number; class AbstractIoData; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: UsbJoystick
//
// Notes:
//    1) Standard MS Windows joysticks have 8 channels, which are mapped ...
//          channel     Axis
//             0         X
//             1         Y
//             2         Z
//             3         R
//             4         U
//             5         V
//             6         POV: back(1.0); forward(-1.0); center(0.0)
//             7         POV: right(1.0); left(-1.0); center(0.0)
//
// Factory name: UsbJoystick
//------------------------------------------------------------------------------
class UsbJoystick : public IoDevice
{
    DECLARE_SUBCLASS(UsbJoystick, IoDevice)

public:
   UsbJoystick();

   void reset() override;

   // DI methods
   int getNumDiscreteInputChannels() const override                                               { return static_cast<int>(di.size()); }
   int getNumDiscreteInputPorts() const override                                                  { return 1;     }
   bool getDiscreteInput(bool* const value, const int channel, const int port) const override;

   // DO methods
   int getNumDiscreteOutputChannels() const override                                              { return 0; }
   int getNumDiscreteOutputPorts() const override                                                 { return 0; }
   bool setDiscreteOutput(const bool value, const int channel, const int port) override           { return false; }

   // AI methods
   int getNumAnalogInputs() const override                                                        { return static_cast<int>(ai.size()); }
   bool getAnalogInput(double* const value, const int channel) const override;

   // AO methods
   int getNumAnalogOutputs() const override                                                       { return 0;     }
   bool setAnalogOutput(const double value, const int channel) override                           { return false; }

private:
   void readInputs() final;
   void writeOutputs() override    {}

private:
   int deviceIndex {};
   void queryDeviceSpecs();
   bool setMaxMin(const int channel, const double max, const double min);
   bool setInputScaled(const int channel, const double raw);

   // data buffer for analog and discrete value arrays
   std::vector<double> ai;
   std::vector<bool> di;

   std::vector<double> cmin;   // channel min values
   std::vector<double> cmax;   // channel max values

private:
   // slot table helper methods
   bool setSlotDeviceIndex(const base::Number* const);
};

}
}

#endif


