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
