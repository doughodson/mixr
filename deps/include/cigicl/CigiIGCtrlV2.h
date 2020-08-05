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
 *  FILENAME:   CigiIGCtrlV2.h
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
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the SetFrameCntr accessor method.
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

#if !defined(_CIGI_IG_CTRL_V2_INCLUDED_)
#define _CIGI_IG_CTRL_V2_INCLUDED_


#include "CigiBaseIGCtrl.h"





//=========================================================
//! The class for the IG Control packet for CIGI version 2
//!
class CIGI_SPEC CigiIGCtrlV2 : public CigiBaseIGCtrl  
{
public:
	CigiIGCtrlV2();
	virtual ~CigiIGCtrlV2();


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
      LastRcvdIGFrame = FrameCntrIn;

      return(CIGI_SUCCESS);
   }


   //+> Timestamp

   //=========================================================
   //! Gets the Timestamp value for V2
   //! \return TimeStampV2 The timestamp value
   //!
   float GetTimeStamp(void) const { return(TimeStampV2); }

   //=========================================================
   //! Sets the Timestamp value for V2.<br>
   //!  This also converts to and sets the V1 and V3 timestamp values.
   //! \param TimeStamp - Specifies the timestamp value.
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetTimeStamp(const float TimeStamp, bool bndchk=true);


   //+> Tracking Device Enable

   //=========================================================
   //! Gets the Tracking Device Enable flag for V2
   //! \return TrackDeviceEn - Tracking Device enabled or disabled.
   //!
   TrackDeviceEnGrp GetTrackDeviceEn(void) const { return(TrackDeviceEn); }

   //=========================================================
   //! Sets the Tracking Device Enable flag for V2.<br>
   //! \param TrackDeviceEnIn - Specifies the Tracking Device Enable flag.
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetTrackDeviceEn(const TrackDeviceEnGrp TrackDeviceEnIn, bool bndchk=true);


   //+> Boresight Tracking Device

   //=========================================================
   //! Gets the Boresight Tracking Device flag for V2
   //! \return BoresightTrackDevice - Boresight Tracking Device command.
   //!
   bool GetBoresightTrackDevice(void) const { return(BoresightTrackDevice); }

   //=========================================================
   //! Sets the Boresight Tracking Device flag for V2.<br>
   //! \param BoresightTrackDeviceIn - Specifies the 
   //!   Boresight Tracking Device command.
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetBoresightTrackDevice(const bool BoresightTrackDeviceIn,
                                 bool bndchk=true)
   {
      BoresightTrackDevice = BoresightTrackDeviceIn;

      return(CIGI_SUCCESS);

   }




};

#endif // !defined(_CIGI_IG_CTRL_V2_INCLUDED_)
