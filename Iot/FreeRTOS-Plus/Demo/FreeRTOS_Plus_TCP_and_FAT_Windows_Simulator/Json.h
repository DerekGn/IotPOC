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
