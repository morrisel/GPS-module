/*
 * NMEA.h
 *
 * Header file for parsing NMEA sentences into structured data.
 */

#ifndef INC_NMEA_H_
#define INC_NMEA_H_

#include <stdint.h>

/* Define TIME structure to hold time data */
struct TIME 
{
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
};
#define TIME struct TIME

/* Define LOCATION structure to hold GPS location data */
struct LOCATION 
{
    float latitude;
    char  NS;
    float longitude;
    char  EW;
};
#define LOCATION struct LOCATION

/* Define ALTITUDE structure to hold altitude data */
struct ALTITUDE 
{
    float altitude;
    char  unit;
};
#define ALTITUDE struct ALTITUDE

/* Define DATE structure to hold date data */
struct DATE 
{
    uint8_t   day;
    uint8_t   month;
    uint16_t  year;
};
#define DATE struct DATE

/* Define GGASTRUCT for holding GGA sentence data */
struct GGASTRUCT 
{
    LOCATION    location;
    TIME        time;
    int         is_fix_valid;
    ALTITUDE    altitude;
    int         num_of_sat;
};
#define GGASTRUCT struct GGASTRUCT

/* Define RMCSTRUCT for holding RMC sentence data */
struct RMCSTRUCT 
{
    DATE    date;
    float   speed_knots;
    float   course;
    int     is_data_valid;
};
#define RMCSTRUCT struct RMCSTRUCT

/* Define GPSSTRUCT for holding all GPS data */
struct GPSSTRUCT 
{
    GGASTRUCT ggastruct;
    RMCSTRUCT rmcstruct;
};
#define GPSSTRUCT struct GPSSTRUCT

/* Function to convert string to float */
float simple_atof(const char* str);

/* Functions to decode NMEA sentences */
int decodeGGA(char *GGAbuffer, GGASTRUCT *gga);
int decodeRMC(char *RMCbuffer, RMCSTRUCT *rmc);

/* Function to initialize a GPSSTRUCT */
void initGPS(GPSSTRUCT *gps);

/* Function to populate GPS data from GGA and RMC sentences */
int populateGPSData(char *ggaSentence, char *rmcSentence, GPSSTRUCT *gps);

#endif /* INC_NMEA_H_ */

