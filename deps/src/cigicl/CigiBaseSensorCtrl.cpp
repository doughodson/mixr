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
 *  FILENAME:   CigiBaseSensorCtrl.cpp
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

#include "CigiBaseSensorCtrl.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiBaseSensorCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSensorCtrl::CigiBaseSensorCtrl()
{

}



// ================================================
// ~CigiBaseSensorCtrl
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSensorCtrl::~CigiBaseSensorCtrl()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSensorCtrl::GetCnvt(CigiVersionID &CnvtVersion,
                                CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::ProcStd;

   if(CnvtVersion.CigiMajorVersion < 3)
      CnvtInfo.CnvtPacketID = CIGI_SENSOR_CTRL_PACKET_ID_V2;
   else
      CnvtInfo.CnvtPacketID = CIGI_SENSOR_CTRL_PACKET_ID_V3;

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// Polarity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSensorCtrl::SetPolarity(const PolarityGrp PolarityIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((PolarityIn < 0)||(PolarityIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Polarity",(PolarityGrp)PolarityIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Polarity = PolarityIn;
   return(CIGI_SUCCESS);

}


// ================================================
// TrackMode
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSensorCtrl::SetTrackMode(const TrackModeGrp TrackModeIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((TrackModeIn < 0)||(TrackModeIn > 7)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("TrackMode",(TrackModeGrp)TrackModeIn,0,7);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   TrackMode = TrackModeIn;
   return(CIGI_SUCCESS);

}


// ================================================
// TrackPolarity
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSensorCtrl::SetTrackPolarity(const TrackPolarityGrp TrackPolarityIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((TrackPolarityIn < 0)||(TrackPolarityIn > 1)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("TrackPolarity",(TrackPolarityGrp)TrackPolarityIn,0,1);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   TrackPolarity = TrackPolarityIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Level
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSensorCtrl::SetLevel(const float LevelIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((LevelIn < 0.0)||(LevelIn > 1.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Level",(float)LevelIn,0.0,1.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Level = LevelIn;
   return(CIGI_SUCCESS);

}


// ================================================
// ACCoupling
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSensorCtrl::SetACCoupling(const float ACCouplingIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && (ACCouplingIn < 0.0))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("ACCoupling",(float)ACCouplingIn,">=",0.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   ACCoupling = ACCouplingIn;
   return(CIGI_SUCCESS);

}


// ================================================
// Noise
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSensorCtrl::SetNoise(const float NoiseIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((NoiseIn < 0.0)||(NoiseIn > 1.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("Noise",(float)NoiseIn,0.0,1.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   Noise = NoiseIn;
   return(CIGI_SUCCESS);

}


