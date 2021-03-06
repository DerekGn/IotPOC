/*
* Copyright 2018 Derek Goslin
*
* Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files(the "Software"),
* to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
*
* The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*
*/

#ifndef API_HANDLER_H
#define API_HANDLER_H

/* FreeRTOS */
#include <FreeRTOS.h>

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_TCP_server.h"
#include "FreeRTOS_server_private.h"

#include "jsmn.h"

typedef void (*xApiHandlerAction_t) ( HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount);

typedef struct ApiHandler
{
    BaseType_t xRouteLen;
    const char * const pcRoute;
    xApiHandlerAction_t xApiHandlerAction;
} ApiHandler_t;

BaseType_t xApiHandlerEntries( void );

BaseType_t xSendApiResponse(HTTPClient_t *pxClient, const char *pcContentType);

BaseType_t xParseId(const char *pcCurrent, BaseType_t *pxId);

#endif /* API_HANDLER_H */
