#include <FreeRTOS.h>
#include "task.h"
#include "FreeRTOS_IP.h"

#include "FreeRTOS_HTTP_pipeline.h"

#if !defined( ARRAY_SIZE )
	#define ARRAY_SIZE(x) ( BaseType_t ) (sizeof( x ) / sizeof( x )[ 0 ] )
#endif

HttpHandler_t *pxHttpPipeline[] =
{
	NULL,
	NULL,
	NULL
};

BaseType_t xHandlerEntryCount( )
{
	return ( BaseType_t ) ARRAY_SIZE( pxHttpPipeline );
}

void vAddHttpHandler( HttpHandler_t *pxHttpHandler )
{
BaseType_t xIndex;
const BaseType_t xEntries = ARRAY_SIZE(pxHttpPipeline);

	for ( xIndex = 0; xIndex < xEntries; xIndex++ )
	{
		if ( pxHttpPipeline[ xIndex ] == NULL )
		{
			pxHttpPipeline[ xIndex ] = pxHttpHandler;
			break;
		}
	}

	configASSERT( xIndex < xEntries );
}

BaseType_t xProcessHttpRequest( HTTPClient_t * pxClient, BaseType_t xIndex )
{
BaseType_t xPiplineIndex;
const BaseType_t xEntries = ARRAY_SIZE(pxHttpPipeline);

	for ( xPiplineIndex = 0; xPiplineIndex < xEntries; xPiplineIndex++ )
	{
		if ( pxHttpPipeline[ xPiplineIndex ] != NULL )
		{
			eHttpHandlerResult_t xResult = pxHttpPipeline[ xPiplineIndex ]->xHttpHandlerAction( pxClient, xIndex );

			FreeRTOS_debug_printf(("xProcessHttpRequest: %s handled http request. Result: %i \n",
				pxHttpPipeline[ xPiplineIndex ]->pucName,
				xResult));

			if ( xResult == eSkipNext )
			{
				break;
			}
		}
	}

	return pdTRUE;
}
