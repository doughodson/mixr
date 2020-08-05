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
 *  FILENAME:   CigiIGMsgV2.cpp
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
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed native C++ types to use CCL-defined types instead where appropriate.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the class constructor to initialize the MinorVersion member 
 *  variable.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Corrected a few initialization and message setting problems.
 *  Added Variable length packet processing
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiIGMsgV2.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"

#include <string.h>


#ifdef CIGI_LITTLE_ENDIAN
   #define CIGI_SCOPY2 CigiSwap2
   #define CIGI_SCOPY4 CigiSwap4
   #define CIGI_SCOPY8 CigiSwap8
#else
   #define CIGI_SCOPY2 CigiCopy2
   #define CIGI_SCOPY4 CigiCopy4
   #define CIGI_SCOPY8 CigiCopy8
#endif




// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiIGMsgV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiIGMsgV2::CigiIGMsgV2()
{

   PacketID = CIGI_IG_MSG_PACKET_ID_V2;
   PacketSize = 8;
   Version = 2;
   MinorVersion = 0;

   MsgID = 0;
   memset(Msg,0,4);
   VariableDataSize = MsgLen = 4;

}

// ================================================
// ~CigiIGMsgV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiIGMsgV2::~CigiIGMsgV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGMsgV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{

   Cigi_int16 tbuf;

   CigiBaseIGMsg * Data = ( CigiBaseIGMsg *)Base;

   Cigi_uint8 *CDta = Buff;

   *CDta++ = PacketID;
   *CDta++ = Data->PacketSize;

   CIGI_SCOPY2(&tbuf, &Data->MsgID);
   memcpy(CDta,&tbuf,2);
   CDta += 2;


   memcpy(CDta,Data->Msg,Data->MsgLen);

   return(Data->PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGMsgV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{

   Cigi_int16 tbuf;
   int tPktSz;

   Cigi_uint8 *CDta = Buff;

   CDta++;  // Step over packet id

   tPktSz = *CDta++;  // Get packet size

   memcpy(&tbuf,CDta,2);
   CIGI_SCOPY2(&MsgID, &tbuf);

   CDta += 2;

   if(tPktSz > 104)
   {
      PacketSize = 104;
      memcpy(&Msg,CDta,99);
      Msg[99] = 0;
   }
   else if(tPktSz <= 5)
   {
      PacketSize = tPktSz;
      Msg[0] = 0;
   }
   else
   {
      PacketSize = tPktSz;
      memcpy(&Msg,CDta,(tPktSz - 4));
      Msg[(tPktSz - 5)] = 0;
   }

   MsgLen = PacketSize - 4;

   return(tPktSz);

}


// ================================================
// GetTruePacketSize
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGMsgV2::GetTruePacketSize(CigiBaseVariableSizePckt &refPacket)
{
   return(refPacket.GetVariableDataSize() + 4);
}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SetMsg
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGMsgV2::SetMsg(const Cigi_Ascii *MsgIn, bool bndchk)
{

   MsgLen = strlen(MsgIn);

   if(MsgLen == 0)
   {
      MsgLen = 4;
      memset(Msg,0,4);
   }
   else if(MsgLen >= 95)
   {
      MsgLen = 100;
      memcpy(Msg,MsgIn,95);
      memset(&Msg[MsgLen],0,5);
      Msg[99] = 0;
   }
   else
   {
      memcpy(Msg,MsgIn,MsgLen);

      int tadj = (MsgLen + 1) % 4;

      tadj = (tadj == 0) ? 1 : (5 - tadj);

      memset(&Msg[MsgLen],0,(tadj+4));

      MsgLen += tadj;
   }

   VariableDataSize = MsgLen;
   PacketSize = VariableDataSize + 4;

   return(CIGI_SUCCESS);
}

