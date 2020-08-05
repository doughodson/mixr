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
 *  FILENAME:   CigiBaseEntityCtrl.cpp
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
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  Moved Packet information to base packet.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#define _EXPORT_CCL_

#include "CigiBaseEntityCtrl.h"
#include "CigiExceptions.h"

// Animation State conversion Tables
const CigiBaseEntityCtrl::AnimationStateGrp CigiBaseEntityCtrl::ToV1[10] =
{
   DeactivateUnload, // Stop
   Deactivate,       // Pause
   LoadActivate,     // Play
   Continue,         // Continue
   NoAction,         // NoAction
   Load,             // Load
   LoadActivate,     // LoadActivate
   Activate,         // Activate
   Deactivate,       // Deactivate
   DeactivateUnload  // DeactivateUnload
};

const CigiBaseEntityCtrl::AnimationStateGrp CigiBaseEntityCtrl::ToV3[10] =
{
   Stop,      // Stop
   Pause,     // Pause
   Play,      // Play
   Continue,  // Continue
   NoAction,  // NoAction
   Stop,      // Load
   Play,      // LoadActivate
   Play,      // Activate
   Pause,     // Deactivate
   Stop       // DeactivateUnload
};

// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseEntityCtrl::CigiBaseEntityCtrl()
{

}

// ================================================
// ~CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseEntityCtrl::~CigiBaseEntityCtrl()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEntityCtrl::GetCnvt(CigiVersionID &CnvtVersion,
                                CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::ProcEntity;

   // All versions of this packet have the same packet id number
   CnvtInfo.CnvtPacketID = CIGI_ENTITY_CTRL_PACKET_ID_V3;

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEntityCtrl::SetPitch(const float PitchIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((PitchIn < -90.0f)||(PitchIn > 90.0f)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Pitch",(double)PitchIn,-90.0,90.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Pitch = PitchIn;

   return(CIGI_SUCCESS);

}

// ================================================
// CigiBaseEntityCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEntityCtrl::SetRoll(const float RollIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((RollIn < -180.0f)||(RollIn > 180.0f)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Roll",(double)RollIn,-180.0,180.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Roll = RollIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetLat
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEntityCtrl::SetLat(const double Lat, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((Lat < -90.0f)||(Lat > 90.0f)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Lat",Lat,-90.0,90.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   LatOrXoff = Lat;

   return(CIGI_SUCCESS);

}

// ================================================
// SetLon
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEntityCtrl::SetLon(const double Lon, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((Lon < -180.0f)||(Lon > 180.0f)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Lon",Lon,-180.0,180.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   LonOrYoff = Lon;

   return(CIGI_SUCCESS);

}


// ================================================
// SetAttachState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEntityCtrl::SetAttachState(const AttachStateGrp AttachStateIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((AttachStateIn < 0)||(AttachStateIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("AttachState",AttachStateIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   AttachState = AttachStateIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetCollisionDetectEn
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEntityCtrl::SetCollisionDetectEn(const CollisionDetectGrp CollisionDetectEnIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((CollisionDetectEnIn < 0)||(CollisionDetectEnIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("CollisionDetectEn",CollisionDetectEnIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   CollisionDetectEn = CollisionDetectEnIn;

   return(CIGI_SUCCESS);

}


