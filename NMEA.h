/*
 * NMEA.h
 *
 * This header defines the data structures and functions for parsing NMEA
 * sentences (GGA and RMC) for GPS data processing. The defined structures
 * (GGASTRUCT, RMCSTRUCT, GPSSTRUCT) organize information about location,
 * time, altitude, and satellite data, providing a straightforward way
 * to store and access parsed GPS information.
 *
 * Structures:
 *   TIME        - Holds UTC time in hours, minutes, and seconds.
 *   LOCATION    - Contains latitude and longitude information, along with
 *                 directional indicators (N/S and E/W).
 *   ALTITUDE    - Represents altitude above sea level with a specified unit.
 *   DATE        - Stores day, month, and year of the GPS fix.
 *   GGASTRUCT   - Combines location, time, altitude, and satellite count,
 *                 along with a validity flag to check the GPS fix.
 *   RMCSTRUCT   - Contains date, speed, course, and a validity flag.
 *   GPSSTRUCT   - Bundles GGA and RMC data structures for complete GPS data.
 *
 * Functions:
 *   decodeGGA() - Parses GGA sentences to extract basic GPS fix data and
 *                 store it in a GGASTRUCT.
 *   decodeRMC() - Parses RMC sentences for information on date, speed, and
 *                 course, storing it in an RMCSTRUCT.
 *
 * Usage:
 * Include this header to access structured data parsing for GPS applications.
 * Use decodeGGA() and decodeRMC() to fill GGASTRUCT and RMCSTRUCT structures
 * with parsed data from NMEA sentences.
 *
 * Created on: Nov 20, 2024
 * Author: morris
 *
 */



#ifndef INC_NMEA_H_
#define INC_NMEA_H_

#include <stdint.h>

struct TIME
{
   uint8_t hour;
   uint8_t min;
   uint8_t sec;
};
#define TIME struct TIME

struct LOCATION
{
   float latitude;
   char NS;
   float longitude;
   char EW;
};
#define LOCATION struct LOCATION

struct ALTITUDE
{
   float altitude;
   char unit;
};
#define ALTITUDE struct ALTITUDE

struct DATE
{
   uint8_t day;
   uint8_t month;
   uint16_t year;
};
#define DATE struct DATE

struct GGASTRUCT
{
   LOCATION location;
   TIME time;
   int is_fix_valid;
   ALTITUDE altitude;
   int num_of_sat;
};
#define GGASTRUCT struct GGASTRUCT

struct RMCSTRUCT
{
   DATE date;
   float speed_knots;
   float course;
   int is_data_valid;
};
#define RMCSTRUCT struct RMCSTRUCT

struct GPSSTRUCT
{
   GGASTRUCT ggastruct;
   RMCSTRUCT rmcstruct;
};
#define GPSSTRUCT struct GPSSTRUCT

/* Function to decode GGA NMEA sentence into GGASTRUCT */
int decodeGGA(char *GGAbuffer, GGASTRUCT *gga);

/* Function to decode RMC NMEA sentence into RMCSTRUCT */
int decodeRMC(char *RMCbuffer, RMCSTRUCT *rmc);


#endif /* INC_NMEA_H_ */
