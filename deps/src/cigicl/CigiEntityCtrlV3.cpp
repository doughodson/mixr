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
 *  FILENAME:   CigiEntityCtrlV1.h
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
 *  05/09/2008 Greg Basler                       Version 2.2.0
 *  Added Extrapolation or Interpolation Smoothing flag and
 *  initialize it to false.
 *
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiEntityCtrlV3.h"
#include "CigiExceptions.h"
#include "CigiSwapping.h"
#include "CigiAnimationTable.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiEntityCtrlV3
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiEntityCtrlV3::CigiEntityCtrlV3()
{

   PacketID = CIGI_ENTITY_CTRL_PACKET_ID_V3;
   PacketSize = CIGI_ENTITY_CTRL_PACKET_SIZE_V3;
   Version = 3;
   MinorVersion = 0;

   EntityID = 0;
   EntityType = 0;
   ParentID = 0;

   Alpha = 255;
   Opacity = 100.0f;
   Temperature = 0.0f;

   Roll = 0.0f;
   Pitch = 0.0f;
   Yaw = 0.0f;
   LatOrXoff = 0.0f;
   LonOrYoff = 0.0f;
   AltOrZoff = 0.0f;

   EntityState = Standby;
   AttachState = Detach;
   CollisionDetectEn = Disable;
   InheritAlpha = NoInherit;
   GrndClamp = NoClamp;
   AnimationDir = Forward;
   AnimationLoopMode = OneShot;
   AnimationState = Stop;
   PastAnimationState = Stop;
   SmoothingEn = false;

}

// ================================================
// ~CigiEntityCtrlV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiEntityCtrlV3::~CigiEntityCtrlV3()
{

}


// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV3::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   PackPointer CDta;

   CigiAnimationTable *ATbl = (CigiAnimationTable *) Spec;

   CigiBaseEntityCtrl *Data = ( CigiBaseEntityCtrl *) Base;


   CDta.c = Buff;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;
   *CDta.s++ = Data->EntityID;
   
   Cigi_uint8 HDta = (Cigi_uint8)((Data->EntityState & 0x03) |
                                        ((Data->AttachState << 2) & 0x04) |
                                        ((Data->CollisionDetectEn << 3) & 0x08) |
                                        ((Data->InheritAlpha << 4) & 0x10) |
                                        ((Data->GrndClamp << 5) & 0x60));

   *CDta.c++ = HDta;

   AnimationStateGrp tAState = Data->AnimationState;
   if((tAState >= 0)&&(tAState <= 9))
      tAState = ToV3[tAState];
   else
      tAState = NoAction;

   if(tAState == NoAction)
   {
      unsigned int PastAState = ATbl->GetPastAnimation(Data->EntityID);
      if((PastAState == Stop) ||
         (PastAState == Pause))
         tAState = (AnimationStateGrp)PastAState;
      else
         tAState = Continue;
   }

   ATbl->SetPastAnimation(Data->EntityID,tAState);

   HDta = (Cigi_uint8)((Data->AnimationDir & 0x01) |
                          ((Data->AnimationLoopMode << 1) & 0x02) |
                          ((tAState << 2) & 0x0C));

   *CDta.c++ = HDta;

   *CDta.c++ = Data->Alpha;
   *CDta.c++ = 0;
   *CDta.s++ = Data->EntityType;
   *CDta.s++ = Data->ParentID;
   *CDta.f++ = Data->Roll;
   *CDta.f++ = Data->Pitch;
   *CDta.f++ = Data->Yaw;
   *CDta.d++ = Data->LatOrXoff;
   *CDta.d++ = Data->LonOrYoff;
   *CDta.d++ = Data->AltOrZoff;

   return(PacketSize);

}


// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV3::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{

   PackPointer CDta;

   CigiAnimationTable *ATbl = (CigiAnimationTable *) Spec;

   Cigi_uint8 HDta1, HDta2;

   CDta.c = Buff;

   CDta.c += 2;

   if(!Swap)
   {
      EntityID = *CDta.s++;

      HDta1 = *CDta.c++;
      HDta2 = *CDta.c++;

      Alpha = *CDta.c++;
      Opacity = ((float)Alpha)/2.55f;
      CDta.c++;
      EntityType = *CDta.s++;
      ParentID = *CDta.s++;
      Roll = *CDta.f++;
      Pitch = *CDta.f++;
      Yaw = *CDta.f++;
      LatOrXoff = *CDta.d++;
      LonOrYoff = *CDta.d++;
      AltOrZoff = *CDta.d++;
   }
   else
   {
      CigiSwap2(&EntityID, CDta.s++);

      HDta1 = *CDta.c++;
      HDta2 = *CDta.c++;

      Alpha = *CDta.c++;
      Opacity = ((float)Alpha)/2.55f;
      CDta.c++;
      CigiSwap2(&EntityType, CDta.s++);
      CigiSwap2(&ParentID, CDta.s++);
      CigiSwap4(&Roll, CDta.f++);
      CigiSwap4(&Pitch, CDta.f++);
      CigiSwap4(&Yaw, CDta.f++);
      CigiSwap8(&LatOrXoff, CDta.d++);
      CigiSwap8(&LonOrYoff, CDta.d++);
      CigiSwap8(&AltOrZoff, CDta.d++);
   }


   EntityState = (EntityStateGrp)(HDta1 & 0x03);
   AttachState = (AttachStateGrp)((HDta1 >> 2) & 0x01);
   CollisionDetectEn = (CollisionDetectGrp)((HDta1 >> 3) & 0x01);
   InheritAlpha = (InheritAlphaGrp)((HDta1 >> 4) & 0x01);
   GrndClamp = (GrndClampGrp)((HDta1 >> 5) & 0x03);

   AnimationDir = (AnimationDirGrp)(HDta2 & 0x01);
   AnimationLoopMode = (AnimationLoopModeGrp)((HDta2 >> 1) & 0x01);
   AnimationState = (AnimationStateGrp)((HDta2 >> 2) & 0x03);

   PastAnimationState = (AnimationStateGrp)ATbl->GetPastAnimation(EntityID);

   ATbl->SetPastAnimation(EntityID,AnimationState);

   return(CIGI_SUCCESS);

}


// ====================================================================
// Accessors
// ====================================================================



// ================================================
// SetYaw
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV3::SetYaw(const float YawIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((YawIn < 0.0f)||(YawIn > 360.0f)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Yaw",(double)YawIn,0.0,360.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Yaw = YawIn;
   return(CIGI_SUCCESS);

}

// ================================================
// SetEntityState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV3::SetEntityState(const EntityStateGrp EntityStateIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((EntityStateIn < 0)||(EntityStateIn > 2)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("EntityState",EntityStateIn,0,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   EntityState = EntityStateIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetGrndClamp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV3::SetGrndClamp(const GrndClampGrp GrndClampIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((GrndClampIn < 0)||(GrndClampIn > 2)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("GrndClamp",GrndClampIn,0,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   GrndClamp = GrndClampIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetInheritAlpha
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV3::SetInheritAlpha(const InheritAlphaGrp InheritAlphaIn, bool bndchk)
{
   
#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((InheritAlphaIn < 0)||(InheritAlphaIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("InheritAlpha",(int)InheritAlphaIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   InheritAlpha = InheritAlphaIn;
   return(CIGI_SUCCESS);

}

// ================================================
// SetAnimDir
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV3::SetAnimationDir(const AnimationDirGrp AnimationDirIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((AnimationDirIn < 0)||(AnimationDirIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("AnimationDir",AnimationDirIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   AnimationDir = AnimationDirIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetAnimLoopMode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV3::SetAnimationLoopMode(const AnimationLoopModeGrp AnimationLoopModeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((AnimationLoopModeIn < 0)||(AnimationLoopModeIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("AnimationLoopMode",AnimationLoopModeIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   AnimationLoopMode = AnimationLoopModeIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetAnimationState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV3::SetAnimationState(const AnimationStateGrp AnimationStateIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((AnimationStateIn < 0)||(AnimationStateIn > 3)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("AnimationState",AnimationStateIn,0,3);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   AnimationState = AnimationStateIn;

   return(CIGI_SUCCESS);

}

// ================================================
// GetAnimationState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseEntityCtrl::AnimationStateGrp CigiEntityCtrlV3::GetAnimationState() const
{


   AnimationStateGrp tAState = AnimationState;
   if((tAState >= 0)&&(tAState <= 9))
      tAState = ToV3[tAState];
   else
      tAState = NoAction;

   if(tAState == NoAction)
   {
      if((PastAnimationState == Stop) ||
         (PastAnimationState == Pause))
         tAState = (AnimationStateGrp)PastAnimationState;
      else
         tAState = Continue;
   }

   return(tAState);

}

