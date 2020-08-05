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
 *  FILENAME:   CigiBaseShortArtPartCtrl.cpp
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
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiBaseShortArtPartCtrl.h"
#include "CigiArtPartCtrlV3.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiBaseShortArtPartCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseShortArtPartCtrl::CigiBaseShortArtPartCtrl()
{

}



// ================================================
// ~CigiBaseShortArtPartCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseShortArtPartCtrl::~CigiBaseShortArtPartCtrl()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseShortArtPartCtrl::GetCnvt(CigiVersionID &CnvtVersion,
                                      CigiCnvtInfoType::Type &CnvtInfo)
{
   if(CnvtVersion.CigiMajorVersion < 3)
   {
      // V1 & V2 of the Art Part packet
      //  uses the same packet id number
      CnvtInfo.ProcID = CigiProcessType::ProcShortArtPartToArtPart;
      CnvtInfo.CnvtPacketID = CIGI_ART_PART_CTRL_PACKET_ID_V2;
   }
   else
   {
      CnvtInfo.ProcID = CigiProcessType::ProcStd;
      CnvtInfo.CnvtPacketID = CIGI_SHORT_ART_PART_CTRL_PACKET_ID_V3;
   }

   return(CIGI_SUCCESS);
}


// ================================================
// SpecialConversion
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseShortArtPartCtrl::SpecialConversion(CigiVersionID &CnvtVersion,
   Cigi_uint8 ArtPartID, CigiArtPartCtrlV3 *ArtPart)
{
   if(ArtPart == NULL)
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiNullPointerException();
#endif
      return(CIGI_ERROR_UNEXPECTED_NULL);
   }

   ArtPart->SetArtPartID(ArtPartID);
   ArtPart->SetEntityID(EntityID);

   if((ArtPart1 == ArtPartID) && ArtPart1En)
   {
      ArtPart->SetArtPartEn(true);
      switch(DofSelect1)
      {
      case Xoff:
         ArtPart->SetXOff(Dof1);
         ArtPart->SetXOffEn(true);
         break;
      case Yoff:
         ArtPart->SetYOff(Dof1);
         ArtPart->SetYOffEn(true);
         break;
      case Zoff:
         ArtPart->SetZOff(Dof1);
         ArtPart->SetZOffEn(true);
         break;
      case Yaw:
         ArtPart->SetYaw(Dof1);
         ArtPart->SetYawEn(true);
         break;
      case Pitch:
         ArtPart->SetPitch(Dof1);
         ArtPart->SetPitchEn(true);
         break;
      case Roll:
         ArtPart->SetRoll(Dof1);
         ArtPart->SetRollEn(true);
         break;
      default:
         break;
      }
   }

   if((ArtPart2 == ArtPartID) && ArtPart2En)
   {
      ArtPart->SetArtPartEn(true);
      switch(DofSelect2)
      {
      case Xoff:
         ArtPart->SetXOff(Dof2);
         ArtPart->SetXOffEn(true);
         break;
      case Yoff:
         ArtPart->SetYOff(Dof2);
         ArtPart->SetYOffEn(true);
         break;
      case Zoff:
         ArtPart->SetZOff(Dof2);
         ArtPart->SetZOffEn(true);
         break;
      case Yaw:
         ArtPart->SetYaw(Dof2);
         ArtPart->SetYawEn(true);
         break;
      case Pitch:
         ArtPart->SetPitch(Dof2);
         ArtPart->SetPitchEn(true);
         break;
      case Roll:
         ArtPart->SetRoll(Dof2);
         ArtPart->SetRollEn(true);
         break;
      default:
         break;
      }
   }

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// DofSelect1
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseShortArtPartCtrl::SetDofSelect1(const DofSelectGrp DofSelect1In, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((DofSelect1In < 0)||(DofSelect1In > 6)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DofSelect1",(DofSelectGrp)DofSelect1In,0,6);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   DofSelect1 = DofSelect1In;
   return(CIGI_SUCCESS);

}


// ================================================
// DofSelect2
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseShortArtPartCtrl::SetDofSelect2(const DofSelectGrp DofSelect2In, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((DofSelect2In < 0)||(DofSelect2In > 6)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("DofSelect2",(DofSelectGrp)DofSelect2In,0,6);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   DofSelect2 = DofSelect2In;
   return(CIGI_SUCCESS);

}


