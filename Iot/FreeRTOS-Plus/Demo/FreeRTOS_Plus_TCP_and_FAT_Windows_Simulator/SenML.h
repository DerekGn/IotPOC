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

#ifndef SENML_H
#define SENML_H

// Units as defined in https://tools.ietf.org/html/draft-ietf-core-senml-16

#include <stdint.h>

#include "SenMLUnits.h"

#define SENML_SUPPORTED_VERSION (10)    // The highest SenML version this implementation supports

#define SENML_BASE_NAME         "bn"    // Key for the base name attribute
#define SENML_BASE_TIME         "bt"    // Key for the base time attribute
#define SENML_BASE_UNIT         "bu"    // Key for the base unit attribute
#define SENML_BASE_VALUE        "bv"    // Key the base value attribute
#define SENML_BASE_SUM          "bs"    // Key the base sum attribute
#define SENML_VERSION           "bver"  // Key for the version attribute
#define SENML_NAME              "n"     // Key for the name attribute
#define SENML_UNIT              "u"     // Key for the unit attribute
#define SENML_VALUE             "v"     // Key for a number value
#define SENML_VALUE_STRING      "vs"    // Key for a string value
#define SENML_VALUE_BOOL        "vb"    // Key for a boolean value
#define SENML_VALUE_DATA        "vd"    // Key for a base64 string with URL safe alphabet as defined in Section 5 of[RFC4648], with padding omitted
#define SENML_VALUE_SUM         "s"     // Key for a number value
#define SENML_TIME              "t"     // Key for a time number value
#define SENML_TIME_UPDATE       "ut"    // Key for a period if time number value

#define SENML_MAX_NAME          20      // The max length of name
#define SENML_MAX_UNIT          10      // The max length of unit
#define SENML_MAX_BASENAME      20      // The max length of base name
#define SENML_MAX_BASEUNIT      10      // The max length of base unit

typedef struct SenMLBaseSet
{
    uint8_t ucBaseVersionSet : 1;       // Indicates the base version is assigned
    uint8_t ucBaseNameSet : 1;          // Indicates the base name is assigned
    uint8_t ucBaseUnitSet : 1;          // Indicates the base units is assigned
    uint8_t ucBaseTimeSet : 1;          // Indicates the base time is assigned
    uint8_t ucBaseValueSet : 1;         // Indicates the base value is assigned
    uint8_t ucBaseSumSet : 1;           // Indicates the base sum is assigned
} SenMLBaseSet_t;

typedef struct SenMLRecordSet
{
    uint8_t ucNameSet : 1;              // Indicates the name is assigned
    uint8_t ucUnitSet : 1;              // Indicates the units is assigned
    uint8_t ucTimeSet : 1;              // Indicates the time is assigned
    uint8_t ucValueSet : 1;             // Indicates the value is assigned
    uint8_t ucSumSet : 1;               // Indicates the sum is assigned
    uint8_t ucUpdateTimeSet : 1;        // Indicates the update time is assigned
} SenMLRecordSet_t;

typedef struct SenMLBaseInfo
{
    uint8_t ucVersion;                      // Version number of media type format.  This field is an optional positive integer
    char cBaseName[SENML_MAX_BASENAME];     // This is a string that is prepended to the names found in	the entries.
    char xBaseUnit[SENML_MAX_BASEUNIT];     // Base unit that is assumed for all entries, unless otherwise indicated.
    double dBaseTime;                       // A base time that is added to the time found in an entry
    double dBaseValue;                      // A base value is added to the value found in an entry, similar to Base Time.
    double dBaseSum;                        // A base sum is added to the sum found in an entry, similar to Base Time.
    SenMLBaseSet_t xBaseSet;
} SenMLBaseInfo_t;

typedef struct SenMLRecord
{
    char cName[SENML_MAX_NAME];             // Name of the sensor or parameter
    char cUnit[SENML_MAX_UNIT];             // Unit for a measurement value.  Optional.
    double dSum;                            // Integrated sum of the values over time.  Optional. This field is in the unit specified in the Unit value multiplied by seconds.
    double dTime;                           // Time when value was recorded.  Optional.
    double dUpdateTime;                     // Period of time in seconds that represents the maximum time before this sensor will provide an updated reading for a measurement.Optional.
    SenMLRecordSet_t xRecordSet;            // Indicates which field of the record is set

    union {
        double dValue;                      // A value of type float
        char *pcValue;                      // A value of type string
        uint8_t ucValue;                    // A boolean value
        char *pcBase64;                     // A binary value
    } xValue;
} SenMLRecord_t;

typedef struct SenMLPack
{
    SenMLBaseInfo_t xBaseInfo;              // The base info structure
    SenMLRecord_t *pxRecords;               // The record structure
    size_t xCount;                          // The number of records
} SenMLPack_t;

#endif SENML_H