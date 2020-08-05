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
 *  FILENAME:   CigiBaseSymbolLineDef.h
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

#if !defined(_CIGI_BASE_SYMBOL_LINE_DEF_INCLUDED_)
#define _CIGI_BASE_SYMBOL_LINE_DEF_INCLUDED_


#include "CigiBaseVariableSizePckt.h"
#include "CigiBaseVertexSymbolData.h"
#include <vector>


// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_SYMBOL_LINE_DEFINITION_PACKET_ID_V3_3 32
#define CIGI_SYMBOL_LINE_DEFINITION_PACKET_SIZE_V3_3 16


class CigiSymbolLineDefV3_3;


//=========================================================
//! The base class for all packets.
//!
class CIGI_SPEC CigiBaseSymbolLineDef :
   public CigiBaseVariableSizePckt
{

   friend class CigiSymbolLineDefV3_3;

public:


   //=========================================================
   //! The enumeration for the CigiBaseSymbolLineDef
   //!   Datum type Group
   //!
   enum PrimitiveGrp
   {
      Points=0,
      Lines=1,
      LineStrip=2,
      LineLoop=3,
      Triangles=4,
      TriangleStrip=5,
      TriangleFan=6
   };


   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseSymbolLineDef(void);

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseSymbolLineDef(void);

   //=========================================================
   //! Copy Constructor
   //!
   CigiBaseSymbolLineDef(const CigiBaseSymbolLineDef &BaseIn);

   //=========================================================
   //! Assignment operator
   //!
   CigiBaseSymbolLineDef & operator=(const CigiBaseSymbolLineDef &BaseIn);


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
	virtual int GetTruePacketSize(CigiBaseVariableSizePckt &refPacket)
   { return(PacketSize + refPacket.GetVariableDataSize()); }

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

   //+> SymbolID

   //=========================================================
   //! Sets the SymbolID with bound checking control
   //! \param SymbolIDIn - The ID number of this symbol.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetSymbolID(const Cigi_uint16 SymbolIDIn, bool bndchk=true)
   {
      SymbolID = SymbolIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the SymbolID with bound checking control
   //! \return The ID number of this Symbol.
   //!
   Cigi_uint16 GetSymbolID(void) const { return(SymbolID); }
   

   //+> Primitive

   //=========================================================
   //! Sets the Primitive with bound checking control
   //! \param PrimitiveIn - Specifies the primatives used in this symbol.
   //!   (Points, Lines, LineStrip, LineLoop, Triangles,
   //!   TriangleStrip, TriangleFan)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetPrimitive(const PrimitiveGrp PrimitiveIn, bool bndchk=true);

   //=========================================================
   //! Gets the Primitive with bound checking control
   //! \return The Primitive used in this symbol
   //!   (Points, Lines, LineStrip, LineLoop, Triangles,
   //!   TriangleStrip, TriangleFan)
   //!
   PrimitiveGrp GetPrimitive(void) const { return(Primitive); }


   //+> StipplePattern

   //=========================================================
   //! Sets the StipplePattern with bound checking control
   //! \param StipplePatternIn - Specifies the binary stipple pattern
   //!   to be applied to the lines.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetStipplePattern(const Cigi_uint16 StipplePatternIn, bool bndchk=true)
   {
      StipplePattern = StipplePatternIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the StipplePattern with bound checking control
   //! \return The binary Stipple Pattern to be applied to the lines.
   //!
   Cigi_uint16 GetStipplePattern(void) const { return(StipplePattern); }


   //+> LineWidth

   //=========================================================
   //! Sets the LineWidth with bound checking control
   //! \param LineWidthIn - The width in symbol surface units of
   //!   the lines.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLineWidth(const float LineWidthIn, bool bndchk=true);

   //=========================================================
   //! Gets the LineWidth with bound checking control
   //! \return The width in symbol surface units of
   //!   the lines.
   //!
   float GetLineWidth(void) const { return(LineWidth); }


   //+> StipplePatternLen

   //=========================================================
   //! Sets the StipplePatternLen with bound checking control
   //! \param StipplePatternLenIn - Specifies the length in symbol
   //!   surface units that a single copy of the stipple pattern occupies
   //!   in a line.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetStipplePatternLen(const float StipplePatternLenIn, bool bndchk=true);

   //=========================================================
   //! Gets the StipplePatternLen with bound checking control
   //! \return The length in symbol surface units that a single
   //!   copy of the stipple pattern occupies in a line.
   //!
   float GetStipplePatternLen(void) const { return(StipplePatternLen); }


   //+> Vertices

   //=========================================================
   //! Gets the number of vertices contained in this symbol
   //!   with bound checking control.
   //! \return The vertex count.
   //!
   int GetVertexCount(void);

   //=========================================================
   //! A pure virtual Create Vertex function.  
   //! This function is not implemented in this class.
   //! The implemented function will create a Vertex object and
   //!   insert a pointer into the Vertex Vector.
   //!
   //! \return The pointer to the new Vertex object.
   //!   If the Vertex exceeds the maximum number of Vertices
   //!   or the Vertex object cannot be created, this will
   //!   return a NULL pointer.
   virtual CigiBaseVertexSymbolData * AddVertex(void) =0;

   //=========================================================
   //! A pure virtual Remove Vertex function.  
   //! This function is not implemented in this class.
   //! The implemented function will remove a Vertex object
   //!   from the Vertex Vector.
   //! \param VertexIndex - The index of the vertex to be removed.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   virtual int RemoveVertex(int VertexIndex, bool bndchk=true) =0;

   //=========================================================
   //! Gets a pointer to a Vertex object.
   //! \param VertexIndex - The index of the desired vertex.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return The pointer to the desired Vertex object.
   //!   If the Vertex does not exist, the return is a NULL pointer.
   CigiBaseVertexSymbolData * GetVertex(int VertexIndex, bool bndchk=true);

   //=========================================================
   //! Removes all vertices from the symbol.
   //!
   void ClearVertices(void);



protected:

   //==> Member variables

   //=========================================================
   //! SymbolID<br>
   //! The ID number of the symbol.
   //!
   Cigi_uint16 SymbolID;

   //=========================================================
   //! Primitive<br>
   //! The drawing primitive used in this symbol.<br>
   //!   Points<br>
   //!   Lines<br>
   //!   LineStrip<br>
   //!   LineLoop<br>
   //!   Triangles<br>
   //!   TriangleStrip<br>
   //!   TriangleFan
   //!
   PrimitiveGrp Primitive;

   //=========================================================
   //! StipplePattern<br>
   //! The binary stipple pattern to be applied to the lines
   //!
   Cigi_uint16 StipplePattern;

   //=========================================================
   //! LineWidth<br>
   //! The width of the lines in symbol surface units
   //!
   float LineWidth;

   //=========================================================
   //! StipplePatternLen<br>
   //! The length in symbol surface units that a single copy of the
   //!   stipple pattern will occupy in the lines.
   //!
   float StipplePatternLen;

   //=========================================================
   //! Vertices<br>
   //! A dynamic array of vertices.
   //!
   std::vector<CigiBaseVertexSymbolData *> Vertices;


};



#endif   //  #if !defined(_CIGI_BASE_SYMBOL_LINE_DEF_INCLUDED_)

