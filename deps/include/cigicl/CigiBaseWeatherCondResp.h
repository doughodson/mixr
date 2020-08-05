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
 *  FILENAME:   CigiBaseWeatherCondResp.h
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


#if !defined(_CIGI_BASE_WEATHER_COND_RESP_INCLUDED_)
#define _CIGI_BASE_WEATHER_COND_RESP_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_WEATHER_COND_RESP_PACKET_ID_V3 109
#define CIGI_WEATHER_COND_RESP_PACKET_SIZE_V3 32


class CigiWeatherCondRespV3;


class CIGI_SPEC CigiBaseWeatherCondResp : public CigiBasePacket
{

friend class CigiWeatherCondRespV3;

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseWeatherCondResp();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseWeatherCondResp();


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

   //+> RequestID

   //=========================================================
   //! Sets the RequestID with bound checking control
   //! \param RequestIDIn - Request ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRequestID(const Cigi_uint8 RequestIDIn, bool bndchk=true)
   {
      RequestID = RequestIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RequestID value.
   //! \return the current RequestID.
   Cigi_uint8 GetRequestID(void) const { return(RequestID); }


   //+> Humidity

   //=========================================================
   //! Sets the Humidity with bound checking control
   //! \param HumidityIn - Humidity
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetHumidity(const Cigi_uint8 HumidityIn, bool bndchk=true);

   //=========================================================
   //! Gets the Humidity value.
   //! \return the current Humidity.
   Cigi_uint8 GetHumidity(void) const { return(Humidity); }


   //+> AirTemp

   //=========================================================
   //! Sets the AirTemp with bound checking control
   //! \param AirTempIn - air temperature
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


   //+> Visibility

   //=========================================================
   //! Sets the Visibility with bound checking control
   //! \param VisibilityIn - Visibility range
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVisibility(const float VisibilityIn, bool bndchk=true);

   //=========================================================
   //! Gets the Visibility value.
   //! \return the current Visibility.
   float GetVisibility(void) const { return(Visibility); }


   //+> HorizWindSp

   //=========================================================
   //! Sets the HorizWindSp with bound checking control
   //! \param HorizWindSpIn - Horizontal wind speed
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetHorizWindSp(const float HorizWindSpIn, bool bndchk=true);

   //=========================================================
   //! Gets the HorizWindSp value.
   //! \return the current HorizWindSp.
   float GetHorizWindSp(void) const { return(HorizWindSp); }


   //+> VertWindSp

   //=========================================================
   //! Sets the VertWindSp with bound checking control
   //! \param VertWindSpIn - Vertical wind speed
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
   //! Gets the VertWindSp value.
   //! \return the current VertWindSp.
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
   //! Gets the WindDir value.
   //! \return the current WindDir.
   float GetWindDir(void) const { return(WindDir); }


   //+> BaroPress

   //=========================================================
   //! Sets the BaroPress with bound checking control
   //! \param BaroPressIn - Barometric pressure
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetBaroPress(const float BaroPressIn, bool bndchk=true);

   //=========================================================
   //! Gets the BaroPress value.
   //! \return the current BaroPress.
   float GetBaroPress(void) const { return(BaroPress); }



protected:

   //==> Member variables

   //=========================================================
   //! RequestID<br>
   //! Request ID
   //!
   Cigi_uint8 RequestID;

   //=========================================================
   //! Humidity<br>
   //!
   Cigi_uint8 Humidity;

   //=========================================================
   //! AirTemp<br>
   //! Air temperature
   //!
   float AirTemp;

   //=========================================================
   //! Visibility<br>
   //! Visibility range
   //!
   float Visibility;

   //=========================================================
   //! HorizWindSp<br>
   //! Horizontal wind speed
   //!
   float HorizWindSp;

   //=========================================================
   //! VertWindSp<br>
   //! Vertical wind speed
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


};

#endif // #if !defined(_CIGI_BASE_WEATHER_COND_RESP_INCLUDED_)
