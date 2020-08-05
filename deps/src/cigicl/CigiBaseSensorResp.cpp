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
 *  FILENAME:   CigiBaseSensorResp.cpp
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

#include "CigiBaseSensorResp.h"
#include "CigiSwapping.h"
#include "CigiExceptions.h"


// ====================================================================
// Construction/Destruction
// ====================================================================


// ================================================
// CigiBaseSensorResp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSensorResp::CigiBaseSensorResp()
{

}



// ================================================
// ~CigiBaseSensorResp
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiBaseSensorResp::~CigiBaseSensorResp()
{

}


// ====================================================================
// Conversion Control
// ====================================================================


// ================================================
// GetCnvt
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSensorResp::GetCnvt(CigiVersionID &CnvtVersion,
                                CigiCnvtInfoType::Type &CnvtInfo)
{
   CnvtInfo.ProcID = CigiProcessType::ProcStd;

   // Note: CIGI_SENSOR_RESP_PACKET_ID_V1 &
   // CIGI_SENSOR_RESP_PACKET_ID_V2 are the same
   if(CnvtVersion.CigiMajorVersion < 3)
      CnvtInfo.CnvtPacketID = CIGI_SENSOR_RESP_PACKET_ID_V2;
   else
      CnvtInfo.CnvtPacketID = CIGI_SENSOR_RESP_PACKET_ID_V3;

   return(CIGI_SUCCESS);
}


// ====================================================================
// Accessors
// ====================================================================


// ================================================
// SensorStat
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSensorResp::SetSensorStat(const SensorStatGrp SensorStatIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((SensorStatIn < 0)||(SensorStatIn > 3)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("SensorStat",(SensorStatGrp)SensorStatIn,0,3);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   SensorStat = SensorStatIn;
   return(CIGI_SUCCESS);

}


// ================================================
// GateXoff
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSensorResp::SetGateXoff(const float GateXoffIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((GateXoffIn < -180.0)||(GateXoffIn > 180.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("GateXoff",(float)GateXoffIn,-180.0,180.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   GateXoff = GateXoffIn;
   return(CIGI_SUCCESS);

}


// ================================================
// GateYoff
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiBaseSensorResp::SetGateYoff(const float GateYoffIn, bool bndchk)
{

#ifndef CIGI_NO_BND_CHK
   if(bndchk && ((GateYoffIn < -180.0)||(GateYoffIn > 180.0)))
   {
#ifndef CIGI_NO_EXCEPT
      throw CigiValueOutOfRangeException("GateYoff",(float)GateYoffIn,-180.0,180.0);
#endif
      return(CIGI_ERROR_VALUE_OUT_OF_RANGE);
   }
#endif

   GateYoff = GateYoffIn;
   return(CIGI_SUCCESS);

}


