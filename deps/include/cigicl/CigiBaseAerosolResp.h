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
 *  FILENAME:   CigiBaseAerosolResp.h
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


#if !defined(_CIGI_BASE_AEROSOL_RESP_INCLUDED_)
#define _CIGI_BASE_AEROSOL_RESP_INCLUDED_

#include "CigiBasePacket.h"


// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_AEROSOL_RESP_PACKET_ID_V3 110
#define CIGI_AEROSOL_RESP_PACKET_SIZE_V3 8


class CigiAerosolRespV3;


class CIGI_SPEC CigiBaseAerosolResp : public CigiBasePacket
{

friend class CigiAerosolRespV3;

public:

   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseAerosolResp();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseAerosolResp();


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
                       CigiCnvtInfoType::Type &CnvtInfo);



   //==> Accessing Member Variable Values functions

   //+> RequestID

   //=========================================================
   //! Sets the RequestID with bound checking control
   //! \param RequestIDIn - Request ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetRequestID(const Cigi_uint8 RequestIDIn, bool bndchk=true)
   {
      RequestID = RequestIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the RequestID value.
   //! \return the current RequestID.
   Cigi_uint8 GetRequestID(void) const { return(RequestID); }


   //+> LayerID

   //=========================================================
   //! Sets the LayerID with bound checking control
   //! \param LayerIDIn - Layer ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLayerID(const Cigi_uint8 LayerIDIn, bool bndchk=true)
   {
      LayerID = LayerIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the LayerID value.
   //! \return the current LayerID.
   Cigi_uint8 GetLayerID(void) const { return(LayerID); }


   //+> AerosolConcentration

   //=========================================================
   //! Sets the AerosolConcentration with bound checking control
   //! \param AerosolConcentrationIn - Aerosol Concentration
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetAerosolConcentration(const float AerosolConcentrationIn, bool bndchk=true);

   //=========================================================
   //! Gets the AerosolConcentration value.
   //! \return the current AerosolConcentration.
   float GetAerosolConcentration(void) const { return(AerosolConcentration); }



protected:

   //==> Member variables

   //=========================================================
   //! RequestID<br>
   //! Request ID
   //!
   Cigi_uint8 RequestID;

   //=========================================================
   //! LayerID<br>
   //! Layer ID
   //!
   Cigi_uint8 LayerID;

   //=========================================================
   //! AerosolConcentration<br>
   //!
   float AerosolConcentration;


};

#endif // #if !defined(_CIGI_BASE_AEROSOL_RESP_INCLUDED_)
