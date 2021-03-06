/* Editor Settings: expandtabs and use 4 spaces for indentation
 * ex: set softtabstop=4 tabstop=8 expandtab shiftwidth=4: *
 * -*- mode: c, c-basic-offset: 4 -*- */

/*
 * Copyright Likewise Software
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
 *        ipc_protocol.c
 *
 * Abstract:
 *
 *        Likewise Security and Authentication Subsystem (LSASS)
 *
 *        Inter-process communication API
 *
 * Authors: Krishna Ganugapati (krishnag@likewisesoftware.com)
 *
 */

#include "ipc.h"

static LWMsgTypeSpec gLsaVmdirIPCStringSpec[] =
{
    LWMSG_PSTR,
    LWMSG_TYPE_END
};

static LWMsgTypeSpec gLsaVmdirIPCSignalReqSpec[] =
{
    LWMSG_STRUCT_BEGIN(LSA_VMDIR_IPC_SIGNAL_REQ),
    LWMSG_MEMBER_UINT32(LSA_VMDIR_IPC_SIGNAL_REQ, dwFlags),
    LWMSG_STRUCT_END,
    LWMSG_TYPE_END
};

LWMsgTypeSpec*
LsaVmdirIPCGetStringSpec(
    VOID
    )
{
    return gLsaVmdirIPCStringSpec;
}

LWMsgTypeSpec*
LsaVmdirIPCGetSignalReqSpec(
    void
    )
{
    return gLsaVmdirIPCSignalReqSpec;
}

static
LWMsgStatus
LsaVmdirIPCAllocate(
    IN size_t Size,
    OUT PVOID* ppMemory,
    IN PVOID pContext
    )
{
    DWORD dwError = LwAllocateMemory((DWORD)Size, ppMemory);
    return dwError ? LWMSG_STATUS_MEMORY : LWMSG_STATUS_SUCCESS;
}

static
VOID
LsaVmdirIPCFree(
    IN PVOID pMemory,
    IN PVOID pContext
    )
{
    LwFreeMemory(pMemory);
}

VOID
LsaVmdirIPCSetMemoryFunctions(
    IN LWMsgContext* pContext
    )
{
    lwmsg_context_set_memory_functions(pContext, LsaVmdirIPCAllocate, LsaVmdirIPCFree, NULL, NULL);
}
