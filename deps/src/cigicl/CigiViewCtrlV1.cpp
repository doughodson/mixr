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
 *  FILENAME:   CigiViewCtrlV1.cpp
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
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiViewCtrlV1.h"
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
// CigiViewCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiViewCtrlV1::CigiViewCtrlV1()
{

   PacketID = CIGI_VIEW_CTRL_PACKET_ID_V1;
   PacketSize = CIGI_VIEW_CTRL_PACKET_SIZE_V1;
   Version = 1;
   MinorVersion = 0;

   EntityID = 0;
   ViewID = 0;
   GroupID = 0;
   XOffEn = true;
   YOffEn = true;
   ZOffEn = true;
   RollEn = true;
   PitchEn = true;
   YawEn = true;
   XOff = 0.0;
   YOff = 0.0;
   ZOff = 0.0;
   Roll = 0.0;
   Pitch = 0.0;
   Yaw = 0.0;

}

// ================================================
// ~CigiViewCtrlV1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiViewCtrlV1::~CigiViewCtrlV1()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiViewCtrlV1::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseViewCtrl * Data = ( CigiBaseViewCtrl *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->EntityID);

   Cigi_uint8 HDta = 0;

   if(Data->ViewID < 32)
      HDta = (Cigi_uint8)((Data->ViewID << 3) & 0xf8);
   else
      HDta = (Cigi_uint8)0xf8;

   if(Data->GroupID < 8)
      HDta |= (Cigi_uint8)(Data->GroupID & 0x07);
   else
      HDta |= (Cigi_uint8)0x07;

   *CDta.c++ = HDta;
   *CDta.c++ = 0;
   *CDta.c++ = 0;
   *CDta.c++ = 0;

   CIGI_SCOPY4(CDta.f++, &Data->XOff);
   CIGI_SCOPY4(CDta.f++, &Data->YOff);
   CIGI_SCOPY4(CDta.f++, &Data->ZOff);
   CIGI_SCOPY4(CDta.f++, &Data->Roll);
   CIGI_SCOPY4(CDta.f++, &Data->Pitch);
   CIGI_SCOPY4(CDta.f++, &Data->Yaw);

   memcpy(Buff,tBuf,CIGI_VIEW_CTRL_PACKET_SIZE_V1);

   return(PacketSize);



}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiViewCtrlV1::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_VIEW_CTRL_PACKET_SIZE_V1);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&EntityID, CDta.s++);

   Cigi_uint8 HDta = *CDta.c++;

   ViewID = (Cigi_uint16)((HDta >> 3) & 0x1f);
   GroupID = (Cigi_uint8)(HDta & 0x07);

   CDta.c += 3;
   
   CIGI_SCOPY4(&XOff, CDta.f++);
   CIGI_SCOPY4(&YOff, CDta.f++);
   CIGI_SCOPY4(&ZOff, CDta.f++);
   CIGI_SCOPY4(&Roll, CDta.f++);
   CIGI_SCOPY4(&Pitch, CDta.f++);
   CIGI_SCOPY4(&Yaw, CDta.f++);

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// ViewID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiViewCtrlV1::SetViewID(const Cigi_uint8 ViewIDIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ViewIDIn < 0)||(ViewIDIn > 31)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ViewID",(Cigi_uint16)ViewIDIn,0,31);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ViewID = (Cigi_uint16)ViewIDIn;
   return(CIGI_SUCCESS);

}


// ================================================
// GroupID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiViewCtrlV1::SetGroupID(const Cigi_uint8 GroupIDIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((GroupIDIn < 0)||(GroupIDIn > 7)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("GroupID",(Cigi_uint8)GroupIDIn,0,7);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   GroupID = GroupIDIn;
   return(CIGI_SUCCESS);

}


