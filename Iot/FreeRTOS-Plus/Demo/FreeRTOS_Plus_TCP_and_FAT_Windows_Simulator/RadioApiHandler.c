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

#define radioConfigRADIO_COUNT 4

#define DEVICES "devices"
#define ENABLED "enabled"
#define FAULTED "faulted"
#define RADIOTYPE "typed"

typedef enum
{
	eBle,
	eLora,
	eNRF,
	eISM
} eRadioType;

typedef struct 
{
	BaseType_t xEnabled;
	BaseType_t xFaulted;
	eRadioType xType;
} Radio_t;

static BaseType_t prvParseDeviceGet(const char *pcUrlData, BaseType_t *pxDeviceId, BaseType_t *pxDevice, BaseType_t *pxSettings );

static Radio_t radios[radioConfigRADIO_COUNT] =
{
	{ 0, 0, 0 },
	{ 0, 0, 1 },
	{ 0, 0, 2 },
	{ 0, 0, 3 },
};

static void prvAddRadio(JsonGenerator_t *pxGenerator, const Radio_t *pxRadio);

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
			JsonGenerator_t xGenerator;

			vJsonInit(&xGenerator, pxClient->pxParent->pcCommandBuffer, sizeof(pxClient->pxParent->pcCommandBuffer));

			if (!xDevice)
			{
				vJsonOpenKey(&xGenerator, DEVICES);
				vJsonAddValue(&xGenerator, eArray, "");

				for (BaseType_t i = 0; i < radioConfigRADIO_COUNT; i++)
				{
					prvAddRadio(&xGenerator, &radios[0]);
					
					if(i < radioConfigRADIO_COUNT - 1)
						vJsonCloseNode(&xGenerator, eValue);
				}

				vJsonCloseNode(&xGenerator, eArray);
				vJsonCloseNode(&xGenerator, eObject);
			}

			/*if( xSettings)
			{
			}
			else if( xDevice )
			{
			}
			else
			{
				
			}*/

			xSendApiResponse(pxClient);
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

	*pxDevice = pdFALSE;
	*pxSettings = pdFALSE;

	while ((pcNext = strchr(pcCurrent, '/')) != NULL)
	{
		if (pcCurrent != pcNext)
		{
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

static void prvAddRadio(JsonGenerator_t *pxGenerator, const Radio_t *pxRadio)
{
	char cBuffer[2];
	itoa(pxRadio->xType, cBuffer, 10);
	vJsonAddValue(pxGenerator, eObject, "");
	vJsonOpenKey(pxGenerator, ENABLED);
	vJsonAddValue(pxGenerator, pxRadio->xEnabled ? eTrue : eFalse, "");
	vJsonCloseNode(pxGenerator, eTrue);
	vJsonOpenKey(pxGenerator, FAULTED);
	vJsonAddValue(pxGenerator, pxRadio->xEnabled ? eTrue : eFalse, "");
	vJsonCloseNode(pxGenerator, eTrue);
	vJsonOpenKey(pxGenerator, RADIOTYPE);
	vJsonAddValue(pxGenerator, eNumber, cBuffer);
	vJsonCloseNode(pxGenerator, eObject);
}
