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
} ValueType_t;

typedef enum
{
	eJsonEmpty = -5,
	eJsonInvalidStartCount = -6,
	eJsonInvalidPair = -7
} JsonParseType_t;

typedef BaseType_t(*xJsonNodeHandler_t) ( char *pcJson, jsmntok_t *pxTokens, BaseType_t xTokensCount, BaseType_t *pxIndex );

BaseType_t xParseJson( char *pcJson, xJsonNodeHandler_t xJsonNodeHandler );

void vJsonInit(char *pcBuffer, BaseType_t xBufferSize);
void vJsonOpenKey(char *pcBuffer, BaseType_t xBufferSize, const char *pcName);
void vJsonAddValue(char *pcBuffer, BaseType_t xBufferSize, ValueType_t xValueType, const char *pcValue);
void vJsonCloseKey(char *pcBuffer, BaseType_t xBufferSize, ValueType_t xValueType);

#endif /* JSON_H */
