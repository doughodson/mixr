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
 *  FILENAME:   CigiMotionTrackCtrlV3.cpp
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

#include "CigiMotionTrackCtrlV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiMotionTrackCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiMotionTrackCtrlV3::CigiMotionTrackCtrlV3()
{

   PacketID = CIGI_MOTION_TRACK_CTRL_PACKET_ID_V3;
   PacketSize = CIGI_MOTION_TRACK_CTRL_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   ViewID = 0;
   TrackerID = 0;
   TrackerEn = false;
   BoresightEn = false;
   XEn = false;
   YEn = false;
   ZEn = false;
   RollEn = false;
   PitchEn = false;
   YawEn = false;
   Scope = View;

}

// ================================================
// ~CigiMotionTrackCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiMotionTrackCtrlV3::~CigiMotionTrackCtrlV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiMotionTrackCtrlV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseMotionTrackCtrl * Data = ( CigiBaseMotionTrackCtrl *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->ViewID;
   *CDta.c++ = Data->TrackerID;

   Cigi_uint8 HDta = (Data->TrackerEn) ? 0x01 : 0;
   HDta |= (Data->BoresightEn) ? 0x02 : 0;
   HDta |= (Data->XEn) ? 0x04 : 0;
   HDta |= (Data->YEn) ? 0x08 : 0;
   HDta |= (Data->ZEn) ? 0x10 : 0;
   HDta |= (Data->RollEn) ? 0x20 : 0;
   HDta |= (Data->PitchEn) ? 0x40 : 0;
   HDta |= (Data->YawEn) ? 0x80 : 0;
   *CDta.c++ = HDta;

   HDta = (Data->Scope) ? 0x01 : 0;
   *CDta.c++ = HDta;

   *CDta.c++ = 0;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiMotionTrackCtrlV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
      ViewID = *CDta.s++;
   else
      CigiSwap2(&ViewID, CDta.s++);

   TrackerID = *CDta.c++;

   Cigi_uint8 HDta = *CDta.c++;
   TrackerEn = ((HDta & 0x01) != 0);
   BoresightEn = ((HDta & 0x02) != 0);
   XEn = ((HDta & 0x04) != 0);
   YEn = ((HDta & 0x08) != 0);
   ZEn = ((HDta & 0x10) != 0);
   RollEn = ((HDta & 0x20) != 0);
   PitchEn = ((HDta & 0x40) != 0);
   YawEn = ((HDta & 0x80) != 0);

   HDta = *CDta.c++;
   Scope = (ScopeGrp)(HDta & 0x01);

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


