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

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"

#include "jsmn.h"

#include "ApiHandlers.h"

#include "FreeRTOS_HTTP_io.h"
#include "FreeRTOS_HTTP_commands.h"
#include "FreeRTOS_HTTP_pipeline.h"

#define apiconfigTOKEN_COUNT	10

eHttpHandlerResult_t xApiProcessHttpRequest( HTTPClient_t *pxClient, BaseType_t xIndex )
{
extern ApiHandler_t xApiHandlers[];
eHttpHandlerResult_t xResult = eInvokeNext;
BaseType_t xRc = 1;
jsmn_parser xParser;
jsmntok_t xTokens[apiconfigTOKEN_COUNT];

	if( _strnicmp( pxClient->pcUrlData, "/api", 4 ) == 0 )
    {
		char *payload = strstr( pxClient->pcRestData, "\r\n\r\n" );
		
		if( payload )
		{
			payload += 4;

			if ( strlen( payload ) > 0 )
			{
				jsmn_init(&xParser);

				xRc = jsmn_parse( &xParser, payload, strlen( payload ), xTokens, apiconfigTOKEN_COUNT );
			}
		}
		
		if( xRc > 0 )
		{
			for( BaseType_t xCount = 0; xCount < xApiHandlerEntries( ); xCount++ )
			{
				if( _strnicmp( pxClient->pcUrlData + 4, xApiHandlers[ xCount ].pcRoute, xApiHandlers[ xCount ].xRouteLen ) == 0 )
				{
					FreeRTOS_printf( ( "Api processing request: %s\n", xApiHandlers[ xCount ].pcRoute ) );

					xApiHandlers[ xCount ].xApiHandlerAction( pxClient, xIndex, payload, xTokens, xRc );
					break;
				}
			}
		}
		
		if( xRc < 0 )
		{
			strcpy( pxClient->pxParent->pcExtraContents, "Content-Length: 0\r\n" );
			xRc = xSendReply( pxClient, WEB_BAD_REQUEST );
		}

        xResult = eSkipNext;
    }

    return xResult;
}