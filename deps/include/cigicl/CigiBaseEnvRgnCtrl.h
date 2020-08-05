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
 *  FILENAME:   CigiBaseEnvRgnCtrl.h
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


#if !defined(_CIGI_BASE_ENV_RGN_CTRL_INCLUDED_)
#define _CIGI_BASE_ENV_RGN_CTRL_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_ENV_RGN_CTRL_PACKET_ID_V3 11
#define CIGI_ENV_RGN_CTRL_PACKET_SIZE_V3 48


class CigiEnvRgnCtrlV3;


class CIGI_SPEC CigiBaseEnvRgnCtrl : public CigiBasePacket
{

friend class CigiEnvRgnCtrlV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseEnvRgnCtrl Group
   //!
   enum RgnStateGrp
   {
      Inactive=0,
      Active=1,
      Destroyed=2
   };

   //=========================================================
   //! The enumeration for the CigiBaseEnvRgnCtrl Group
   //!
   enum MergeCtrlGrp
   {
      UseLast=0,
      Merge=1
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseEnvRgnCtrl();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseEnvRgnCtrl();


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
   //! \param Spec - A pointer to special data.
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


   //+> RgnState

   //=========================================================
   //! Sets the RgnState with bound checking control
   //! \param RgnStateIn - Region state control<br>
   //!   Inactive=0<br>
   //!   Active=1<br>
   //!   Destroyed=2<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRgnState(const RgnStateGrp RgnStateIn, bool bndchk=true);

   //=========================================================
   //! Gets the RgnState value.
   //! \return the current RgnState.
   RgnStateGrp GetRgnState(void) const { return(RgnState); }


   //+> WeatherProp

   //=========================================================
   //! Sets the WeatherProp with bound checking control
   //! \param WeatherPropIn - Weather property
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetWeatherProp(const MergeCtrlGrp WeatherPropIn, bool bndchk=true);

   //=========================================================
   //! Gets the WeatherProp value.
   //! \return the current WeatherProp.
   MergeCtrlGrp GetWeatherProp(void) const { return(WeatherProp); }


   //+> Aerosol

   //=========================================================
   //! Sets the Aerosol with bound checking control
   //! \param AerosolIn - Aerosol concentration
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAerosol(const MergeCtrlGrp AerosolIn, bool bndchk=true);

   //=========================================================
   //! Gets the Aerosol value.
   //! \return the current Aerosol.
   MergeCtrlGrp GetAerosol(void) const { return(Aerosol); }


   //+> MaritimeSurface

   //=========================================================
   //! Sets the MaritimeSurface with bound checking control
   //! \param MaritimeSurfaceIn - Maritime surface conditions
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetMaritimeSurface(const MergeCtrlGrp MaritimeSurfaceIn, bool bndchk=true);

   //=========================================================
   //! Gets the MaritimeSurface value.
   //! \return the current MaritimeSurface.
   MergeCtrlGrp GetMaritimeSurface(void) const { return(MaritimeSurface); }


   //+> TerrestrialSurface

   //=========================================================
   //! Sets the TerrestrialSurface with bound checking control
   //! \param TerrestrialSurfaceIn - Terrestrial surface conditions
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetTerrestrialSurface(const MergeCtrlGrp TerrestrialSurfaceIn, bool bndchk=true);

   //=========================================================
   //! Gets the TerrestrialSurface value.
   //! \return the current TerrestrialSurface.
   MergeCtrlGrp GetTerrestrialSurface(void) const { return(TerrestrialSurface); }


   //+> Lat

   //=========================================================
   //! Sets the Lat with bound checking control
   //! \param LatIn - Latitude
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLat(const double LatIn, bool bndchk=true);

   //=========================================================
   //! Gets the Lat value.
   //! \return the current Lat.
   double GetLat(void) const { return(Lat); }


   //+> Lon

   //=========================================================
   //! Sets the Lon with bound checking control
   //! \param LonIn - Longitude
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLon(const double LonIn, bool bndchk=true);

   //=========================================================
   //! Gets the Lon value.
   //! \return the current Lon.
   double GetLon(void) const { return(Lon); }


   //+> XSize

   //=========================================================
   //! Sets the XSize with bound checking control
   //! \param XSizeIn - X size of the region
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetXSize(const float XSizeIn, bool bndchk=true)
   {
      XSize = XSizeIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the XSize value.
   //! \return the current XSize.
   float GetXSize(void) const { return(XSize); }


   //+> YSize

   //=========================================================
   //! Sets the YSize with bound checking control
   //! \param YSizeIn - Y size of the region
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYSize(const float YSizeIn, bool bndchk=true)
   {
      YSize = YSizeIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the YSize value.
   //! \return the current YSize.
   float GetYSize(void) const { return(YSize); }


   //+> CornerRadius

   //=========================================================
   //! Sets the CornerRadius with bound checking control
   //! \param CornerRadiusIn - Corner Radius
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCornerRadius(const float CornerRadiusIn, bool bndchk=true)
   {
      CornerRadius = CornerRadiusIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the CornerRadius value.
   //! \return the current CornerRadius.
   float GetCornerRadius(void) const { return(CornerRadius); }


   //+> Rotation

   //=========================================================
   //! Sets the Rotation with bound checking control
   //! \param RotationIn - The rotation of the region from true north
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRotation(const float RotationIn, bool bndchk=true);

   //=========================================================
   //! Gets the Rotation value.
   //! \return the current Rotation.
   float GetRotation(void) const { return(Rotation); }


   //+> Transition

   //=========================================================
   //! Sets the Transition with bound checking control
   //! \param TransitionIn - Width of the trasition band
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



protected:

   //==> Member variables

   //=========================================================
   //! RegionID<br>
   //! Region ID
   //!
   Cigi_uint16 RegionID;

   //=========================================================
   //! RgnState<br>
   //! Region state control<br>
   //!  0-Inactive<br>
   //!  1-Active<br>
   //!  2-Destroyed
   //!
   RgnStateGrp RgnState;

   //=========================================================
   //! WeatherProp<br>
   //! Weather property merge control
   //!
   MergeCtrlGrp WeatherProp;

   //=========================================================
   //! Aerosol<br>
   //! Aerosol Concentration merge control
   //!
   MergeCtrlGrp Aerosol;

   //=========================================================
   //! MaritimeSurface<br>
   //! Maritime surface conditions merge control
   //!
   MergeCtrlGrp MaritimeSurface;

   //=========================================================
   //! TerrestrialSurface<br>
   //! Terrestrial surface conditions merge control
   //!
   MergeCtrlGrp TerrestrialSurface;

   //=========================================================
   //! Lat<br>
   //! Latitude
   //!
   double Lat;

   //=========================================================
   //! Lon<br>
   //! Longitude
   //!
   double Lon;

   //=========================================================
   //! XSize<br>
   //! X size of the region
   //!
   float XSize;

   //=========================================================
   //! YSize<br>
   //! Y size of the region
   //!
   float YSize;

   //=========================================================
   //! CornerRadius<br>
   //! Radius of the corners
   //!
   float CornerRadius;

   //=========================================================
   //! Rotation<br>
   //! Rotation of the region from true north
   //!
   float Rotation;

   //=========================================================
   //! Transition<br>
   //! Width of the transition band.
   //!
   float Transition;


};

#endif // #if !defined(_CIGI_BASE_ENV_RGN_CTRL_INCLUDED_)
