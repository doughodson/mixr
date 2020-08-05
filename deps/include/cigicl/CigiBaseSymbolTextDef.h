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
 *  FILENAME:   CigiBaseSymbolTextDef.h
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

#if !defined(_CIGI_BASE_SYMBOL_TEXT_DEF_INCLUDED_)
#define _CIGI_BASE_SYMBOL_TEXT_DEF_INCLUDED_


#include "CigiBaseVariableSizePckt.h"

#include <string>

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_SYMBOL_TEXT_DEFINITION_PACKET_ID_V3_3 30
#define CIGI_SYMBOL_TEXT_DEFINITION_PACKET_SIZE_V3_3 12


class CigiSymbolTextDefV3_3;


//=========================================================
//! The base class for all packets.
//!
class CIGI_SPEC CigiBaseSymbolTextDef :
   public CigiBaseVariableSizePckt
{

   friend class CigiSymbolTextDefV3_3;

public:


   //=========================================================
   //! The enumeration for the CigiBaseSymbolTextDef
   //!   Datum type Group
   //!
   enum AlignmentGrp
   {
      TopLeft=0,
      TopCenter=1,
      TopRight=2,
      CenterLeft=3,
      Center=4,
      CenterRight=5,
      BottomLeft=6,
      BottomCenter=7,
      BottomRight=8,
   };

   //=========================================================
   //! The enumeration for the CigiBaseSymbolTextDef
   //!   Datum type Group
   //!
   enum OrientationGrp
   {
      LeftToRight=0,
      TopToBottom=1,
      RightToLeft=2,
      BottomToTop=3,
   };


   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseSymbolTextDef(void);

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseSymbolTextDef(void);

   //=========================================================
   //! Copy Constructor
   //!
   CigiBaseSymbolTextDef(const CigiBaseSymbolTextDef &BaseIn);

   //=========================================================
   //! Assignment operator
   //!
   CigiBaseSymbolTextDef & operator=(const CigiBaseSymbolTextDef &BaseIn);


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
   //! A virtual function to determine the size that the
   //!  packet will take up when packed.
   //! \param refPacket - A pointer to the packet being checked.
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
   //!    including the type of conversion and the packet id.
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
   //! \param SymbolIDIn - The user assigned Symbol ID
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
   //! \return The ID of this Symbol.
   //!
   Cigi_uint16 GetSymbolID(void) const { return(SymbolID); }
   

   //+> Alignment

   //=========================================================
   //! Sets the Alignment with bound checking control
   //! \param AlignmentIn - Specifies the type of text alignment
   //!    to be used for this text. (TopLeft, TopCenter, TopRight,
   //!   CenterLeft, Center, CenterRight, BottomLeft, BottomCenter,
   //!   BottomRight)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAlignment(const AlignmentGrp AlignmentIn, bool bndchk=true);

   //=========================================================
   //! Gets the Alignment with bound checking control
   //! \return The text alignment to be used for this text.
   //!   (TopLeft, TopCenter, TopRight, CenterLeft, Center,
   //!   CenterRight, BottomLeft, BottomCenter, BottomRight)
   //!
   AlignmentGrp GetAlignment(void) const { return(Alignment); }


   //+> Orientation

   //=========================================================
   //! Sets the Orientation with bound checking control
   //! \param OrientationIn - Specifies the text orientation
   //!    to be used for this text.  (LeftToRight, TopToBottom,
   //!   RightToLeft, BottomToTop)
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetOrientation(const OrientationGrp OrientationIn, bool bndchk=true);

   //=========================================================
   //! Gets the Orientation with bound checking control
   //! \return The text orientation to be used for this text.
   //!   (LeftToRight, TopToBottom, RightToLeft, BottomToTop)
   //!
   OrientationGrp GetOrientation(void) const { return(Orientation); }


   //+> FontID

   //=========================================================
   //! Sets the FontID with bound checking control
   //! \param FontIDIn - Specifies the font (by ID number) to
   //!    be used for this text.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFontID(const Cigi_uint8 FontIDIn, bool bndchk=true)
   {
      FontID = FontIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the FontID with bound checking control
   //! \return The ID of the font to be used for this text.
   //!
   Cigi_uint8 GetFontID(void) const { return(FontID); }


   //+> FontSize

   //=========================================================
   //! Sets the FontSize with bound checking control
   //! \param FontSizeIn - Specifies the size of the font
   //!    in UV units to be used for this text.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFontSize(const float FontSizeIn, bool bndchk=true);

   //=========================================================
   //! Gets the FontSize with bound checking control
   //! \return The size of the font to be used in
   //!
   float GetFontSize(void) const { return(FontSize); }


   //+> Text

   //=========================================================
   //! Sets the Text with bound checking control
   //! \param TextIn - A standard string of text.
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetText(const std::string &TextIn, bool bndchk=true);

   //=========================================================
   //! Gets the TextSize with bound checking control
   //! \return The current size of the text.
   //!
   int GetTextSize(void) const { return(VariableDataSize); }

   //=========================================================
   //! Gets the Text
   //! \return The current symbology text
   const std::string & GetText(void) { return(Text); }



protected:

   //==> Member variables

   //=========================================================
   //! SymbolID<br>
   //! The Id for this symbol
   //!
   Cigi_uint16 SymbolID;

   //=========================================================
   //! SymbolState<br>
   //! The alignment of this symbol.<br>
   //! This is the position of the anchor or reference point.<br>
   //!   TopLeft<br>
   //!   TopCenter<br>
   //!   TopRight<br>
   //!   CenterLeft<br>
   //!   Center<br>
   //!   CenterRight<br>
   //!   BottomLeft<br>
   //!   BottomCenter<br>
   //!   BottomRight<br>
   //!
   AlignmentGrp Alignment;

   //=========================================================
   //! Orientation<br>
   //! The orientation of the text.<br>
   //!   LeftToRight<br>
   //!   TopToBottom<br>
   //!   RightToLeft<br>
   //!   BottomToTop<br>
   //!
   OrientationGrp Orientation;

   //=========================================================
   //! FontID<br>
   //! The font id number.
   //!
   Cigi_uint8 FontID;

   //=========================================================
   //! FontSize<br>
   //! The size of the font in UV coordinates
   //!
   float FontSize;

   //=========================================================
   //! Text<br>
   //! The symbol text
   //!
   std::string Text;

   //=========================================================
   //! MaxCharCnt<br>
   //! The maximum number of bytes not including NULLs
   //!
   static const int MaxCharCnt = 235;

};



#endif   //  #if !defined(_CIGI_BASE_SYMBOL_TEXT_DEF_INCLUDED_)

