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
 *  FILENAME:   CigiBaseEnvRgnCtrl.cpp
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

#include "CigiBaseEnvRgnCtrl.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiBaseEnvRgnCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseEnvRgnCtrl::CigiBaseEnvRgnCtrl()
{

}



// ================================================
// ~CigiBaseEnvRgnCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseEnvRgnCtrl::~CigiBaseEnvRgnCtrl()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEnvRgnCtrl::GetCnvt(CigiVersionID &CnvtVersion,
                                CigiCnvtInfoType::Type &CnvtInfo)
{
   if(CnvtVersion.CigiMajorVersion < 3)
   {
      CnvtInfo.ProcID = CigiProcessType::ProcNone;
      CnvtInfo.CnvtPacketID = 0;
   }
   else
   {
      CnvtInfo.ProcID = CigiProcessType::ProcStd;
      CnvtInfo.CnvtPacketID = CIGI_ENV_RGN_CTRL_PACKET_ID_V3;
   }

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// RgnState
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEnvRgnCtrl::SetRgnState(const RgnStateGrp RgnStateIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((RgnStateIn < 0)||(RgnStateIn > 2)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("RgnState",(RgnStateGrp)RgnStateIn,0,2);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   RgnState = RgnStateIn;
   return(CIGI_SUCCESS);

}


// ================================================
// WeatherProp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEnvRgnCtrl::SetWeatherProp(const MergeCtrlGrp WeatherPropIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((WeatherPropIn < 0)||(WeatherPropIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("WeatherProp",(MergeCtrlGrp)WeatherPropIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   WeatherProp = WeatherPropIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Aerosol
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEnvRgnCtrl::SetAerosol(const MergeCtrlGrp AerosolIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((AerosolIn < 0)||(AerosolIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Aerosol",(MergeCtrlGrp)AerosolIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Aerosol = AerosolIn;
   return(CIGI_SUCCESS);

}


// ================================================
// MaritimeSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEnvRgnCtrl::SetMaritimeSurface(const MergeCtrlGrp MaritimeSurfaceIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((MaritimeSurfaceIn < 0)||(MaritimeSurfaceIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("MaritimeSurface",(MergeCtrlGrp)MaritimeSurfaceIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   MaritimeSurface = MaritimeSurfaceIn;
   return(CIGI_SUCCESS);

}


// ================================================
// TerrestrialSurface
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEnvRgnCtrl::SetTerrestrialSurface(const MergeCtrlGrp TerrestrialSurfaceIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((TerrestrialSurfaceIn < 0)||(TerrestrialSurfaceIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("TerrestrialSurface",(MergeCtrlGrp)TerrestrialSurfaceIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   TerrestrialSurface = TerrestrialSurfaceIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Lat
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEnvRgnCtrl::SetLat(const double LatIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((LatIn < -90.0)||(LatIn > 90.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Lat",(double)LatIn,-90.0,90.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Lat = LatIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Lon
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEnvRgnCtrl::SetLon(const double LonIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((LonIn < -180.0)||(LonIn > 180.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Lon",(double)LonIn,-180.0,180.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Lon = LonIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Rotation
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseEnvRgnCtrl::SetRotation(const float RotationIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((RotationIn < -180.0)||(RotationIn > 180.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Rotation",(float)RotationIn,-180.0,180.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Rotation = RotationIn;
   return(CIGI_SUCCESS);

}


