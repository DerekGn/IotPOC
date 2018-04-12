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
