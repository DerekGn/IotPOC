#include "ApiHandlers.h"

#include "FreeRTOS_IP.h"

extern void vHandleGatewayApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xNumTokens);

extern void vHandleNetworkApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xNumTokens);

extern void vHandleRadioApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xNumTokens);

ApiHandler_t xApiHandlers[] =
{
	{ 7, "/gateway", vHandleGatewayApi },
	{ 8, "/network", vHandleNetworkApi },
	{ 5, "/radio", vHandleRadioApi }
};

BaseType_t xApiHandlerEntries( void )
{
	return (BaseType_t) ( sizeof( xApiHandlers ) / sizeof( ApiHandler_t ) );
}

BaseType_t xSendApiResponse(HTTPClient_t *pxClient)
{
	struct xTCP_SERVER *pxParent = pxClient->pxParent;
	BaseType_t xRc;

	/* A normal command reply on the main socket (port 21). */
	char *pcBuffer = pxParent->pcFileBuffer;

	snprintf(pxClient->pxParent->pcExtraContents, sizeof(pxClient->pxParent->pcExtraContents),
		"Content-Length: %d\r\n", (int)strlen(pxClient->pxParent->pcCommandBuffer));

	xRc = snprintf(pcBuffer, sizeof(pxParent->pcFileBuffer),
		"HTTP/1.1 %d %s\r\n"
#if	USE_HTML_CHUNKS
		"Transfer-Encoding: chunked\r\n"
#endif
		"Content-Type: application/json\r\n"
		"Connection: keep-alive\r\n"
		"%s\r\n"
		"%s",
		(int)200,
		webCodename(200),
		pxParent->pcExtraContents,
		pxParent->pcCommandBuffer);

	pxParent->pcContentsType[0] = '\0';
	pxParent->pcExtraContents[0] = '\0';

	xRc = FreeRTOS_send(pxClient->xSocket, (const void *)pcBuffer, xRc, 0);
	pxClient->bits.bReplySent = pdTRUE_UNSIGNED;

	return xRc;
}
