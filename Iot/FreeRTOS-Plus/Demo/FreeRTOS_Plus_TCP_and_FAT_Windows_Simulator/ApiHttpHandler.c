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

static void prvTrim();

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