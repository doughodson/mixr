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
 *  FILENAME:   CigiBaseSymbolCircleDef.h
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

#if !defined(_CIGI_BASE_SYMBOL_CIRCLE_DEF_INCLUDED_)
#define _CIGI_BASE_SYMBOL_CIRCLE_DEF_INCLUDED_


#include "CigiBaseVariableSizePckt.h"
#include "CigiBaseCircleSymbolData.h"
#include <vector>


// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_ID_V3_3 31
#define CIGI_SYMBOL_CIRCLE_DEFINITION_PACKET_SIZE_V3_3 16


class CigiSymbolCircleDefV3_3;


//=========================================================
//! The base class for all packets.
//!
class CIGI_SPEC CigiBaseSymbolCircleDef :
   public CigiBaseVariableSizePckt
{

   friend class CigiSymbolCircleDefV3_3;

public:


   //=========================================================
   //! The enumeration for the CigiBaseSymbolCircleDef
   //!   Datum type Group
   //!
   enum DrawingStyleGrp
   {
      Line=0,
      Fill=1
   };


   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseSymbolCircleDef(void);

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseSymbolCircleDef(void);

   //=========================================================
   //! Copy Constructor
   //!
   CigiBaseSymbolCircleDef(const CigiBaseSymbolCircleDef &BaseIn);

   //=========================================================
   //! Assignment operator
   //!
   CigiBaseSymbolCircleDef & operator=(const CigiBaseSymbolCircleDef &BaseIn);


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
   //! \param SymbolIDIn - Specifies the ID number for this symbol
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
   //! \return SymbolID
   //!
   Cigi_uint16 GetSymbolID(void) const { return(SymbolID); }
   

   //+> DrawingStyle

   //=========================================================
   //! Sets the DrawingStyle with bound checking control
   //! \param DrawingStyleIn - Specifies the drawing style for this
   //!   symbol (Line, Fill)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDrawingStyle(const DrawingStyleGrp DrawingStyleIn, bool bndchk=true);

   //=========================================================
   //! Gets the DrawingStyle with bound checking control
   //! \return DrawingStyle (Line, Fill)
   //!
   DrawingStyleGrp GetDrawingStyle(void) const { return(DrawingStyle); }


   //+> StipplePattern

   //=========================================================
   //! Sets the StipplePattern with bound checking control
   //! \param StipplePatternIn - Specifies the stipple pattern to be
   //!   used with this symbol.
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
   //! \return The StipplePattern to be used with this symbol.
   //!
   Cigi_uint16 GetStipplePattern(void) const { return(StipplePattern); }


   //+> LineWidth

   //=========================================================
   //! Sets the LineWidth with bound checking control
   //! \param LineWidthIn - Specifies the line width for this symbol
   //!   in Symbol Surface units.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLineWidth(const float LineWidthIn, bool bndchk=true);

   //=========================================================
   //! Gets the LineWidth with bound checking control
   //! \return LineWidth of this symbol in Symbol
   //!   Surface units.
   //!
   float GetLineWidth(void) const { return(LineWidth); }


   //+> StipplePatternLen

   //=========================================================
   //! Sets the StipplePatternLen with bound checking control
   //! \param StipplePatternLenIn - Specifies the length in symbol
   //!   surface units of the line that a single copy
   //!   of the stipple pattern occupies.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetStipplePatternLen(const float StipplePatternLenIn, bool bndchk=true);

   //=========================================================
   //! Gets the StipplePatternLen with bound checking control
   //! \return The length in symbol surface units of the line
   //!   that a single copy of the stipple pattern
   //!   occupies. (StipplePatternLen)
   //!
   float GetStipplePatternLen(void) const { return(StipplePatternLen); }


   //+> Circles

   //=========================================================
   //! Gets the StipplePatternLen with bound checking control
   //! \return Gets the number of circles contained in this symbol.
   //!
   int GetCircleCount(void);

   //=========================================================
   //! A pure virtual Create Circle function.  
   //! This function is not implemented in this class.
   //! The implemented function will create a circle object and
   //!   insert a pointer into the Circle Vector.
   //!
   //! \return The pointer to the new circle object.
   //!   If the circle exceeds the maximum number of circles
   //!   or the circle object cannot be created, this will
   //!   return a NULL pointer.
   virtual CigiBaseCircleSymbolData * AddCircle(void) =0;

   //=========================================================
   //! A pure virtual Remove Circle function.  
   //! This function is not implemented in this class.
   //! The implemented function will remove a Circle object
   //!   from the Circle Vector.
   //! \param CircleIndex - The index of the circle to delete from this
   //!   symbol.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   virtual int RemoveCircle(int CircleIndex, bool bndchk=true) =0;

   //=========================================================
   //! Gets a pointer to a circle object.
   //! \param CircleIndex - The index of the desired circle.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return The pointer to the desired circle object.
   //!   If the circle does not exist, the return is a NULL pointer.
   CigiBaseCircleSymbolData * GetCircle(int CircleIndex, bool bndchk=true);

   //=========================================================
   //! Removes all the circles in this symbol.
   //!
   void ClearCircles(void);



protected:

   //==> Member variables

   //=========================================================
   //! SymbolID<br>
   //! The ID number of this symbol.
   //!
   Cigi_uint16 SymbolID;

   //=========================================================
   //! DrawingStyle<br>
   //! The drawing style of this symbol.<br>
   //!   Line<br>
   //!   Fill
   //!
   DrawingStyleGrp DrawingStyle;

   //=========================================================
   //! StipplePattern<br>
   //! The binary Stipple Pattern to use on this symbol.
   //!
   Cigi_uint16 StipplePattern;

   //=========================================================
   //! LineWidth<br>
   //! The width of the line used in this symbol in symbol surface units.
   //!
   float LineWidth;

   //=========================================================
   //! StipplePatternLen<br>
   //! The length in symbol surface units along the line that a single
   //!   copy of the stipple pattern will occupy.
   //!
   float StipplePatternLen;

   //=========================================================
   //! Circles<br>
   //! A dynamic array of circles in this symbol
   //!
   std::vector<CigiBaseCircleSymbolData *> Circles;


};



#endif   //  #if !defined(_CIGI_BASE_SYMBOL_CIRCLE_DEF_INCLUDED_)

