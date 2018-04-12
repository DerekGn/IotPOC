/* Standard includes. */
#include <stdio.h>
#include <stdlib.h>

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"

#include "FreeRTOS_HTTP_io.h"
#include "FreeRTOS_HTTP_commands.h"
#include "FreeRTOS_HTTP_pipeline.h"

#ifndef ipconfigHTTP_DEFAULT_ROOT_FILE
	#define ipconfigHTTP_DEFAULT_ROOT_FILE	"index.html"
#endif

/*-----------------------------------------------------------*/

static void prvHandleFileGet( HTTPClient_t *pxClient );

/*-----------------------------------------------------------*/

static void prvHandleFileGet( HTTPClient_t * pxClient )
{
char pcSlash[2];
BaseType_t xRc;

	if ( strcmp( pxClient->pcUrlData, "/" ) == 0 )
	{
		strcat( pxClient->pcUrlData, ipconfigHTTP_DEFAULT_ROOT_FILE );
	}

	pxClient->bits.ulFlags = 0;

	if ( pxClient->pcUrlData[0] != '/' )
	{
		/* Insert a slash before the file name. */
		pcSlash[0] = '/';
		pcSlash[1] = '\0';
	}
	else
	{
		/* The browser provided a starting '/' already. */
		pcSlash[0] = '\0';
	}
	snprintf( pxClient->pcCurrentFilename, sizeof( pxClient->pcCurrentFilename ), "%s%s%s",
		pxClient->pcRootDir,
		pcSlash,
		pxClient->pcUrlData );

	pxClient->pxFileHandle = ff_fopen( pxClient->pcCurrentFilename, "rb" );

	FreeRTOS_printf( ("Open file '%s': %s\n", pxClient->pcCurrentFilename,
		pxClient->pxFileHandle != NULL ? "Ok" : strerror( stdioGET_ERRNO() ) ) );

	if (pxClient->pxFileHandle == NULL)
	{
		/* "404 File not found". */
		strcpy( pxClient->pxParent->pcExtraContents, "Content-Length: 0\r\n" );
		xRc = xSendReply(pxClient, WEB_NOT_FOUND);
	}
	else
	{
		pxClient->uxBytesLeft = (size_t)pxClient->pxFileHandle->ulFileSize;
		xRc = xSendFile(pxClient);
	}
}
/*-----------------------------------------------------------*/

eHttpHandlerResult_t xFileProcessHttpRequest( HTTPClient_t *pxClient, BaseType_t xIndex )
{
eHttpHandlerResult_t xResult = eInvokeNext;

	/* A new command has been received. Process it. */
	switch( xIndex )
	{
	case ECMD_GET:
		prvHandleFileGet( pxClient );
		xResult = eSkipNext;
		break;
	case ECMD_HEAD:
	case ECMD_POST:
	case ECMD_PUT:
	case ECMD_DELETE:
	case ECMD_TRACE:
	case ECMD_OPTIONS:
	case ECMD_CONNECT:
	case ECMD_PATCH:
	case ECMD_UNK:
		{
			FreeRTOS_printf( ( "xFileProcessRequest: Not implemented: %s\n",
				xWebCommands[xIndex].pcCommandName ) );
		}
		break;
	}


	return xResult;
}