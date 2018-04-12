#ifndef FREERTOS_HTTP_IO_H
#define	FREERTOS_HTTP_IO_H

/* FreeRTOS */
#include <FreeRTOS.h>

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_server_private.h"

void vFileClose(HTTPClient_t *pxClient);
BaseType_t xSendFile(HTTPClient_t *pxClient);
const char *pcGetContentsType(const char *apFname);
BaseType_t xSendReply(HTTPClient_t *pxClient, BaseType_t xCode);

#endif FREERTOS_HTTP_IO_H  /* FREERTOS_HTTP_IO_H */