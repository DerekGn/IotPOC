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

#ifndef FREERTOS_HTTP_PIPELINE_H
#define	FREERTOS_HTTP_PIPELINE_H

/* FreeRTOS */
#include <FreeRTOS.h>

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_server_private.h"

typedef enum
{
	eInvokeNext,
	eSkipNext
} eHttpHandlerResult_t;

typedef eHttpHandlerResult_t ( *xHttpHandlerAction_t ) ( HTTPClient_t *pxClient, BaseType_t xIndex );

typedef struct HttpHandler
{
	const char * const pucName;					/* Text description for the handler. */
	xHttpHandlerAction_t xHttpHandlerAction;	/* Action to perform on the http request. */
} HttpHandler_t;

BaseType_t xHandlerEntryCount( );

void vAddHttpHandler( HttpHandler_t *pxHttpHandler );

BaseType_t xProcessHttpRequest( HTTPClient_t *pxClient, BaseType_t xIndex );

#endif FREERTOS_HTTP_PIPELINE_H  /* FREERTOS_HTTP_PIPELINE_H */
