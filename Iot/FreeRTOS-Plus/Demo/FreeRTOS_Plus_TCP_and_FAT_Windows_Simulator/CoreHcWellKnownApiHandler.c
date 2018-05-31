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

#include <string.h>

#include "jsmn.h"
#include "Json.h"
#include "Coap.h"
#include "CoapHttp.h"
#include "CoreLink.h"
#include "MediaTypes.h"
#include "ApiHandlers.h"
#include "CoreRescDir.h"

typedef enum
{
	eAll = 0x3F,
	eResDir = 1,
	eGroup = 2,
	eLookupResources = 4,
	eLookupEndpoint = 8,
	eLookupGroup = 16,
	eHttpProxy = 32
} eResourceTypeQuery_t;

eResourceTypeQuery_t xExtractQuery(const char *pcUrlData);

void vAddRdAnchor(JsonGenerator_t *jsonGenerator);
void vAddRdGroupAnchor(JsonGenerator_t *jsonGenerator);
void vAddHcProxyAnchor(JsonGenerator_t *jsonGenerator);
void vAddRdLookupGroupAnchor(JsonGenerator_t * jsonGenerator);
void vAddRdLookupEndpointAnchor(JsonGenerator_t * jsonGenerator);
void vAddRdLookupResourceAnchor(JsonGenerator_t * jsonGenerator);

/*
* Implements the core wellknown endpoint handling
* https://tools.ietf.org/html/draft-ietf-core-resource-directory-13#section-5.2
*/
void vHandleWellKnownApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount)
{
BaseType_t xCode = WEB_BAD_REQUEST;

    strcpy(pxClient->pxParent->pcExtraContents, "Content-Length: 0\r\n");

    switch (xIndex)
    {
	case ECMD_POST:
		FreeRTOS_debug_printf(("%s: Handling POST\n", __func__));
		//TODO simple registration for https://tools.ietf.org/html/draft-ietf-core-resource-directory-13#section-5.3.1
		break;
    case ECMD_GET:
        FreeRTOS_debug_printf(("%s: Handling GET\n", __func__));
        
		eResourceTypeQuery_t query = xExtractQuery(pxClient->pcUrlData);
		
		JsonGenerator_t xGenerator;

		vJsonInit(&xGenerator, pxClient->pxParent->pcCommandBuffer, sizeof(pxClient->pxParent->pcCommandBuffer));
		vJsonAddValue(&xGenerator, eArray, "");

		if((query & eHttpProxy) == eHttpProxy)
			vAddHcProxyAnchor(&xGenerator);
		
		if ((query & eResDir) == eResDir)
		{
			if ((query & eHttpProxy) == eHttpProxy)
				vJsonCloseNode(&xGenerator, eValue);

			vAddRdAnchor(&xGenerator);
		}
		
		if ((query & eLookupEndpoint) == eLookupEndpoint)
		{
			if ((query & eResDir) == eResDir)
				vJsonCloseNode(&xGenerator, eValue);

			vAddRdLookupEndpointAnchor(&xGenerator);
		}
		
		if ((query & eLookupResources) == eLookupResources)
		{
			if ((query & eLookupEndpoint) == eLookupEndpoint)
				vJsonCloseNode(&xGenerator, eValue);

			vAddRdLookupResourceAnchor(&xGenerator);
		}
		
		if ((query & eLookupGroup) == eLookupGroup)
		{
			if ((query & eLookupResources) == eLookupResources)
				vJsonCloseNode(&xGenerator, eValue);
			
			vAddRdLookupGroupAnchor(&xGenerator);
		}
		
		if ((query & eGroup) == eGroup)
		{
			if ((query & eLookupGroup) == eLookupGroup)
				vJsonCloseNode(&xGenerator, eValue);
			
			vAddRdGroupAnchor(&xGenerator);
		}

		vJsonCloseNode(&xGenerator, eArray);

		xCode = WEB_REPLY_OK;
		xSendApiResponse(pxClient, MEDIA_TYPE_APP_LINK_FORMAT_JSON);

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
	vJsonAddValue(jsonGenerator, eString, COAP_HTTP_PROXY_RT);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_URI_TEMPLATE_ATTRIBUTE);
	vJsonAddValue(jsonGenerator, eString, COAP_HTTP_URI_MAPPING_TEMPLATE);
	vJsonCloseNode(jsonGenerator, eObject);
}

void vAddRdAnchor(JsonGenerator_t * jsonGenerator)
{
	vJsonAddValue(jsonGenerator, eObject, "");
	vJsonOpenKey(jsonGenerator, CORE_HREF);
	vJsonAddValue(jsonGenerator, eString, CORE_RSRC_DIR_HREF);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, CORE_RESOURCE_TYPE);
	vJsonAddValue(jsonGenerator, eString, CORE_RSRC_DIR_RT_RES_DIR);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_CONTENT_TYPE_ATTR);
	vJsonAddValue(jsonGenerator, eNumber, COAP_CONTENT_TYPE_APP_LINK_FORMAT);
	vJsonCloseNode(jsonGenerator, eObject);
}

void vAddRdGroupAnchor(JsonGenerator_t * jsonGenerator)
{
	vJsonAddValue(jsonGenerator, eObject, "");
	vJsonOpenKey(jsonGenerator, CORE_HREF);
	vJsonAddValue(jsonGenerator, eString, CORE_RSRC_DIR_GRP_HREF);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, CORE_RESOURCE_TYPE);
	vJsonAddValue(jsonGenerator, eString, CORE_RSRC_DIR_RT_GROUP);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_CONTENT_TYPE_ATTR);
	vJsonAddValue(jsonGenerator, eNumber, COAP_CONTENT_TYPE_APP_LINK_FORMAT);
	vJsonCloseNode(jsonGenerator, eObject);
}

void vAddRdLookupEndpointAnchor(JsonGenerator_t * jsonGenerator)
{
	vJsonAddValue(jsonGenerator, eObject, "");
	vJsonOpenKey(jsonGenerator, CORE_HREF);
	vJsonAddValue(jsonGenerator, eString, CORE_RSRC_DIR_LU_EP_HREF);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, CORE_RESOURCE_TYPE);
	vJsonAddValue(jsonGenerator, eString, CORE_RSRC_DIR_RT_LOOKUP_EP);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_CONTENT_TYPE_ATTR);
	vJsonAddValue(jsonGenerator, eNumber, COAP_CONTENT_TYPE_APP_LINK_FORMAT);
	vJsonCloseNode(jsonGenerator, eObject);
}

void vAddRdLookupGroupAnchor(JsonGenerator_t * jsonGenerator)
{
	vJsonAddValue(jsonGenerator, eObject, "");
	vJsonOpenKey(jsonGenerator, CORE_HREF);
	vJsonAddValue(jsonGenerator, eString, CORE_RSRC_DIR_LU_GRP_HREF);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, CORE_RESOURCE_TYPE);
	vJsonAddValue(jsonGenerator, eString, CORE_RSRC_DIR_RT_LOOKUP_GRP);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_CONTENT_TYPE_ATTR);
	vJsonAddValue(jsonGenerator, eNumber, COAP_CONTENT_TYPE_APP_LINK_FORMAT);
	vJsonCloseNode(jsonGenerator, eObject);
}

void vAddRdLookupResourceAnchor(JsonGenerator_t * jsonGenerator)
{
	vJsonAddValue(jsonGenerator, eObject, "");
	vJsonOpenKey(jsonGenerator, CORE_HREF);
	vJsonAddValue(jsonGenerator, eString, CORE_RSRC_DIR_LU_RES_HREF);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, CORE_RESOURCE_TYPE);
	vJsonAddValue(jsonGenerator, eString, CORE_RSRC_DIR_RT_LOOKUP_RES);
	vJsonCloseNode(jsonGenerator, eString);
	vJsonOpenKey(jsonGenerator, COAP_HTTP_CONTENT_TYPE_ATTR);
	vJsonAddValue(jsonGenerator, eNumber, COAP_CONTENT_TYPE_APP_LINK_FORMAT);
	vJsonCloseNode(jsonGenerator, eObject);
}

eResourceTypeQuery_t xExtractQuery(const char * pcUrlData)
{
	eResourceTypeQuery_t result = eAll;

	char *pcQuery = strstr(pcUrlData, "?rt=");

	if (pcQuery != NULL)
	{
		pcQuery += 4;

		if (strncmp(pcQuery, COAP_HTTP_PROXY_RT, strlen(COAP_HTTP_PROXY_RT)) == 0)
			result = eHttpProxy;
		else if (strncmp(pcQuery, CORE_RSRC_DIR_LOOKUP_MATCH, strlen(CORE_RSRC_DIR_LOOKUP_MATCH)) == 0)
			result = eLookupResources | eLookupEndpoint | eLookupGroup;
		else if (strncmp(pcQuery, CORE_RSRC_DIR_RES_DIR_MATCH, strlen(CORE_RSRC_DIR_RES_DIR_MATCH)) == 0)
			result = eResDir | eGroup | eLookupResources | eLookupEndpoint | eLookupGroup;
		else if (strncmp(pcQuery, CORE_RSRC_DIR_RT_LOOKUP_RES, strlen(CORE_RSRC_DIR_RT_LOOKUP_RES)) == 0)
			result = eLookupResources;
		else if (strncmp(pcQuery, CORE_RSRC_DIR_RT_LOOKUP_GRP, strlen(CORE_RSRC_DIR_RT_LOOKUP_GRP)) == 0)
			result = eLookupGroup;
		else if (strncmp(pcQuery, CORE_RSRC_DIR_RT_LOOKUP_EP, strlen(CORE_RSRC_DIR_RT_LOOKUP_EP)) == 0)
			result = eLookupEndpoint;
		else if (strncmp(pcQuery, CORE_RSRC_DIR_RT_GROUP, strlen(CORE_RSRC_DIR_RT_GROUP)) == 0)
			result = eGroup;
		else if (strncmp(pcQuery, CORE_RSRC_DIR_RT_RES_DIR, strlen(CORE_RSRC_DIR_RT_RES_DIR)) == 0)
			result = eResDir;
	}

	return result;
}