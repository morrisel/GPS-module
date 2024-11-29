/*
 * NMEA.h
 *
 * Header file for parsing NMEA sentences for GPS data
 */

#ifndef INC_NMEA_H_
#define INC_NMEA_H_

#include <stdint.h>


// <AB0-1:
// LOCATION structure with fixed-point representation
typedef struct
{                               //  times 1000000 values --->  (1.234567° -> 1234567)
    int32_t     latitude;       //  4 byte     Latitude  * 1e7 (10^6) (example: 12.345678 -> 123456780)
    int32_t     longitude;      //  4 byte     Longitude * 1e7 (10^6)
    char        NS;             //  1 byte
    char        EW;             //  1 byte
    uint16_t    padding;        //             bytes aligment up-to 4 byte
} LOCATION;


// TIME structure packed into 32-bit
typedef struct
{   
    uint32_t time;          // HHMMSS format in uint32_t (example, 123456 for 12:34:56)
                            //                                     |||||| 
                            //                                     HHMMSS 
                            // 
                            // uint8_t   hour = (time >> 16) & 0xFF;
                            // uint8_t   min  = (time >> 8)  & 0xFF;
                            // uint8_t   sec  = time         & 0xFF;
} TIME;
// :AB0-1>




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
/* <t1: */
int32_t nmea_atof_fixed(const char* str, int scale);
/* :t1> */

/* <d2: */
int decodeGGA(char *GGAbuffer, GGASTRUCT *gga);
/* :d2> */

int decodeRMC(char *RMCbuffer, RMCSTRUCT *rmc);
void initGPS(GPSSTRUCT *gps);
int populateGPSData(char *ggaSentence, char *rmcSentence, GPSSTRUCT *gps);

#endif /* INC_NMEA_H_ */

