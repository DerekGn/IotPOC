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

#include <FreeRTOS.h>
#include "task.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_TCP_server.h"
#include "FreeRTOS_server_private.h"

#include "FreeRTOS_HTTP_io.h"
#include "FreeRTOS_HTTP_commands.h"

#include "jsmn.h"
#include "Json.h"
#include "HttpCoap.h"
#include "CoreLink.h"
#include "MediaTypes.h"
#include "ApiHandlers.h"

BaseType_t bValidQuery(const char *pcUrlData, BaseType_t bHRef, BaseType_t bAttribute);

BaseType_t bIsProxyDiscovery(const char *pcUrlData);

// Filtering may be performed on any of the link format attributes [GET /.well-known/core?rt=temperature-c] 
// 4.1.  Query Filtering /.well-known/core{?search*}
void vHandleHcProxyDiscoApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount)
{
BaseType_t xCode = WEB_BAD_REQUEST;
BaseType_t bHRef = pdFALSE;
BaseType_t bAttribute = pdFALSE;

    strcpy(pxClient->pxParent->pcExtraContents, "Content-Length: 0\r\n");

    switch (xIndex)
    {
    case ECMD_GET:
        FreeRTOS_debug_printf(("%s: Handling GET\n", __func__));
        
		if (bIsProxyDiscovery(pxClient->pcUrlData))
		{
			JsonGenerator_t xGenerator;

			vJsonInit(&xGenerator, pxClient->pxParent->pcCommandBuffer, sizeof(pxClient->pxParent->pcCommandBuffer));
			vJsonAddValue(&xGenerator, eArray, "");
			vJsonAddValue(&xGenerator, eObject, "");
			vJsonOpenKey(&xGenerator, CORE_HREF);
			vJsonAddValue(&xGenerator, eString, "/hc/");
			vJsonCloseNode(&xGenerator, eString);
			vJsonOpenKey(&xGenerator, CORE_RESOURCE_TYPE);
			vJsonAddValue(&xGenerator, eString, CORE_HTTP_CORE_RT);
			vJsonCloseNode(&xGenerator, eObject);
			vJsonCloseNode(&xGenerator, eArray);

			xCode = WEB_REPLY_OK;
			xSendApiResponse(pxClient, MEDIA_TYPE_APP_LINK_FORMAT_JSON);
		}
		else if(bValidQuery(pxClient->pcUrlData, bHRef, bAttribute))
		{

		}

        break;
    }

    if (xCode != WEB_REPLY_OK)
    {
        xSendReply(pxClient, xCode);
    }
}

BaseType_t bValidQuery( const char *pcUrlData, BaseType_t bHRef, BaseType_t bAttribute )
{
	return pdFALSE;
}

BaseType_t bIsProxyDiscovery(const char *pcUrlData)
{
	return strstr(pcUrlData, "?rt=core.hc") != NULL;
}
