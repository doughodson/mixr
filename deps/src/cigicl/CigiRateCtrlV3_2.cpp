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
 *  FILENAME:   CigiRateCtrlV3_2.cpp
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
 *  03/14/2006 Greg Basler                       CIGI_CR_DR_1
 *  Initial Release.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  05/15/2008 Greg Basler                       Version 2.2.0
 *  Fixed Version Number.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiRateCtrlV3_2.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiRateCtrlV3_2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiRateCtrlV3_2::CigiRateCtrlV3_2()
{

   PacketID = CIGI_RATE_CTRL_PACKET_ID_V3_2;
   PacketSize = CIGI_RATE_CTRL_PACKET_SIZE_V3_2;
   Version = 3;
   MinorVersion = 2;

   EntityID = 0;
   ArtPartIDV1 = 0;
   ArtPartIDV3 = 0;
   ApplyToArtPart = false;
   CoordSys = Local;
   XRate = 0.0;
   YRate = 0.0;
   ZRate = 0.0;
   RollRate = 0.0;
   PitchRate = 0.0;
   YawRate = 0.0;

}

// ================================================
// ~CigiRateCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiRateCtrlV3_2::~CigiRateCtrlV3_2()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiRateCtrlV3_2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseRateCtrl * Data = ( CigiBaseRateCtrl *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->EntityID;
   *CDta.c++ = Data->ArtPartIDV3;

   Cigi_uint8 HDta = 0;

   if(Data->ApplyToArtPart)
      HDta = 0x01;
   
   if(Data->CoordSys == Local)
      HDta |= 0x02;

   *CDta.c++ = HDta;
   *CDta.s++ = 0;

   *CDta.f++ = Data->XRate;
   *CDta.f++ = Data->YRate;
   *CDta.f++ = Data->ZRate;
   *CDta.f++ = Data->RollRate;
   *CDta.f++ = Data->PitchRate;
   *CDta.f++ = Data->YawRate;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiRateCtrlV3_2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   union ArtPartConv
   {
      Cigi_uint8 uc;
      Cigi_int8 c;
   } ArtC;

   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      EntityID = *CDta.s++;
      ArtPartIDV3 = *CDta.c++;

      Cigi_uint16 HDta = *CDta.c++;

      ApplyToArtPart = ((HDta & 0x01) != 0);
      CoordSys = (CoordSysGrp)((HDta >> 1) & 0x01);

      CDta.s++;

      XRate = *CDta.f++;
      YRate = *CDta.f++;
      ZRate = *CDta.f++;
      RollRate = *CDta.f++;
      PitchRate = *CDta.f++;
      YawRate = *CDta.f++;
   }
   else
   {
      CigiSwap2(&EntityID, CDta.s++);
      ArtPartIDV3 = *CDta.c++;

      Cigi_uint16 HDta = *CDta.c++;

      ApplyToArtPart = ((HDta & 0x01) != 0);
      CoordSys = (CoordSysGrp)((HDta >> 1) & 0x01);

      CDta.s++;

      CigiSwap4(&XRate, CDta.f++);
      CigiSwap4(&YRate, CDta.f++);
      CigiSwap4(&ZRate, CDta.f++);
      CigiSwap4(&RollRate, CDta.f++);
      CigiSwap4(&PitchRate, CDta.f++);
      CigiSwap4(&YawRate, CDta.f++);
   }

   if(ApplyToArtPart)
   {

      ArtC.uc = ArtPartIDV3;
      ArtPartIDV1 = ArtC.c;

   }
   else
      ArtPartIDV1 = -1;


   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SetArtPartID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiRateCtrlV3_2::SetArtPartID(const Cigi_uint8 ArtPartIDIn, bool bndchk)
{
   union ArtPartConv
   {
      Cigi_uint8 uc;
      Cigi_int8 c;
   } ArtC;

   ArtPartIDV3 = ArtPartIDIn;

   if(ApplyToArtPart)
   {

      ArtC.uc = ArtPartIDV3;
      ArtPartIDV1 = ArtC.c;

   }
   else
      ArtPartIDV1 = -1;


   return(CIGI_SUCCESS);
}

// ================================================
// SetApplyToArtPart
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiRateCtrlV3_2::SetApplyToArtPart(const bool ApplyToArtPartIn, bool bndchk)
{
   union ArtPartConv
   {
      Cigi_uint8 uc;
      Cigi_int8 c;
   } ArtC;

   ApplyToArtPart = ApplyToArtPartIn;

   if(ApplyToArtPart)
   {

      ArtC.uc = ArtPartIDV3;
      ArtPartIDV1 = ArtC.c;

   }
   else
      ArtPartIDV1 = -1;

      return(CIGI_SUCCESS);
   }


// ================================================
// SrcCoordSys
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiRateCtrlV3_2::SetCoordSys(const CoordSysGrp CoordSysIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((CoordSysIn < 0)||(CoordSysIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("CoordSys",(CoordSysGrp)CoordSysIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   CoordSys = CoordSysIn;

   return(CIGI_SUCCESS);

}




