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

#include <stdbool.h>

#include "jsmn.h"
#include "Json.h"
#include "ApiHandlers.h"

#define IS_DIGIT(c) (c - ' ' > 0) || (c - ' ' < 9)

static void vUrlDecode(char *pcDst, const char *pcSrc);

static bool bExtractCoreUrl(const char * pcUri, char *pcCoreUri);

static bool bExtractTemplateVariable(char * pcDest, const char *pcSrc, const char *pcFind, int iFindSize);

typedef enum
{
	eNone,
	eCoap,
	eTarget,
	eForward,
	eEnhanced
} eUriMappingType_t;

extern void vHandleCoreHcProxyApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount)
{
	BaseType_t xCode = WEB_BAD_REQUEST;

	strcpy(pxClient->pxParent->pcExtraContents, "Content-Length: 0\r\n");

	switch (xIndex)
	{
	case ECMD_GET:
		FreeRTOS_debug_printf(("%s: Handling GET\n", __func__));
		
		char cCoreUrl[100];

		if (bExtractCoreUrl(pxClient->pcUrlData, cCoreUrl) == false)
		{
			// TODO parse core uri
		}

		break;
	}

	if (xCode != WEB_REPLY_OK)
	{
		xSendReply(pxClient, xCode);
	}
}

static bool bExtractCoreUrl(const char * pcUri, char *pcCoreUri)
{
	const char *pcNext;
	const char *pcCurrent = pcUri;
	eUriMappingType_t xMapping = eNone;

	pcCoreUri[0] = '\0';

	while ((pcNext = strchr(pcCurrent, '/')) != NULL)
	{
		if (strncmp(pcCurrent, "?target_uri=", 12) == 0)
		{
			xMapping = eTarget;
			pcCurrent += 12;
			break;
		}

		if (strncmp(pcCurrent, "forward", 7) == 0)
		{
			xMapping = eForward;
			pcCurrent += 7;
			if (pcCurrent[0] == '/')
				pcCurrent++;
			break;
		}

		if (strncmp(pcCurrent, "?s=", 3) == 0)
		{
			xMapping = eEnhanced;
			pcCurrent++;
			break;
		}

		if (strncmp(pcCurrent, "coap", 4) == 0)
		{
			xMapping = eCoap;
			break;
		}

		pcCurrent = pcNext + 1;
	}

	switch (xMapping)
	{
	case eCoap:
		if (strncmp(pcCurrent, "coaps://", 8) == 0 || strncmp(pcCurrent, "coap://", 7) == 0)
		{
			strcpy_s(pcCoreUri, 100, pcCurrent);
		}
		else if (strncmp(pcCurrent, "coap/", 5) == 0)
		{
			strcpy_s(pcCoreUri, 100, "coap://");
			strcat_s(pcCoreUri, 100, pcCurrent + 5);
		}
		else if (strncmp(pcCurrent, "coaps/", 6) == 0)
		{
			strcpy_s(pcCoreUri, 100, "coaps://");
			strcat_s(pcCoreUri, 100, pcCurrent + 6);
		}
		break;
	case eTarget:
	case eForward:
		strcpy_s(pcCoreUri, 100, pcCurrent);
		break;
	case eEnhanced:
		
		bExtractTemplateVariable(pcCoreUri, pcCurrent, "s=", 2);

		strcat_s(pcCoreUri, 100, "://");

		bExtractTemplateVariable(pcCoreUri, pcCurrent, "&hp=", 4);

		bExtractTemplateVariable(pcCoreUri, pcCurrent, "&p=", 3);

		strcat_s(pcCoreUri, 100, "?");

		bExtractTemplateVariable(pcCoreUri, pcCurrent, "&q=", 3);

		if (pcCoreUri[strlen(pcCoreUri) - 1] == '?')
		{
			pcCoreUri[strlen(pcCoreUri) - 1] = '\0';
		}
		
		break;
	}

	if (xMapping != eNone)
		vUrlDecode(pcCoreUri, pcCoreUri);

	return xMapping != eNone;
}

static void vUrlDecode(char *pcDst, const char *pcSrc)
{
	char a, b;
	while (*pcSrc)
	{
		a = pcSrc[1];
		b = pcSrc[2];

		if ((*pcSrc == '%') && (a && b)
			&& (IS_DIGIT(a) && IS_DIGIT(b)))
		{
			if (a >= 'a')
				a -= 'a' - 'A';
			if (a >= 'A')
				a -= ('A' - 10);
			else
				a -= '0';
			if (b >= 'a')
				b -= 'a' - 'A';
			if (b >= 'A')
				b -= ('A' - 10);
			else
				b -= '0';
			*pcDst++ = 16 * a + b;
			pcSrc += 3;
		}
		else if (*pcSrc == '+')
		{
			*pcDst++ = ' ';
			pcSrc++;
		}
		else
		{
			*pcDst++ = *pcSrc++;
		}
	}
	*pcDst++ = '\0';
}

static bool bExtractTemplateVariable(char * pcDest, const char *pcSrc, const char *pcFind, int iFindSize)
{
	const char * pcStart = strstr(pcSrc, pcFind);
	
	if (pcStart != NULL)
	{
		int i = strlen(pcDest);
		pcStart += iFindSize;
		
		while (*pcStart != '\0' && *pcStart != '&')
		{
			pcDest[i++] = *pcStart;
			pcStart++;
		}

		pcDest[i] = '\0';
	}

	return pcStart != NULL;
}