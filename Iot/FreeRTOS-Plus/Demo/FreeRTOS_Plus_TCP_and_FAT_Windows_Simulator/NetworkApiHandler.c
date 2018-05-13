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
#include "MediaTypes.h"
#include "ApiHandlers.h"

#define NETMASK     "networkmask"
#define GWADDR      "gateway"
#define DNSADDR     "dns"
#define MACADDR     "mac"
#define IPADDRESS   "ip"

static const char* prvIPv4ToJsonArray(char *pcBuffer, uint32_t ulAddress);
static const char* prvMacToJsonArray(char *pcBuffer, const uint8_t *pucMacAddress);

void vHandleNetworkApi( HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount)
{
BaseType_t xCode = WEB_BAD_REQUEST;
uint32_t ulIPAddress, ulNetMask, ulGatewayAddress, ulDNSServerAddress;
char cBuffer[49];

    strcpy(pxClient->pxParent->pcExtraContents, "Content-Length: 0\r\n");

    switch (xIndex)
    {
        case ECMD_GET:
            FreeRTOS_debug_printf(("%s: Handling GET\n", __func__));
            FreeRTOS_GetAddressConfiguration(&ulIPAddress, &ulNetMask, &ulGatewayAddress, &ulDNSServerAddress);

            JsonGenerator_t xGenerator;

            vJsonInit(&xGenerator, pxClient->pxParent->pcCommandBuffer, sizeof(pxClient->pxParent->pcCommandBuffer));
            vJsonAddValue(&xGenerator, eObject, "");
            vJsonOpenKey(&xGenerator, IPADDRESS);
            vJsonAddValue(&xGenerator, eArray, prvIPv4ToJsonArray(cBuffer, ulIPAddress));
            vJsonOpenKey(&xGenerator, NETMASK);
            vJsonAddValue(&xGenerator, eArray, prvIPv4ToJsonArray(cBuffer, ulNetMask));
            vJsonOpenKey(&xGenerator, GWADDR);
            vJsonAddValue(&xGenerator, eArray, prvIPv4ToJsonArray(cBuffer, ulGatewayAddress));
            vJsonOpenKey(&xGenerator, DNSADDR);
            vJsonAddValue(&xGenerator, eArray, prvIPv4ToJsonArray(cBuffer, ulDNSServerAddress));
            vJsonOpenKey(&xGenerator, MACADDR);
            vJsonAddValue(&xGenerator, eArray, prvMacToJsonArray(cBuffer, FreeRTOS_GetMACAddress()));
            
            xCode = WEB_REPLY_OK;
            xSendApiResponse(pxClient, MEDIA_TYPE_APP_JSON);
            break;
    }

    if (xCode != WEB_REPLY_OK)
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
