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
 *  FILENAME:   CigiMessage.h
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
 *  11/15/2004 Andrew Sampson                    Version 1.4a Beta
 *  Added the GetValidIGCtrlSOF() function.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Added the CIGI_SPEC modifier to the class declaration for exporting the 
 *  class in a Windows DLL.
 *  
 *  02/20/2006 Greg Basler                       Version 1.6.0
 *  Added the PackagedMsg variable to allow for correct updating of the
 *  host's IG_Ctrl's frame counter.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_MESSAGE_INCLUDED_)
#define _CIGI_MESSAGE_INCLUDED_

#include <list>

#include "CigiAnimationTable.h"
#include "CigiHoldEnvCtrl.h"
#include "CigiMessageBuffer.h"

class CigiSession;

using namespace std;

//=========================================================
//! The base class for Messages
//!
class CIGI_SPEC CigiMessage  
{
public:

friend class CigiSession;


   //==> Management

   //=========================================================
   //! General Constructor
   //!
	CigiMessage();

   //=========================================================
   //! General Destructor
   //!
	virtual ~CigiMessage();


   //==> Public Accessor functions

   //=========================================================
   //! Gets the value indicating if a valid IGCtrl or SOF 
   //!   packet has been packed for the message.
   //!
   //! \return This returns a value indicating if a valid 
   //!   IGCtrl or SOF packet has been packed
   //!
    bool GetValidIGCtrlSOF()
    {
       bool Rslt = false;
       if(CrntMsgBuf != NULL)
          Rslt = CrntMsgBuf->IsValidIGCtrlSOF();

       return(Rslt);
    }


   //==> Processing Member functions
   
   //=========================================================
   //! This creates the message buffers.
   //! \param NumBuf - Number of buffers to create
   //! \param BufLen - The size in bytes of each buffer
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int CreateBuffer(const int NumBuf = 2, const int BufLen = 16384);


protected:

   //==> Member variables

   //=========================================================
   //! Active buffers
   //!
   list<CigiMessageBuffer *> Buffers;

   //=========================================================
   //! Available buffers
   //!
   list<CigiMessageBuffer *> AvailBuff;

   //=========================================================
   //! Current buffer
   //!
   CigiMessageBuffer *CrntMsgBuf;

   //=========================================================
   //! Current fill Buffer<br>
   //! Points to the buffer being currently filled
   //!
   CigiMessageBuffer *CrntFillBuf;

   //=========================================================
   //! Packaged Message<br>
   //! Points to the currently packaged message buffer
   //!
   CigiMessageBuffer *PackagedMsg;

   //=========================================================
   //! Buffer Size<br>
   //! Specifies the size of each buffer.
   //!
	int BufferSize;

   //=========================================================
   //! Animation Table object<br>
   //! A pointer to the Animation Table object for this session
   //!
   CigiAnimationTable *ATbl;

   //=========================================================
   //! 
   //! 
   //!
   CigiHoldEnvCtrl EnvHoldObj;

   //=========================================================
   //! The parent CIGI session for this outgoing message
   //!
   CigiSession *Session;


   //==> Member Protected Functions


   //=========================================================
   //! Sets the Animation Table object pointer
   //! \param AnimationTable - Specifies a Animation table object
   //!
   void SetAnimationTable(CigiAnimationTable *AnimationTable) { ATbl = AnimationTable; }


   //=========================================================
   //! Sets the Session object pointer
   //! \param SessionIn - Specifies a session object
   //!
   void SetSession(CigiSession *SessionIn) { Session = SessionIn; }

};

#endif // !defined(_CIGI_MESSAGE_INCLUDED_)
