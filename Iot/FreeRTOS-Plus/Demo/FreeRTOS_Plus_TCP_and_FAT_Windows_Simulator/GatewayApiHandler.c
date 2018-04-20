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
#include "ApiHandlers.h"

#define GATEWAYNAME "name"
#define NICKNAME "nickname"

static char cName[100] = "modiot";
static char cNickName[100] = "nick";

static BaseType_t xProcessPatchOperation(char *pcJson, eOperationType_t xOperation, jsmntok_t *pxPathToken, jsmntok_t *pxValueToken, jsmntok_t *pxFromToken);

void vHandleGatewayApi( HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount )
{
BaseType_t xCode = WEB_BAD_REQUEST;

	strcpy(pxClient->pxParent->pcExtraContents, "Content-Length: 0\r\n");

	switch( xIndex )
	{
	case ECMD_GET:
			FreeRTOS_debug_printf( ( "%s: Handling GET\n", __func__ ) );

			JsonGenerator_t xGenerator;

			vJsonInit(&xGenerator, pxClient->pxParent->pcCommandBuffer, sizeof(pxClient->pxParent->pcCommandBuffer));
			vJsonAddValue(&xGenerator, eObject, "");
			vJsonOpenKey(&xGenerator, GATEWAYNAME);
			vJsonAddValue(&xGenerator, eString, cName);
			vJsonCloseNode(&xGenerator, eString);
			vJsonOpenKey(&xGenerator, NICKNAME);
			vJsonAddValue(&xGenerator, eString, cNickName);
			vJsonCloseNode(&xGenerator, eObject);
			
			xCode = WEB_REPLY_OK;
			xSendApiResponse( pxClient );
		break;
	case ECMD_PATCH:
			FreeRTOS_debug_printf( ( "%s: Handling PATCH\n", __func__ ) );

			BaseType_t xResult = xProcessPatchDocument( pcPayload, pxTokens, xJsonTokenCount, xProcessPatchOperation );

			if(xResult == eProcessed)
			{
				// TODO apply settings when all processing is complete

				xCode = WEB_REPLY_OK;
			}
		break;
	}

	if (xCode != WEB_BAD_REQUEST)
	{
		xSendReply( pxClient, xCode );
	}
}

static BaseType_t xProcessPatchOperation(char * pcJson, eOperationType_t xOperation, jsmntok_t *pxPathToken, jsmntok_t *pxValueToken, jsmntok_t *pxFromToken)
{
	if (xOperation == eReplace)
	{
		if(strncmp(pcJson + pxPathToken->start, GATEWAYNAME, pxPathToken->end - pxPathToken->start) == 0)
		{
			strncpy_s(cName, 100, pcJson + pxValueToken->start, pxValueToken->end - pxValueToken->start);
		}
		else if (strncmp(pcJson + pxPathToken->start, NICKNAME, pxPathToken->end - pxPathToken->start) == 0)
		{
			strncpy_s(cNickName, 100, pcJson + pxValueToken->start, pxValueToken->end - pxValueToken->start);
		}

		return pdTRUE;
	}
	return pdFALSE;
}
