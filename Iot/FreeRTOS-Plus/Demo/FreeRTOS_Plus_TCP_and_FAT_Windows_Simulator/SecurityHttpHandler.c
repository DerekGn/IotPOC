#include <FreeRTOS.h>

#include "FreeRTOS_HTTP_pipeline.h"

/*-----------------------------------------------------------*/

eHttpHandlerResult_t xSecurityProcessHttpRequest( HTTPClient_t *pxClient, BaseType_t xIndex )
{
	( xIndex );
	return eInvokeNext;
}