#include <FreeRTOS.h>
#include "task.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"
#include "FreeRTOS_Sockets.h"
#include "FreeRTOS_server_private.h"

#include "FreeRTOS_HTTP_io.h"
#include "FreeRTOS_HTTP_commands.h"

#include "jsmn.h"
#include "ApiHandlers.h"

#define NETMASK		"networkmask"
#define IPADDRESS	"ip"
#define GWADDR		"gateway"
#define DNSADDR		"dns"
#define MACADDR		"mac"

static const char* prvIPv4ToJsonArray(char *pcBuffer, uint32_t ulAddress);
static const char* prvMacToJsonArray(char *pcBuffer, const uint8_t *pucMacAddress);

void vHandleNetworkApi( HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xNumTokens )
{
uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
char cBuffer[4][33];
char cMacBuffer[49];
BaseType_t xCode = 0;

	strcpy(pxClient->pxParent->pcExtraContents, "Content-Length: 0\r\n");

	switch (xIndex)
	{
		case ECMD_GET:
			FreeRTOS_debug_printf(("%s: Handling GET\n", __func__));
			FreeRTOS_GetAddressConfiguration(&ulIPAddress, &ulNetMask, &ulGatewayAddress, &ulDNSServerAddress);
			snprintf(pxClient->pxParent->pcCommandBuffer, sizeof(pxClient->pxParent->pcCommandBuffer),
				"{\"%s\": [%s], \"%s\": [%s], \"%s\": [%s],\"%s\": [%s], \"%s\": [%s] }", 
				IPADDRESS, prvIPv4ToJsonArray(cBuffer[0], ulIPAddress), 
				NETMASK, prvIPv4ToJsonArray(cBuffer[1], ulNetMask),
				GWADDR, prvIPv4ToJsonArray(cBuffer[2], ulGatewayAddress),
				DNSADDR, prvIPv4ToJsonArray(cBuffer[3], ulDNSServerAddress),
				MACADDR, prvMacToJsonArray(cMacBuffer, FreeRTOS_GetMACAddress()));
			xSendApiResponse(pxClient);
			break;
		case ECMD_PATCH:
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

const char * prvIPv4ToJsonArray(char *pcBuffer, uint32_t ulAddress)
{
BaseType_t i = 0;

	FreeRTOS_inet_ntoa(ulAddress, pcBuffer);

	while (pcBuffer[i] != '\0')
	{
		if (pcBuffer[i] == '.')
		{
			pcBuffer[i] = ',';
		}

		i++;
	}

	return pcBuffer;
}

const char * prvMacToJsonArray(char * pcBuffer, const uint8_t * pucMacAddress)
{
	sprintf(pcBuffer, "%u,%u,%u,%u,%u,%u", 
		pucMacAddress[0],
		pucMacAddress[1],
		pucMacAddress[2],
		pucMacAddress[3],
		pucMacAddress[4],
		pucMacAddress[5]);

	return pcBuffer;
}
