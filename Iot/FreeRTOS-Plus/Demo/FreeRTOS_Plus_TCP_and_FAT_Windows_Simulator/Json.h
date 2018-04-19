#ifndef JSON_H
#define JSON_H

#include <FreeRTOS.h>

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

typedef BaseType_t(*xJsonNodeHandler_t) ( char *pcJson, jsmntok_t *pxTokens, BaseType_t xTokensCount, BaseType_t *pxIndex );

BaseType_t xParseJson( char *pcJson, xJsonNodeHandler_t xJsonNodeHandler );

void vJsonInit(JsonGenerator_t *pxGenerator, char *pcBuffer, BaseType_t xBufferSize);

void vJsonOpenKey(JsonGenerator_t *pxGenerator, const char *pcName);

void vJsonAddValue(JsonGenerator_t *pxGenerator, eValueType_t xValueType, const char *pcValue);

void vJsonCloseNode(JsonGenerator_t *pxGenerator, eValueType_t xValueType);

#endif /* JSON_H */
