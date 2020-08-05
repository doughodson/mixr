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
 *  FILENAME:   CigiCompCtrlV3.h
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
 *  
 *  05/15/2008 Greg Basler                       Version 2.2.0
 *  Changed the Component class Conversion table sizing to a unified
 *   constant.
 *  
 *  04/22/2009 Greg Basler                       Version 3.3.1
 *  Corrected a problem with component state.  In CIGI V1 and V2
 *  the component state is Cigi_uint16.  In Cigi V3.0 and above
 *  the component state is Cigi_uint8.  Converting between these
 *  can and probably will cause problems.  In addition, the
 *  accessors should reflect the correct size of the value per
 *  the ICD
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_COMP_CTRL_V3_INCLUDED_)
#define _CIGI_COMP_CTRL_V3_INCLUDED_

#include "CigiBaseCompCtrl.h"



class CIGI_SPEC CigiCompCtrlV3 : public CigiBaseCompCtrl
{

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiCompCtrlV3();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiCompCtrlV3();


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

   //+> InstanceID

   //=========================================================
   //! Sets the InstanceID with bound checking control
   //! \param InstanceIDIn - 
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetInstanceID(const Cigi_uint16 InstanceIDIn, bool bndchk=true)
   {
      InstanceID = InstanceIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the InstanceID value.
   //! \return the current InstanceID.
   Cigi_uint16 GetInstanceID(void) const { return(InstanceID); }


   //+> CompState

   //=========================================================
   //! Sets the CompState with bound checking control
   //! \param CompStateIn - Component state value
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompState(const Cigi_uint8 CompStateIn, bool bndchk=true)
   {
      CompState = (Cigi_uint16) CompStateIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the CompState value.
   //! \return the current CompState.
   Cigi_uint8 GetCompState(void) const
   {
      Cigi_uint8 V3State = (Cigi_uint8)((CompState > 0x00ff) ? 0xff : CompState);
      return(V3State);
   }


   //+> CompClassV3

   //=========================================================
   //! Sets the CompClassV3 with bound checking control
   //! \param CompClassV3In - <br>
   //!   EntityV3=0<br>
   //!   ViewV3=1<br>
   //!   ViewGrpV3=2<br>
   //!   SensorV3=3<br>
   //!   RegionalSeaSurfaceV3=4<br>
   //!   RegionalTerrainSurfaceV3=5<br>
   //!   RegionalLayeredWeatherV3=6<br>
   //!   GlobalSeaSurfaceV3=7<br>
   //!   GlobalTerrainSurfaceV3=8<br>
   //!   GlobalLayeredWeatherV3=9<br>
   //!   AtmosphereV3=10<br>
   //!   CelestialSphereV3=11<br>
   //!   EventV3=12<br>
   //!   SystemV3=13<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompClassV3(const CompClassV3Grp CompClassV3In, bool bndchk=true);

   //=========================================================
   //! Gets the CompClassV3 value.
   //! \return the current CompClassV3.
   CompClassV3Grp GetCompClassV3(void) const { return(CompClassV3); }


   //+> CompData

   //=========================================================
   //! Sets the specified CompData with bound checking control
   //! \param CompDataIn - The byte data
   //! \param Word - The word position
   //! \param Pos - The byte position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompData(const Cigi_uint8 CompDataIn,
                       const unsigned int Word,
                       const BytePos Pos,
                       bool bndchk=true);

   //=========================================================
   //! Sets the specified CompData with bound checking control
   //! \param CompDataIn - The byte data
   //! \param Word - The word position
   //! \param Pos - The byte position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompData(const Cigi_int8 CompDataIn,
                       const unsigned int Word,
                       const BytePos Pos,
                       bool bndchk=true);

   //=========================================================
   //! Sets the specified CompData with bound checking control
   //! \param CompDataIn - The byte data
   //! \param Word - The word position
   //! \param Pos - The half word position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompData(const Cigi_uint16 CompDataIn,
                       const unsigned int Word,
                       const HalfWordPos Pos,
                       bool bndchk=true);

   //=========================================================
   //! Sets the specified CompData with bound checking control
   //! \param CompDataIn - The byte data
   //! \param Word - The word position
   //! \param Pos - The half word position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompData(const Cigi_int16 CompDataIn,
                       const unsigned int Word,
                       const HalfWordPos Pos,
                       bool bndchk=true);

   //=========================================================
   //! Sets the specified CompData with bound checking control
   //! \param CompDataIn - The byte data
   //! \param Word - The word position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompData(const Cigi_uint32 CompDataIn,
                   const unsigned int Word,
                   bool bndchk=true);

   //=========================================================
   //! Sets the specified CompData with bound checking control
   //! \param CompDataIn - The byte data
   //! \param Word - The word position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompData(const Cigi_int32 CompDataIn,
                   const unsigned int Word,
                   bool bndchk=true);

   //=========================================================
   //! Sets the specified CompData with bound checking control
   //! \param CompDataIn - The byte data
   //! \param Word - The word position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompData(const float CompDataIn,
                   const unsigned int Word,
                   bool bndchk=true);

   //=========================================================
   //! Sets the specified CompData with bound checking control
   //! \param CompDataIn - The byte data
   //! \param Pos - The double word position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompData(const Cigi_uint64 CompDataIn,
                   const unsigned int Pos,
                   bool bndchk=true);

   //=========================================================
   //! Sets the specified CompData with bound checking control
   //! \param CompDataIn - The byte data
   //! \param Pos - The double word position
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompData(const double CompDataIn,
                   const unsigned int Pos,
                   bool bndchk=true);


   //=========================================================
   //! Gets the specified CompData
   //! \param Word - The word position
   //! \param Pos - The byte position
   //!
   //! \return This returns the specified Cigi_uint8 value.
   Cigi_uint8 GetUCharCompData(const unsigned int Word,
                                  const BytePos Pos);

   //=========================================================
   //! Gets the specified CompData
   //! \param Word - The word position
   //! \param Pos - The byte position
   //!
   //! \return This returns the specified Cigi_int8 value.
   Cigi_int8 GetCharCompData(const unsigned int Word,
                        const BytePos Pos);

   //=========================================================
   //! Gets the specified CompData
   //! \param Word - The word position
   //! \param Pos - The half word position
   //!
   //! \return This returns the specified Cigi_uint16 value.
   Cigi_uint16 GetUShortCompData(const unsigned int Word,
                                    const HalfWordPos Pos);

   //=========================================================
   //! Gets the specified CompData
   //! \param Word - The word position
   //! \param Pos - The half word position
   //!
   //! \return This returns the specified Cigi_int16 value.
   Cigi_int16 GetShortCompData(const unsigned int Word,
                          const HalfWordPos Pos);

   //=========================================================
   //! Gets the specified CompData
   //! \param Word - The word position
   //!
   //! \return This returns the specified unsigned Long value.
   Cigi_uint32 GetULongCompData(const unsigned int Word);

   //=========================================================
   //! Gets the specified CompData
   //! \param Word - The word position
   //!
   //! \return This returns the specified Long value.
   Cigi_int32 GetLongCompData(const unsigned int Word);

   //=========================================================
   //! Gets the specified CompData
   //! \param Word - The word position
   //!
   //! \return This returns the specified float value.
   float GetFloatCompData(const unsigned int Word);

   //=========================================================
   //! Gets the specified CompData
   //! \param Pos - The double word position
   //!
   //! \return This returns the specified Int64 value.
   Cigi_uint64 GetI64CompData(const unsigned int Pos);

   //=========================================================
   //! Gets the specified CompData
   //! \param Pos - The double word position
   //!
   //! \return This returns the specified double value.
   double GetDoubleCompData(const unsigned int Pos);




protected:

   //==> Conversion Tables
   static const int CompClassCnvtSz = 14;
   static const CompAssocGrp CompClassV3xV1[CompClassCnvtSz];
   static const CompClassV2Grp CompClassV3xV2[CompClassCnvtSz];


};

#endif // #if !defined(_CIGI_COMP_CTRL_V3_INCLUDED_)
