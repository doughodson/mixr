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
 *  FILENAME:   CigiSignalType.h
 *  LANGUAGE:   C++
 *  CLASS:      UNCLASSIFIED
 *  PROJECT:    Common Image Generator Interface (CIGI) Class Library
 *  
 *  PROGRAM DESCRIPTION: 
 *  ...
 *  
 *  MODIFICATION NOTES:
 *  DATE     NAME                                Project version
 *  DESCRIPTION OF CHANGE........................
 *  
 *  2006.10.10 Greg Basler                       0.1
 *  Initial Release.
 *  
 *  2008.04.03 Greg Basler                       2.1.0
 *  Fixed signals and added symbol signals.
 *  
 *  Author: The Boeing Company
 *
 * </pre>
 */

#if !defined(_CIGI_SIGNAL_TYPE_INCLUDED_)
#define _CIGI_SIGNAL_TYPE_INCLUDED_


//=========================================================
//! The enumeration or Table of Signal Types.
//!
namespace CigiSignalType
{
   typedef enum {
      SigNone=0,
      SigIGCtrl,
      SigEntityCtrl,
      SigViewDef,
      SigViewCtrl,
      SigRateCtrl,
      SigSensorCtrl,
      SigTrajectory,
      SigWeatherCtrl,
      SigCollDetSegDef,
      SigLosSegReq,
      SigLosVectReq,
      SigHatHotReq,
      SigEnvCtrl,
      SigSpecEffDef,
      SigArtPartCtrl,
      SigCollDetVolDef,
      SigShortArtPartCtrl,
      SigConfClampEntityCtrl,
      SigMaritimeSurfaceCtrl,
      SigEnvRgnCtrl,
      SigTerrestrialSurfaceCtrl,
      SigMotionTrackCtrl,
      SigEarthModelDef,
      SigPostionReq,
      SigEnvCondReq,
      SigSOF,
      SigCollDetSegResp,
      SigSensorResp,
      SigSensorXResp,
      SigLosResp,
      SigLosXResp,
      SigHatHotResp,
      SigCollDetVolResp,
      SigPositionResp,
      SigWeatherCondResp,
      SigAerosolResp,
      SigMaritimeSurfaceResp,
      SigTerrestrialSurfaceResp,
      SigAnimationStop,
      SigEventNotification,
      SigHatReq,
      SigHotReq,
      SigHatResp,
      SigHotResp,
      SigHatHotXResp,
      SigWaveCtrl,
      SigIGMsg,
      SigAtmosCtrl,
      SigCelestialCtrl,
      SigCompCtrl,
      SigShortCompCtrl,
      SigSymbolSurfaceDef,
      SigSymbolCtrl,
      SigShortSymbolCtrl,
      SigSymbolTextDef,
      SigSymbolCircleDef,
      SigSymbolLineDef,
      SigSymbolClone
   } Type;
}


#endif   // _CIGI_SIGNAL_TYPE_INCLUDED_


