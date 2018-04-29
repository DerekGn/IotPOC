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
#include "ApiHandlers.h"

#define radioConfigRADIO_COUNT 4

#define RADIOTYPE   "type"
#define DEVICES     "devices"
#define ENABLED     "enabled"
#define FAULTED     "faulted"

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

static void prvAddRadio(JsonGenerator_t *pxGenerator, const Radio_t *pxRadio);

static BaseType_t prvParseRadioGet(const char *pcUrlData, BaseType_t *pxRadioId, BaseType_t *pxRadio, BaseType_t *pxSettings);

static Radio_t radios[radioConfigRADIO_COUNT] =
{
    { 0, 0, 0 },
    { 0, 0, 1 },
    { 0, 0, 2 },
    { 0, 0, 3 },
};

void vHandleRadioApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount)
{
BaseType_t xCode = WEB_BAD_REQUEST;
BaseType_t xDeviceId;
BaseType_t xDevice;
BaseType_t xSettings;

    switch (xIndex)
    {
    case ECMD_GET:
        FreeRTOS_debug_printf(("%s: Handling GET\n", __func__));

        if (prvParseRadioGet(pxClient->pcUrlData, &xDeviceId, &xDevice, &xSettings))
        {
            JsonGenerator_t xGenerator;

            vJsonInit(&xGenerator, pxClient->pxParent->pcCommandBuffer, sizeof(pxClient->pxParent->pcCommandBuffer));

            if (!xDevice)
            {
                vJsonAddValue(&xGenerator, eObject, "");
                vJsonOpenKey(&xGenerator, DEVICES);
                vJsonAddValue(&xGenerator, eArray, "");

                for (BaseType_t i = 0; i < radioConfigRADIO_COUNT; i++)
                {
                    prvAddRadio(&xGenerator, &radios[i]);

                    if (i < radioConfigRADIO_COUNT - 1)
                    {
                        vJsonCloseNode(&xGenerator, eValue);
                    }
                }

                vJsonCloseNode(&xGenerator, eArray);
                vJsonCloseNode(&xGenerator, eObject);
            }
            else if (xSettings)
            {
                //TODO get settings
            }
            else
            {
                prvAddRadio(&xGenerator, &radios[xDeviceId]);
            }

            xCode = WEB_REPLY_OK;
            xSendApiResponse(pxClient);
        }
        
        break;
    case ECMD_PATCH:
        FreeRTOS_debug_printf(("%s: Handling PATCH\n", __func__));
        if (prvParseRadioGet(pxClient->pcUrlData, &xDeviceId, &xDevice, &xSettings))
        {
            if (xSettings)
            {
                //TODO update settings
            }
            else if (xDevice)
            {
                //TODO update device
            }
        }
        break;
    }

    if (xCode != WEB_REPLY_OK)
    {
        xSendReply(pxClient, xCode);
    }
}

static BaseType_t prvParseRadioGet(const char *pcUrlData, BaseType_t *pxRadioId, BaseType_t *pxRadio, BaseType_t *pxSettings)
{
    char *pcNext;
    const char *pcCurrent = pcUrlData;
    BaseType_t xTokenCount = 0;
    BaseType_t xResult = pdTRUE;

    *pxRadio = pdFALSE;
    *pxSettings = pdFALSE;

    while ((pcNext = strchr(pcCurrent, '/')) != NULL)
    {
        if (pcCurrent != pcNext)
        {
            if (xTokenCount == 2)
            {
                if (!xParseId(pcCurrent, pxRadioId))
                {
                    xResult = pdFALSE;
                    break;
                }

                *pxRadio = pdTRUE;
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

    if (*pcCurrent != '\0')
    {
        if (strcmp(pcCurrent, "radio") == 0)
        {
            xResult = pdTRUE;
        }
        else if (xParseId(pcCurrent, pxRadioId))
        {
            *pxRadio = pdTRUE;
        }
        else
        {
            xResult = pdFALSE;
        }
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