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
#include "CoapHttp.h"
#include "CoreLink.h"
#include "MediaTypes.h"
#include "ApiHandlers.h"
#include "CoreResourceDirectory.h"

void vAddRdAnchor(JsonGenerator_t *jsonGenerator);
void vAddRdGroupAnchor(JsonGenerator_t *jsonGenerator);
void vAddHcProxyAnchor(JsonGenerator_t *jsonGenerator);
void vAddRdLookupGroupAnchor(JsonGenerator_t * jsonGenerator);
void vAddRdLookupEndpointAnchor(JsonGenerator_t * jsonGenerator);
void vAddRdLookupResourceAnchor(JsonGenerator_t * jsonGenerator);

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
        
		//bIsQuerySpecified();

		//if (bIsProxyDiscovery(pxClient->pcUrlData))
		//{
			JsonGenerator_t xGenerator;

			vJsonInit(&xGenerator, pxClient->pxParent->pcCommandBuffer, sizeof(pxClient->pxParent->pcCommandBuffer));
			
			vJsonAddValue(&xGenerator, eArray, "");

			vAddHcProxyAnchor(&xGenerator);
			
			vJsonCloseNode(&xGenerator, eValue);
			
			vAddRdAnchor(&xGenerator);

			vJsonCloseNode(&xGenerator, eValue);

			vAddRdLookupEndpointAnchor(&xGenerator);

			vJsonCloseNode(&xGenerator, eValue);

			vAddRdLookupResourceAnchor(&xGenerator);

			vJsonCloseNode(&xGenerator, eValue);

			vAddRdLookupGroupAnchor(&xGenerator);

			vJsonCloseNode(&xGenerator, eValue);

			vAddRdGroupAnchor(&xGenerator);
			
			vJsonCloseNode(&xGenerator, eArray);

			xCode = WEB_REPLY_OK;
			xSendApiResponse(pxClient, MEDIA_TYPE_APP_LINK_FORMAT_JSON);
		//}
		//else if(bValidQuery(pxClient->pcUrlData, bHRef, bAttribute))
		//{
		//}

        break;
    }

    if (xCode != WEB_REPLY_OK)
    {
        xSendReply(pxClient, xCode);
    }
}

void vAddHcProxyAnchor(JsonGenerator_t *jsonGenerator)
{
	vJsonAddValue(jsonGenerator, eObject, "");
	vJsonOpenKey(jsonGenerator, CORE_HREF);
	vJsonAddValue(jsonGenerator, eString, COAP_HTTP_PROXY_HREF);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, CORE_RESOURCE_TYPE);
	vJsonAddValue(jsonGenerator, eString, COAP_HTTP_PROXY_RESOURCE_TYPE);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_URI_TEMPLATE_ATTRIBUTE);
	vJsonAddValue(jsonGenerator, eString, COAP_HTTP_URI_MAPPING_TEMPLATE);
	vJsonCloseNode(jsonGenerator, eObject);
}

void vAddRdAnchor(JsonGenerator_t * jsonGenerator)
{
	vJsonAddValue(jsonGenerator, eObject, "");
	vJsonOpenKey(jsonGenerator, CORE_HREF);
	vJsonAddValue(jsonGenerator, eString, CORE_RESRC_DIR_HREF);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, CORE_RESOURCE_TYPE);
	vJsonAddValue(jsonGenerator, eString, CORE_RESRC_DIR_RT_RES_DIR);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_CONTENT_TYPE_ATTR);
	vJsonAddValue(jsonGenerator, eNumber, "40");
	vJsonCloseNode(jsonGenerator, eObject);
}

void vAddRdGroupAnchor(JsonGenerator_t * jsonGenerator)
{
	vJsonAddValue(jsonGenerator, eObject, "");
	vJsonOpenKey(jsonGenerator, CORE_HREF);
	vJsonAddValue(jsonGenerator, eString, CORE_RESRC_DIR_GRP_HREF);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, CORE_RESOURCE_TYPE);
	vJsonAddValue(jsonGenerator, eString, CORE_RESRC_DIR_RT_GROUP);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_CONTENT_TYPE_ATTR);
	vJsonAddValue(jsonGenerator, eNumber, "40");
	vJsonCloseNode(jsonGenerator, eObject);
}

void vAddRdLookupEndpointAnchor(JsonGenerator_t * jsonGenerator)
{
	vJsonAddValue(jsonGenerator, eObject, "");
	vJsonOpenKey(jsonGenerator, CORE_HREF);
	vJsonAddValue(jsonGenerator, eString, CORE_RESRC_DIR_LU_EP_HREF);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, CORE_RESOURCE_TYPE);
	vJsonAddValue(jsonGenerator, eString, CORE_RESRC_DIR_RT_LOOKUP_EP);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_CONTENT_TYPE_ATTR);
	vJsonAddValue(jsonGenerator, eNumber, "40");
	vJsonCloseNode(jsonGenerator, eObject);
}

void vAddRdLookupGroupAnchor(JsonGenerator_t * jsonGenerator)
{
	vJsonAddValue(jsonGenerator, eObject, "");
	vJsonOpenKey(jsonGenerator, CORE_HREF);
	vJsonAddValue(jsonGenerator, eString, CORE_RESRC_DIR_LU_GRP_HREF);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, CORE_RESOURCE_TYPE);
	vJsonAddValue(jsonGenerator, eString, CORE_RESRC_DIR_RT_LOOKUP_GRP);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_CONTENT_TYPE_ATTR);
	vJsonAddValue(jsonGenerator, eNumber, "40");
	vJsonCloseNode(jsonGenerator, eObject);
}

void vAddRdLookupResourceAnchor(JsonGenerator_t * jsonGenerator)
{
	vJsonAddValue(jsonGenerator, eObject, "");
	vJsonOpenKey(jsonGenerator, CORE_HREF);
	vJsonAddValue(jsonGenerator, eString, CORE_RESRC_DIR_LU_RES_HREF);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, CORE_RESOURCE_TYPE);
	vJsonAddValue(jsonGenerator, eString, CORE_RESRC_DIR_RT_LOOKUP_RES);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_CONTENT_TYPE_ATTR);
	vJsonAddValue(jsonGenerator, eNumber, "40");
	vJsonCloseNode(jsonGenerator, eObject);
}