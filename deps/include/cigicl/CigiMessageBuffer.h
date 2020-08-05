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
 *  FILENAME:   CigiMessageBuffer.h
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
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Initial Release.
 *  
 *  07/15/2008 Greg Basler                       Version 2.2.0
 *  Improved buffer creation (provided by Andrew Sampson).
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_MESSAGE_BUFFER_INCLUDED_)
#define _CIGI_MESSAGE_BUFFER_INCLUDED_

#include <string.h> // for memset

#include "CigiTypes.h"
#include "CigiVersionID.h"



class CigiOutgoingMsg;
class CigiIncomingMsg;
class CigiBaseIGCtrl;
class CigiBaseSOF;


//=========================================================
//! The class Containing the Message buffer
//!
class CIGI_SPEC CigiMessageBuffer
{
public:

   friend class CigiOutgoingMsg;
   friend class CigiIncomingMsg;


   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiMessageBuffer(const int BufLen = 16384)
      : Buffer(NULL)
      , BufferSize(0)
      , FillBufferPos(NULL)
      , BufferFillCnt(0)
      , Active(false)
      , Locked(false)
      , ValidIGCtrlSOF(false)
      , DataPresent(false)
      , PackIGCtrl(NULL)
      , PackSOF(NULL)
   {
      // This creates a buffer that has double word aligned size

      // Divide to get the number of double-words.  The +1 is to cover any 
      // remainder.
      BufferSize = ((BufLen/sizeof(Cigi_uint64)) + 1);
      // Multiply to convert the number of double-words to bytes
      BufferSize *= sizeof(Cigi_uint64);

      // Allocate the message buffer
      Buffer = new Cigi_uint8[BufferSize];

      // Initialize empty buffer
      memset( Buffer, 0, BufferSize );

   }

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiMessageBuffer(void)
   {
      if(Buffer != NULL)
      {
         delete [] Buffer;
         Buffer = NULL;
      }
   }


   //==> Public Accessor functions

   //=========================================================
   //! Gets the Cigi Version used by this buffer.
   //!
   //! \return The Cigi Version used by this buffer.
   //!
   CigiVersionID GetCigiVersion(void) { return(CigiVersion); }

   //=========================================================
   //! Gets the Major Cigi Version used by this buffer.
   //!
   //! \return The Major Cigi Version used by this buffer.
   //!
   int GetCigiMajorVersion(void) { return(CigiVersion.CigiMajorVersion); }

   //=========================================================
   //! Gets the Minor Cigi Version used by this buffer.
   //!
   //! \return The Minor Cigi Version used by this buffer.
   //!
   int GetCigiMinorVersion(void) { return(CigiVersion.CigiMinorVersion); }

   //=========================================================
   //! Gets the size of the buffer.
   //!
   //! \return The size of the buffer.
   //!
   int GetBufferSize(void) { return(BufferSize); }

   //=========================================================
   //! Gets the value indicating if the buffer is active.
   //!
   //! \return This returns a value indicating if the buffer
   //!    is active
   //!
   bool IsActive(void) { return(Active); }

   //=========================================================
   //! Gets the value indicating if the buffer is locked.
   //!
   //! \return This returns a value indicating if the buffer
   //!    is locked
   //!
   bool IsLocked(void) { return(Locked); }

   //=========================================================
   //! Gets the value indicating if a valid IG Control or SOF
   //!   packet has been packed into the buffer.
   //!
   //! \return This returns a value indicating if a valid IG
   //!   Control or SOF packet has been packed into the buffer.
   //!
   bool IsValidIGCtrlSOF(void) { return(ValidIGCtrlSOF); }

   //=========================================================
   //! Gets the value indicating if valid data is in the buffer.
   //!
   //! \return This returns a value indicating if valid data
   //!    is in the buffer
   //!
   bool IsDataPresent(void) { return(DataPresent); }




protected:

   //==> Member variables

   //=========================================================
   //! Buffer<br>
   //! The pointer to the buffer
   //!
   Cigi_uint8 *Buffer;

   //=========================================================
   //! CigiVersion<br>
   //! The CIGI Version currently used by this buffer.
   //!
   CigiVersionID CigiVersion;

   //=========================================================
   //! BufferSize<br>
   //! The size of the buffer.
   //!
   int BufferSize;

   //=========================================================
   //! FillBufferPos<br>
   //! A pointer to the next byte available for filling.
   //!
   Cigi_uint8 *FillBufferPos;

   //=========================================================
   //! BufferFillCnt<br>
   //! The number of bytes currently packed into the buffer.
   //!
   int BufferFillCnt;

   //=========================================================
   //! Active<br>
   //! A flag indicating whether the buffer is active.
   //!
   bool Active;

   //=========================================================
   //! Locked<br>
   //! A flag indicating whether the buffer is Locked.
   //!
   bool Locked;

   //=========================================================
   //! ValidIGCtrlSOF<br>
   //! A flag indicating whether the buffer contains a valid
   //!   IG Control or SOF packet.
   //!
   bool ValidIGCtrlSOF;

   //=========================================================
   //! DataPresent<br>
   //! A flag indicating whether the buffer contains valid data.
   //!
   bool DataPresent;

   //=========================================================
   //! Pack IG Ctrl pointer<br>
   //! A pointer to the IG Control packet object that will
   //!   pack the IG Control data into this buffer.
   //!
   CigiBaseIGCtrl *PackIGCtrl;

   //=========================================================
   //! Pack SOF pointer<br>
   //! A pointer to the SOF packet object that will
   //!   pack the SOF data into this buffer.
   //!
   CigiBaseSOF *PackSOF;

};


#endif   //  _CIGI_MESSAGE_BUFFER_INCLUDED_

