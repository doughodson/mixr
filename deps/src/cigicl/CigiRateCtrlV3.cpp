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
 *  FILENAME:   CigiRateCtrlV3.cpp
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
 *  Modified the class constructor to initialize the MinorVersion and 
 *  CoordSys member variables.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the Unpack method to set the CoordSys member variable.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiRateCtrlV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiRateCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiRateCtrlV3::CigiRateCtrlV3()
{

   PacketID = CIGI_RATE_CTRL_PACKET_ID_V3;
   PacketSize = CIGI_RATE_CTRL_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

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
CigiRateCtrlV3::~CigiRateCtrlV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiRateCtrlV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
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
int CigiRateCtrlV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
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

   CoordSys = Local;

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SetArtPartID
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiRateCtrlV3::SetArtPartID(const Cigi_uint8 ArtPartIDIn, bool bndchk)
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
int CigiRateCtrlV3::SetApplyToArtPart(const bool ApplyToArtPartIn, bool bndchk)
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
