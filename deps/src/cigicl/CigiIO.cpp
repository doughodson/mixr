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
 *  FILENAME:   CigiIO.cpp
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
 *  Defined _EXPORT_CCL_ for exporting the class in a Windows DLL.
 *
 *  10/28/2005 Greg Basler                       Version 1.7
 *  Added a receive (Recv) member function that retreives and stores
 *    the incoming message but does not process it.
 *
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  06/10/2008 Greg Basler                       Version 2.2.0
 *  Corrected the Receive function to work with the new CigiIncomingMsg
 *  functionality.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#define _EXPORT_CCL_


#include "CigiIO.h"


// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiIO
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiIO::CigiIO(CigiIncomingMsg &InMsg, CigiOutgoingMsg &OutMsg)
       : MsgIn(InMsg), MsgOut(OutMsg)
{
   int stat = CIGI_SUCCESS;

   stat = MsgOut.BeginMsg();

}

// ================================================
// ~CigiIO
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiIO::~CigiIO()
{

}

// ====================================================================
// Processing
// ====================================================================

// ================================================
// Send
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIO::Send()
{
   Cigi_uint8 *buff;
   int len;

   int stat = CIGI_SUCCESS;

   stat = MsgOut.UpdateFrameCntr();

   stat = MsgOut.LockMsg();

 	buff = MsgOut.GetMsg(len);

   Write(buff,len);

   stat = MsgOut.UnlockMsg();

   stat = MsgOut.BeginMsg();  // prep for next message

   return(stat);

}

// ================================================
// Receive
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIO::Receive()
{
   int stat = CIGI_SUCCESS;

   MsgIn.AdvanceCrntBuffer();

   Cigi_uint8 *NextBuf = MsgIn.GetMsgBuffer();

   int size = Read(NextBuf,MsgIn.GetMsgBufSize());

   if(size > 0)
   {
      MsgIn.SetCrntMsgSize(size);

      MsgIn.ProcessIncomingMsg();

   }
   else
   {
      size = 0;
      stat = CIGI_ERROR_MISCELLANEOUS;
      MsgIn.SetCrntMsgSize(size);
   }

   return(stat);

}


// ================================================
// Recv
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIO::Recv()
{
   int stat = CIGI_SUCCESS;

   MsgIn.AdvanceCrntBuffer();

   Cigi_uint8 *NextBuf = MsgIn.GetMsgBuffer();

   int size = Read(NextBuf,MsgIn.GetMsgBufSize());

   if(size > 0)
   {
      MsgIn.SetCrntMsgSize(size);
   }
   else
   {
      size = 0;
      stat = CIGI_ERROR_MISCELLANEOUS;
      MsgIn.SetCrntMsgSize(size);
   }

   return(stat);

}



