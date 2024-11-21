/*
 * NMEA.c
 *
 * This file provides functions for decoding NMEA sentences (GGA and RMC) into
 * structured data representations. It includes functions for parsing GPS data
 * and converting them into human-readable and usable structures for further
 * processing in the system.
 *
 * Supported NMEA sentences:
 * - GGA (Global Positioning System Fix Data)
 * - RMC (Recommended Minimum Specific GPS/Transit Data)
 *
 * Structures:
 * - GGASTRUCT: Holds GGA sentence data (time, location, altitude, fix validity, number of satellites)
 * - RMCSTRUCT: Holds RMC sentence data (date, speed, course, validity)
 * - GPSSTRUCT: Combines GGASTRUCT and RMCSTRUCT for complete GPS information
 *
 * Functions:
 * - decodeGGA: Parses a GGA sentence and populates a GGASTRUCT.
 * - decodeRMC: Parses an RMC sentence and populates an RMCSTRUCT.
 * - initGPS: Initializes a GPSSTRUCT to default values.
 * - populateGPSData: Fills a GPSSTRUCT by parsing both GGA and RMC sentences.
 *
 *  Created on: Nov 20, 2024
 *      Author: morris
 *
 *
 */

#include "NMEA.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * parse_coordinate - Parses a coordinate from the NMEA format.
 *
 * This function takes a coordinate string in the NMEA format (DDMM.MMMM)
 * and converts it into decimal degrees. The direction (N/S, E/W) is used
 * to apply a correction to the sign of the coordinate.
 *
 * Arguments:
 *   str - Coordinate string in NMEA format (DDMM.MMMM).
 *   direction - Direction indicator ('N', 'S', 'E', 'W').
 *
 * Returns:
 *   The coordinate as a float in decimal degrees.
 */
static float parse_coordinate(const char *str, char direction) 
{
    float degrees = atof(str);
    int int_degrees = (int)(degrees / 100);
    float minutes = degrees - (int_degrees * 100);
    float decimal_degrees = int_degrees + (minutes / 60.0);

    /* Apply direction correction (negative for South or West) */
    if (direction == 'S' || direction == 'W') 
    {
        decimal_degrees = -decimal_degrees;
    }
    return decimal_degrees;
}

/*
 * decodeGGA - Decodes the GGA sentence into a GGASTRUCT.
 *
 * This function parses a GGA sentence, extracting the time, latitude,
 * longitude, altitude, fix validity, and the number of satellites from
 * the comma-separated fields.
 *
 * Arguments:
 *   GGAbuffer - The input GGA sentence as a string.
 *   gga - Pointer to a GGASTRUCT to be populated.
 *
 * Returns:
 *   0 on success, -1 on failure (e.g., malformed sentence).
 */
int decodeGGA(char *GGAbuffer, GGASTRUCT *gga) 
{
    /* Validate input */
    if (!GGAbuffer || !gga) 
    {
        return -1; /* Invalid input */
    }

    char *token = strtok(GGAbuffer, ",");
    int field_num = 0;

    /* Parse each comma-separated field */
    while (token != NULL) 
    {
        switch (field_num) 
        {
            case 1: /* UTC Time (HHMMSS) */
                gga->time.hour = (token[0] - '0') * 10 + (token[1] - '0');
                gga->time.min = (token[2] - '0') * 10 + (token[3] - '0');
                gga->time.sec = (token[4] - '0') * 10 + (token[5] - '0');
                break;
            case 2: /* Latitude */
                gga->location.latitude = parse_coordinate(token, gga->location.NS);
                break;
            case 3: /* N/S Indicator */
                gga->location.NS = token[0];
                break;
            case 4: /* Longitude */
                gga->location.longitude = parse_coordinate(token, gga->location.EW);
                break;
            case 5: /* E/W Indicator */
                gga->location.EW = token[0];
                break;
            case 6: /* Fix Validity */
                gga->is_fix_valid = atoi(token);
                break;
            case 7: /* Number of Satellites */
                gga->num_of_sat = atoi(token);
                break;
            case 9: /* Altitude */
                gga->altitude.altitude = atof(token);
                break;
            case 10: /* Altitude Unit */
                gga->altitude.unit = token[0];
                break;
        }
        token = strtok(NULL, ",");
        field_num++;
    }
    return 0; /* Success */
}

/*
 * decodeRMC - Decodes the RMC sentence into an RMCSTRUCT.
 *
 * This function parses an RMC sentence, extracting the time, validity,
 * latitude, longitude, speed, course, and date from the comma-separated fields.
 *
 * Arguments:
 *   RMCbuffer - The input RMC sentence as a string.
 *   rmc - Pointer to an RMCSTRUCT to be populated.
 *
 * Returns:
 *   0 on success, -1 on failure (e.g., malformed sentence).
 */
int decodeRMC(char *RMCbuffer, RMCSTRUCT *rmc) 
{
    /* Validate input */
    if (!RMCbuffer || !rmc) 
    {
        return -1; /* Invalid input */
    }

    char *token = strtok(RMCbuffer, ",");
    int field_num = 0;

    /* Parse each comma-separated field */
    while (token != NULL) 
    {
        switch (field_num) 
        {
            case 1: /* UTC Time (HHMMSS) */
                rmc->date.day = (token[0] - '0') * 10 + (token[1] - '0');
                rmc->date.month = (token[2] - '0') * 10 + (token[3] - '0');
                rmc->date.year = 2000 + (token[4] - '0') * 10 + (token[5] - '0'); /* Adjust for 21st century */
                break;
            case 2: /* Validity ('A' = valid, 'V' = invalid) */
                rmc->is_data_valid = (token[0] == 'A') ? 1 : 0;
                break;
            case 3: /* Latitude */
                rmc->location.latitude = parse_coordinate(token, rmc->location.NS);
                break;
            case 4: /* N/S Indicator */
                rmc->location.NS = token[0];
                break;
            case 5: /* Longitude */
                rmc->location.longitude = parse_coordinate(token, rmc->location.EW);
                break;
            case 6: /* E/W Indicator */
                rmc->location.EW = token[0];
                break;
            case 7: /* Speed over ground in knots */
                rmc->speed_knots = atof(token);
                break;
            case 8: /* Course over ground */
                rmc->course = atof(token);
                break;
        }
        token = strtok(NULL, ",");
        field_num++;
    }
    return 0; /* Success */
}

/*
 * initGPS - Initializes a GPSSTRUCT to default values.
 *
 * Arguments:
 *   gps - Pointer to a GPSSTRUCT to be initialized.
 */
void initGPS(GPSSTRUCT *gps)
{
    memset(gps, 0, sizeof(GPSSTRUCT)); /* Set all fields to 0 */
}

/*
 * populateGPSData - Populates a GPSSTRUCT with data from GGA and RMC sentences.
 *
 * This function decodes a GGA sentence and an RMC sentence and fills a
 * GPSSTRUCT with the parsed data.
 *
 * Arguments:
 *   ggaSentence - The GGA sentence as a string.
 *   rmcSentence - The RMC sentence as a string.
 *   gps - Pointer to the GPSSTRUCT to be populated.
 *
 * Returns:
 *   0 on success, -1 if any decoding fails.
 */
int populateGPSData(char *ggaSentence, char *rmcSentence, GPSSTRUCT *gps)
{
    if (decodeGGA(ggaSentence, &gps->ggastruct) != 0)

    {
        return -1; /* Failed to parse GGA data */
    }

    if (decodeRMC(rmcSentence, &gps->rmcstruct) != 0) 
    {
        return -1; /* Failed to parse RMC data */
    }

    return 0; /* Success */
}

