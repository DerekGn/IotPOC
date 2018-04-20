#ifndef JSON_H
#define JSON_H

#include <FreeRTOS.h>

typedef enum
{
	eAdd,
	eRemove,
	eReplace,
	eMove,
	eCopy,
	eTest
} eOperationType_t;

typedef enum
{
	eValue,
	eString,
	eNumber,
	eObject,
	eArray,
	eTrue,
	eFalse,
	eNull
} eValueType_t;

typedef enum
{
	eJsonEmpty = -5,
	eJsonInvalidStartCount = -6,
	eJsonInvalidPair = -7
} eJsonParseType_t;

typedef struct
{
	char *pcBuffer;
	BaseType_t xBufferSize;
} JsonGenerator_t;

typedef enum
{
	eProcessed,
	eInvalidNode,
	eInvalidOp,
	eMissingTokens,
	eProcessingFailed
} eProcessPatchResult;

typedef BaseType_t(*xProcessPatch_t) (char *pcJson, eOperationType_t xOperation, jsmntok_t *pxPathToken, jsmntok_t *pxValueToken, jsmntok_t *pxFromToken);

void vJsonInit(JsonGenerator_t *pxGenerator, char *pcBuffer, BaseType_t xBufferSize);

void vJsonOpenKey(JsonGenerator_t *pxGenerator, const char *pcName);

void vJsonAddValue(JsonGenerator_t *pxGenerator, eValueType_t xValueType, const char *pcValue);

void vJsonCloseNode(JsonGenerator_t *pxGenerator, eValueType_t xValueType);

eProcessPatchResult xProcessPatchDocument(char *pcJson, jsmntok_t * pxTokens, BaseType_t xJsonTokenCount, xProcessPatch_t xProcessPatch);

BaseType_t xParsePatchOperation(char * pcJson, jsmntok_t * pxTokens, eOperationType_t *pxOperation, jsmntok_t **pxPathToken, jsmntok_t **pxValueToken, jsmntok_t **pxFromToken);

#endif /* JSON_H */
