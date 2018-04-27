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

#include "ApiHandlers.h"

#include "FreeRTOS_IP.h"
#include "FreeRTOS_HTTP_commands.h"

extern void vHandleGatewayApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount);

extern void vHandleNetworkApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount);

extern void vHandleRadioApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount);

extern void vHandleCoreDiscoApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount);

extern void vHandleCoreHcProxyApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount);


ApiHandler_t xApiHandlers[] =
{
	{ 7, "/gateway", vHandleGatewayApi },
	{ 8, "/network", vHandleNetworkApi },
	{ 5, "/radio", vHandleRadioApi },
	{ 4, "/hc/", vHandleCoreHcProxyApi },
	{ 17, "/.well-known/core", vHandleCoreDiscoApi }
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
		"Access-Control-Allow-Origin: *\r\n"
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

BaseType_t xParseId(const char *pcCurrent, BaseType_t *pxId)
{
	char *pcStop;

	errno = 0;

	*pxId = strtol(pcCurrent, &pcStop, 10);

	return !(errno == ERANGE || pcCurrent == pcStop);
}