/* Editor Settings: expandtabs and use 4 spaces for indentation
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 * -*- mode: c, c-basic-offset: 4 -*- */

/*
 * Copyright Likewise Software
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.  You should have received a copy of the GNU General
 * Public License along with this program.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * LIKEWISE SOFTWARE MAKES THIS SOFTWARE AVAILABLE UNDER OTHER LICENSING
 * TERMS AS WELL.  IF YOU HAVE ENTERED INTO A SEPARATE LICENSE AGREEMENT
 * WITH LIKEWISE SOFTWARE, THEN YOU MAY ELECT TO USE THE SOFTWARE UNDER THE
 * TERMS OF THAT SOFTWARE LICENSE AGREEMENT INSTEAD OF THE TERMS OF THE GNU
 * GENERAL PUBLIC LICENSE, NOTWITHSTANDING THE ABOVE NOTICE.  IF YOU
 * HAVE QUESTIONS, OR WISH TO REQUEST A COPY OF THE ALTERNATE LICENSING
 * TERMS OFFERED BY LIKEWISE SOFTWARE, PLEASE CONTACT LIKEWISE SOFTWARE AT
 * license@likewisesoftware.com
 */



/*
 * Copyright (C) Likewise Software. All rights reserved.
 *
 * Module Name:
 *
 *        dirattr.c
 *
 * Abstract:
 *
 *
 *      Likewise Directory Wrapper Interface
 *
 *      Directory Attribute Management
 *
 * Authors: Krishna Ganugapati (krishnag@likewisesoftware.com)
 *
 */
#include "includes.h"

VOID
DirectoryFreeEntries(
    PDIRECTORY_ENTRY pEntries,
    DWORD            dwNumEntries
    )
{
    DWORD iEntry = 0;

    for (; iEntry < dwNumEntries; iEntry++)
    {
        PDIRECTORY_ENTRY pEntry = &pEntries[iEntry];

        if (pEntry->pDirectoryAttributes)
        {
            DWORD iDirAttr = 0;

            for (; iDirAttr < pEntry->ulNumAttributes; iDirAttr++)
            {
                PDIRECTORY_ATTRIBUTE pDirAttr = NULL;

                pDirAttr = &pEntry->pDirectoryAttributes[iDirAttr];

                if (pDirAttr->pwszAttributeName)
                {
                    DirectoryFreeStringW(pDirAttr->pwszAttributeName);
                }

                if (pDirAttr->pAttributeValues)
                {
                    DirectoryFreeAttributeValues(
                            pDirAttr->pAttributeValues,
                            pDirAttr->ulNumValues);
                }
            }

            DirectoryFreeMemory(pEntry->pDirectoryAttributes);
        }
    }

    DirectoryFreeMemory(pEntries);
}

VOID
DirectoryFreeAttributeValues(
    PATTRIBUTE_VALUE pAttrValues,
    DWORD            dwNumValues
    )
{
    DWORD iValue = 0;

    for (; iValue < dwNumValues; iValue++)
    {
        PATTRIBUTE_VALUE pAttrValue = &pAttrValues[iValue];

        switch (pAttrValue->Type)
        {
            case DIRECTORY_ATTR_TYPE_UNICODE_STRING:

                if (pAttrValue->pwszStringValue)
                {
                    DirectoryFreeMemory(pAttrValue->pwszStringValue);
                }

                break;

            case DIRECTORY_ATTR_TYPE_ANSI_STRING:

                if (pAttrValue->pszStringValue)
                {
                    DirectoryFreeMemory(pAttrValue->pszStringValue);
                }

                break;

            default:

                break;
        }
    }

    DirectoryFreeMemory(pAttrValues);
}
