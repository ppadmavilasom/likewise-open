/* Editor Settings: expandtabs and use 4 spaces for indentation
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 */

/*
 * Copyright Likewise Software    2004-2009
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
 * Copyright (C) Likewise Software. All rights reserved.
 *
 * Module Name:
 *
 *        dsrbinding.h
 *
 * Abstract:
 *
 *        Remote Procedure Call (RPC) Client Interface
 *
 *        DsSetup interface binding
 *
 * Authors: Rafal Szczesniak (rafal@likewise.com)
 */

#ifndef _DSRBINDING_H_
#define _DSRBINDING_H_

#include <lwrpc/types.h>
#include <lwio/lwio.h>

#define DSR_DEFAULT_PROT_SEQ   "ncacn_np"
#define DSR_DEFAULT_ENDPOINT   "\\PIPE\\lsarpc"


RPCSTATUS
InitDsrBindingDefault(
    handle_t *binding,
    const char *hostname,
    PIO_ACCESS_TOKEN access_token
    );


RPCSTATUS
InitDsrBindingFull(
    handle_t *binding,
    const char *prot_seq,
    const char *hostname,
    const char *endpoint,
    const char *uuid,
    const char *options,
    PIO_ACCESS_TOKEN access_token
    );


RPCSTATUS
FreeDsrBinding(
    handle_t *binding
    );


#endif /* _DSRBINDING_H_ */
