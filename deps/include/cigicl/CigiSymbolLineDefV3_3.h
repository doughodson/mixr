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
 *  FILENAME:   CigiSymbolLineDefV3_3.h
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
 *  03/11/2008 Greg Basler                       CIGI_SYM_1
 *  Initial Release.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_SYMBOL_LINE_DEF_V3_3_INCLUDED_)
#define _CIGI_SYMBOL_LINE_DEF_V3_3_INCLUDED_


#include "CigiBaseSymbolLineDef.h"
#include "CigiVertexSymbolDataV3_3.h"



//=========================================================
//! The base class for all packets.
//!
class CIGI_SPEC CigiSymbolLineDefV3_3 :
   public CigiBaseSymbolLineDef
{
public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiSymbolLineDefV3_3(void);

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiSymbolLineDefV3_3(void);

   //=========================================================
   //! Copy Constructor
   //!
   CigiSymbolLineDefV3_3(const CigiSymbolLineDefV3_3 &BaseIn);

   //=========================================================
   //! Assignment operator
   //!
   CigiSymbolLineDefV3_3 & operator=(const CigiSymbolLineDefV3_3 &BaseIn);


   //==> Buffer Packing/Unpacking

   //=========================================================
   //! A virtual Pack function.  
   //! \param Base - A pointer to the instance of the packet 
   //!          to be packed. (Downcast to CigiBasePacket)
   //! \param Buff - A pointer to the current pack point.
   //! \param Spec - A pointer to special data.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Pack(CigiBasePacket * Base, Cigi_uint8 * Buff, void *Spec) const;

   //=========================================================
   //! A virtual Unpack function.
   //! \param Buff - A pointer to the current pack point.
   //! \param Swap - N/A for V1 & V2
   //! \param Spec - A pointer to special data.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Unpack(Cigi_uint8 * Buff, bool Swap, void *Spec);


   //+> Vertices

   //=========================================================
   //! A virtual Create Vertex function.  
   //! This will create a Vertex object and insert a pointer to
   //!   that created vertex into the Vertex Vector.
   //!
   //! \return The pointer to the new Vertex object.
   //!   If the Vertex exceeds the maximum number of Vertices
   //!   or the Vertex object cannot be created, this will
   //!   return a NULL pointer.
   virtual CigiBaseVertexSymbolData * AddVertex(void);

   //=========================================================
   //! A virtual Remove Vertex function.  
   //! This will remove a Vertex object from the Vertex Vector.
   //! \param VertexIndex - Specifies the index of the desired vertex
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   virtual int RemoveVertex(int VertexIndex, bool bndchk=true);


};



#endif   //  #if !defined(_CIGI_SYMBOL_LINE_DEF_V3_3_INCLUDED_)

