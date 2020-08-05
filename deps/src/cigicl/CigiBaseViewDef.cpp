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
 *  FILENAME:   CigiBaseViewDef.cpp
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
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiBaseViewDef.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiBaseViewDef
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseViewDef::CigiBaseViewDef()
{

}



// ================================================
// ~CigiBaseViewDef
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseViewDef::~CigiBaseViewDef()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseViewDef::GetCnvt(CigiVersionID &CnvtVersion,
                             CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::ProcStd;

   // V1 & V2 have the same packet id number
   if(CnvtVersion.CigiMajorVersion < 3)
      CnvtInfo.CnvtPacketID = CIGI_VIEW_DEF_PACKET_ID_V2;
   else
      CnvtInfo.CnvtPacketID = CIGI_VIEW_DEF_PACKET_ID_V3;

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// MirrorMode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseViewDef::SetMirrorMode(const MirrorModeGrp MirrorModeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((MirrorModeIn < 0)||(MirrorModeIn > 3)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("MirrorMode",(MirrorModeGrp)MirrorModeIn,0,3);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   MirrorMode = MirrorModeIn;
   return(CIGI_SUCCESS);

}


// ================================================
// PixelReplicateMode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseViewDef::SetPixelReplicateMode(const PixelReplicateModeGrp PixelReplicateModeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((PixelReplicateModeIn < 0)||(PixelReplicateModeIn > 7)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("PixelReplicateMode",(PixelReplicateModeGrp)PixelReplicateModeIn,0,7);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   PixelReplicateMode = PixelReplicateModeIn;
   return(CIGI_SUCCESS);

}


// ================================================
// ViewType
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseViewDef::SetViewType(const Cigi_uint8 ViewTypeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((ViewTypeIn < 0)||(ViewTypeIn > 7)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ViewType",(Cigi_uint8)ViewTypeIn,0,7);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ViewType = ViewTypeIn;
   return(CIGI_SUCCESS);

}


