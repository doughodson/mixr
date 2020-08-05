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
 *  FILENAME:   CigiViewDefV1.h
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
 *  Changed the ViewIDIn parameter type from const unsigned short to const
 *  unsigned char in the SetViewID method.
 *
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed the return type from unsigned short to unsigned char for the
 *  GetViewID method. Changed the inline implementation of the GetViewID
 *  method to check for a valid view ID before returning a value.
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


#if !defined(_CIGI_VIEW_DEF_V1_INCLUDED_)
#define _CIGI_VIEW_DEF_V1_INCLUDED_

#include "CigiBaseViewDef.h"



class CIGI_SPEC CigiViewDefV1 : public CigiBaseViewDef
{

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiViewDefV1();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiViewDefV1();


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
   //! \param ViewIDIn - The View this packet to which this packet is applied.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetViewID(const Cigi_uint8 ViewIDIn, bool bndchk=true);

   //=========================================================
   //! Gets the ViewID value.
   //! \return the current ViewID.
   Cigi_uint8 GetViewID(void) const
   {
      if(ViewID < 31)
         return((Cigi_uint8)ViewID);
      else
         return((Cigi_uint8)0x1f);
   }


   //+> GroupID

   //=========================================================
   //! Sets the GroupID with bound checking control
   //! \param GroupIDIn - The group in which this view is place.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetGroupID(const Cigi_uint8 GroupIDIn, bool bndchk=true);

   //=========================================================
   //! Gets the GroupID value.
   //! \return the current GroupID.
   Cigi_uint8 GetGroupID(void) const { return(GroupID); }


   //+> TrackerAssigned

   //=========================================================
   //! Sets the TrackerAssigned with bound checking control
   //! \param TrackerAssignedIn - Specifies if there is a tracker
   //!   assigned to this view.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetTrackerAssigned(const bool TrackerAssignedIn, bool bndchk=true)
   {
      TrackerAssigned = TrackerAssignedIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the TrackerAssigned value.
   //! \return the current TrackerAssigned.
   bool GetTrackerAssigned(void) const { return(TrackerAssigned); }

};


#endif // #if !defined(_CIGI_VIEW_DEF_V1_INCLUDED_)
