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
 *  FILENAME:   CigiIGCtrlV3_2.h
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
 *  Based on CigiIGCtrlV3.h"
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Moved Packet information to base packet.
 *  
 *  05/09/2008 Greg Basler                       Version 2.2.0
 *  Corrected the SetTimeStampValid description.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_IG_CONTROL_V3_2_INCLUDED_)
#define _CIGI_IG_CONTROL_V3_2_INCLUDED_



#include "CigiBaseIGCtrl.h"





//=========================================================
//! The class for the IG Control packet for CIGI version 3
//!
class CIGI_SPEC CigiIGCtrlV3_2 : public CigiBaseIGCtrl  
{
public:
	CigiIGCtrlV3_2();
	virtual ~CigiIGCtrlV3_2();


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
   //! Gets the Timestamp value for V3
   //! \return TimeStampV2 The timestamp value
   //!
   Cigi_uint32 GetTimeStamp(void) const { return(TimeStampV3); }

   //=========================================================
   //! Sets the Timestamp value for V2.<br>
   //!  This also converts to and sets the V1 and V2 timestamp values.
   //! \param TimeStamp - Specifies the timestamp value.
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetTimeStamp(const Cigi_uint32 TimeStamp, bool bndchk=true);



   //+> Timestamp Valid

   //=========================================================
   //! Gets the Timestamp valid flag for V3
   //! \return TimestampValid - The timestamp valid flag
   //!
   bool GetTimeStampValid(void) const { return(TimestampValid); }

   //=========================================================
   //! Sets the Timestamp valid flag for V3
   //! \param TimeStampValidIn - Sets or clears the Time Stamp Valid flag
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetTimeStampValid(const bool TimeStampValidIn, bool bndchk=true)
   {
      TimestampValid = TimeStampValidIn;

      return(CIGI_SUCCESS);
   }


   //+> Last Received IG Frame

   //=========================================================
   //! Gets the last received IG Frame id value
   //! \return Current last received IG Frame
   //!
   Cigi_uint32 GetLastRcvdIGFrame(void) const { return(LastRcvdIGFrame); }

   //=========================================================
   //! Sets the last received IG Frame with bound checking control
   //! \param LastRcvdIGFrameIn - Specifies the last received IG frame.
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   int SetLastRcvdIGFrame(const Cigi_uint32 LastRcvdIGFrameIn, bool bndchk=true)
   {
      LastRcvdIGFrame = LastRcvdIGFrameIn;

      return(CIGI_SUCCESS);
   }



};

#endif // !defined(_CIGI_IG_CONTROL_V3_2_INCLUDED_)
