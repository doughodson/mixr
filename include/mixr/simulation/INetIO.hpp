
#ifndef __mixr_simulation_INetIO_HPP__
#define __mixr_simulation_INetIO_HPP__

#include "mixr/base/Component.hpp"

namespace mixr {
namespace simulation {

//------------------------------------------------------------------------------
// Class: INetIO
// Description: Interface class for interoperability Network I/O.
//
//    Defines the framework for interoperability networks (e.g., DIS, HLA,
//    TENA, etc).  Derived classes will manage the network specific protocols.
//
// Factory name: INetIO
//
// Input/Output frames:
//
//    The functions inputFrame() and outputFrame() need to be called by our
//    container object (e.g., Station class) to process the incoming and out-
//    going entities, respectively.  For DIS, these can be called by different
//    threads, and for HLA they need to be called from the same thread.
//------------------------------------------------------------------------------
class INetIO : public base::Component
{
   DECLARE_SUBCLASS(INetIO, base::Component)

public:
   INetIO();

   // Max network ID
   static const int MAX_NETWORK_ID{2};

   //
   // interface used by Station class
   //

   // Updates the 'input' side of the network
   virtual void inputFrame(const double dt) =0;

   // Updates the 'output' side of the network
   virtual void outputFrame(const double dt) =0;

   // Network ID number
   virtual unsigned short getNetworkID() const =0;

};

}
}

#endif
