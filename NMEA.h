/*
 * NMEA.h
 *
 * Header file for parsing NMEA sentences for GPS data
 */

#ifndef INC_NMEA_H_
#define INC_NMEA_H_

#include <stdint.h>

// Define TIME structure for storing time information
typedef struct
{
    uint32_t time;     
} TIME;

// Define LOCATION structure for storing location information
typedef struct
{
    int32_t latitude; 
    char NS;
    int32_t longitude; 
    char EW;
} LOCATION;

// Define ALTITUDE structure for storing altitude information
typedef struct
{
    //float altitude;
    int32_t altitude;  
    char unit;
} ALTITUDE;

// Define DATE structure for storing date information
typedef struct
{
    uint8_t day;
    uint8_t month;
    uint16_t year;
} DATE;

// Define GGASTRUCT for storing parsed GGA sentence data
typedef struct
{
    LOCATION location;  
    TIME time;          
    ALTITUDE altitude;  
    int8_t is_fix_valid;
    uint8_t num_of_sat; 
} GGASTRUCT;

// Define RMCSTRUCT for storing parsed RMC sentence data
typedef struct
{
    DATE date;
    float speed_knots;
    float course;
    int is_data_valid;
} RMCSTRUCT;

// Define GPSSTRUCT for storing both GGA and RMC data
typedef struct
{
    GGASTRUCT ggastruct;
    RMCSTRUCT rmcstruct;
} GPSSTRUCT;

// Public function declarations
float simple_atof(const char* str);
int decodeGGA(char *GGAbuffer, GGASTRUCT *gga);
int decodeRMC(char *RMCbuffer, RMCSTRUCT *rmc);
void initGPS(GPSSTRUCT *gps);
int populateGPSData(char *ggaSentence, char *rmcSentence, GPSSTRUCT *gps);

#endif /* INC_NMEA_H_ */

