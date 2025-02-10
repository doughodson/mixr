
#ifndef __mixr_linkage_IoData_HPP__
#define __mixr_linkage_IoData_HPP__

#include "mixr/base/concepts/linkage/IIoData.hpp"

#include <vector>

namespace mixr {
namespace base { class Integer; }
namespace linkage {

//------------------------------------------------------------------------------
// Class: IoData
//
// Description: General purpose I/O data buffer; users can specify the number
//              channels for each I/O type.
//
// Note:
//    *** Channel numbers are all one(1) based.  For example, the range of
//    AI channels is from one to getNumAnalogInputChannels(). ***
//
// Factory name: IoData
// Slots:
//    numAI    <Integer>   ! Number of analog inputs (AIs)
//    numAO    <Integer>   ! Number of analog outputs (AOs)
//    numDI    <Integer>   ! Number of discrete inputs (DIs)
//    numDO    <Integer>   ! Number of discrete outputs (DOs)
//
//------------------------------------------------------------------------------
class IoData : public base::IIoData
{
   DECLARE_SUBCLASS(IoData, base::IIoData)

public:
   IoData();

   bool setNumAI(const int);
   bool setNumAO(const int);
   bool setNumDI(const int);
   bool setNumDO(const int);

   // ---
   // Input channels
   // ---
   int getNumAnalogInputChannels() const override;
   int getNumDiscreteInputChannels() const override;

   bool getAnalogInput(const int channel, double* const value) const override;
   bool getDiscreteInput(const int channel, bool* const value) const override;
   bool setAnalogInput(const int channel, const double value) override;
   bool setDiscreteInput(const int channel, const bool value) override;

   // ---
   // Output channels
   // ---
   int getNumAnalogOutputChannels() const override;
   int getNumDiscreteOutputChannels() const override;

   bool getAnalogOutput(const int channel, double* const value) const override;
   bool getDiscreteOutput(const int channel, bool* const value) const override;
   bool setAnalogOutput(const int channel, const double value) override;
   bool setDiscreteOutput(const int channel, const bool value) override;

   void clear() override;

private:
   std::vector<double> ai_table;
   std::vector<double> ao_table;
   std::vector<double> di_table;
   std::vector<double> do_table;

private:
   // slot table helper methods
   bool setSlotNumAI(const base::Integer* const);
   bool setSlotNumAO(const base::Integer* const);
   bool setSlotNumDI(const base::Integer* const);
   bool setSlotNumDO(const base::Integer* const);
};

}
}

#endif
