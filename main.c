/* main.c ---> for tests */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "NMEA.h"

/* generate a random float within a specified range */
float random_float(float min, float max) 
{
    return min + (rand() / (RAND_MAX / (max - min)));
}

/* generate a random GGA/RMC sentence */
void generate_random_GGA(char *gga_sentence) 
{
    int hour = rand() % 24;
    int min = rand() % 60;
    int sec = rand() % 60;
    float lat = random_float(-90.0, 90.0);  /* latitude from -90 to 90 degree */
    float lon = random_float(-180.0, 180.0); /* longitude from  -180 to 180 degree */
    int fix_valid = rand() % 2;  /* 0 or 1 */
    int satellites = rand() % 12 + 4;  /* 4-15 satellites */
    float altitude = random_float(0.0, 5000.0);  /* altitude from 0 to 5000 meters */

    /* formantting GGA sentence */
    snprintf(gga_sentence, 100,
            "$GPGGA,%02d%02d%02d.00,%f,%c,%f,%c,%d,%d,1.0,%.2f,M,0.0,M,,*",
            hour, min, sec, lat, lat >= 0 ? 'N' : 'S', lon, lon >= 0 ? 'E' : 'W', fix_valid, satellites, altitude);
}

/* Function to generate a random RMC sentence */
void generate_random_RMC(char *rmc_sentence) 
{
    int hour = rand() % 24;
    int min = rand() % 60;
    int sec = rand() % 60;
    int day = rand() % 31 + 1;
    int month = rand() % 12 + 1;
    int year = 2024;  /* for simplicity, we use a fixed year */
    float lat = random_float(-90.0, 90.0);  /* latitude from -90 to 90 */
    float lon = random_float(-180.0, 180.0); /* longitude from  -180 to 180 */
    float speed = random_float(0.0, 50.0);  /* Speed from 0 to 50 knots */
    float course = random_float(0.0, 360.0); /* Course from 0 to 360 */

    /* Format the RMC sentence */
    snprintf(rmc_sentence, 100,
            "$GPRMC,%02d%02d%02d,A,%f,%c,%f,%c,%f,%f,%02d%02d%02d,,*",
            hour, min, sec, lat, lat >= 0 ? 'N' : 'S', lon, lon >= 0 ? 'E' : 'W', speed, course, day, month, year);
}

int main() 
{
    srand(time(NULL));  /* Initialize the random number generator */

    char gga_sentence[100];
    char rmc_sentence[100];

    GGASTRUCT gga_data;
    RMCSTRUCT rmc_data;
    GPSSTRUCT gps_data;

    /* Generate random NMEA sentences */
    generate_random_GGA(gga_sentence);
    generate_random_RMC(rmc_sentence);

    /* Print generated data */
    printf("Generated GGA sentence: %s\n", gga_sentence);
    printf("Generated RMC sentence: %s\n", rmc_sentence);

    /* Decode GGA and RMC sentences */
    if (decodeGGA(gga_sentence, &gga_data) == 0) 
    {
        printf("GGA Decoding Successful:\n");
        printf("Time: %02d:%02d:%02d\n", gga_data.time.hour, gga_data.time.min, gga_data.time.sec);
        printf("Latitude: %.6f, Longitude: %.6f\n", gga_data.location.latitude, gga_data.location.longitude);
        printf("Fix Valid: %d, Satellites: %d\n", gga_data.is_fix_valid, gga_data.num_of_sat);
        printf("Altitude: %.2f %c\n", gga_data.altitude.altitude, gga_data.altitude.unit);
    }
    else 
    {
        printf("Failed to decode GGA sentence.\n");
    }

    if (decodeRMC(rmc_sentence, &rmc_data) == 0) 
    {
        printf("RMC Decoding Successful:\n");
        printf("Date: %02d-%02d-%02d\n", rmc_data.date.day, rmc_data.date.month, rmc_data.date.year);
        printf("Latitude: %.6f, Longitude: %.6f\n", rmc_data.location.latitude, rmc_data.location.longitude);
        printf("Speed: %.2f knots, Course: %.2f\n", rmc_data.speed_knots, rmc_data.course);
    } else 
    {
        printf("Failed to decode RMC sentence.\n");
    }

    /* Populate GPS structure with GGA and RMC data */
    initGPS(&gps_data);
    if (populateGPSData(gga_sentence, rmc_sentence, &gps_data) == 0) 
    {
        printf("GPS Data populated successfully.\n");
    } else 
    {
        printf("Failed to populate GPS data.\n");
    }

    return 0;
}

