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
 *  FILENAME:   CigiEnvRgnCtrlV3.cpp
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
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiEnvRgnCtrlV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiEnvRgnCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiEnvRgnCtrlV3::CigiEnvRgnCtrlV3()
{

   PacketID = CIGI_ENV_RGN_CTRL_PACKET_ID_V3;
   PacketSize = CIGI_ENV_RGN_CTRL_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   RegionID = 0;
   RgnState = Inactive;
   WeatherProp = UseLast;
   Aerosol = UseLast;
   MaritimeSurface = UseLast;
   TerrestrialSurface = UseLast;
   Lat = 0.0;
   Lon = 0.0;
   XSize = 0.0;
   YSize = 0.0;
   CornerRadius = 0.0;
   Rotation = 0.0;
   Transition = 0.0;

}

// ================================================
// ~CigiEnvRgnCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiEnvRgnCtrlV3::~CigiEnvRgnCtrlV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvRgnCtrlV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseEnvRgnCtrl * Data = ( CigiBaseEnvRgnCtrl *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->RegionID;

   Cigi_uint8 MDta = Data->RgnState & 0x03;
   MDta |= (Data->WeatherProp << 2) & 0x04;
   MDta |= (Data->Aerosol << 3) & 0x08;
   MDta |= (Data->MaritimeSurface << 4) & 0x10;
   MDta |= (Data->TerrestrialSurface << 5) & 0x20;
   *CDta.c++ = MDta;

   *CDta.c++ = 0;
   *CDta.s++ = 0;

   *CDta.d++ = Data->Lat;
   *CDta.d++ = Data->Lon;
   *CDta.f++ = Data->XSize;
   *CDta.f++ = Data->YSize;
   *CDta.f++ = Data->CornerRadius;
   *CDta.f++ = Data->Rotation;
   *CDta.f++ = Data->Transition;
   *CDta.l++ = 0;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEnvRgnCtrlV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      RegionID = *CDta.s++;

      Cigi_uint8 HDta = *CDta.c++;
      RgnState = (RgnStateGrp)(HDta & 0x03);
      WeatherProp = (MergeCtrlGrp)((HDta >> 2) & 0x01);
      Aerosol = (MergeCtrlGrp)((HDta >> 3) & 0x01);
      MaritimeSurface = (MergeCtrlGrp)((HDta >> 4) & 0x01);
      TerrestrialSurface = (MergeCtrlGrp)((HDta >> 5) & 0x01);

      CDta.c += 3;

      Lat = *CDta.d++;
      Lon = *CDta.d++;
      XSize = *CDta.f++;
      YSize = *CDta.f++;
      CornerRadius = *CDta.f++;
      Rotation = *CDta.f++;
      Transition = *CDta.f++;
   }
   else
   {
      CigiSwap2(&RegionID, CDta.s++);

      Cigi_uint8 HDta = *CDta.c++;
      RgnState = (RgnStateGrp)(HDta & 0x03);
      WeatherProp = (MergeCtrlGrp)((HDta >> 2) & 0x01);
      Aerosol = (MergeCtrlGrp)((HDta >> 3) & 0x01);
      MaritimeSurface = (MergeCtrlGrp)((HDta >> 4) & 0x01);
      TerrestrialSurface = (MergeCtrlGrp)((HDta >> 5) & 0x01);

      CDta.c += 3;

      CigiSwap8(&Lat, CDta.d++);
      CigiSwap8(&Lon, CDta.d++);
      CigiSwap4(&XSize, CDta.f++);
      CigiSwap4(&YSize, CDta.f++);
      CigiSwap4(&CornerRadius, CDta.f++);
      CigiSwap4(&Rotation, CDta.f++);
      CigiSwap4(&Transition, CDta.f++);
   }

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


