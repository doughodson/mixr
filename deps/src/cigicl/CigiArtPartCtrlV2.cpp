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
 *  FILENAME:   CigiArtPartCtrlV2.cpp
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

#include "CigiArtPartCtrlV2.h"
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
// CigiArtPartCtrlV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiArtPartCtrlV2::CigiArtPartCtrlV2()
{

   PacketID = CIGI_ART_PART_CTRL_PACKET_ID_V2;
   PacketSize = CIGI_ART_PART_CTRL_PACKET_SIZE_V2;
   Version = 2;
   MinorVersion = 0;

   EntityID = 0;
   ArtPartIDV1 = 0;
   ArtPartIDV3 = 0;
   ArtPartEn = false;
   XOffEn = false;
   YOffEn = false;
   ZOffEn = false;
   RollEn = false;
   PitchEn = false;
   YawEn = false;
   XOff = 0.0;
   YOff = 0.0;
   ZOff = 0.0;
   Roll = 0.0;
   Pitch = 0.0;
   Yaw = 0.0;

}

// ================================================
// ~CigiArtPartCtrlV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiArtPartCtrlV2::~CigiArtPartCtrlV2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiArtPartCtrlV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   CigiBaseArtPartCtrl * Data = ( CigiBaseArtPartCtrl *)Base;

   CDta.d = DBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   CIGI_SCOPY2(CDta.s++, &Data->EntityID);
   *CDta.c++ = Data->ArtPartIDV1;

   Cigi_uint8 HDta = 0;

   if(Data->ArtPartEn)
      HDta = 0x80;

   if(Data->XOffEn)
      HDta |= 0x40;

   if(Data->YOffEn)
      HDta |= 0x20;

   if(Data->ZOffEn)
      HDta |= 0x10;

   if(Data->RollEn)
      HDta |= 0x08;

   if(Data->PitchEn)
      HDta |= 0x04;

   if(Data->YawEn)
      HDta |= 0x02;

   *CDta.c++ = HDta;

   *CDta.s++ = 0;

   CIGI_SCOPY4(CDta.f++, &Data->XOff);
   CIGI_SCOPY4(CDta.f++, &Data->YOff);
   CIGI_SCOPY4(CDta.f++, &Data->ZOff);
   CIGI_SCOPY4(CDta.f++, &Data->Roll);
   CIGI_SCOPY4(CDta.f++, &Data->Pitch);
   CIGI_SCOPY4(CDta.f++, &Data->Yaw);

   memcpy(Buff,tBuf,CIGI_ART_PART_CTRL_PACKET_SIZE_V2);

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiArtPartCtrlV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   union ArtPartConv
   {
      Cigi_uint8 uc;
      Cigi_int8 c;
   } ArtC;

   double DBuf[6];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;

   memcpy(tBuf,Buff,CIGI_ART_PART_CTRL_PACKET_SIZE_V2);

   CDta.d = DBuf;

   CDta.c += 2;  // Step over packet id and size

   CIGI_SCOPY2(&EntityID, CDta.s++);
   ArtPartIDV1 = *CDta.c++;

   Cigi_uint16 HDta = *CDta.c++;

   ArtPartEn = ((HDta & 0x80) != 0);
   XOffEn  = ((HDta & 0x40) != 0);
   YOffEn  = ((HDta & 0x20) != 0);
   ZOffEn  = ((HDta & 0x10) != 0);
   RollEn  = ((HDta & 0x08) != 0);
   PitchEn = ((HDta & 0x04) != 0);
   YawEn   = ((HDta & 0x02) != 0);

   CDta.s++;

   CIGI_SCOPY4(&XOff, CDta.f++);
   CIGI_SCOPY4(&YOff, CDta.f++);
   CIGI_SCOPY4(&ZOff, CDta.f++);
   CIGI_SCOPY4(&Roll, CDta.f++);
   CIGI_SCOPY4(&Pitch, CDta.f++);
   CIGI_SCOPY4(&Yaw, CDta.f++);

   ArtC.c = ArtPartIDV1;
   ArtPartIDV3 = ArtC.uc;

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SetArtPartID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiArtPartCtrlV2::SetArtPartID(const Cigi_int8 ArtPartIDIn, bool bndchk)
{
   union ArtPartConv
   {
      Cigi_uint8 uc;
      Cigi_int8 c;
   } ArtC;

   ArtPartIDV1 = ArtPartIDIn;

   ArtC.c = ArtPartIDV1;
   ArtPartIDV3 = ArtC.uc;

   return(CIGI_SUCCESS);
}



