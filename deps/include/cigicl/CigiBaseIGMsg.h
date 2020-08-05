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
 *  FILENAME:   CigiBaseIGMsg.h
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
 *  Added Variable length packet processing
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_BASE_IG_MSG_INCLUDED_)
#define _CIGI_BASE_IG_MSG_INCLUDED_

#include "CigiBaseVariableSizePckt.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_IG_MSG_PACKET_ID_V2 108

#define CIGI_IG_MSG_PACKET_ID_V3 117


class CigiIGMsgV2;
class CigiIGMsgV3;


class CIGI_SPEC CigiBaseIGMsg : public CigiBaseVariableSizePckt
{

friend class CigiIGMsgV2;
friend class CigiIGMsgV3;

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseIGMsg() { };

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseIGMsg() { };


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
                       CigiCnvtInfoType::Type &CnvtInfo)   {
      CnvtInfo.ProcID = CigiProcessType::ProcStd;

      if(CnvtVersion.CigiMajorVersion < 2)
      {
         CnvtInfo.ProcID = CigiProcessType::ProcNone;
         CnvtInfo.CnvtPacketID = 0;
      }
      else
      {
         CnvtInfo.ProcID = CigiProcessType::ProcVarSize;
         if(CnvtVersion.CigiMajorVersion < 3)
            CnvtInfo.CnvtPacketID = CIGI_IG_MSG_PACKET_ID_V2;
         else
            CnvtInfo.CnvtPacketID = CIGI_IG_MSG_PACKET_ID_V3;
      }

      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! A pure virtual function to determine the size that the
   //!  packet will take up when packed.
   //! This function is not implemented in this class.
   //! \param refPacket - A pointer to the current pack point.
   //!
   //! \return The size that the packet will take up when packed
   //!
	virtual int GetTruePacketSize(CigiBaseVariableSizePckt &refPacket) =0;




   //==> Accessing Member Variable Values functions

   //+> MsgID

   //=========================================================
   //! Sets the MsgID with bound checking control
   //! \param MsgIDIn - Message ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetMsgID(const Cigi_uint16 MsgIDIn, bool bndchk=true)
   {
      MsgID = MsgIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the MsgID value.
   //! \return the current MsgID.
   Cigi_uint16 GetMsgID(void) const { return(MsgID); }


   //+> Msg

   //=========================================================
   //! Gets the Msg value.
   //! \return the current Msg.
   Cigi_int8 *GetMsg(void) { return(Msg); }


   //+> MsgLen

   //=========================================================
   //! Gets the Msg Length.
   //! \return the current MsgLen.
   int GetMsgLen(void) const { return(MsgLen); }



protected:

   //==> Member variables

   //=========================================================
   //! MsgID<br>
   //! The message ID
   //!
   Cigi_uint16 MsgID;

   //=========================================================
   //! Msg<br>
   //! The message string
   //!
   Cigi_int8 Msg[100];

   //=========================================================
   //! MsgLen<br>
   //! The message string length
   //!
   int MsgLen;


};

#endif // #if !defined(_CIGI_BASE_IG_MSG_INCLUDED_)
