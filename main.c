/* main.c ---> for tests */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "NMEA.h"


int main(void)
{
    // Example NMEA sentences
    char ggaSentence[] = "$GPGGA,123456.00,3749.1234,N,12225.5678,W,1,08,1.0,15.6,M,,,*47";
    char rmcSentence[] = "$GPRMC,123456.00,A,3749.1234,N,12225.5678,W,0.5,90.0,101221,,,A*68";

    // Structures to store decoded data
    GGASTRUCT ggaData;
    RMCSTRUCT rmcData;
    GPSSTRUCT gpsData;

    // Initialize GPS structure
    initGPS(&gpsData);

    printf("==== NMEA Parsing Function Tests ====\n");

    // Testing decodeGGA
    if (decodeGGA(ggaSentence, &ggaData) == 0)
    {
        printf("GGA sentence successfully decoded:\n");
        printf("  Time: %02d:%02d:%02d\n", DECODE_HOUR(ggaData.time.time),
               DECODE_MIN(ggaData.time.time), DECODE_SEC(ggaData.time.time));
        printf("  Latitude: %d (North/South: %c)\n", ggaData.location.latitude, ggaData.location.NS);
        printf("  Longitude: %d (East/West: %c)\n", ggaData.location.longitude, ggaData.location.EW);
        printf("  Altitude: %d mm (%c)\n", ggaData.altitude.altitude, ggaData.altitude.unit);
        printf("  Number of satellites: %d\n", ggaData.numsat);
        printf("  Fix valid: %s\n", ggaData.is_fix_valid ? "Yes" : "No");
    }
    else
    {
        printf("Error parsing GGA sentence.\n");
    }

    // Testing decodeRMC
    if (decodeRMC(rmcSentence, &rmcData) == 0)
    {
        printf("\nRMC sentence successfully decoded:\n");
        printf("  Date: %02d/%02d/%04d\n", rmcData.date.day, rmcData.date.month, rmcData.date.year);
        printf("  Speed: %d knots (x1000)\n", rmcData.speed_knots);
        printf("  Course: %d degrees (x100)\n", rmcData.course);
        printf("  Data valid: %s\n", rmcData.is_data_valid ? "Yes" : "No");
    }
    else
    {
        printf("Error parsing RMC sentence.\n");
    }

    // Testing populateGPSData
    if (populateGPSData(ggaSentence, rmcSentence, &gpsData) == 0)
    {
        printf("\nGPS structure successfully populated:\n");
        printf("  Time: %02d:%02d:%02d\n", DECODE_HOUR(gpsData.ggastruct.time.time),
               DECODE_MIN(gpsData.ggastruct.time.time), DECODE_SEC(gpsData.ggastruct.time.time));
        printf("  Latitude: %d (North/South: %c)\n", gpsData.ggastruct.location.latitude, gpsData.ggastruct.location.NS);
        printf("  Longitude: %d (East/West: %c)\n", gpsData.ggastruct.location.longitude, gpsData.ggastruct.location.EW);
        printf("  Altitude: %d mm (%c)\n", gpsData.ggastruct.altitude.altitude, gpsData.ggastruct.altitude.unit);
        printf("  Speed: %d knots (x1000)\n", gpsData.rmcstruct.speed_knots);
        printf("  Course: %d degrees (x100)\n", gpsData.rmcstruct.course);
    }
    else
    {
        printf("Error populating GPS structure.\n");
    }

    // Testing nmea_atof_fixed
    const char *nmeaNumber = "1234.5678";
    int32_t fixedValue = nmea_atof_fixed(nmeaNumber, 1000000);
    printf("\nConverting string '%s' to fixed-point representation: %d\n", nmeaNumber, fixedValue);

    printf("\n==== Tests completed ====\n");
    return 0;
}


