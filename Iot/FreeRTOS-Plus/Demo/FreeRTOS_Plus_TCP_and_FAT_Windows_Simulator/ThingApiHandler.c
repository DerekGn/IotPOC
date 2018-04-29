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

typedef enum
{
    eReadOnly,
    eReadWrite,
    eWriteOnly
} eThingAccess;

typedef enum
{
    eBinary
} eThingType;

typedef struct
{
    UBaseType_t xId;
    eThingType xType;
    eThingAccess xAccessType;
    char cName[30];
    //char cUnit[10];
    //char cValue[10];
} thing_t;

#define thingConfigTHING_COUNT 4

//static thing_t things[thingConfigTHING_COUNT] =
//{
//	{1, "temp", "C", "100" },
//	{ 2, "temp", "C", "100" }
//};

static BaseType_t prvParseThingGet(const char *pcUrlData, BaseType_t *pxThingId, BaseType_t *pxThing);

void vHandleThingApi(HTTPClient_t *pxClient, BaseType_t xIndex, char *pcPayload, jsmntok_t *pxTokens, BaseType_t xJsonTokenCount)
{
BaseType_t xCode = WEB_BAD_REQUEST;
BaseType_t xThingId;
BaseType_t xThing;

    switch (xIndex)
    {
    case ECMD_GET:
        FreeRTOS_debug_printf(("%s: Handling GET\n", __func__));

        if (prvParseThingGet(pxClient->pcUrlData, &xThingId, &xThing))
        {
            JsonGenerator_t xGenerator;

            vJsonInit(&xGenerator, pxClient->pxParent->pcCommandBuffer, sizeof(pxClient->pxParent->pcCommandBuffer));

            if (!xThing)
            {
            }
            else
            {
            }
        }

        break;
    case ECMD_PATCH:
        FreeRTOS_debug_printf(("%s: Handling PATCH\n", __func__));
        break;
    }

    if (xCode != WEB_REPLY_OK)
    {
        xSendReply(pxClient, xCode);
    }
}

BaseType_t prvParseThingGet(const char * pcUrlData, BaseType_t * pxThingId, BaseType_t * pxThing)
{
    char *pcNext;
    const char *pcCurrent = pcUrlData;
    BaseType_t xTokenCount = 0;
    BaseType_t xResult = pdTRUE;

    *pxThing = pdFALSE;

    while ((pcNext = strchr(pcCurrent, '/')) != NULL)
    {
        if (pcCurrent != pcNext)
        {
            if (xTokenCount == 2)
            {
                if (!xParseId(pcCurrent, pxThingId))
                {
                    xResult = pdFALSE;
                    break;
                }

                *pxThing = pdTRUE;
            }
            
            xTokenCount++;
        }

        pcCurrent = pcNext + 1;
    }

    if (*pcCurrent != '\0')
    {
        if (strcmp(pcCurrent, "thing") == 0)
        {
            xResult = pdTRUE;
        }
        else if (xParseId(pcCurrent, pxThingId))
        {
            *pxThing = pdTRUE;
        }
        else
        {
            xResult = pdFALSE;
        }
    }

    return xResult;
}
