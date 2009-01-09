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

#include <lwrpc/types.h>
#include <lw/ntstatus.h>

/*
 * Provide dummy stubs for those functions that aren't implemented
 * yet are needed not to block development of other projects relying
 * on NetLib
 */


NTSTATUS NetShareEnum(wchar16_t *servername, uint32 level, void **bufptr,
		      uint32 prefmaxlen, uint32 *entriesread,
		      uint32 *totalentries, uint32 *resume_handle)

{
	return STATUS_NOT_IMPLEMENTED;
}


NTSTATUS NetFileEnum(wchar16_t *servername, wchar16_t *basepath,
		     wchar16_t *username, uint32 level, void **bufptr,
		     uint32 prefmaxlen, uint32 *entriesread,
		     uint32 *totalentries, uint32 *resume_handle)
{
	return STATUS_NOT_IMPLEMENTED;
}


NTSTATUS NetFileClose(wchar16_t *servername, uint32 fileid)
{
	return STATUS_NOT_IMPLEMENTED;
}


NTSTATUS NetShareDel(wchar16_t *servername, wchar16_t *netname, uint32 reserved)
{
	return STATUS_NOT_IMPLEMENTED;
}
