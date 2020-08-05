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
 *  FILENAME:   CigiBaseEarthModelDef.h
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
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 *  
 *  11/20/2007 Greg Basler                       Version 2.0.0
 *  Added new version conversion method.
 *  Moved Packet information to base packet.
 *  
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_BASE_EARTH_MODEL_DEF_INCLUDED_)
#define _CIGI_BASE_EARTH_MODEL_DEF_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_EARTH_MODEL_DEF_PACKET_ID_V3 19
#define CIGI_EARTH_MODEL_DEF_PACKET_SIZE_V3 24


class CigiEarthModelDefV3;


class CIGI_SPEC CigiBaseEarthModelDef : public CigiBasePacket
{

friend class CigiEarthModelDefV3;

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseEarthModelDef() { };

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseEarthModelDef() { };


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
                       CigiCnvtInfoType::Type &CnvtInfo)
   {
      if(CnvtVersion.CigiMajorVersion < 3)
      {
         CnvtInfo.ProcID = CigiProcessType::ProcNone;
         CnvtInfo.CnvtPacketID = 0;
      }
      else
      {
         CnvtInfo.ProcID = CigiProcessType::ProcStd;
         CnvtInfo.CnvtPacketID = CIGI_EARTH_MODEL_DEF_PACKET_ID_V3;
      }

      return(CIGI_SUCCESS);
   }



   //==> Accessing Member Variable Values functions

   //+> CustomERMEn

   //=========================================================
   //! Sets the CustomERMEn with bound checking control
   //! \param CustomERMEnIn - Custom Earth Referance Model Enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetCustomERMEn(const bool CustomERMEnIn, bool bndchk=true)
   {
      CustomERMEn = CustomERMEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the CustomERMEn value.
   //! \return the current CustomERMEn.
   bool GetCustomERMEn(void) const { return(CustomERMEn); }


   //+> EquatorialRadius

   //=========================================================
   //! Sets the EquatorialRadius with bound checking control
   //! \param EquatorialRadiusIn - Equatorial Radius
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEquatorialRadius(const double EquatorialRadiusIn, bool bndchk=true)
   {
      EquatorialRadius = EquatorialRadiusIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EquatorialRadius value.
   //! \return the current EquatorialRadius.
   double GetEquatorialRadius(void) const { return(EquatorialRadius); }


   //+> Flattening

   //=========================================================
   //! Sets the Flattening with bound checking control
   //! \param FlatteningIn - Flattening
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetFlattening(const double FlatteningIn, bool bndchk=true)
   {
      Flattening = FlatteningIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the Flattening value.
   //! \return the current Flattening.
   double GetFlattening(void) const { return(Flattening); }



protected:

   //==> Member variables

   //=========================================================
   //! CustomERMEn<br>
   //! Custom Earth Referance Model Enable
   //!
   bool CustomERMEn;

   //=========================================================
   //! EquatorialRadius<br>
   //! Equatorial Radius
   //!
   double EquatorialRadius;

   //=========================================================
   //! Flattening<br>
   //! Adjustment parameter that specifies the ellipsiod shape.
   //!
   double Flattening;


};

#endif // #if !defined(_CIGI_BASE_EARTH_MODEL_DEF_INCLUDED_)
