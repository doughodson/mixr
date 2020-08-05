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
 *  FILENAME:   CigiTrajectoryDefV3.cpp
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

#include <math.h>

#define _EXPORT_CCL_

#include "CigiTrajectoryDefV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiTrajectoryDefV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiTrajectoryDefV3::CigiTrajectoryDefV3()
{

   PacketID = CIGI_TRAJECTORY_DEF_PACKET_ID_V3;
   PacketSize = CIGI_TRAJECTORY_DEF_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   EntityID = 0;
   Accel = 0.0;
   RetardationRate = 0.0;
   TermVel = 0.0;
   Xoff = 0.0;
   Yoff = 0.0;
   Zoff = 0.0;
   AccelX = 0.0f;
   AccelY = 0.0f;
   AccelZ = 0.0f;

}

// ================================================
// ~CigiTrajectoryDefV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiTrajectoryDefV3::~CigiTrajectoryDefV3()
{

}

// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiTrajectoryDefV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiBaseTrajectoryDef * Data = ( CigiBaseTrajectoryDef *)Base;

   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;

   *CDta.s++ = Data->EntityID;
   *CDta.f++ = Data->AccelX;
   *CDta.f++ = Data->AccelY;
   *CDta.f++ = Data->AccelZ;
   *CDta.f++ = Data->RetardationRate;
   *CDta.f++ = Data->TermVel;

   return(PacketSize);

}

// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiTrajectoryDefV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{
   PackPointer CDta;

   CDta.c = Buff;

   CDta.c += 2;  // Step over packet id and size

   if(!Swap)
   {
      EntityID = *CDta.s++;
      AccelX = *CDta.f++;
      AccelY = *CDta.f++;
      AccelZ = *CDta.f++;
      RetardationRate = *CDta.f++;
      TermVel = *CDta.f++;
   }
   else
   {
      CigiSwap2(&EntityID, CDta.s++);
      CigiSwap4(&AccelX, CDta.f++);
      CigiSwap4(&AccelY, CDta.f++);
      CigiSwap4(&AccelZ, CDta.f++);
      CigiSwap4(&RetardationRate, CDta.f++);
      CigiSwap4(&TermVel, CDta.f++);
   }

   Accel = (float)sqrt((double)((AccelX*AccelX) + (AccelY*AccelY) + (AccelZ*AccelZ)));

   Xoff = 0.0;
   Yoff = 0.0;
   Zoff = 0.0;

   return(PacketSize);

}



// ====================================================================
// Accessors
// ====================================================================


// ================================================
// AccelX
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiTrajectoryDefV3::SetAccelX(const float AccelIn, bool bndchk)
{
   AccelX = AccelIn;
   Accel = (float)sqrt((double)((AccelX*AccelX) + (AccelY*AccelY) + (AccelZ*AccelZ)));

   return(CIGI_SUCCESS);
}


// ================================================
// AccelY
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiTrajectoryDefV3::SetAccelY(const float AccelIn, bool bndchk)
{
   AccelY = AccelIn;
   Accel = (float)sqrt((double)((AccelX*AccelX) + (AccelY*AccelY) + (AccelZ*AccelZ)));

   return(CIGI_SUCCESS);
}


// ================================================
// AccelZ
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiTrajectoryDefV3::SetAccelZ(const float AccelIn, bool bndchk)
{
   AccelZ = AccelIn;
   Accel = (float)sqrt((double)((AccelX*AccelX) + (AccelY*AccelY) + (AccelZ*AccelZ)));

   return(CIGI_SUCCESS);
}
