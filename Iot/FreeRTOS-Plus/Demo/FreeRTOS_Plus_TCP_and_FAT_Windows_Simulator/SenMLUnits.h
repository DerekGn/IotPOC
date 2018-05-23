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

// Units as defined in https://tools.ietf.org/html/draft-ietf-core-senml-16
#ifndef SENML_UNITS_H
#define SENML_UNITS_H

#define METER           "m"         // meter
#define KILO            "kg"        // kilogram
#define GRAM            "g"         // gram * legacy unit
#define SEC             "s"         // second
#define AMP             "A"         // ampere
#define KELVIN          "K"         // kelvin
#define CANDELA         "cd"        // candela
#define MOL             "mol"       // mole
#define HZ              "Hz"        // hertz
#define RADIAN          "rad"       // radian
#define STERADIAN       "sr"        // steradian
#define NEWTON          "N"         // newton
#define PASCAL          "Pa"        // pascal
#define JOULE           "J"         // joule
#define WATT            "W"         // watt
#define COULOMB         "C"         // coulomb
#define VOLT            "V"         // volt
#define FARAD           "F"         // farad
#define OHM             "Ohm"       // ohm
#define SIEMENS         "S"         // siemens
#define WEBER           "Wb"        // weber
#define TESLA           "T"         // tesla
#define HENRY           "H"         // henry
#define CELSIUS         "Cel"       // degrees Celsius
#define LUMEN           "lm"        // lumen
#define LUX             "lx"        // lux
#define BECQUEREL       "Bq"        // becquerel
#define GRAY            "Gy"        // gray
#define SIEVERT         "Sv"        // sievert
#define KATAL           "kat"       // katal
#define SQMTR           "m2"        // square meter(area)
#define CUBMTR          "m3"        // cubic meter(volume)
#define LTR             "l"         // liter(volume)*
#define MTRSEC          "m/s"       // meter per second(velocity)
#define MTRSEC2         "m/s2"      // meter per square second (acceleration)
#define MTRSEC3         "m3/s"      // cubic meter per second(flow rate)
#define LTRSEC          "l/s"       // liter per second(flow rate)*
#define WATTMTR2        "W/m2"      // watt per square meter(irradiance)
#define CDMTR2          "cd/m2"     // candela per square meter (luminance)
#define BIT             "bit"       // bit(information content)
#define BITSEC          "bit/s"     // bit per second(data rate)
#define LAT             "lat"       // degrees latitude(note 1)
#define LON             "lon"       // degrees longitude(note 1)
#define PH              "pH"        // pH value(acidity; logarithmic quantity)
#define DB              "dB"        // decibel(logarithmic quantity)
#define DBW             "dBW"       // decibel relative to 1 W(power level)
#define BEL             "Bspl"      // bel(sound pressure level logarithmic quantity)*
#define COUNT           "count"     // 1 (counter value)
#define SW_RATIO_A      "/"         // 1 (Ratio e.g., value of a switch
#define SW_RATIO_B      "%"         // 1 (Ratio e.g., value of a switch
#define REL_HUM         "%RH"       // Percentage(Relative Humidity)
#define BATT_PERC       "%EL"       // Percentage(remaining battery energy level)
#define BATT_SEC        "EL"        // seconds(remaining battery energy level)
#define EVT_RATE_SEC    "1/s"       // 1 per second(event rate)
#define EVT_RATE_MIN    "1/min"     // 1 per minute(event rate, "rpm")*
#define BEAT_PER_MIN    "beat/min"  // 1 per minute(Heart rate in beats per minute)*
#define HEART_BEATS     "beats"     // 1 (Cumulative number of heart beats)*
#define SIEMENS_MTR     "S/m"   // Siemens per meter(conductivity)

typedef enum
{
    eMeter,         // meter
    eKilo,          // kilogram
    eGram,          // gram *
    eSecond,        // second
    eAmp,           // ampere
    eKelvin,        // kelvin
    eCandela,       // candela
    eMole,          // mole
    eHz,            // hertz
    eRadian,        // radian
    eSteradian,     // steradian
    eNewton,        // newton
    ePascal,        // pascal
    eJoule,         // joule
    eWatt,          // watt
    eCoulomb,       // coulomb
    eVolt,          // volt
    eFarad,         // farad
    eOhm,           // ohm
    eSiemens,       // siemens
    eWeber,         // weber
    eTesla,         // tesla
    eHenry,         // henry
    eCel,           // degrees Celsius
    eLumen,         // lumen
    eLux,           // lux
    eBecquerel,     // becquerel
    eGray,          // gray
    eSievert,       // sievert
    eKatal,         // katal
    eMtr2,          // square meter(area)
    eMtr3,          // cubic meter(volume)
    eLiter,         // liter(volume)*
    eMtrSec,        // meter per second(velocity)
    eMtrSec2,       // meter per square second (acceleration)
    eMtr3Sec,       // cubic meter per second(flow rate)
    eLiterSec,      // liter per second(flow rate)*
    eWattMtr2,      // watt per square meter(irradiance)
    eCandelaMtr2,   // candela per square meter (luminance)
    eBit,           // bit(information content)
    eBitSec,        // bit per second(data rate)
    eLat,           // degrees latitude(note 1)
    eLon,           // degrees longitude(note 1)
    ePH,            // pH value(acidity; logarithmic quantity)
    edB,            // decibel(logarithmic quantity)
    edBW,           // decibel relative to 1 W(power level)
    eBspl,          // bel(sound pressure level logarithmic quantity)*
    ecount,         // 1 (counter value)
    eSwitchRatio,   // 1 (Ratio e.g., value of a switch
    eSwitchRatioB,  // 1 (Ratio e.g., value of a switch
    eRelHumidity,   // Percentage(Relative Humidity)
    eBattPercnt,    // Percentage(remaining battery energy level)
    eBattSec,       // seconds(remaining battery energy level)
    eEventCount,    // 1 per second(event rate)
    eEventCountMin, // 1 per minute(event rate, "rpm")*
    eBeatMin,       // 1 per minute(Heart rate in beats per minute)*
    eHeartBeats,    // 1 (Cumulative number of heart beats)*
    eSiemensMtr,    // Siemens per meter(conductivity)
} eSenMLUnit;

#endif SENML_UNITS_H
