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
 *  FILENAME:   CigiBasePacket.h
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
 *  Changed the type of the Version member variable from int to Cigi_uint8.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the MinorVersion member variable.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Added the GetMinorVersion method for accessing the minor version.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_BASE_PACKET_INCLUDED_)
#define _CIGI_BASE_PACKET_INCLUDED_


#include "CigiTypes.h"
#include "CigiErrorCodes.h"
#include "CigiCnvtInfoType.h"
#include "CigiVersionID.h"



//=========================================================
//! The base class for all packets.
//!
class CIGI_SPEC CigiBasePacket  
{
public:

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBasePacket() { };

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
   //!  packet.  Most packets will have there own GetCnvt member
   //!  function.  User Defined packets usually do not need their
   //!  own GetCnvt member function and can use this general one.
   //! \param CnvtVersion - The CIGI version to which this packet
   //!    is being converted.
   //! \param CnvtInfo - The information needed for conversion
   //!    
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
	virtual int GetCnvt(CigiVersionID &CnvtVersion,
                       CigiCnvtInfoType::Type &CnvtInfo)
   {
      CnvtInfo.ProcID = CigiProcessType::ProcStd;
      CnvtInfo.CnvtPacketID = PacketID;

      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the packet id.
   //! \return The packet id.
   //!
   Cigi_uint8 GetPacketID(void) const { return(PacketID); }

   //=========================================================
   //! Gets the size of the packet.
   //! \return The size in bytes of the packet.
   //!
   Cigi_uint8 GetPacketSize(void) const { return(PacketSize); }

   //=========================================================
   //! Gets the CIGI version of this packet.
   //! \return The CIGI version of this packet.
   //!
   Cigi_uint8 GetVersion(void) const { return(Version); }

   //=========================================================
   //! Gets the CIGI minor version of this packet.
   //! \return The CIGI minor version of this packet.
   //!
   Cigi_uint8 GetMinorVersion(void) const { return(MinorVersion); }


protected:

   //=========================================================
   //! Packet ID or Opcode
   //!
	Cigi_uint8 PacketID;

   //=========================================================
   //! Packet size in bytes
   //!
	Cigi_uint8 PacketSize;

   //=========================================================
   //! CIGI version of this packet
   //!
	Cigi_uint8 Version;

   //=========================================================
   //! CIGI minor version of this packet
   //!
	Cigi_uint8 MinorVersion;

};

#endif // !defined(_CIGI_BASE_PACKET_INCLUDED_)
