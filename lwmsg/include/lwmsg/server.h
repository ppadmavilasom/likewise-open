/*
 * Copyright (c) Likewise Software.  All rights Reserved.
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
 * Module Name:
 *
 *        server.h
 *
 * Abstract:
 *
 *        Multi-threaded server API (public header)
 *
 * Authors: Brian Koropoff (bkoropoff@likewisesoftware.com)
 *
 */
#ifndef __LWMSG_SERVER_H__
#define __LWMSG_SERVER_H__

#include <lwmsg/status.h>
#include <lwmsg/protocol.h>
#include <lwmsg/time.h>
#include <lwmsg/assoc.h>

/**
 * @file server.h
 * @brief Server API
 */

/**
 * @defgroup server Servers
 * @ingroup public
 * @brief Multi-threaded server implementation
 *
 * The server API provided here automates the process of creating listening
 * servers which wait for incoming connections and can service multiple clients
 * simultaneously.  It uses a thread pool internally to service clients and a separate
 * listener thread to queue new connections for the pool, providing a completely
 * asynchronous external interface.
 *
 * Because threads are not desirable in some applications, this functionality is 
 * only available by defining LWMSG_THREADS before including <lwmsg/lwmsg.h>
 * and linking against liblwmsgthr instead of liblwmsg.
 *
 */

/**
 * @ingroup server
 * @brief Dispatch specification
 *
 * This structure defines a table of dispatch functions
 * to handle incoming messages in a server.  It should
 * be constructed as a statically-initialized array
 * using #LWMSG_DISPATCH() and #LWMSG_ENDDISPATCH macros.
 */
typedef struct LWMsgDispatchSpec
#ifndef DOXYGEN
{
    LWMsgMessageTag tag;
    LWMsgDispatchFunction func;
}
#endif
LWMsgDispatchSpec;

/**
 * @ingroup server
 * @brief Define message handle in a dispatch table
 *
 * This macro is used in dispatch table construction to
 * define the handler for a particular message type.
 * @param _type the message type
 * @param _func the callback to handle the specified message type
 * @hideinitializer
 */
#define LWMSG_DISPATCH(_type, _func) {(_type), (_func)}

/**
 * @ingroup server
 * @brief Terminate dispatch table
 *
 * This macro is used in dispatch table construction to
 * mark the end of the table
 * @hideinitializer
 */
#define LWMSG_ENDDISPATCH {0, NULL}


/**
 * @ingroup server
 * @brief Opaque server object
 *
 * Opqaue server object which can be used to start a listening message server.
 */
typedef struct LWMsgServer LWMsgServer;

/**
 * @ingroup server
 * @brief Server listening mode
 *
 * Specifies whether a server should listen on a local or remote socket
 */
typedef enum LWMsgServerMode
{
    /** No server mode specified */
    LWMSG_SERVER_MODE_NONE,
    /** Server should be local only (listen on a local UNIX domain socket) */
    LWMSG_SERVER_MODE_LOCAL,
    /** Server should support remote connectiosn (listen on a TCP socket) */
    LWMSG_SERVER_MODE_REMOTE
} LWMsgServerMode;

/**
 * @ingroup server
 * @brief Create a new server object
 *
 * Creates a new, unconfigured server object.  The created server must be
 * configured with a protocol, dispatch table, and endpoint before it
 * can be started.
 *
 * @param out_server the created server object
 * @return LWMSG_STATUS_SUCCESS on success, LWMSG_STATUS_MEMORY if out of memory
 */
LWMsgStatus
lwmsg_server_new(
    LWMsgServer** out_server
    );

/**
 * @ingroup server
 * @brief Create a new server object with context
 *
 * Creates a new, unconfigured server object which inherits settings from
 * the specified context.  Any connections created by the server will in
 * turn inherit these settigns.
 *
 * @param context the context from which to inherit
 * @param out_server the created server object
 * @return LWMSG_STATUS_SUCCESS on success, LWMSG_STATUS_MEMORY if out of memory
 */
LWMsgStatus
lwmsg_server_new_with_context(
    LWMsgContext* context,
    LWMsgServer** out_server
    );

/**
 * @ingroup server
 * @brief Delete a server object
 *
 * Deletes a server object.
 *
 * @warning Attempting to delete a server which has been started but not stopped
 * will block until the server stops
 *
 * @param server the server object to delete
 */
void
lwmsg_server_delete(
    LWMsgServer* server
    );

/**
 * @ingroup server
 * @brief Set operation timeout
 *
 * Sets the timeout that will be used for client associations.
 * See lwmsg_assoc_set_timeout() for more information.
 *
 * @param server the server object
 * @param timeout the timeout to set, or NULL for no timeout
 * @return LWMSG_STATUS_SUCCESS on success, LWMSG_STATUS_INVALID on invalid timeout
 */
LWMsgStatus
lwmsg_server_set_timeout(
    LWMsgServer* server,
    LWMsgTime* timeout
    );

/**
 * @ingroup server
 * @brief Set maximum number of simultaneous active connections
 *
 * Sets the maximum numbers of clients which the server will service
 * simultaneously.  This corresponds to the size of the thread pool
 * used by the server.  Connections beyond this maximum will be answered
 * but queued until a slot becomes available.
 *
 * @param server the server object
 * @param max_clients the maximum number of simultaneous clients to support
 * @return LWMSG_STATUS_SUCCESS on success, LWMSG_STATUS_INVALID if the server
 * has already been started
 */
LWMsgStatus
lwmsg_server_set_max_clients(
    LWMsgServer* server,
    unsigned int max_clients
    );

/**
 * @ingroup server
 * @brief Set maximum number of queued connections
 *
 * Sets the maximum numbers of clients which the server will answer
 * but place into a queue until a slot is available to service the
 * connection.  If all active connection slots and the backlog queue
 * are full, the server will cease accepting connections until space
 * becomes available.
 *
 * @param server the server object
 * @param max_backlog the maximum number of clients to queue
 * @return LWMSG_STATUS_SUCCESS on success, LWMSG_STATUS_INVALID if the server
 * has already been started
 */
LWMsgStatus
lwmsg_server_set_max_backlog(
    LWMsgServer* server,
    unsigned int max_backlog
    );

/**
 * @ingroup server
 * @brief Add a protocol specification
 *
 * Adds a protocol specification to the specified server object,
 * which will become part of the protocol supported by the server.
 * This function may be invoked multiple times to combine several
 * protocol specifications.  This function should not be used in
 * concert with lwmsg_server_set_protocol().
 *
 * @param server the server object
 * @param spec the protocol specification
 * @lwmsg_status
 * @lwmsg_success
 * @lwmsg_memory
 * @lwmsg_etc{any error returned by lwmsg_prototol_add_spec()}
 * @lwmsg_endstatus
 */
LWMsgStatus
lwmsg_server_add_protocol_spec(
    LWMsgServer* server,
    LWMsgProtocolSpec* spec
    );

/**
 * @ingroup server
 * @brief Set server protocol
 *
 * Sets the protocol object which describes the protocol understood
 * by the server.  This function should not be used in concert
 * with lwmsg_server_add_protocol_spec().
 *
 * @param server the server object
 * @param prot the protocol object
 * @lwmsg_status
 * @lwmsg_success
 * @lwmsg_memory
 * @lwmsg_etc{any error returned by lwmsg_prototol_add_spec()}
 * @lwmsg_endstatus
 */
LWMsgStatus
lwmsg_server_set_protocol(
    LWMsgServer* server,
    LWMsgProtocol* prot
    );

/**
 * @ingroup server
 * @brief Add a message dispatch specification
 *
 * Adds a set of message dispatch functions to the specified
 * server object.
 *
 * @param server the server object
 * @param spec the dispatch specification
 * @lwmsg_status
 * @lwmsg_success
 * @lwmsg_memory
 * @lwmsg_endstatus
 */
LWMsgStatus
lwmsg_server_add_dispatch_spec(
    LWMsgServer* server,
    LWMsgDispatchSpec* spec
    );

/**
 * @ingroup server
 * @brief Set listening socket
 *
 * Sets the socket on which the server will listen for connections.
 * This function must be passed a valid file descriptor which is socket
 * that matches the specified mode and is already listening (has had
 * listen() called on it).
 *
 * @param server the server object
 * @param mode the connection mode (local or remote)
 * @param fd the socket on which to listen
 * @return LWMSG_STATUS_SUCCESS on success, LWMSG_STATUS_INVALID on a bad file descriptor
 * or if a listening endpoint is already set
 */
LWMsgStatus
lwmsg_server_set_fd(
    LWMsgServer* server,
    LWMsgServerMode mode,
    int fd
    );
    
/**
 * @ingroup server
 * @brief Set listening endpoint
 *
 * Sets the endpoint on which the server will listen for connections.
 * For local (UNIX domain) endpoints, this is the path of the socket file.
 * For remote (TCP) endpoints, this is the address and port to bind to.
 *
 * @param server the server object
 * @param mode the connection mode (local or remote)
 * @param endpoint the endpoint on which to listen
 * @param permissions permissions for the endpoint (only applicable to local mode)
 * @return LWMSG_STATUS_SUCCESS on success, LWMSG_STATUS_INVALID on a bad endpoint
 * or if a listening endpoint is already set
 */
LWMsgStatus
lwmsg_server_set_endpoint(
    LWMsgServer* server,
    LWMsgServerMode mode,
    const char* endpoint,
    mode_t      permissions
    );

/**
 * @ingroup server
 * @brief Start accepting connections
 *
 * Starts listening for and servicing connections in a separate thread.
 * This function will not block, so the calling function should arrange
 * to do something afterwards while the server is running, such as handling
 * UNIX signals.
 *
 * @param server the server object
 * @return LWMSG_STATUS_SUCCESS on success, or an appropriate status code on failure
 */
LWMsgStatus
lwmsg_server_start(
    LWMsgServer* server
    );

/**
 * @ingroup server
 * @brief Aggressively stop server
 *
 * Stops the specified server accepting new connections and aggressively
 * terminates any connections in progress or queued for service.  After
 * this function returns successfully, the server may be safely deleted with
 * lwmsg_server_delete().
 *
 * @param server the server object
 * @return LWMSG_STATUS_SUCCESS on success, or an appropriate status code on failure
 */
LWMsgStatus
lwmsg_server_stop(
    LWMsgServer* server
    );

#endif
