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
 *  FILENAME:   CigiWeatherCtrlV3.h
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
 *  Moved Packet information to base packet.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_WEATHER_CTRL_V3_INCLUDED_)
#define _CIGI_WEATHER_CTRL_V3_INCLUDED_

#include "CigiBaseWeatherCtrl.h"



class CIGI_SPEC CigiWeatherCtrlV3 : public CigiBaseWeatherCtrl
{

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiWeatherCtrlV3();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiWeatherCtrlV3();


   //==> Buffer Packing/Unpacking

   //=========================================================
   //! The virtual Pack function for CIGI 3
   //! \param Base - A pointer to the instance of the packet 
   //!          to be packed. (Downcast to CigiBasePacket)
   //! \param Buff - A pointer to the current pack point.
   //! \param Spec - A pointer to special data -
   //!          This is not used in this class.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const;

   //=========================================================
   //! The virtual Unpack function for CIGI 3
   //! \param Buff - A pointer to the current pack point.
   //! \param Swap - N/A for V1 & V2
   //! \param Spec - A pointer to special data -
   //!          This is not used in this class.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec);



   //==> Accessing Member Variable Values functions

   //+> RegionID

   //=========================================================
   //! Sets the RegionID with bound checking control
   //! \param RegionIDIn - Region ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRegionID(const Cigi_uint16 RegionIDIn, bool bndchk=true)
   {
      RegionID = RegionIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RegionID value.
   //! \return the current RegionID.
   Cigi_uint16 GetRegionID(void) const { return(RegionID); }


   //+> LayerID

   //=========================================================
   //! Sets the LayerID with bound checking control
   //! \param LayerIDIn - Layer ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLayerID(const Cigi_uint8 LayerIDIn, bool bndchk=true);

   //=========================================================
   //! Gets the LayerID value.
   //! \return the current LayerID.
   Cigi_uint8 GetLayerID(void) const { return(LayerID); }


   //+> Humidity

   //=========================================================
   //! Sets the Humidity with bound checking control
   //! \param HumidityIn - The weather area's humidity
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetHumidity(const Cigi_uint8 HumidityIn, bool bndchk=true);

   //=========================================================
   //! Gets the Humidity value.
   //! \return the current Humidity.
   Cigi_uint8 GetHumidity(void) const { return(Humidity); }


   //+> RandomLightningEn

   //=========================================================
   //! Sets the RandomLightningEn with bound checking control
   //! \param RandomLightningEnIn - Random lightning enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRandomLightningEn(const bool RandomLightningEnIn, bool bndchk=true)
   {
      RandomLightningEn = RandomLightningEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RandomLightningEn value.
   //! \return the current RandomLightningEn.
   bool GetRandomLightningEn(void) const { return(RandomLightningEn); }


   //+> CloudType

   //=========================================================
   //! Sets the CloudType with bound checking control
   //! \param CloudTypeIn - Cloud type specification<br>
   //!   None=0<br>
   //!   Altocumulus=1<br>
   //!   Altostratus=2<br>
   //!   Cirrocumulus=3<br>
   //!   Cirrostratus=4<br>
   //!   Cirrus=5<br>
   //!   Cumulonimbus=6<br>
   //!   Cumulus=7<br>
   //!   Nimbostratus=8<br>
   //!   Stratocumulus=9<br>
   //!   Stratus=10<br>
   //!   DefA=11<br>
   //!   DefB=12<br>
   //!   DefC=13<br>
   //!   DefD=14<br>
   //!   DefE=15<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCloudType(const CloudTypeGrp CloudTypeIn, bool bndchk=true);

   //=========================================================
   //! Gets the CloudType value.
   //! \return the current CloudType.
   CloudTypeGrp GetCloudType(void) const { return(CloudType); }


   //+> Scope

   //=========================================================
   //! Sets the Scope with bound checking control
   //! \param ScopeIn - Weather effects scope<br>
   //!   Global=0<br>
   //!   Regional=1<br>
   //!   Entity=2<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetScope(const ScopeGrp ScopeIn, bool bndchk=true);

   //=========================================================
   //! Gets the Scope value.
   //! \return the current Scope.
   ScopeGrp GetScope(void) const { return(Scope); }


   //+> VertWindSp

   //=========================================================
   //! Sets the VertWindSp with bound checking control
   //! \param VertWindSpIn - Verticle Wind Speed
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVertWindSp(const float VertWindSpIn, bool bndchk=true)
   {
      VertWindSp = VertWindSpIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the WindDirEl value.
   //! \return the current WindDirEl.
   float GetVertWindSp(void) const { return(VertWindSp); }


   //+> WindDir

   //=========================================================
   //! Sets the WindDir with bound checking control
   //! \param WindDirIn - Wind direction
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetWindDir(const float WindDirIn, bool bndchk=true);

   //=========================================================
   //! Gets the WindDirAz value.
   //! \return the current WindDirAz.
   float GetWindDir(void) const { return(WindDir); }


   //+> BaroPress

   //=========================================================
   //! Sets the BaroPress with bound checking control
   //! \param BaroPressIn - Barometric pressure
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetBaroPress(const float BaroPressIn, bool bndchk=true)
   {
      BaroPress = BaroPressIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the BaroPress value.
   //! \return the current BaroPress.
   float GetBaroPress(void) const { return(BaroPress); }


   //+> Aeorsol

   //=========================================================
   //! Sets the Aerosol with bound checking control
   //! \param AerosolIn - Aerosol consentration
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAerosol(const float AerosolIn, bool bndchk=true)
   {
      Aerosol = AerosolIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Aerosol value.
   //! \return the current Aerosol.
   float GetAerosol(void) const { return(Aerosol); }



};

#endif // #if !defined(_CIGI_WEATHER_CTRL_V3_INCLUDED_)
