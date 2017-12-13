
#ifndef __mixr_linkage_IoData_H__
#define __mixr_linkage_IoData_H__

#include "mixr/base/concepts/linkage/AbstractIoData.hpp"

#include <vector>

namespace mixr {
namespace base { class Number; }
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
//    numAI    <Number>   ! Number of analog inputs (AIs)
//    numAO    <Number>   ! Number of analog outputs (AOs)
//    numDI    <Number>   ! Number of discrete inputs (DIs)
//    numDO    <Number>   ! Number of discrete outputs (DOs)
//
//------------------------------------------------------------------------------
class IoData : public base::AbstractIoData
{
   DECLARE_SUBCLASS(IoData, base::AbstractIoData)

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
   bool setSlotNumAI(const base::Number* const);
   bool setSlotNumAO(const base::Number* const);
   bool setSlotNumDI(const base::Number* const);
   bool setSlotNumDO(const base::Number* const);
};

}
}

#endif
