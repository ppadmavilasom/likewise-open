/* Editor Settings: expandtabs and use 4 spaces for indentation
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 * -*- mode: c, c-basic-offset: 4 -*- */

/*
 * Copyright Likewise Software    2004-2008
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the license, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser
 * General Public License for more details.  You should have received a copy
 * of the GNU Lesser General Public License along with this program.  If
 * not, see <http://www.gnu.org/licenses/>.
 *
 * LIKEWISE SOFTWARE MAKES THIS SOFTWARE AVAILABLE UNDER OTHER LICENSING
 * TERMS AS WELL.  IF YOU HAVE ENTERED INTO A SEPARATE LICENSE AGREEMENT
 * WITH LIKEWISE SOFTWARE, THEN YOU MAY ELECT TO USE THE SOFTWARE UNDER THE
 * TERMS OF THAT SOFTWARE LICENSE AGREEMENT INSTEAD OF THE TERMS OF THE GNU
 * LESSER GENERAL PUBLIC LICENSE, NOTWITHSTANDING THE ABOVE NOTICE.  IF YOU
 * HAVE QUESTIONS, OR WISH TO REQUEST A COPY OF THE ALTERNATE LICENSING
 * TERMS OFFERED BY LIKEWISE SOFTWARE, PLEASE CONTACT LIKEWISE SOFTWARE AT
 * license@likewisesoftware.com
 */

/*
 * Copyright (C) Centeris Corporation 2004-2007
 * Copyright (C) Likewise Software 2007
 * All rights reserved.
 *
 * Authors: Krishna Ganugapati (krishnag@likewisesoftware.com)
 *          Sriram Nambakam (snambakam@likewisesoftware.com)
 *
 * Server Service String Utilities
 *
 */

#include "includes.h"

DWORD
SRVSVCStrndup(
    PCSTR pszInputString,
    size_t size,
    PSTR * ppszOutputString
)
{
    DWORD dwError = 0;
    size_t copylen = 0;
    PSTR pszOutputString = NULL;

    if (!pszInputString || !ppszOutputString){
        dwError = EINVAL;
        BAIL_ON_SRVSVC_ERROR(dwError);
    }

    copylen = strlen(pszInputString);
    if (copylen > size)
        copylen = size;

    dwError = SRVSVCAllocateMemory(copylen+1, (PVOID *)&pszOutputString);
    BAIL_ON_SRVSVC_ERROR(dwError);

    memcpy(pszOutputString, pszInputString, copylen);
    pszOutputString[copylen] = 0;

error:

    *ppszOutputString = pszOutputString;

    return(dwError);
}

/* return a string description of the TableCategory type */
PCSTR
TableCategoryToStr(
    DWORD tableCategory
    )
{

    switch(tableCategory) {
    case 0:
        return "Application";
    case 1:
        return "WebBrowser";
    case 2:
        return "Security";
    case 3:
        return "System";
    default:
        return "Unknown";
    }

    return "";

}



BOOLEAN
SRVSVCIsWhiteSpace(
    char c
    )
{
    switch(c) {
    case '\n':
    case '\r':
    case '\t':
    case ' ':
        return true;
    default:
        return false;
    }
}


/* modify PSTR in-place to conver sequences of whitespace characters into single spaces (0x20) */
DWORD
SRVSVCCompressWhitespace(
    PSTR pszString
    )
{
    DWORD pszStringLen = 0;
    DWORD i = 0;
    DWORD j = 0;
    BOOLEAN whitespace = false;

    if (pszString == NULL) {
        return -1;
    }

    pszStringLen = strlen(pszString);

    for (i = 0; i < pszStringLen; i++) {

    if (SRVSVCIsWhiteSpace(pszString[i])) {
        if (!whitespace) {
        whitespace = true;
        pszString[j++] = ' ';
        }
    }
    else {
        whitespace = false;
        pszString[j++] = pszString[i];
    }

    }
    pszString[j] = 0;
    return 0;
}

/* convert a 16-bit string to an 8-bit string, allocating new memory in the process */
DWORD
SRVSVCLpwStrToLpStr(
    PCWSTR pszwString,
    PSTR* ppszString
    )
{

    DWORD dwError = 0;
    DWORD pszwStringLen = 0;
    PSTR pszString = NULL;

    DWORD i = 0;

    if (ppszString == NULL || pszwString == NULL)
    {
        return -1;
    }

    pszwStringLen = wc16slen(pszwString);

    dwError = SRVSVCAllocateMemory(pszwStringLen+1, (PVOID*)ppszString);
    BAIL_ON_SRVSVC_ERROR(dwError);

    pszString = *ppszString;

    for (i = 0; i < pszwStringLen; i++) {
        pszString[i] = (char)((WORD)pszwString[i]);
    }

    pszString[pszwStringLen] = 0;

 error:

    return dwError;



}

void
SRVSVCStripLeadingWhitespace(
    PSTR pszString
    )
{
    PSTR pszNew = pszString;
    PSTR pszTmp = pszString;

    if (pszString == NULL || *pszString == '\0' || !isspace((int)*pszString)) {
        return;
    }

    while (pszTmp != NULL && *pszTmp != '\0' && isspace((int)*pszTmp)) {
        pszTmp++;
    }

    while (pszTmp != NULL && *pszTmp != '\0') {
        *pszNew++ = *pszTmp++;
    }
    *pszNew = '\0';
}

void
SRVSVCStripTrailingWhitespace(
    PSTR pszString
    )
{
    PSTR pszLastSpace = NULL;
    PSTR pszTmp = pszString;

    if (pszString == NULL || *pszString == '\0') {
        return;
    }

    while (pszTmp != NULL && *pszTmp != '\0') {
        pszLastSpace = (isspace((int)*pszTmp) ? (pszLastSpace ? pszLastSpace : pszTmp) : NULL);
        pszTmp++;
    }

    if (pszLastSpace != NULL) {
        *pszLastSpace = '\0';
    }
}

void
SRVSVCStripWhitespace(
    PSTR pszString,
    BOOLEAN bLeading,
    BOOLEAN bTrailing
    )
{
    if (pszString == NULL || *pszString == '\0') {
        return;
    }

    if (bLeading) {
        SRVSVCStripLeadingWhitespace(pszString);
    }

    if (bTrailing) {
        SRVSVCStripTrailingWhitespace(pszString);
    }
}

void
SRVSVCStrToUpper(
    PSTR pszString
    )
{

    if (pszString != NULL) {
         while (*pszString != '\0') {
             *pszString = toupper(*pszString);
             pszString++;
         }
    }
}

void
SRVSVCStrToLower(
    PSTR pszString
    )
{

    if (pszString != NULL) {
         while (*pszString != '\0') {
             *pszString = tolower(*pszString);
             pszString++;
         }
    }
}

DWORD
SRVSVCEscapeString(
    PSTR pszOrig,
    PSTR * ppszEscapedString
    )
{
    DWORD dwError = 0;
    int nQuotes = 0;
    PSTR pszTmp = pszOrig;
    PSTR pszNew = NULL;
    PSTR pszNewTmp = NULL;

    if ( !ppszEscapedString || !pszOrig ) {
         dwError = EINVAL;
         BAIL_ON_SRVSVC_ERROR(dwError);
    }

    while (pszTmp && *pszTmp)
    {
         if (*pszTmp=='\'') {
             nQuotes++;
         }
         pszTmp++;
    }

    if (!nQuotes) {
         dwError = SRVSVCAllocateString(pszOrig, &pszNew);
         BAIL_ON_SRVSVC_ERROR(dwError);
    } else {
         /*
            * We are going to escape each single quote and enclose it in two other
            * single-quotes
            */
         dwError = SRVSVCAllocateMemory( strlen(pszOrig)+3*nQuotes+1, (PVOID*)&pszNew );
         BAIL_ON_SRVSVC_ERROR(dwError);

         pszTmp = pszOrig;
         pszNewTmp = pszNew;

         while (pszTmp && *pszTmp)
         {
             if (*pszTmp=='\'') {
                 *pszNewTmp++='\'';
                 *pszNewTmp++='\\';
                 *pszNewTmp++='\'';
                 *pszNewTmp++='\'';
                 pszTmp++;
             }
             else {
                 *pszNewTmp++ = *pszTmp++;
             }
         }
         *pszNewTmp = '\0';
    }

    *ppszEscapedString = pszNew;
    return dwError;

error:

    if (pszNew) {
        SRVSVCFreeMemory(pszNew);
    }

    return dwError;
}

DWORD
SRVSVCAllocateStringPrintf(
    PSTR* ppszOutputString,
    PCSTR pszFormat,
    ...
    )
{
    DWORD dwError = 0;
    va_list args;

    va_start(args, pszFormat);

    dwError = SRVSVCAllocateStringPrintfV(
                      ppszOutputString,
                      pszFormat,
                      args);

    va_end(args);

    return dwError;
}

DWORD
SRVSVCAllocateStringPrintfV(
    PSTR*   ppszOutputString,
    PCSTR   pszFormat,
    va_list args
    )
{
    DWORD dwError = 0;
    PSTR  pszSmallBuffer = NULL;
    DWORD dwBufsize = 0;
    INT   requiredLength = 0;
    DWORD dwNewRequiredLength = 0;
    PSTR  pszOutputString = NULL;
    va_list args2;

    va_copy(args2, args);

    dwBufsize = 4;
    /* Use a small buffer in case libc does not like NULL */
    do
    {
        dwError = SRVSVCAllocateMemory(
                        dwBufsize,
                        (PVOID*) &pszSmallBuffer);
        BAIL_ON_SRVSVC_ERROR(dwError);

        requiredLength = vsnprintf(
                              pszSmallBuffer,
                              dwBufsize,
                              pszFormat,
                              args);
        if (requiredLength < 0)
        {
            dwBufsize *= 2;
        }
        SRVSVCFreeMemory(pszSmallBuffer);
        pszSmallBuffer = NULL;

    } while (requiredLength < 0);

    if (requiredLength >= (UINT32_MAX - 1))
    {
        dwError = ENOMEM;
        BAIL_ON_SRVSVC_ERROR(dwError);
    }

    dwError = SRVSVCAllocateMemory(
                    requiredLength + 2,
                    (PVOID*)&pszOutputString);
    BAIL_ON_SRVSVC_ERROR(dwError);

    dwNewRequiredLength = vsnprintf(
                            pszOutputString,
                            requiredLength + 1,
                            pszFormat,
                            args2);
    if (dwNewRequiredLength < 0)
    {
        dwError = errno;
        BAIL_ON_SRVSVC_ERROR(dwError);
    }
    else if (dwNewRequiredLength > requiredLength)
    {
        /* unexpected, ideally should log something, or use better error code */
        dwError = ENOMEM;
        BAIL_ON_SRVSVC_ERROR(dwError);
    }
    else if (dwNewRequiredLength < requiredLength)
    {
        /* unexpected, ideally should log something -- do not need an error, though */
    }

    *ppszOutputString = pszOutputString;

cleanup:

    va_end(args2);

    return dwError;

error:

    SRVSVC_SAFE_FREE_MEMORY(pszOutputString);

    *ppszOutputString = NULL;

    goto cleanup;
}

