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

#include <stdio.h>
#include <string.h>

#include "jsmn.h"
#include "Json.h"

void vJsonInit(JsonGenerator_t *pxGenerator, char *pcBuffer, BaseType_t xBufferSize)
{
	pxGenerator->pcBuffer = pcBuffer;
	pxGenerator->pcBuffer[0] = '\0';
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
		strcpy(cFormat, "true");
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

eProcessPatchResult xProcessPatchDocument(char *pcJson, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount, xProcessPatch_t xProcessPatch)
{
	unsigned char ucOffset = 1;
	jsmntok_t *pxPathToken = 0;
	jsmntok_t *pxFromToken = 0;
	jsmntok_t *pxValueToken = 0;
	eOperationType_t xOperation;
	eProcessPatchResult xResult;

	char cBuffer[100];

	if (xJsonTokenCount > 0)
	{
		if (pxTokens[0].type == JSMN_ARRAY)
		{
			for (unsigned char i = 0; i < pxTokens[0].size; i++)
			{
				xResult = xParsePatchOperation(pcJson, &pxTokens[ucOffset + i], &xOperation, &pxPathToken, &pxValueToken, &pxFromToken);

				if (xResult != eProcessed)
				{
					return xResult;
				}

				ucOffset += 5;

				switch (xOperation)
				{
				case eAdd:
				case eReplace:
				case eTest:
					if (!pxPathToken || !pxValueToken)
						return eMissingTokens;

					ucOffset += 2;
					break;
				case eMove:
				case eCopy:
					if (!pxPathToken || !pxFromToken)
						return eMissingTokens;

					ucOffset += 2;
					break;
				case eRemove:
					if (!pxPathToken)
						return eMissingTokens;
					break;
				}

				if (!xProcessPatch(pcJson, xOperation, pxPathToken, pxValueToken, pxFromToken))
				{
					return eProcessingFailed;
				}
			}
		}
	}

	return eProcessed;
}

BaseType_t xParsePatchOperation(char * pcJson, jsmntok_t * pxTokens, eOperationType_t *pxOperation, jsmntok_t **pxPathToken, jsmntok_t **pxValueToken, jsmntok_t **pxFromToken)
{
	if (pxTokens[0].type == JSMN_OBJECT && pxTokens[0].size >= 2)
	{
		unsigned char ucMaxCount = 6;
		unsigned char ucIndex = 0;

		do
		{
			if (pxTokens[++ucIndex].type != JSMN_STRING)
			{
				return eInvalidNode;
			}

			char *pStr = pcJson + pxTokens[ucIndex].start;
			unsigned char ucLen = pxTokens[ucIndex].end - pxTokens[ucIndex].start;

			if (strncmp(pStr, "op", ucLen) == 0)
			{
				ucIndex++;

				pStr = pcJson + pxTokens[ucIndex].start;
				ucLen = pxTokens[ucIndex].end - pxTokens[ucIndex].start;

				if (_strnicmp(pStr, "add", ucLen) == 0)
				{
					*pxOperation = eAdd;
				}
				else if (_strnicmp(pStr, "remove", ucLen) == 0)
				{
					*pxOperation = eRemove;
					ucMaxCount = 4;
				}
				else if (_strnicmp(pStr, "replace", ucLen) == 0)
				{
					*pxOperation = eReplace;
				}
				else if (_strnicmp(pStr, "move", ucLen) == 0)
				{
					*pxOperation = eMove;
				}
				else if (_strnicmp(pStr, "copy", ucLen) == 0)
				{
					*pxOperation = eCopy;
				}
				else if (_strnicmp(pStr, "test", ucLen) == 0)
				{
					*pxOperation = eTest;
				}
				else
				{
					return eInvalidOp;
				}
			}
			else if (strncmp(pStr, "path", ucLen) == 0)
			{
				*pxPathToken = &pxTokens[++ucIndex];
			}
			else if (strncmp(pStr, "value", ucLen) == 0)
			{
				*pxValueToken = &pxTokens[++ucIndex];
			}
			else if (strncmp(pStr, "from", ucLen) == 0)
			{
				*pxFromToken = &pxTokens[++ucIndex];
			}

		} while (ucIndex < ucMaxCount);

	}

	return eProcessed;
}