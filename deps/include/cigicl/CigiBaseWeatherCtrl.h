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
 *  FILENAME:   CigiBaseWeatherCtrl.h
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
 *  Added the CIGI_SPEC modifier to the class declaration for exporting the 
 *  class in a Windows DLL.
 *  
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed native C++ types in function declarations to CCL-defined types.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  Moved Packet information to base packet.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_BASE_WEATHER_CTRL_INCLUDED_)
#define _CIGI_BASE_WEATHER_CTRL_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_WEATHER_CTRL_PACKET_ID_V1 7
#define CIGI_WEATHER_CTRL_PACKET_SIZE_V1 44

#define CIGI_WEATHER_CTRL_PACKET_ID_V2 7
#define CIGI_WEATHER_CTRL_PACKET_SIZE_V2 44

#define CIGI_WEATHER_CTRL_PACKET_ID_V3 12
#define CIGI_WEATHER_CTRL_PACKET_SIZE_V3 56


class CigiWeatherCtrlV1;
class CigiWeatherCtrlV2;
class CigiWeatherCtrlV3;


class CIGI_SPEC CigiBaseWeatherCtrl : public CigiBasePacket
{

friend class CigiWeatherCtrlV1;
friend class CigiWeatherCtrlV2;
friend class CigiWeatherCtrlV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseWeatherCtrl Group
   //!
   enum CloudTypeGrp
   {
      None=0,
      Altocumulus=1,
      Altostratus=2,
      Cirrocumulus=3,
      Cirrostratus=4,
      Cirrus=5,
      Cumulonimbus=6,
      Cumulus=7,
      Nimbostratus=8,
      Stratocumulus=9,
      Stratus=10,
      DefA=11,
      DefB=12,
      DefC=13,
      DefD=14,
      DefE=15
   };

   //=========================================================
   //! The enumeration for the CigiBaseWeatherCtrl Group
   //!
   enum ScopeGrp
   {
      Global=0,
      Regional=1,
      Entity=2
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseWeatherCtrl();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseWeatherCtrl();


   //==> Buffer Packing/Unpacking

   //=========================================================
   //! A pure virtual Pack function.  
   //! This function is not implemented in this class.
   //! \param Base - A pointer to the instance of the packet 
   //!          to be packed. (Downcast to CigiBasePacket)
   //! \param Buff - A pointer to the current pack point.
   //! \param Spec - A pointer to special data.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const=0;

   //=========================================================
   //! A pure virtual Unpack function.
   //! This function is not implemented in this class.
   //! \param Buff - A pointer to the current pack point.
   //! \param Swap - N/A for V1 & V2
   //! \param Spec - A pointer to special data -
   //!          This is not used in this class.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec) =0;

   //=========================================================
   //! A virtual Conversion Information function.
   //! This function provides conversion information for this
   //!  packet.
   //! \param CnvtVersion - The CIGI version to which this packet
   //!    is being converted.
   //! \param CnvtInfo - The information needed for conversion
   //!    
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	virtual int GetCnvt(CigiVersionID &CnvtVersion,
                       CigiCnvtInfoType::Type &CnvtInfo);



   //==> Accessing Member Variable Values functions

   //+> EntityID

   //=========================================================
   //! Sets the EntityID with bound checking control
   //! \param EntityIDIn - The entity to which this weather is attached
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEntityID(const Cigi_uint16 EntityIDIn, bool bndchk=true)
   {
      EntityID = EntityIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EntityID value.
   //! \return the current EntityID.
   Cigi_uint16 GetEntityID(void) const { return(EntityID); }


   //+> WeatherEn

   //=========================================================
   //! Sets the WeatherEn with bound checking control
   //! \param WeatherEnIn - Weather enable(true)/disable(false)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetWeatherEn(const bool WeatherEnIn, bool bndchk=true)
   {
      WeatherEn = WeatherEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the WeatherEn value.
   //! \return the current WeatherEn.
   bool GetWeatherEn(void) const { return(WeatherEn); }


   //+> ScudEn

   //=========================================================
   //! Sets the ScudEn with bound checking control
   //! \param ScudEnIn - Scud enable(true)/disable(false)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetScudEn(const bool ScudEnIn, bool bndchk=true)
   {
      ScudEn = ScudEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ScudEn value.
   //! \return the current ScudEn.
   bool GetScudEn(void) const { return(ScudEn); }


   //+> RandomWindsEn

   //=========================================================
   //! Sets the RandomWindsEn with bound checking control
   //! \param RandomWindsEnIn - Random winds enable(true)/disable(false)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRandomWindsEn(const bool RandomWindsEnIn, bool bndchk=true)
   {
      RandomWindsEn = RandomWindsEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RandomWindsEn value.
   //! \return the current RandomWindsEn.
   bool GetRandomWindsEn(void) const { return(RandomWindsEn); }


   //+> Severity

   //=========================================================
   //! Sets the Severity with bound checking control
   //! \param SeverityIn - Severity 0(least) to 5(most severe)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSeverity(const Cigi_uint8 SeverityIn, bool bndchk=true);

   //=========================================================
   //! Gets the Severity value.
   //! \return the current Severity.
   Cigi_uint8 GetSeverity(void) const { return(Severity); }


   //+> AirTemp

   //=========================================================
   //! Sets the AirTemp with bound checking control
   //! \param AirTempIn - Air Temperature
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAirTemp(const float AirTempIn, bool bndchk=true)
   {
      AirTemp = AirTempIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the AirTemp value.
   //! \return the current AirTemp.
   float GetAirTemp(void) const { return(AirTemp); }


   //+> VisibilityRng

   //=========================================================
   //! Sets the VisibilityRng with bound checking control
   //! \param VisibilityRngIn - Visibility range
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVisibilityRng(const float VisibilityRngIn, bool bndchk=true);

   //=========================================================
   //! Gets the VisibilityRng value.
   //! \return the current VisibilityRng.
   float GetVisibilityRng(void) const { return(VisibilityRng); }


   //+> ScudFreq

   //=========================================================
   //! Sets the ScudFreq with bound checking control
   //! \param ScudFreqIn - Scud coverage percentage
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetScudFreq(const float ScudFreqIn, bool bndchk=true);

   //=========================================================
   //! Gets the ScudFreq value.
   //! \return the current ScudFreq.
   float GetScudFreq(void) const { return(ScudFreq); }


   //+> Coverage

   //=========================================================
   //! Sets the Coverage with bound checking control
   //! \param CoverageIn - Weather coverage percentage
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCoverage(const float CoverageIn, bool bndchk=true);

   //=========================================================
   //! Gets the Coverage value.
   //! \return the current Coverage.
   float GetCoverage(void) const { return(Coverage); }


   //+> BaseElev

   //=========================================================
   //! Sets the BaseElev with bound checking control
   //! \param BaseElevIn - Elevation of the cloud base (bottom)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetBaseElev(const float BaseElevIn, bool bndchk=true)
   {
      BaseElev = BaseElevIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the BaseElev value.
   //! \return the current BaseElev.
   float GetBaseElev(void) const { return(BaseElev); }


   //+> Thickness

   //=========================================================
   //! Sets the Thickness with bound checking control
   //! \param ThicknessIn - Thickness or height(bottom to top) of the cloud
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetThickness(const float ThicknessIn, bool bndchk=true)
   {
      Thickness = ThicknessIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Thickness value.
   //! \return the current Thickness.
   float GetThickness(void) const { return(Thickness); }


   //+> Transition

   //=========================================================
   //! Sets the Transition with bound checking control
   //! \param TransitionIn - Thickness of the transition band
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetTransition(const float TransitionIn, bool bndchk=true)
   {
      Transition = TransitionIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Transition value.
   //! \return the current Transition.
   float GetTransition(void) const { return(Transition); }


   //+> HorizWindSp

   //=========================================================
   //! Sets the HorizWindSp with bound checking control
   //! \param HorizWindSpIn - Horizontal Wind speed
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetHorizWindSp(const float HorizWindSpIn, bool bndchk=true)
   {
      HorizWindSp = HorizWindSpIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the HorizWindSp value.
   //! \return the current HorizWindSp.
   float GetHorizWindSp(void) const { return(HorizWindSp); }



protected:

   //==> Member variables

   //=========================================================
   //! EntityID<br>
   //! The Entity ID to which this weather is attached
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! RegionID<br>
   //! The Region ID to which this weather is attached
   //!
   Cigi_uint16 RegionID;

   //=========================================================
   //! LayerID<br>
   //! The Layer ID that this packet controls
   //!
   Cigi_uint8 LayerID;

   //=========================================================
   //! Humidity<br>
   //! The humidity in this weather zone
   //!
   Cigi_uint8 Humidity;

   //=========================================================
   //! PhenomenonType<br>
   //! The type of weather being controlled
   //!
   Cigi_uint16 PhenomenonType;

   //=========================================================
   //! WeatherEn<br>
   //! Enables weather
   //!
   bool WeatherEn;

   //=========================================================
   //! ScudEn<br>
   //! Enables Scud
   //!
   bool ScudEn;

   //=========================================================
   //! RandomWindsEn<br>
   //! Enables random winds
   //!
   bool RandomWindsEn;

   //=========================================================
   //! RandomLightningEn<br>
   //! Enables random lightning
   //!
   bool RandomLightningEn;

   //=========================================================
   //! CloudType<br>
   //! Type of clouds being controlled.<br>
   //!  0-None<br>
   //!  1-Altocumulus<br>
   //!  2-Altostratus<br>
   //!  3-Cirrocumulus<br>
   //!  4-Cirrostratus<br>
   //!  5-Cirrus<br>
   //!  6-Cumulonimbus<br>
   //!  7-Cumulus<br>
   //!  8-Nimbostratus<br>
   //!  9-Stratocumulus<br>
   //!  10-Stratus<br>
   //!  11-DefA<br>
   //!  12-DefB<br>
   //!  13-DefC<br>
   //!  14-DefD<br>
   //!  15-DefE
   //!
   //!
   CloudTypeGrp CloudType;

   //=========================================================
   //! Scope<br>
   //! Specifies whether the weather being controlled is to be<br>
   //!   global, attached to a region, or attached to an entity.<br>
   //!  0-Global<br>
   //!  1-Regional<br>
   //!  2-Entity
   //!
   //!
   ScopeGrp Scope;

   //=========================================================
   //! Severity<br>
   //! Weather severity<br>
   //!  0-Least severe
   //!  5-Most severe
   //!
   Cigi_uint8 Severity;

   //=========================================================
   //! AirTemp<br>
   //! Temperature of the air.
   //!
   float AirTemp;

   //=========================================================
   //! VisibilityRng<br>
   //! Visibility range
   //!
   float VisibilityRng;

   //=========================================================
   //! Opacity<br>
   //! The percentage opacity of the weather
   //!
   float Opacity;

   //=========================================================
   //! ScudFreq<br>
   //! Scud frequency (i.e. percentage coverage.)
   //!
   float ScudFreq;

   //=========================================================
   //! Coverage<br>
   //! Percentage of coverage.
   //!
   float Coverage;

   //=========================================================
   //! BaseElev<br>
   //! Altitude of the base of the cloud
   //!
   float BaseElev;

   //=========================================================
   //! Thickness<br>
   //! Thickness of the cloud from bottom to top.
   //!
   float Thickness;

   //=========================================================
   //! Transition<br>
   //! Thickness of the transition band
   //!
   float Transition;

   //=========================================================
   //! HorizWindSp<br>
   //! Horizontal Wind Speed
   //!
   float HorizWindSp;

   //=========================================================
   //! VertWindSp<br>
   //! Verticle Wind Speed
   //!
   float VertWindSp;

   //=========================================================
   //! WindDir<br>
   //! Wind direction
   //!
   float WindDir;

   //=========================================================
   //! BaroPress<br>
   //! Barometric pressure
   //!
   float BaroPress;

   //=========================================================
   //! Aeorsol<br>
   //! Amount of aerosol
   //!
   float Aerosol;


};

#endif // #if !defined(_CIGI_BASE_WEATHER_CTRL_INCLUDED_)
