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
 *  FILENAME:   CigiIGMsgV3.cpp
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
 *  Removed the inline defnitions for the PackedPointer union.
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

#include "CigiIGMsgV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiIGMsgV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiIGMsgV3::CigiIGMsgV3()
{

   PacketID = CIGI_IG_MSG_PACKET_ID_V3;
   Version = 3;
   MinorVersion = 0;

   MsgID = 0;

   VariableDataSize = MsgLen = 4;
   memset(Msg,0,4);
   PacketSize = 8;

}

// ================================================
// ~CigiIGMsgV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiIGMsgV3::~CigiIGMsgV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGMsgV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{

   PackPointer CDta;

   CigiBaseIGMsg * Data = ( CigiBaseIGMsg *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = Data->PacketSize;

   *CDta.s++ = Data->MsgID;

   memcpy(CDta.c,Data->Msg,Data->MsgLen);

   return(Data->PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGMsgV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{

   PackPointer CDta;


   int tPktSz;

   CDta.c = Buff;

   CDta.c++;  // Step over packet id

   tPktSz = *CDta.c++;  // Get packet size

   if(!Swap)
      MsgID = *CDta.s++;
   else
      CigiSwap2(&MsgID, CDta.s++);

   if(tPktSz > 104)
   {
      PacketSize = 104;
      memcpy(&Msg,CDta.c,99);
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
      memcpy(&Msg,CDta.c,(tPktSz - 4));
      Msg[(tPktSz - 5)] = 0;
   }

   MsgLen = PacketSize - 4;

   return(tPktSz);

}


// ================================================
// GetTruePacketSize
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGMsgV3::GetTruePacketSize(CigiBaseVariableSizePckt &refPacket)
{
   return(refPacket.GetVariableDataSize() + 4);
}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SetMsg
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiIGMsgV3::SetMsg(const Cigi_Ascii *MsgIn, bool bndchk)
{

   MsgLen = strlen(MsgIn);

   if(MsgLen == 0)
   {
      MsgLen = 4;
      memset(Msg,0,4);
   }
   else if(MsgLen >= 99)
   {
      MsgLen = 100;
      memcpy(Msg,MsgIn,99);
      Msg[99] = 0;
   }
   else
   {
      memcpy(Msg,MsgIn,MsgLen);

      int tadj = (MsgLen + 5) % 8;

      tadj = (tadj == 0) ? 1 : (9 - tadj);

      memset(&Msg[MsgLen],0,tadj);

      MsgLen += tadj;
   }

   VariableDataSize = MsgLen;
   PacketSize = VariableDataSize + 4;

   return(CIGI_SUCCESS);
}

