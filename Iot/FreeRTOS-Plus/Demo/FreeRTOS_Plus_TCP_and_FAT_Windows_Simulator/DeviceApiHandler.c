#include <FreeRTOS.h>
#include "task.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_server_private.h"

#include "FreeRTOS_HTTP_io.h"
#include "FreeRTOS_HTTP_commands.h"

#include "jsmn.h"
#include "Json.h"
#include "ApiHandlers.h"

#define DEVICENAME "devicename"
#define NICKNAME "nickname"

static char cDeviceName[100] = "modiot";
static char cNickName[100] = "nick";

BaseType_t xHandleDeviceJsonParse(char *pcJson, jsmntok_t *pxTokens, BaseType_t xTokensCount, BaseType_t *pxIndex)
{
BaseType_t xResult = 0;
BaseType_t xLen = 0;
char *pcValue = NULL;
char cKey[11];

	(xTokensCount);

	for (size_t i = 0; i < 2; i++)
	{
		if (i == 0)
		{
			strcpy(cKey, DEVICENAME);
			pcValue = cDeviceName;
			xLen = 10;
		}
		else
		{
			strcpy(cKey, NICKNAME);
			pcValue = cNickName;
			xLen = 8;
		}

		if (strncmp(pcJson + pxTokens[*pxIndex].start, cKey, xLen) == 0)
		{
			if (pxTokens[*pxIndex].size == 1)
			{
				(*pxIndex)++;
				strncpy(pcValue, pcJson + pxTokens[*pxIndex].start, pxTokens[*pxIndex].end - pxTokens[*pxIndex].start);
				pcValue[pxTokens[*pxIndex].end - pxTokens[*pxIndex].start] = '\0';
				break;
			}
			else
			{
				xResult = eJsonInvalidPair;
			}
		}
	}

	return xResult;
}

void vHandleDeviceApi( HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xNumTokens )
{
BaseType_t xCode = 0;

	strcpy(pxClient->pxParent->pcExtraContents, "Content-Length: 0\r\n");

	switch( xIndex )
	{
	case ECMD_GET:
			FreeRTOS_debug_printf( ( "%s: Handling GET\n", __func__ ) );
			snprintf( pxClient->pxParent->pcCommandBuffer, sizeof( pxClient->pxParent->pcCommandBuffer ),
				"{\"%s\": \"%s\", \"%s\": \"%s\" }", DEVICENAME, cDeviceName, NICKNAME, cNickName );
			xSendApiResponse( pxClient );
		break;
	case ECMD_PATCH:
			FreeRTOS_debug_printf( ( "%s: Handling PATCH\n", __func__ ) );

			BaseType_t xRc = xParseJson( pcPayload, xHandleDeviceJsonParse );

			if (xRc > 0)
			{
				xCode = WEB_REPLY_OK;
			}
			else
			{
				xCode = WEB_BAD_REQUEST;
			}
		break;
	default:
			xCode = WEB_BAD_REQUEST;
		break;
	}

	if (xCode != 0)
	{
		xSendReply( pxClient, xCode );
	}
}
