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
 *  FILENAME:   CigiViewCtrlV3.h
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


#if !defined(_CIGI_VIEW_CTRL_V3_INCLUDED_)
#define _CIGI_VIEW_CTRL_V3_INCLUDED_

#include "CigiBaseViewCtrl.h"



class CIGI_SPEC CigiViewCtrlV3 : public CigiBaseViewCtrl
{

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiViewCtrlV3();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiViewCtrlV3();


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

   //+> ViewID

   //=========================================================
   //! Sets the ViewID with bound checking control
   //! \param ViewIDIn - Specifies the view that this packet controls.<br>
   //!  If The Group ID is not 0, ViewID is ignored.
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


   //+> GroupID

   //=========================================================
   //! Sets the GroupID with bound checking control
   //! \param GroupIDIn - Specifies the view group that this packet controls.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetGroupID(const Cigi_uint8 GroupIDIn, bool bndchk=true)
   {
      GroupID = GroupIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the GroupID value.
   //! \return the current GroupID.
   Cigi_uint8 GetGroupID(void) const { return(GroupID); }


   //+> XOffEn

   //=========================================================
   //! Sets the XOffEn with bound checking control
   //! \param XOffEnIn - Specifies if the X Offset data is valid
   //!   and to be used.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetXOffEn(const bool XOffEnIn, bool bndchk=true)
   {
      XOffEn = XOffEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the XOffEn value.
   //! \return the current XOffEn.
   bool GetXOffEn(void) const { return(XOffEn); }


   //+> YOffEn

   //=========================================================
   //! Sets the YOffEn with bound checking control
   //! \param YOffEnIn - Specifies if the Y Offset data is valid
   //!   and to be used.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetYOffEn(const bool YOffEnIn, bool bndchk=true)
   {
      YOffEn = YOffEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the YOffEn value.
   //! \return the current YOffEn.
   bool GetYOffEn(void) const { return(YOffEn); }


   //+> ZOffEn

   //=========================================================
   //! Sets the ZOffEn with bound checking control
   //! \param ZOffEnIn - Specifies if the Z Offset data is valid
   //!   and to be used.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetZOffEn(const bool ZOffEnIn, bool bndchk=true)
   {
      ZOffEn = ZOffEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the ZOffEn value.
   //! \return the current ZOffEn.
   bool GetZOffEn(void) const { return(ZOffEn); }


   //+> RollEn

   //=========================================================
   //! Sets the RollEn with bound checking control
   //! \param RollEnIn - Specifies if the roll data is valid
   //!   and to be used.
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
   //! \param PitchEnIn - Specifies if the pitch data is valid
   //!   and to be used.
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
   //! \param YawEnIn - Specifies if the yaw data is valid
   //!   and to be used.
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



};

#endif // #if !defined(_CIGI_VIEW_CTRL_V3_INCLUDED_)
