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
 *  FILENAME:   CigiSOFV3_2.h
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
 *  04/14/2006 Greg Basler                       Version 1.7
 *  Initial Release for CIGI 3.2 compatibility.
 *  Based on CigiSOFV3.h"
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

#if !defined(_CIGI_SOF_V3_2_INCLUDED_)
#define _CIGI_SOF_V3_2_INCLUDED_


#include "CigiBaseSOF.h"





//=========================================================
//! The class for the Start Of Frame packet for CIGI version 3
//!
class CIGI_SPEC CigiSOFV3_2 : public CigiBaseSOF  
{
public:
	CigiSOFV3_2();
	virtual ~CigiSOFV3_2();



   //==> Buffer Packing/Unpacking

   //=========================================================
   //! The virtual Pack function for CIGI 1
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
   //! The virtual Unpack function for CIGI 1
   //! \param Buff - A pointer to the current pack point.
   //! \param Swap - Byte swapping required.
   //! \param Spec - A pointer to special data -
   //!          This is not used in this class.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	virtual int Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec);


   //==> Accessing Member Variable Values functions


   //+> Frame Counter

   //=========================================================
   //! Gets the Frame Counter value
   //! \return Current frame this data represents
   //!
   Cigi_uint32 GetFrameCntr(void) const { return(FrameCntr); }

   //=========================================================
   //! Sets the Frame Counter with bound checking control
   //! \param FrameCntrIn - Specifies the frame.
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   int SetFrameCntr(const Cigi_uint32 FrameCntrIn, bool bndchk=true)
   {
      FrameCntr = FrameCntrIn;

      return(CIGI_SUCCESS);
   }


   //+> Timestamp

   //=========================================================
   //! Gets the V3 timestamp.
   //! \return The timestamp count in 10 microseconds increments.
   //!
   Cigi_uint32 GetTimeStamp(void) const { return(TimeStampV3); }

   //=========================================================
   //! Sets the timestamp V3 value with bound checking control.
   //! \param TimeStamp - Specifies the timestamp count in 
   //!   10 microseconds increments.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetTimeStamp(const Cigi_uint32 TimeStamp, bool bndchk=true);


   //+> Timestamp Valid

   //=========================================================
   //! Gets the timestamp valid flag.
   //! \return The timestamp valid flag.
   //!
   bool GetTimeStampValid(void) const { return(TimestampValid); }

   //=========================================================
   //! Sets the timestamp valid flag with bound checking control.
   //! \param TimeStampValidIn - Specifies whether the timestamp 
   //!   is valid for use.<br>
   //!   true - Timestamp is valid.<br>
   //!   false - Timestamp is not valid.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetTimeStampValid(const bool TimeStampValidIn, bool bndchk=true)
   {
      TimestampValid = TimeStampValidIn;

      return(CIGI_SUCCESS);
   }


   //+> Timestamp Valid

   //=========================================================
   //! Gets the earth reference model indicator.
   //! \return The earth reference model indicator.<br>
   //!   0 - WGS84.<br>
   //!   1 - System defined.
   //!
   EarthRefModelGrp GetEarthRefModel(void) const { return(EarthRefModel); }

   //=========================================================
   //! Sets the earth reference model indicator with bound checking control.
   //! \param EarthRefModelIn - Specifies the earth reference model 
   //!   is valid for use.<br>
   //!   0 - WGS84.<br>
   //!   1 - System defined.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   int SetEarthRefModel(const EarthRefModelGrp EarthRefModelIn, bool bndchk=true);


   //+> Last Received Host Frame

   //=========================================================
   //! Gets the last received host frame id value
   //! \return last received host frame
   //!
   Cigi_uint32 GetLastRcvdHostFrame(void) const { return(LastRcvdHostFrame); }

   //=========================================================
   //! Sets the last received host frame with bound checking control
   //! \param LastRcvdHostFrameIn - Specifies the last received host frame.
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   int SetLastRcvdHostFrame(const Cigi_uint32 LastRcvdHostFrameIn, bool bndchk=true)
   {
      LastRcvdHostFrame = LastRcvdHostFrameIn;

      return(CIGI_SUCCESS);
   }



};

#endif // !defined(_CIGI_SOF_V3_2_INCLUDED_)
