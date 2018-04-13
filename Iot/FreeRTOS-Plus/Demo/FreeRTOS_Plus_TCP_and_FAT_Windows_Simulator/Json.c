#include <FreeRTOS.h>

#include <stdio.h>
#include <string.h>

#include "jsmn.h"
#include "Json.h"

BaseType_t xParseJson(char *pcJson, xJsonNodeHandler_t xJsonNodeHandler)
{
	jsmn_parser xParser;
	jsmntok_t xTokens[10];

	jsmn_init(&xParser);

	BaseType_t xRc = jsmn_parse(&xParser, pcJson, strlen(pcJson), xTokens, 10);

	if (xRc == 0)
	{
		xRc = eJsonEmpty;
	}
	else
	{
		BaseType_t i = 1;

		while (i < xRc)
		{
			if (i == 0 && xTokens[0].type != JSMN_OBJECT)
			{
				xRc = eJsonInvalidStartCount;
				break;
			}
			else if (xTokens[i].type == JSMN_STRING)
			{
				BaseType_t xProcess = xJsonNodeHandler(pcJson, xTokens, xRc, &i) < 0;

				if (xProcess < 0)
				{
					xRc = xProcess;
					break;
				}
			}

			i++;
		}
	}

	return xRc;
}

void vJsonInit(JsonGenerator_t *pxGenerator, char *pcBuffer, BaseType_t xBufferSize)
{
	pxGenerator->pcBuffer = pcBuffer;
	strcpy_s(pxGenerator->pcBuffer, xBufferSize, "{");
}

void vJsonOpenKey(JsonGenerator_t *pxGenerator, const char * pcName)
{
	snprintf(pxGenerator->pcBuffer + strlen(pxGenerator->pcBuffer), pxGenerator->xBufferSize, "\"%s\":", pcName);
}

void vJsonAddValue(JsonGenerator_t *pxGenerator, eValueType_t xValueType, const char *pcValue)
{
	char cFormat[10];

	switch (xValueType)
	{
	case eString:
		strcpy_s(cFormat, 10, "\"%s\"");
		break;
	case eNumber:
		strcpy_s(cFormat, 10, "%s");
		break;
	case eObject:
		strcpy_s(cFormat, 10, "{");
		break;
	case eArray:
		strcpy_s(cFormat, 10, "[%s");
		break;
	case eTrue:
		strcpy_s(cFormat, 10, "true");
		break;
	case eFalse:
		strcpy_s(cFormat, 10, "false");
		break;
	case eNull:
		strcpy_s(cFormat, 10, "null");
		break;
	}

	snprintf(pxGenerator->pcBuffer + strlen(pxGenerator->pcBuffer), pxGenerator->xBufferSize, cFormat, pcValue);
}

void vJsonCloseNode(JsonGenerator_t *pxGenerator, eValueType_t xValueType)
{
	char cDelimiter[3];

	switch (xValueType)
	{
	case eValue:
	case eString:
	case eNumber:
	case eTrue:
	case eFalse:
	case eNull:
		strcpy_s(cDelimiter, 3, ",");
		break;
	case eObject:
		strcpy_s(cDelimiter, 3, "}");
		break;
	case eArray:
		strcpy_s(cDelimiter, 3, "]");
		break;
	}

	strcat_s(pxGenerator->pcBuffer, pxGenerator->xBufferSize, cDelimiter);
}
