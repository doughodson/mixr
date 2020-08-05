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
 *  FILENAME:   CigiBaseIGCtrl.h
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
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Implemented the SetDatabaseID function as inline.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Removed the SetFrameCntr function, to be implemented in child classes.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the LastRcvdIGFrame member variable to track the last frame received.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  Moved Packet information to base packet.
 *  
 *  05/09/2008 Greg Basler                       Version 2.2.0
 *  Added Extrapolation or Interpolation Smoothing flag.
 *  Added the defines and declarations for V3_3.
 *
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_BASE_IG_CTRL_INCLUDED_)
#define _CIGI_BASE_IG_CTRL_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_IG_CTRL_PACKET_ID_V1 1
#define CIGI_IG_CTRL_PACKET_SIZE_V1 16

#define CIGI_IG_CTRL_PACKET_ID_V2 1
#define CIGI_IG_CTRL_PACKET_SIZE_V2 16

#define CIGI_IG_CTRL_PACKET_ID_V3 1
#define CIGI_IG_CTRL_PACKET_SIZE_V3 16

#define CIGI_IG_CTRL_PACKET_ID_V3_2 1
#define CIGI_IG_CTRL_PACKET_SIZE_V3_2 24

#define CIGI_IG_CTRL_PACKET_ID_V3_3 1
#define CIGI_IG_CTRL_PACKET_SIZE_V3_3 24


class CigiIGCtrlV1;
class CigiIGCtrlV2;
class CigiIGCtrlV3;
class CigiIGCtrlV3_2;
class CigiIGCtrlV3_3;


//=========================================================
//! The class for the IG Control packet for CIGI
//!
class CIGI_SPEC CigiBaseIGCtrl : public CigiBasePacket  
{

friend class CigiIGCtrlV1;
friend class CigiIGCtrlV2;
friend class CigiIGCtrlV3;
friend class CigiIGCtrlV3_2;
friend class CigiIGCtrlV3_3;

public:

   //=========================================================
   //! The enumeration for the IG Mode
   //!
   enum IGModeGrp
   {
      Reset=0,
      Standby=0,
      Operate=1,
      debug=2
   };

   //=========================================================
   //! The enumeration for the Tracking Device Enable Flag
   //!
   enum TrackDeviceEnGrp
   {
      Disable=0,
      Enable=1
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
	CigiBaseIGCtrl();

   //=========================================================
   //! General Destructor
   //!
	virtual ~CigiBaseIGCtrl();


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

   //+> Swap

   //=========================================================
   //! Gets the Swap required flag<br>
   //! This is used for V3 to determine if automated swapping
   //!   is required.
   //! \return This returns the Swap flag<br>
   //!   If true, this signals that byte swapping is required
   //!   for V3 packets.
   //!
   bool GetSwap(void) const { return(BSwapEn); }


   //+> Database ID

   //=========================================================
   //! Gets the Database ID value
   //! \return DatabaseID - uniquely identifies the specific database
   //!
   Cigi_int8 GetDatabaseID(void) const { return(DatabaseID); }

   //=========================================================
   //! Sets the Database ID with bound checking control
   //! \param DataBaseIDIn - Specifies a new database to load.<br>
   //!   0 - No Action
   //!   All Other Values - Load the specified database
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetDatabaseID(const Cigi_int8 DataBaseIDIn, bool bndchk=true);


   //+> IG Mode

   //=========================================================
   //! Gets the IG Mode value
   //! \return IGMode specifying the commanded IG mode.<br>
   //!  0 - Reset or Standby<br>
   //!  1 - Operate<br>
   //!  2 - debug<br>
   //!
   IGModeGrp GetIGMode(void) const { return(IGMode); }

   //=========================================================
   //! Sets the IG Mode with bound checking control
   //! \param IGModeIn - Specifies the commanded IG mode.<br>
   //!  0 - Reset or Standby<br>
   //!  1 - Operate<br>
   //!  2 - debug<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	int SetIGMode(IGModeGrp IGModeIn, bool bndchk=true);


   //+> Frame Counter

   //=========================================================
   //! Gets the Frame Counter value
   //! \return Current frame this data represents
   //!
   Cigi_uint32 GetFrameCntr(void) const { return(FrameCntr); }




protected:

   //==> Member variables

   //=========================================================
   //! Swap<br>
   //! Specifies whether the V3 packets must be byte swapped<br>
   //!
	bool BSwapEn;

   //=========================================================
   //! DatabaseID<br>
   //! The current Database command<br>
   //!  0 - No Action.<br>
   //!  1 - 127 - Command to load the specified Database
   //!
   Cigi_int8 DatabaseID;

   //=========================================================
   //! Frame Counter<br>
   //! Specifies current frame being processed<br>
   //!
   Cigi_uint32 FrameCntr;

   //=========================================================
   //! Last Received IG Frame<br>
   //! Specifies IG frame received last<br>
   //!
   Cigi_uint32 LastRcvdIGFrame;

   //=========================================================
   //! Time Stamp - V1 format<br>
   //! Specifies the time from the beginning of the exercise<br>
   //!
   Cigi_uint32 TimeStampV1;

   //=========================================================
   //! Time Stamp - V2 format<br>
   //! Specifies the time from the beginning of the exercise<br>
   //!
   float TimeStampV2;

   //=========================================================
   //! Time Stamp - V3 format<br>
   //! Specifies the time from the beginning of the exercise<br>
   //!
   Cigi_uint32 TimeStampV3;

   //=========================================================
   //! Byte Swap Constant for V3<br>
   //! Constant used to determine if byteswapping is needed
   //!   in V3.
   //!
   Cigi_uint16 ByteSwap;


   //=========================================================
   //! IG Mode<br>
   //!  0 - Reset or Standby<br>
   //!  1 - Operate<br>
   //!  2 - debug<br>
   //!
   IGModeGrp IGMode;

   //=========================================================
   //! Tracking Device Enable<br>
   //!  true - Enables the tracking device<br>
   //!  false - Disables the tracking device<br>
   //!
   TrackDeviceEnGrp TrackDeviceEn;

   //=========================================================
   //! Boresight The Tracking Device<br>
   //!  true - Boresight the tracking device<br>
   //!  false - No Action<br>
   //!
   bool BoresightTrackDevice;

   //=========================================================
   //! Timestame Valid<br>
   //!  true - The timestamp is valid and usable<br>
   //!  false - The timestamp is not valid.
   //!
   bool TimestampValid;

   //=========================================================
   //! Smoothing Enabled<br>
   //!  Enable or Disable Extrapolation or Interpolation
   //!    motion smoothing.
   //!  true - Smoothing is Enabled<br>
   //!  false - Smoothing is Disabled.
   //!
   bool SmoothingEn;



};

#endif // !defined(_CIGI_BASE_IG_CTRL_INCLUDED_)
