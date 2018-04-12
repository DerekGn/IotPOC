#include <FreeRTOS.h>
#include "task.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_server_private.h"

#include "FreeRTOS_HTTP_io.h"
#include "FreeRTOS_HTTP_commands.h"

#include "jsmn.h"

static BaseType_t prvParseDeviceGet(const char *pcUrlData, BaseType_t *pxDeviceId, BaseType_t *pxDevice, BaseType_t *pxSettings );

void vHandleRadioApi( HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xNumTokens )
{
BaseType_t xCode = 0;
BaseType_t xDeviceId;
BaseType_t xDevice;
BaseType_t xSettings;

	switch (xIndex)
	{
	case ECMD_GET:
		FreeRTOS_debug_printf(("%s: Handling GET\n", __func__));

		if (prvParseDeviceGet( pxClient->pcUrlData, &xDeviceId, &xDevice, &xSettings ))
		{
			if(xSettings)
			{

			}
			else if( xDevice )
			{
			}
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
		xSendReply(pxClient, xCode);
	}
}


static BaseType_t prvParseDeviceGet(const char * pcUrlData, BaseType_t *pxDeviceId, BaseType_t *pxDevice, BaseType_t *pxSettings)
{
	char *pcStop;
	char *pcNext;
	char *pcCurrent = pcUrlData;
	BaseType_t xTokenCount = 0;
	BaseType_t xResult = pdTRUE;

	while ((pcNext = strchr(pcCurrent, '/')) != NULL)
	{
		if (pcCurrent != pcNext)
		{
			printf("TokenCount: %i (%.*s)\n", xTokenCount, pcNext - pcCurrent, pcCurrent);

			if (xTokenCount == 2)
			{
				errno = 0;

				*pxDeviceId = strtol(pcCurrent, &pcStop, 10);

				if (errno == ERANGE || pcCurrent == pcStop)
				{
					xResult = 0;
					break;
				}

				*pxDevice = 1;
			}

			if (xTokenCount == 3)
			{
				if (strncmp(pcCurrent, "settings", pcNext - pcCurrent) == 0)
				{
					*pxSettings = pdTRUE;
					break;
				}
			}

			xTokenCount++;
		}

		pcCurrent = pcNext + 1;
	}

	return xResult;
}
