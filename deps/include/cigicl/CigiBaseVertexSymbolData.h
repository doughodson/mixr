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
 *  FILENAME:   CigiBaseVertexSymbolData.h
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
 * </pre>
 *  Author: The Boeing Company
 *
 */

#if !defined(_CIGI_BASE_VERTEX_SYMBOL_DATA_INCLUDED_)
#define _CIGI_BASE_VERTEX_SYMBOL_DATA_INCLUDED_


#include "CigiTypes.h"
#include "CigiErrorCodes.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_VERTEX_SYMBOL_DATA_SIZE_V3_3 8


class CigiVertexSymbolDataV3_3;

//=========================================================
//! The base class for all packets.
//!
class CIGI_SPEC CigiBaseVertexSymbolData
{

   friend class CigiVertexSymbolDataV3_3;

public:


   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseVertexSymbolData(void) {};

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseVertexSymbolData(void) {};


   //==> Buffer Packing/Unpacking

   //=========================================================
   //! A pure virtual Pack function.  
   //! This function is not implemented in this class.
   //! \param Base - A pointer to the instance of the Circle 
   //!          Data to be packed. (Downcast to CigiBaseVertexSymbolData)
   //! \param Buff - A pointer to the current pack point.
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Pack(CigiBaseVertexSymbolData * Base, Cigi_uint8 * Buff) const=0;

   //=========================================================
   //! A pure virtual Unpack function.
   //! This function is not implemented in this class.
   //! \param Buff - A pointer to the current pack point.
   //! \param Swap - N/A for V1 & V2
   //!
   //! \return This returns CIGI_SUCCESS or an error code 
   //!   defined in CigiErrorCodes.h
   //!
   virtual int Unpack(Cigi_uint8 * Buff, bool Swap) =0;


   //==> Accessing Member Variable Values functions


   //+> Vertices

   //=========================================================
   //! Sets both elements of the Vertex with bound checking control
   //! \param VertexUIn - The Vertex's U position
   //! \param VertexVIn - The Vertex's V position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVertex(const float VertexUIn, const float VertexVIn, bool bndchk=true)
   {
      VertexU = VertexUIn;
      VertexV = VertexVIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets both elements of the Vertex
   //! \param VertexUIn - A reference to receive the
   //!   Vertex's U position
   //! \param VertexVIn - A reference to receive the
   //!   Vertex's V position
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int GetVertex(float &VertexUIn, float &VertexVIn)
   {
      VertexUIn = VertexU;
      VertexVIn = VertexV;
      return(CIGI_SUCCESS);
   }


   //+> VertexU

   //=========================================================
   //! Sets the VertexU with bound checking control
   //! \param VertexUIn - The Vertex's U position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVertexU(const float VertexUIn, bool bndchk=true)
   {
      VertexU = VertexUIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the VertexU with bound checking control
   //! \return VertexU
   //!
   float GetVertexU(void) const { return(VertexU); }


   //+> VertexV

   //=========================================================
   //! Sets the VertexV with bound checking control
   //! \param VertexVIn - The Vertex's V position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetVertexV(const float VertexVIn, bool bndchk=true)
   {
      VertexV = VertexVIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the VertexV with bound checking control
   //! \return VertexV
   //!
   float GetVertexV(void) const { return(VertexV); }



protected:

   //==> Member variables

   //=========================================================
   //! VertexU<br>
   //! The Vertex's U position
   //!
   float VertexU;

   //=========================================================
   //! VertexV<br>
   //! The Vertex's V position
   //!
   float VertexV;


};



#endif   //  #if !defined(_CIGI_BASE_VERTEX_SYMBOL_DATA_INCLUDED_)

