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
 *  FILENAME:   CigiEnvCtrlV1.h
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


#if !defined(_CIGI_ENV_CTRL_V1_INCLUDED_)
#define _CIGI_ENV_CTRL_V1_INCLUDED_

#include "CigiBaseEnvCtrl.h"



class CIGI_SPEC CigiEnvCtrlV1 : public CigiBaseEnvCtrl
{

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiEnvCtrlV1();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiEnvCtrlV1();


   //==> Buffer Packing/Unpacking

   //=========================================================
   //! The virtual Pack function for CIGI 1
   //! \param Base - A pointer to the instance of the packet 
   //!          to be packed. (Downcast to CigiBasePacket)
   //! \param Buff - A pointer to the current pack point.
   //! \param Spec - A pointer to special data -
   //!          This is used to point to the Environmental Control 
   //!            holding object when going between V1 and V3.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const;

   //=========================================================
   //! The virtual Unpack function for CIGI 1
   //! \param Buff - A pointer to the current pack point.
   //! \param Swap - N/A for V1 & V2
   //! \param Spec - A pointer to special data -
   //!          This is used to point to the Environmental Control 
   //!            holding object when going between V1 and V3.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec);


   //==> Hold Object Manipulation

   //=========================================================
   //! The virtual FillHold function.  
   //! \param Hold - A pointer to the Hold object. (Downcast to CigiBaseEnvCtrl)
   //!
   virtual void FillHold(CigiBaseEnvCtrl * Hold) const;



   //==> Accessing Member Variable Values functions

   //+> Hour

   //=========================================================
   //! Sets the Hour with bound checking control
   //! \param HourIn - Current hour
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetHour(const Cigi_uint8 HourIn, bool bndchk=true);

   //=========================================================
   //! Gets the Hour value.
   //! \return the current Hour.
   Cigi_uint8 GetHour(void) const { return(Hour); }


   //+> Minute

   //=========================================================
   //! Sets the Minute with bound checking control
   //! \param MinuteIn - Current minute
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetMinute(const Cigi_uint8 MinuteIn, bool bndchk=true);

   //=========================================================
   //! Gets the Minute value.
   //! \return the current Minute.
   Cigi_uint8 GetMinute(void) const { return(Minute); }


   //+> Month

   //=========================================================
   //! Sets the Month with bound checking control
   //! \param MonthIn - current month
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetMonth(const Cigi_uint8 MonthIn, bool bndchk=true);

   //=========================================================
   //! Gets the Month value.
   //! \return the current Month.
   Cigi_uint8 GetMonth(void) const { return(Month); }


   //+> Day

   //=========================================================
   //! Sets the Day with bound checking control
   //! \param DayIn - current day of the month
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDay(const Cigi_uint8 DayIn, bool bndchk=true);

   //=========================================================
   //! Gets the Day value.
   //! \return the current Day.
   Cigi_uint8 GetDay(void) const { return(Day); }


   //+> Year

   //=========================================================
   //! Sets the Year with bound checking control
   //! \param YearIn - current year
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYear(const Cigi_uint16 YearIn, bool bndchk=true)
   {
      Year = YearIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Year value.
   //! \return the current Year.
   Cigi_uint16 GetYear(void) const { return(Year); }


   //+> EphemerisEn

   //=========================================================
   //! Sets the EphemerisEn with bound checking control
   //! \param EphemerisEnIn - Enable (true) or disable (false)
   //!    ephereris model.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEphemerisEn(const bool EphemerisEnIn, bool bndchk=true)
   {
      EphemerisEn = EphemerisEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EphemerisEn value.
   //! \return the current EphemerisEn.
   bool GetEphemerisEn(void) const { return(EphemerisEn); }


   //+> Humidity

   //=========================================================
   //! Sets the Humidity with bound checking control
   //! \param HumidityIn - Percentage humidity.
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
   //! \param AirTempIn - Current ambient air temperature
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
   //! \param VisibilityIn - Global visibility.
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
   //! \param HorizWindSpIn - Wind speed
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetHorizWindSp(const float HorizWindSpIn, bool bndchk=true);

   //=========================================================
   //! Gets the HorizWindSp value.
   //! \return the current HorizWindSp.
   float GetHorizWindSp(void) const { return(HorizWindSp); }


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


};

#endif // #if !defined(_CIGI_ENV_CTRL_V1_INCLUDED_)
