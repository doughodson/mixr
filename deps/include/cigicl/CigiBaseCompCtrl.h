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
 *  FILENAME:   CigiBaseCompCtrl.h
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
 *  01/21/2005 Greg Basler                       Version 1.5
 *  Changed the enumerated values in the BytePos and HalfWordPos enumeations.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Moved Packet information to base packet.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_BASE_COMP_CTRL_INCLUDED_)
#define _CIGI_BASE_COMP_CTRL_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_COMP_CTRL_PACKET_ID_V1 3
#define CIGI_COMP_CTRL_PACKET_SIZE_V1 20

#define CIGI_COMP_CTRL_PACKET_ID_V2 3
#define CIGI_COMP_CTRL_PACKET_SIZE_V2 20

#define CIGI_COMP_CTRL_PACKET_ID_V3 4
#define CIGI_COMP_CTRL_PACKET_SIZE_V3 32

#define CIGI_SHORT_COMP_CTRL_PACKET_ID_V3 5
#define CIGI_SHORT_COMP_CTRL_PACKET_SIZE_V3 16

#define CIGI_COMP_CTRL_PACKET_ID_V3_3 4
#define CIGI_COMP_CTRL_PACKET_SIZE_V3_3 32

#define CIGI_SHORT_COMP_CTRL_PACKET_ID_V3_3 5
#define CIGI_SHORT_COMP_CTRL_PACKET_SIZE_V3_3 16



class CigiCompCtrlV1;
class CigiCompCtrlV2;
class CigiCompCtrlV3;
class CigiShortCompCtrlV3;
class CigiCompCtrlV3_3;
class CigiShortCompCtrlV3_3;


class CIGI_SPEC CigiBaseCompCtrl : public CigiBasePacket
{

friend class CigiCompCtrlV1;
friend class CigiCompCtrlV2;
friend class CigiCompCtrlV3;
friend class CigiShortCompCtrlV3;
friend class CigiCompCtrlV3_3;
friend class CigiShortCompCtrlV3_3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseCompCtrl Group
   //!
   enum CompAssocGrp
   {
      NoCnvtV1=-1,
      Entity=0,
      Environment=1,
      View=2
   };

   //=========================================================
   //! The enumeration for the CigiBaseCompCtrl Group
   //!
   enum CompClassV2Grp
   {
      NoCnvtV2=-1,
      EntityV2=0,
      EnvironmentV2=1,
      ViewV2=2,
      ViewGrpV2=3,
      SensorV2=4,
      SystemV2=5
   };

   //=========================================================
   //! The enumeration for the CigiBaseCompCtrl Group
   //!
   enum CompClassV3Grp
   {
      NoCnvtV3=-1,
      EntityV3=0,
      ViewV3=1,
      ViewGrpV3=2,
      SensorV3=3,
      RegionalSeaSurfaceV3=4,
      RegionalTerrainSurfaceV3=5,
      RegionalLayeredWeatherV3=6,
      GlobalSeaSurfaceV3=7,
      GlobalTerrainSurfaceV3=8,
      GlobalLayeredWeatherV3=9,
      AtmosphereV3=10,
      CelestialSphereV3=11,
      EventV3=12,
      SystemV3=13,
      SymbolSurfaceV3_3=14,
      SymbolV3_3=15
   };

   //=========================================================
   //! The enumeration for the Byte Position
   //!
   enum BytePos
   {
      MSB=3,   // Most significant byte
      Ord2=2,  // 2nd order byte
      Ord3=1,  // 3rd order byte
      LSB=0    // Least significant byte
   };

   //=========================================================
   //! The enumeration for the Byte Position
   //!
   enum HalfWordPos
   {
      MSHW=1,  // Most significant half word
      LSHW=0   // least significant half word
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseCompCtrl() { };

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseCompCtrl() { };


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
                       CigiCnvtInfoType::Type &CnvtInfo) =0;



   //==> Accessing Member Variable Values functions

   //+> CompID

   //=========================================================
   //! Sets the CompID with bound checking control
   //! \param CompIDIn - Component Control ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCompID(const Cigi_uint16 CompIDIn, bool bndchk=true)
   {
      CompID = CompIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the CompID value.
   //! \return the current CompID.
   Cigi_uint16 GetCompID(void) const { return(CompID); }



protected:

   //==> Member variables

   //=========================================================
   //! CompID<br>
   //! Component Control ID
   //!
   Cigi_uint16 CompID;

   //=========================================================
   //! InstanceID<br>
   //! Instance ID
   //!
   Cigi_uint16 InstanceID;

   //=========================================================
   //! EntityID<br>
   //! Entity ID
   //!
   Cigi_uint16 EntityID;

   //=========================================================
   //! ViewID<br>
   //! View ID
   //!
   Cigi_uint8 ViewID;

   //=========================================================
   //! CompState<br>
   //! Component State Data
   //!
   Cigi_uint16 CompState;

   //=========================================================
   //! CompAssoc<br>
   //! Component Control Association<br>
   //!  0-Entity<br>
   //!  1-Environment<br>
   //!  2-View
   //!
   //!
   CompAssocGrp CompAssoc;

   //=========================================================
   //! CompClassV2<br>
   //! Component Control Class/Association for Cigi V2<br>
   //!  0-EntityV2<br>
   //!  1-EnvironmentV2<br>
   //!  2-ViewV2<br>
   //!  3-ViewGrpV2<br>
   //!  4-SensorV2<br>
   //!  5-SystemV2
   //!
   //!
   CompClassV2Grp CompClassV2;

   //=========================================================
   //! CompClassV3<br>
   //! Component Control Class/Association for Cigi V3<br>
   //!  0-EntityV3<br>
   //!  1-ViewV3<br>
   //!  2-ViewGrpV3<br>
   //!  3-SensorV3<br>
   //!  4-RegionalSeaSurfaceV3<br>
   //!  5-RegionalTerrainSurfaceV3<br>
   //!  6-RegionalLayeredWeatherV3<br>
   //!  7-GlobalSeaSurfaceV3<br>
   //!  8-GlobalTerrainSurfaceV3<br>
   //!  9-GlobalLayeredWeatherV3<br>
   //!  10-AtmosphereV3<br>
   //!  11-CelestialSphereV3<br>
   //!  12-EventV3<br>
   //!  13-SystemV3
   //!  14-SymbolSurfaceV3_3<br>
   //!  15-SymbolV3_3
   //!
   CompClassV3Grp CompClassV3;

   //=========================================================
   //! CompData[6]<br>
   //! Component Data
   //!
   Cigi_uint32 CompData[6];


};

#endif // #if !defined(_CIGI_BASE_COMP_CTRL_INCLUDED_)
