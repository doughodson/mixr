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
 *  FILENAME:   CigiIO.h
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
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_IO_INCLUDED_)
#define _CIGI_IO_INCLUDED_


#include "CigiIncomingMsg.h"
#include "CigiOutgoingMsg.h"


//=========================================================
//! The base class for Cigi IO.<br>
//! This class must be inherited.
//!
class CIGI_SPEC CigiIO  
{
public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiIO(CigiIncomingMsg &InMsg, CigiOutgoingMsg &OutMsg);

   //=========================================================
   //! General Destructor
   //!
	virtual ~CigiIO();



   //==> Transmission Member functions


   //=========================================================
   //! Processes and send a message.
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int Send(void);

   //=========================================================
   //! Receive and processes a message
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int Receive(void);

   //=========================================================
   //! Receive a message and store it for later processing
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int Recv(void);


protected:

   //==> Member variables

   //=========================================================
   //! Message In<br>
   //! A referance to a Cigi incoming message object.
   //!
   CigiIncomingMsg &MsgIn;

   //=========================================================
   //! Message Out<br>
   //! A referance to a Cigi outgoing message object.
   //!
   CigiOutgoingMsg &MsgOut;

   //==> Member protected functions

   //=========================================================
   //! A pure virtual function to output the data.<br>
   //! This function is not implemented by this class.
   //! \param Buffer - A pointer to the buffer to write
   //! \param Length - The length of the message in bytes.
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Write(Cigi_uint8 * Buffer, int Length) =0;

   //=========================================================
   //! A pure virtual function to input the data.<br>
   //! This function is not implemented by this class.
   //! \param Buffer - A pointer to the buffer to fill.
   //! \param MaxLength - The maximum length allowed for the message in bytes.
   //! \return This returns the number of bytes in the message
   //!
   virtual int Read(Cigi_uint8 * Buffer, int MaxLength) =0;


};

#endif // !defined(_CIGI_IO_INCLUDED_)
