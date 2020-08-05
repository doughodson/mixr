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
 *  FILENAME:   CigiBaseVariableSizePckt.h
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
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Initial Release.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_BASE_VARIABLE_SIZE_PACKET_INCLUDED_)
#define _CIGI_BASE_VARIABLE_SIZE_PACKET_INCLUDED_


#include "CigiBasePacket.h"



//=========================================================
//! The base class for all packets.
//!
class CIGI_SPEC CigiBaseVariableSizePckt : public CigiBasePacket  
{
public:

   //=========================================================
   //! General Constructor
   //!
   CigiBaseVariableSizePckt() { };

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseVariableSizePckt() { };

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
   //! A pure virtual function to determine the size that the
   //!  packet will take up when packed.
   //! This function is not implemented in this class.
   //! \param refPacket - A pointer to the current pack point.
   //!
   //! \return The size that the packet will take up when packed
   //!
	virtual int GetTruePacketSize(CigiBaseVariableSizePckt &refPacket) =0;

   //=========================================================
   //! An accessor to the size of the variable data.
   //!
   //! \return The size of the variable data
   //!
	int GetVariableDataSize(void) { return(VariableDataSize); }


protected:

   //=========================================================
   //! The current size of the variable data
   //!
	int VariableDataSize;

};

#endif // !defined(_CIGI_BASE_VARIABLE_SIZE_PACKET_INCLUDED_)
