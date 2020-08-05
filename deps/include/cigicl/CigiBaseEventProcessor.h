/** <pre>
 *  The SDK is used to create and format CIGI compliant messages.
 *  Copyright (c) 2001-2005 The Boeing Company
 *  
 *  This library is free software; you can redistribute it and/or modify it 
 *  under the terms of the GNU Lesser General Public License as published by 
 *  the Free Software Foundation; either version 2.1 of the License, or (at 
 *  your option) any later version.
 *  
 *  This library is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 *  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser Public License for more 
 *  details.
 *  
 *  You should have received a copy of the GNU Lesser General Public License 
 *  along with this library; if not, write to the Free Software Foundation, 
 *  Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *  
 *  FILENAME:   CigiBaseEventProcessor.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface (CIGI) SDK
 *  
 *  PROGRAM DESCRIPTION: 
 *  ...
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                SCR NUMBER
 *  DESCRIPTION OF CHANGE........................
 *  
 *  09/17/2003 Greg Basler                       CIGI_CR_DR_1
 *  Initial Release.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Added the CIGI_SPEC modifier to the class declaration for exporting the 
 *  class in a Windows DLL.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_BASE_EVENT_PROCESSOR_INCLUDED_)
#define _CIGI_BASE_EVENT_PROCESSOR_INCLUDED_

#include "CigiBasePacket.h"


//=========================================================
//! The base class for the Event Packet Processing
//!
class CIGI_SPEC CigiBaseEventProcessor  
{
public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseEventProcessor() { };

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseEventProcessor() { };



   //==> Incoming Packet Processing

   //=========================================================
   //! A pure virtual Incoming Packet Processing function.  
   //! This function is not implemented in this class.
   //!
   virtual void OnPacketReceived(CigiBasePacket *Packet) = 0;

};

#endif // !defined(_CIGI_BASE_EVENT_PROCESSOR_INCLUDED_)
