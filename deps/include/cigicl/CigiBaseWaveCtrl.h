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
 *  FILENAME:   CigiBaseWaveCtrl.h
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
 * </pre>
 *  Author: The Boeing Company
 *
 */


#if !defined(_CIGI_BASE_WAVE_CTRL_INCLUDED_)
#define _CIGI_BASE_WAVE_CTRL_INCLUDED_

#include "CigiBasePacket.h"

// ====================================================================
// preprocessor definitions
// ====================================================================

#define CIGI_WAVE_CTRL_PACKET_ID_V3 14
#define CIGI_WAVE_CTRL_PACKET_SIZE_V3 32


class CigiWaveCtrlV3;


class CIGI_SPEC CigiBaseWaveCtrl : public CigiBasePacket
{

friend class CigiWaveCtrlV3;

public:

   //=========================================================
   //! The enumeration for the CigiBaseWaveCtrl Group
   //!
   enum ScopeGrp
   {
      Global=0,
      Regional=1,
      Entity=2
   };

   //=========================================================
   //! The enumeration for the CigiBaseWaveCtrl Group
   //!
   enum BreakerGrp
   {
      Plunging=0,
      Spilling=1,
      Surging=2
   };




   //==> Management

   //=========================================================
   //! General Constructor
   //!
   CigiBaseWaveCtrl();

   //=========================================================
   //! General Destructor
   //!
   virtual ~CigiBaseWaveCtrl();


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

   //+> EntityRgnID

   //=========================================================
   //! Sets the EntityRgnID with bound checking control
   //! \param EntityRgnIDIn - Entity or Region ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetEntityRgnID(const Cigi_uint16 EntityRgnIDIn, bool bndchk=true)
   {
      EntityRgnID = EntityRgnIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the EntityRgnID value.
   //! \return the current EntityRgnID.
   Cigi_uint16 GetEntityRgnID(void) const { return(EntityRgnID); }


   //+> WaveID

   //=========================================================
   //! Sets the WaveID with bound checking control
   //! \param WaveIDIn - Wave ID
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetWaveID(const Cigi_uint8 WaveIDIn, bool bndchk=true)
   {
      WaveID = WaveIDIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the WaveID value.
   //! \return the current WaveID.
   Cigi_uint8 GetWaveID(void) const { return(WaveID); }


   //+> WaveEn

   //=========================================================
   //! Sets the WaveEn with bound checking control
   //! \param WaveEnIn - Wave enable
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetWaveEn(const bool WaveEnIn, bool bndchk=true)
   {
      WaveEn = WaveEnIn;
      return(CIGI_SUCCESS);
   }

   //=========================================================
   //! Gets the WaveEn value.
   //! \return the current WaveEn.
   bool GetWaveEn(void) const { return(WaveEn); }


   //+> Scope

   //=========================================================
   //! Sets the Scope with bound checking control
   //! \param ScopeIn - Scope of Effect<br>
   //!   Global=0<br>
   //!   Regional=1<br>
   //!   Entity=2<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetScope(const ScopeGrp ScopeIn, bool bndchk=true);

   //=========================================================
   //! Gets the Scope value.
   //! \return the current Scope.
   ScopeGrp GetScope(void) const { return(Scope); }


   //+> Breaker

   //=========================================================
   //! Sets the Breaker with bound checking control
   //! \param BreakerIn - Breaker type<br>
   //!   Plunging=0<br>
   //!   Spilling=1<br>
   //!   Surging=2<br>
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetBreaker(const BreakerGrp BreakerIn, bool bndchk=true);

   //=========================================================
   //! Gets the Breaker value.
   //! \return the current Breaker.
   BreakerGrp GetBreaker(void) const { return(Breaker); }


   //+> WaveHt

   //=========================================================
   //! Sets the WaveHt with bound checking control
   //! \param WaveHtIn - Wave height
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetWaveHt(const float WaveHtIn, bool bndchk=true);

   //=========================================================
   //! Gets the WaveHt value.
   //! \return the current WaveHt.
   float GetWaveHt(void) const { return(WaveHt); }


   //+> WaveLen

   //=========================================================
   //! Sets the WaveLen with bound checking control
   //! \param WaveLenIn - Wave length
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetWaveLen(const float WaveLenIn, bool bndchk=true);

   //=========================================================
   //! Gets the WaveLen value.
   //! \return the current WaveLen.
   float GetWaveLen(void) const { return(WaveLen); }


   //+> Period

   //=========================================================
   //! Sets the Period with bound checking control
   //! \param PeriodIn - Wave period
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetPeriod(const float PeriodIn, bool bndchk=true);

   //=========================================================
   //! Gets the Period value.
   //! \return the current Period.
   float GetPeriod(void) const { return(Period); }


   //+> Direction

   //=========================================================
   //! Sets the Direction with bound checking control
   //! \param DirectionIn - Direction the wave is propagating
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetDirection(const float DirectionIn, bool bndchk=true);

   //=========================================================
   //! Gets the Direction value.
   //! \return the current Direction.
   float GetDirection(void) const { return(Direction); }


   //+> PhaseOff

   //=========================================================
   //! Sets the PhaseOff with bound checking control
   //! \param PhaseOffIn - Phase offset
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetPhaseOff(const float PhaseOffIn, bool bndchk=true);

   //=========================================================
   //! Gets the PhaseOff value.
   //! \return the current PhaseOff.
   float GetPhaseOff(void) const { return(PhaseOff); }


   //+> Leading

   //=========================================================
   //! Sets the Leading with bound checking control
   //! \param LeadingIn - Leading
   //! \param bndchk - Enables (true) or disables (false) bounds checking.
   //!
   //! \return This returns CIGI_SUCCESS or an error code
   //!   defined in CigiErrorCodes.h
   int SetLeading(const float LeadingIn, bool bndchk=true);

   //=========================================================
   //! Gets the Leading value.
   //! \return the current Leading.
   float GetLeading(void) const { return(Leading); }



protected:

   //==> Member variables

   //=========================================================
   //! EntityRgnID<br>
   //! Entity or Region ID
   //!
   Cigi_uint16 EntityRgnID;

   //=========================================================
   //! WaveID<br>
   //! Wave ID
   //!
   Cigi_uint8 WaveID;

   //=========================================================
   //! WaveEn<br>
   //! Wave enable
   //!
   bool WaveEn;

   //=========================================================
   //! Scope<br>
   //! Scope of Effect<br>
   //!  0-Global<br>
   //!  1-Regional<br>
   //!  2-Entity
   //!
   //!
   ScopeGrp Scope;

   //=========================================================
   //! Breaker<br>
   //! Breaker type<br>
   //!  0-Plunging<br>
   //!  1-Spilling<br>
   //!  2-Surging
   //!
   //!
   BreakerGrp Breaker;

   //=========================================================
   //! WaveHt<br>
   //! Wave height
   //!
   float WaveHt;

   //=========================================================
   //! WaveLen<br>
   //! Wave length
   //!
   float WaveLen;

   //=========================================================
   //! Period<br>
   //! Wave period
   //!
   float Period;

   //=========================================================
   //! Direction<br>
   //! Direction the wave is propagating
   //!
   float Direction;

   //=========================================================
   //! PhaseOff<br>
   //! Phase offset
   //!
   float PhaseOff;

   //=========================================================
   //! Leading<br>
   //! Leading
   //!
   float Leading;


};

#endif // #if !defined(_CIGI_BASE_WAVE_CTRL_INCLUDED_)
