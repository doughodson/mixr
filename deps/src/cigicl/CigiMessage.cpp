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
 *  FILENAME:   CigiMessage.cpp
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
 *  Defined _EXPORT_CCL_ for exporting the class in a Windows DLL.
 *  
 *  04/14/2006 Greg Basler                       Version 1.7.0
 *  Modified the CreateBuffer method to add additional padding at the end of 
 *  the buffer when created to allow overruns due to version change problems.
 *  
 *  06/23/2006 Greg Basler                       Version 1.7.1
 *  Changed native char and unsigned char types to CIGI types Cigi_int8 and 
 *  Cigi_uint8.
 * </pre>
 *  Author: The Boeing Company
 *
 */

#define _EXPORT_CCL_

#include "CigiMessage.h"

#include <stdlib.h>

// ====================================================================
// Construction/Destruction
// ====================================================================

// ================================================
// CigiMessage
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiMessage::CigiMessage()
: BufferSize(0)
, CrntMsgBuf(NULL)
, CrntFillBuf(NULL)
, PackagedMsg(NULL)
, ATbl(NULL)
, Session(NULL)
{

}

// ================================================
// ~CigiMessage
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
CigiMessage::~CigiMessage()
{

   if(Buffers.size() > 0)
   {
      list<CigiMessageBuffer *>::iterator iBuf;
      for(iBuf=Buffers.begin();iBuf!=Buffers.end();iBuf++)
      {
         delete (*iBuf);
      }
      Buffers.clear();
   }

   if(AvailBuff.size() > 0)
   {
      list<CigiMessageBuffer *>::iterator iBuf;
      for(iBuf=AvailBuff.begin();iBuf!=AvailBuff.end();iBuf++)
      {
         delete (*iBuf);
      }
      AvailBuff.clear();
   }

}


// ====================================================================
// Processing
// ====================================================================

// ================================================
// CreateBuffer
// vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv
int CigiMessage::CreateBuffer(const int NumBuf, const int BufLen)
{
   if(BufferSize <= 0)
      BufferSize = BufLen;

   int BuffCnt = AvailBuff.size();
   int AddBuff = 0;
   if(BuffCnt < NumBuf)
      AddBuff = NumBuf - BuffCnt;

   if(AddBuff > 0)
   {
      for(int ndx=0;ndx<AddBuff;ndx++)
      {
         CigiMessageBuffer *NewBuff = new CigiMessageBuffer(BufferSize);
         AvailBuff.push_back(NewBuff);
      }
   }

   return(CIGI_SUCCESS);

}
