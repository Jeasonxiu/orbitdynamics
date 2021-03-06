//$Header$
//------------------------------------------------------------------------------
//                             BaseException
//------------------------------------------------------------------------------
// GMAT: Goddard Mission Analysis Tool.
//
// **Legal**
//
// Developed jointly by NASA/GSFC and Thinking Systems, Inc. under contract
// number NNG04CC06P.
//
// Author: Linda Jun (NASA/GSFC)
// Created: 2007/1/18
//
/**
 * Exception class used by the GmatBase base class.
 */
//------------------------------------------------------------------------------

#include "BaseException.h"
#include "stdlib.h"
#include <stdarg.h>                // for va_start(), va_end()
#include <stdio.h>
#include <cstring>

#if _MSC_VER < 1300
const int BaseException::MAX_MESSAGE_LENGTH=3000;
#endif
//------------------------------------------------------------------------------
// void SetDetails(const char *details, ...)
//------------------------------------------------------------------------------
/**
 * constructor taking variable arguments
 */
//------------------------------------------------------------------------------
void BaseException::SetDetails(const char *details, ...)
{
   short    ret;
   short    size;
   va_list  marker;
   char     *msgBuffer;
   
   size = strlen(details) + MAX_MESSAGE_LENGTH;
   
   if ( (msgBuffer = (char *)malloc(size)) != NULL )
   {
      va_start(marker, details);
      
      #ifdef _MSC_VER  // Microsoft Visual C++
      // _vscprintf doesn't count terminating '\0'
	   #if _MSC_VER > 1300
        int len = _vscprintf( details, marker ) + 1; 
        ret = vsprintf_s(msgBuffer, len, details, marker);
	   #else
        int len = vprintf( details, marker ) + 1; 
        ret = vsprintf(msgBuffer, details, marker);
	   #endif // _MSC_VER > 1300
      #else
      ret = vsprintf(msgBuffer, details, marker);
      #endif // _MSC_VER
      
      va_end(marker);
   }

   theDetails = std::string(msgBuffer);
   free(msgBuffer);
}

