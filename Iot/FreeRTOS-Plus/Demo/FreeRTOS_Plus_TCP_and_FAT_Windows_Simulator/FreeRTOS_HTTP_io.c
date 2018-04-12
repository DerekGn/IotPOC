/* FreeRTOS */
#include <FreeRTOS.h>
#include "task.h"

/* FreeRTOS+TCP includes. */
#include "FreeRTOS_IP.h"

/* FreeRTOS Protocol includes. */
#include "FreeRTOS_HTTP_commands.h"

#include "FreeRTOS_HTTP_io.h"

#if !defined( ARRAY_SIZE )
	#define ARRAY_SIZE(x) ( BaseType_t ) (sizeof( x ) / sizeof( x )[ 0 ] )
#endif

typedef struct xTYPE_COUPLE
{
	const char *pcExtension;
	const char *pcType;
} TypeCouple_t;

static TypeCouple_t pxTypeCouples[] =
{
	{ "html", "text/html" },
	{ "css",  "text/css" },
	{ "js",   "text/javascript" },
	{ "png",  "image/png" },
	{ "jpg",  "image/jpeg" },
	{ "gif",  "image/gif" },
	{ "txt",  "text/plain" },
	{ "mp3",  "audio/mpeg3" },
	{ "wav",  "audio/wav" },
	{ "flac", "audio/ogg" },
	{ "pdf",  "application/pdf" },
	{ "ttf",  "application/x-font-ttf" },
	{ "ttc",  "application/x-font-ttf" }
};
/*-----------------------------------------------------------*/

void vFileClose(HTTPClient_t *pxClient)
{
	if (pxClient->pxFileHandle != NULL)
	{
		FreeRTOS_printf(("Closing file: %s\n", pxClient->pcCurrentFilename));
		ff_fclose(pxClient->pxFileHandle);
		pxClient->pxFileHandle = NULL;
	}
}
/*-----------------------------------------------------------*/

BaseType_t xSendReply(HTTPClient_t *pxClient, BaseType_t xCode)
{
	struct xTCP_SERVER *pxParent = pxClient->pxParent;
	BaseType_t xRc;

	/* A normal command reply on the main socket (port 21). */
	char *pcBuffer = pxParent->pcFileBuffer;

	xRc = snprintf(pcBuffer, sizeof(pxParent->pcFileBuffer),
		"HTTP/1.1 %d %s\r\n"
#if	USE_HTML_CHUNKS
		"Transfer-Encoding: chunked\r\n"
#endif
		"Content-Type: %s\r\n"
		"Connection: keep-alive\r\n"
		"%s\r\n",
		(int)xCode,
		webCodename(xCode),
		pxParent->pcContentsType[0] ? pxParent->pcContentsType : "text/html",
		pxParent->pcExtraContents);

	pxParent->pcContentsType[0] = '\0';
	pxParent->pcExtraContents[0] = '\0';

	xRc = FreeRTOS_send(pxClient->xSocket, (const void *)pcBuffer, xRc, 0);
	pxClient->bits.bReplySent = pdTRUE_UNSIGNED;

	return xRc;
}
/*-----------------------------------------------------------*/

BaseType_t xSendFile(HTTPClient_t *pxClient)
{
	size_t uxSpace;
	size_t uxCount;
	BaseType_t xRc = 0;

	if (pxClient->bits.bReplySent == pdFALSE_UNSIGNED)
	{
		pxClient->bits.bReplySent = pdTRUE_UNSIGNED;

		strcpy(pxClient->pxParent->pcContentsType, pcGetContentsType(pxClient->pcCurrentFilename));
		snprintf(pxClient->pxParent->pcExtraContents, sizeof(pxClient->pxParent->pcExtraContents),
			"Content-Length: %d\r\n", (int)pxClient->uxBytesLeft);

		/* "Requested file action OK". */
		xRc = xSendReply(pxClient, WEB_REPLY_OK);
	}

	if (xRc >= 0) do
	{
		uxSpace = FreeRTOS_tx_space(pxClient->xSocket);

		if (pxClient->uxBytesLeft < uxSpace)
		{
			uxCount = pxClient->uxBytesLeft;
		}
		else
		{
			uxCount = uxSpace;
		}

		if (uxCount > 0u)
		{
			if (uxCount > sizeof(pxClient->pxParent->pcFileBuffer))
			{
				uxCount = sizeof(pxClient->pxParent->pcFileBuffer);
			}
			ff_fread(pxClient->pxParent->pcFileBuffer, 1, uxCount, pxClient->pxFileHandle);
			pxClient->uxBytesLeft -= uxCount;

			xRc = FreeRTOS_send(pxClient->xSocket, pxClient->pxParent->pcFileBuffer, uxCount, 0);
			if (xRc < 0)
			{
				break;
			}
		}
	} while (uxCount > 0u);

	if (pxClient->uxBytesLeft == 0u)
	{
		/* Writing is ready, no need for further 'eSELECT_WRITE' events. */
		FreeRTOS_FD_CLR(pxClient->xSocket, pxClient->pxParent->xSocketSet, eSELECT_WRITE);
		vFileClose(pxClient);
	}
	else
	{
		/* Wake up the TCP task as soon as this socket may be written to. */
		FreeRTOS_FD_SET(pxClient->xSocket, pxClient->pxParent->xSocketSet, eSELECT_WRITE);
	}

	return xRc;
}
/*-----------------------------------------------------------*/

const char *pcGetContentsType(const char *apFname)
{
	const char *slash = NULL;
	const char *dot = NULL;
	const char *ptr;
	const char *pcResult = "text/html";
	BaseType_t x;

	for (ptr = apFname; *ptr; ptr++)
	{
		if (*ptr == '.') dot = ptr;
		if (*ptr == '/') slash = ptr;
	}
	if (dot > slash)
	{
		dot++;
		for (x = 0; x < ARRAY_SIZE(pxTypeCouples); x++)
		{
			if (strcasecmp(dot, pxTypeCouples[x].pcExtension) == 0)
			{
				pcResult = pxTypeCouples[x].pcType;
				break;
			}
		}
	}
	return pcResult;
}
