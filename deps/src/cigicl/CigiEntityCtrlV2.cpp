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

#include "CigiEntityCtrlV2.h"
#include "CigiExceptions.h"
#include "CigiSwapping.h"


#ifdef CIGI_LITTLE_ENDIAN
   #define CIGI_SCOPY2 CigiSwap2
   #define CIGI_SCOPY4 CigiSwap4
   #define CIGI_SCOPY8 CigiSwap8
#else
   #define CIGI_SCOPY2 CigiCopy2
   #define CIGI_SCOPY4 CigiCopy4
   #define CIGI_SCOPY8 CigiCopy8
#endif

const CigiEntityCtrlV2::EffectStV2Grp CigiEntityCtrlV2::V3toV2[4] =
{
   StopV2,    // Stop
   StopV2,    // Pause
   RestartV2, // Play
   PlayV2,    // Continue
};

const CigiBaseEntityCtrl::AnimationStateGrp CigiEntityCtrlV2::V2toV3[3] =
{
   Stop,      // Stop
   Continue,  // Play
   Play,      // Restart
};


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiEntityCtrlV2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiEntityCtrlV2::CigiEntityCtrlV2()
{

   PacketID = CIGI_ENTITY_CTRL_PACKET_ID_V2;
   PacketSize = CIGI_ENTITY_CTRL_PACKET_SIZE_V2;
   Version = 2;
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
CigiEntityCtrlV2::~CigiEntityCtrlV2()
{

}


// ====================================================================
// Pack and Unpack
// ====================================================================

// ================================================
// Pack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV2::Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const
{
   EffectStV2Grp rslt;

   double DBuf[7];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   PackPointer CDta;


   CigiAnimationTable *ATbl = (CigiAnimationTable *) Spec;

   CigiBaseEntityCtrl * Data = ( CigiBaseEntityCtrl *)Base;

   CDta.c = tBuf;

   *CDta.c++ = PacketID;
   *CDta.c++ = PacketSize;
   CIGI_SCOPY2(CDta.s++, &Data->EntityID);
   
   Cigi_uint8 HDta = (Cigi_uint8)(((Data->EntityState << 6) & 0xC0) |
                                        ((Data->AttachState << 5) & 0x20) |
                                        ((Data->CollisionDetectEn << 4) & 0x10));


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

   rslt = V3toV2[tAState];

   HDta |= (Cigi_uint8)((rslt << 2) & 0x0C);

   *CDta.c++ = HDta;

   *CDta.c++ = 0;
   *CDta.s++ = 0;
   CIGI_SCOPY2(CDta.s++, &Data->EntityType);
   CIGI_SCOPY2(CDta.s++, &Data->ParentID);
   CIGI_SCOPY4(CDta.f++, &Data->Opacity);
   CIGI_SCOPY4(CDta.f++, &Data->Temperature);
   CIGI_SCOPY4(CDta.f++, &Data->Roll);
   CIGI_SCOPY4(CDta.f++, &Data->Pitch);
   CIGI_SCOPY4(CDta.f++, &Data->Yaw);
   CIGI_SCOPY8(CDta.d++, &Data->AltOrZoff);
   CIGI_SCOPY8(CDta.d++, &Data->LatOrXoff);
   CIGI_SCOPY8(CDta.d++, &Data->LonOrYoff);

   memcpy(Buff,tBuf,CIGI_ENTITY_CTRL_PACKET_SIZE_V2);

   return(PacketSize);

}


// ================================================
// Unpack
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV2::Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec)
{

   PackPointer CDta;

   double DBuf[7];

   Cigi_uint8 *tBuf = (Cigi_uint8 *)DBuf;

   CigiAnimationTable *ATbl = (CigiAnimationTable *) Spec;

   memcpy(tBuf,Buff,CIGI_ENTITY_CTRL_PACKET_SIZE_V2);

   CDta.c = tBuf;

   CDta.c += 2;
   CIGI_SCOPY2(&EntityID, CDta.s++);

   Cigi_uint8 HDta = *CDta.c;
   AttachState = (AttachStateGrp)((HDta >> 5) &0x01);
   CollisionDetectEn = (CollisionDetectGrp)((HDta >> 4) & 0x01);
   EntityState = (EntityStateGrp)((HDta >> 6) & 0x03);
   EffectStV2Grp EffectStateIn = (EffectStV2Grp)((HDta >> 2) & 0x03);

   if((EffectStateIn >= 0)&&(EffectStateIn <= 2))
      AnimationState = V2toV3[EffectStateIn];
   else
      AnimationState = Play;

   PastAnimationState = (AnimationStateGrp)ATbl->GetPastAnimation(EntityID);

   ATbl->SetPastAnimation(EntityID,AnimationState);

   CDta.c += 4;

   CIGI_SCOPY2(&EntityType, CDta.s++);
   CIGI_SCOPY2(&ParentID, CDta.s++);
   CIGI_SCOPY4(&Opacity, CDta.f++);
   Alpha = (unsigned int)(Opacity * 2.55f);
   CIGI_SCOPY4(&Temperature, CDta.f++);
   CIGI_SCOPY4(&Roll, CDta.f++);
   CIGI_SCOPY4(&Pitch, CDta.f++);
   CIGI_SCOPY4(&Yaw, CDta.f++);
   CIGI_SCOPY8(&AltOrZoff, CDta.d++);
   CIGI_SCOPY8(&LatOrXoff, CDta.d++);
   CIGI_SCOPY8(&LonOrYoff, CDta.d++);

   return(CIGI_SUCCESS);

}


// ====================================================================
// Accessors
// ====================================================================



// ================================================
// SetOpacity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV2::SetOpacity(const float OpacityIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((OpacityIn < 0.0f)||(OpacityIn > 100.0f)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Opacity",(double)OpacityIn,0.0,100.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Opacity = OpacityIn;
   Alpha = (unsigned int)(OpacityIn * 2.55f);
   return(CIGI_SUCCESS);

}

// ================================================
// SetHeading
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV2::SetHeading(const float Heading, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((Heading < 0.0f)||(Heading > 360.0f)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Heading",(double)Heading,0.0,360.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Yaw = Heading;
   return(CIGI_SUCCESS);

}


// ================================================
// SetEntityState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV2::SetEntityState(const EntityStV2Grp EntityStateIn, bool bndchk)
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

   EntityState = (EntityStateGrp)EntityStateIn;

   return(CIGI_SUCCESS);

}

// ================================================
// SetEffectState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiEntityCtrlV2::SetEffectState(const EffectStV2Grp EffectStateIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((EffectStateIn < 0)||(EffectStateIn > 2)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("EffectState",EffectStateIn,0,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   if((EffectStateIn >= 0)&&(EffectStateIn <= 2))
      AnimationState = V2toV3[EffectStateIn];
   else
      AnimationState = Play;

   return(CIGI_SUCCESS);

}

// ================================================
// GetEffectState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiEntityCtrlV2::EffectStV2Grp CigiEntityCtrlV2::GetEffectState() const
{
   EffectStV2Grp rslt;


   AnimationStateGrp tAState = AnimationState;
   if((tAState >= 0)&&(tAState <= 9))
      tAState = ToV3[tAState];
   else
      tAState = NoAction;

   if(tAState == NoAction)
   {
      unsigned int PastAState = PastAnimationState;
      if((PastAState == Stop) ||
         (PastAState == Pause))
         tAState = (AnimationStateGrp)PastAState;
      else
         tAState = Continue;
   }

   rslt = V3toV2[tAState];

   return(rslt);

}
