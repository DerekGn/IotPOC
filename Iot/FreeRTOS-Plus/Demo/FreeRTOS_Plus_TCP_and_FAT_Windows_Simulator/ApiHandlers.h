#ifndef API_HANDLER_H
#define API_HANDLER_H

/* FreeRTOS */
#include <FreeRTOS.h>

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_Sockets.h"
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

BaseType_t xSendApiResponse(HTTPClient_t *pxClient);

#endif /* API_HANDLER_H */
