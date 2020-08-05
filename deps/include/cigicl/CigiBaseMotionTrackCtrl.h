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
 *  FILENAME:   CigiBaseMotionTrackCtrl.h
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


#if !defined(_CIGI_BASE_MOTION_TRACK_CTRL_INCLUDED_)
#define _CIGI_BASE_MOTION_TRACK_CTRL_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_MOTION_TRACK_CTRL_PACKET_ID_V3 18
#define CIGI_MOTION_TRACK_CTRL_PACKET_SIZE_V3 8


class CigiMotionTrackCtrlV1;
class CigiMotionTrackCtrlV2;
class CigiMotionTrackCtrlV3;


class CIGI_SPEC CigiBaseMotionTrackCtrl : public CigiBasePacket
{

friend class CigiMotionTrackCtrlV1;
friend class CigiMotionTrackCtrlV2;
friend class CigiMotionTrackCtrlV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseMotionTrackCtrl Group
   //!
   enum ScopeGrp
   {
      View=0,
      ViewGrp=1
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseMotionTrackCtrl();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseMotionTrackCtrl();


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

   //+> ViewID

   //=========================================================
   //! Sets the ViewID with bound checking control
   //! \param ViewIDIn - View or View Group ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetViewID(const Cigi_uint16 ViewIDIn, bool bndchk=true)
   {
      ViewID = ViewIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ViewID value.
   //! \return the current ViewID.
   Cigi_uint16 GetViewID(void) const { return(ViewID); }


   //+> TrackerID

   //=========================================================
   //! Sets the TrackerID with bound checking control
   //! \param TrackerIDIn - Tracker ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetTrackerID(const Cigi_uint8 TrackerIDIn, bool bndchk=true)
   {
      TrackerID = TrackerIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the TrackerID value.
   //! \return the current TrackerID.
   Cigi_uint8 GetTrackerID(void) const { return(TrackerID); }


   //+> TrackEn

   //=========================================================
   //! Sets the TrackEn with bound checking control
   //! \param TrackerEnIn - Tracker enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetTrackerEn(const bool TrackerEnIn, bool bndchk=true)
   {
      TrackerEn = TrackerEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the TrackEn value.
   //! \return the current TrackEn.
   bool GetTrackerEn(void) const { return(TrackerEn); }


   //+> BoresightEn

   //=========================================================
   //! Sets the BoresightEn with bound checking control
   //! \param BoresightEnIn - Boresight enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetBoresightEn(const bool BoresightEnIn, bool bndchk=true)
   {
      BoresightEn = BoresightEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the BoresightEn value.
   //! \return the current BoresightEn.
   bool GetBoresightEn(void) const { return(BoresightEn); }


   //+> XEn

   //=========================================================
   //! Sets the XEn with bound checking control
   //! \param XEnIn - X axis measurement enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetXEn(const bool XEnIn, bool bndchk=true)
   {
      XEn = XEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the XEn value.
   //! \return the current XEn.
   bool GetXEn(void) const { return(XEn); }


   //+> YEn

   //=========================================================
   //! Sets the YEn with bound checking control
   //! \param YEnIn - Y axis measurement enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYEn(const bool YEnIn, bool bndchk=true)
   {
      YEn = YEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the YEn value.
   //! \return the current YEn.
   bool GetYEn(void) const { return(YEn); }


   //+> ZEn

   //=========================================================
   //! Sets the ZEn with bound checking control
   //! \param ZEnIn - Z axis measurement enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetZEn(const bool ZEnIn, bool bndchk=true)
   {
      ZEn = ZEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ZEn value.
   //! \return the current ZEn.
   bool GetZEn(void) const { return(ZEn); }


   //+> RollEn

   //=========================================================
   //! Sets the RollEn with bound checking control
   //! \param RollEnIn - Roll measurement enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRollEn(const bool RollEnIn, bool bndchk=true)
   {
      RollEn = RollEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RollEn value.
   //! \return the current RollEn.
   bool GetRollEn(void) const { return(RollEn); }


   //+> PitchEn

   //=========================================================
   //! Sets the PitchEn with bound checking control
   //! \param PitchEnIn - Pitch measurement enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetPitchEn(const bool PitchEnIn, bool bndchk=true)
   {
      PitchEn = PitchEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the PitchEn value.
   //! \return the current PitchEn.
   bool GetPitchEn(void) const { return(PitchEn); }


   //+> YawEn

   //=========================================================
   //! Sets the YawEn with bound checking control
   //! \param YawEnIn - Yaw measurement enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYawEn(const bool YawEnIn, bool bndchk=true)
   {
      YawEn = YawEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the YawEn value.
   //! \return the current YawEn.
   bool GetYawEn(void) const { return(YawEn); }


   //+> Scope

   //=========================================================
   //! Sets the Scope with bound checking control
   //! \param ScopeIn - Scope of effect<br>
   //!   View=0<br>
   //!   ViewGrp=1<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetScope(const ScopeGrp ScopeIn, bool bndchk=true);

   //=========================================================
   //! Gets the Scope value.
   //! \return the current Scope.
   ScopeGrp GetScope(void) const { return(Scope); }



protected:

   //==> Member variables

   //=========================================================
   //! ViewID<br>
   //! View or View Group ID
   //!
   Cigi_uint16 ViewID;

   //=========================================================
   //! TrackerID<br>
   //! Tracker ID
   //!
   Cigi_uint8 TrackerID;

   //=========================================================
   //! TrackerEn<br>
   //! Tracker enable
   //!
   bool TrackerEn;

   //=========================================================
   //! BoresightEn<br>
   //! Boresight enable
   //!
   bool BoresightEn;

   //=========================================================
   //! XEn<br>
   //! X axis measurement enable
   //!
   bool XEn;

   //=========================================================
   //! YEn<br>
   //! Y axis measurement enable
   //!
   bool YEn;

   //=========================================================
   //! ZEn<br>
   //! Z axis measurement enable
   //!
   bool ZEn;

   //=========================================================
   //! RollEn<br>
   //! Roll measurement enable
   //!
   bool RollEn;

   //=========================================================
   //! PitchEn<br>
   //! Pitch measurement enable
   //!
   bool PitchEn;

   //=========================================================
   //! YawEn<br>
   //! Yaw measurement enable
   //!
   bool YawEn;

   //=========================================================
   //! Scope<br>
   //! Scope of effect<br>
   //!  0-View<br>
   //!  1-ViewGrp
   //!
   //!
   ScopeGrp Scope;


};

#endif // #if !defined(_CIGI_BASE_MOTION_TRACK_CTRL_INCLUDED_)
