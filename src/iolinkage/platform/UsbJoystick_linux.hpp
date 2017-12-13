
#ifndef __mixr_linkage_platform_UsbJoystick_linux_H__
#define __mixr_linkage_platform_UsbJoystick_linux_H__

#include "mixr/iolinkage/IoDevice.hpp"

#include <string>
#include <vector>

namespace mixr {
namespace base { class Number; class AbstractIoData; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: UsbJoystick
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
   void writeOutputs() override     {}

private:
   // data buffer for analog and discrete value arrays
   std::vector<double> ai;
   std::vector<bool> di;

   int deviceIndex {};
   int stream {};
   std::string deviceName;

private:
   // slot table helper methods
   bool setSlotDeviceIndex(const base::Number* const);
};

}
}

#endif

