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
 *  FILENAME:   CigiLosRespV3_2.h
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
 *  Based on CigiLosRespV3.h"
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


#if !defined(_CIGI_LOS_RESP_V3_2_INCLUDED_)
#define _CIGI_LOS_RESP_V3_2_INCLUDED_

#include "CigiBaseLosResp.h"



class CIGI_SPEC CigiLosRespV3_2 : public CigiBaseLosResp
{

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiLosRespV3_2();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiLosRespV3_2();


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

   //+> EntityIDValid

   //=========================================================
   //! Sets the EntityIDValid with bound checking control
   //! \param EntityIDValidIn - intersection with an entity and
   //!    the entity id is valid.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEntityIDValid(const bool EntityIDValidIn, bool bndchk=true)
   {
      EntityIDValid = EntityIDValidIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EntityIDValid value.
   //! \return the current EntityIDValid.
   bool GetEntityIDValid(void) const { return(EntityIDValid); }


   //+> HostFrame

   //=========================================================
   //! Sets the HostFrame with bound checking control
   //! \param HostFrameIn - The host frame when the hat/hot was
   //!   calculated.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetHostFrame(const Cigi_uint8 HostFrameIn, bool bndchk=true)
   {
      HostFrame = HostFrameIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the host frame value.
   //! \return the host frame when the hat/hot was calculated.
   Cigi_uint8 GetHostFrame(void) const { return(HostFrame); }


   //+> RespCount

   //=========================================================
   //! Sets the RespCount with bound checking control
   //! \param RespCountIn - The number of responses for this LOS operation.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRespCount(const Cigi_uint8 RespCountIn, bool bndchk=true)
   {
      RespCount = RespCountIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RespCount value.
   //! \return the current RespCount.
   Cigi_uint8 GetRespCount(void) const { return(RespCount); }


   //+> EntityID

   //=========================================================
   //! Sets the EntityID with bound checking control
   //! \param EntityIDIn - The ID of the entity intersected.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEntityID(const Cigi_uint16 EntityIDIn, bool bndchk=true)
   {
      EntityID = EntityIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EntityID value.
   //! \return the current EntityID.
   Cigi_uint16 GetEntityID(void) const { return(EntityID); }


};

#endif // #if !defined(_CIGI_LOS_RESP_V3_2_INCLUDED_)
