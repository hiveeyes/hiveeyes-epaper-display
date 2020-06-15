#include "image_record.h"
#define PNG  1
#define BMP  2
//##################################################################################
//############## Hiveeyes Config - to get Data from Hiveeyes Server ################
//##################################################################################
#define Hiveeyes
#ifdef Hiveeyes
String hiveeyes_server   = "swarm.hiveeyes.org";
String uri      = "/api-notls/hiveeyes/testdrive/area-38/fipy-workbench-01/data.json";
String hiveeyes_readTime = "&from=now-10m";

// Configure Names of Data from Server 
String ReadingTemperature_outside    = "temperature.0x77.i2c:0";
String ReadingHumidity_outside       = "humidity.0x77.i2c:0";
String ReadingTemperature_inside_1   = "temperature.28ff641d8fc3944f.onewire:0";
String ReadingTemperature_inside_2   = "temperature.28ff641d8fdf18c1.onewire:0";
String ReadingTemperature_inside_3   = "temperature.3333333333333333.onewire:0";
String ReadingTemperature_inside_4   = "temperature.4444444444444444.onewire:0";
String ReadingTemperature_inside_5   = "temperature.5555555555555555.onewire:0";
String ReadingVoltage                = "system.battery-voltage";
String ReadingWeight                 = "weight.0";
String ReadingRssi                   = "system.wifi.rssi";
#endif

//############################################################################################
//############ Forecast beeflight - Get Data from DWD by Apicast-Hiveeyes ####################
//############################################################################################

//#define Apicast // uncomment with "//"" if not use
#ifdef Apicast
String apicast_server   = "apicast.hiveeyes.org";
String apicast_uri_1    = "/beeflight/forecast/germany/";
String apicast_uri_2    = "berlin_brandenburg/berlin"; // * federal-state and Station                              
#endif
// * You can get a list of available Stations here http://apicast.hiveeyes.org/ under Index/List of side slugs
// or build yourself from here https://www.dwd.de/DE/leistungen/biene_flug/bienenflug.html from "Bundesland" and Station
// Example: String apicast_uri_2    = "Bundesland"+"/"+"Station";

//###########################################################################
//############### get Pictures from Http/Https ##############################
//###########################################################################
// add Pictures to Use in hiveeyes-epaper.ino

#define image_available 6                  // Number of Images you will use. 
Image_record_type get_Image[image_available]; //don't Change
void init_image() {                                          //The counter begin with 0. 3 Images_available means Image[0] to image[2]
// ******** Image[0] *********
    get_Image[4].name  = "Test_PNG_300_225‬"; // Own name of Picture it must be different from the other Names!!! to use in future by draw_picture_from_Web
    get_Image[4].host  = "imagecast.hiveeyes.org";
    get_Image[4].uri   = "/?uri=https%3A%2F%2Fswarm.hiveeyes.org%2Fgrafana%2Frender%2Fd-solo%2Fstart%2Fwelcome%3ForgId%3D2%26panelId%3D9%26from%3D1555547678893%26to%3D1557998019523%26var-beekeeper%3DAll%26var-COMMON_CDC_NAME%3DBerlin-Tegel%26var-COMMON_MOSMIX_NAME%3DBERLIN-TEGEL%26var-STATION%3DBerlin-Tegel%26var-sensors%3Ddefault_1_sensors%26theme%3Dlight%26width%3D400%26height%3D300%26tz%3DEurope%252FBerlin&width=200&format=png&monochrome=200";
    get_Image[4].typ   = PNG; // You can use PNG or BMP 
    get_Image[4].https = false; // use https = true / use http = false 
    get_Image[4].Sign  = "";

// ******** image1 *********
// uncomment (delete = //) and add next Picture here if you want to use. It wil begin with Image[1]
// for more Pictures Copy and edit Image[2]

    get_Image[1].name  = "Test_PNG_200_150"; // to use in future by draw_any_picture_from_Web
    get_Image[1].host  = "imagecast.hiveeyes.org";
    get_Image[1].uri   = "/?uri=https%3A%2F%2Fswarm.hiveeyes.org%2Fgrafana%2Frender%2Fd-solo%2Fstart%2Fwelcome%3ForgId%3D2%26panelId%3D9%26from%3D1555547678893%26to%3D1557998019523%26var-beekeeper%3DAll%26var-COMMON_CDC_NAME%3DBerlin-Tegel%26var-COMMON_MOSMIX_NAME%3DBERLIN-TEGEL%26var-STATION%3DBerlin-Tegel%26var-sensors%3Ddefault_1_sensors%26theme%3Dlight%26width%3D400%26height%3D300%26tz%3DEurope%252FBerlin&width=304&format=png&monochrome=200";
    get_Image[1].typ   = PNG;
    get_Image[1].https = false; // use https = true / use http = false 
    get_Image[1].Sign  = "";

    get_Image[2].name  = "Test_PNG_400_300"; // Own name of Picture it must be different from the other Names!!! to use in future by draw_picture_from_Web
    get_Image[2].host  = "imagecast.hiveeyes.org";
    get_Image[2].uri   = "/?uri=https%3A%2F%2Fswarm.hiveeyes.org%2Fgrafana%2Frender%2Fd-solo%2Fstart%2Fwelcome%3ForgId%3D2%26panelId%3D9%26from%3D1555547678893%26to%3D1557998019523%26var-beekeeper%3DAll%26var-COMMON_CDC_NAME%3DBerlin-Tegel%26var-COMMON_MOSMIX_NAME%3DBERLIN-TEGEL%26var-STATION%3DBerlin-Tegel%26var-sensors%3Ddefault_1_sensors%26theme%3Dlight%26width%3D400%26height%3D300%26tz%3DEurope%252FBerlin&width=400&format=png&monochrome=200";
    get_Image[2].typ   = PNG; // You can use PNG or BMP 
    get_Image[2].https = false; // use https = true / use http = false 
    get_Image[2].Sign  = "";

    get_Image[3].name  = "Test_PNG_400_200"; // Own name of Picture it must be different from the other Names!!! to use in future by draw_picture_from_Web
    get_Image[3].host  = "imagecast.hiveeyes.org";
    get_Image[3].uri   = "/?uri=https%3A%2F%2Fswarm.hiveeyes.org%2Fgrafana%2Frender%2Fd-solo%2Fstart%2Fwelcome%3ForgId%3D2%26panelId%3D9%26from%3D1555547678893%26to%3D1557998019523%26var-beekeeper%3DAll%26var-COMMON_CDC_NAME%3DBerlin-Tegel%26var-COMMON_MOSMIX_NAME%3DBERLIN-TEGEL%26var-STATION%3DBerlin-Tegel%26var-sensors%3Ddefault_1_sensors%26theme%3Dlight%26width%3D400%26height%3D200%26tz%3DEurope%252FBerlin&width=400&format=png&monochrome=200";
    get_Image[3].typ   = PNG; // You can use PNG or BMP 
    get_Image[3].https = false; // use https = true / use http = false 
    get_Image[3].Sign  = "";

    get_Image[5].name  = "Test2_PNG_160_270"; // Own name of Picture it must be different from the other Names!!! to use in future by draw_picture_from_Web
    get_Image[5].host  = "imagecast.hiveeyes.org";
    get_Image[5].uri   = "/?uri=https%3A%2F%2Fswarm.hiveeyes.org%2Fgrafana%2Frender%2Fd-solo%2FOvnLNPqiz%2Fstatista-stockubersicht-and-bienenwetter-testvolk-zk-u%3FpanelId%3D23%26orgId%3D2%26var-beekeeper%3Dhiveeyes_open_hive_test_statista%26var-sensors%3DAll%26var-COMMON_CDC_NAME%3DBerlin-Tegel%26var-COMMON_MOSMIX_NAME%3DBERLIN-TEGEL%26var-STATION%3DBerlin-Tegel%26theme%3Dlight%26width%3D200%26height%3D400%26tz%3DEurope%252FBerlin&monochrome=200&format=png";
    get_Image[5].typ   = PNG; // You can use PNG or BMP 
    get_Image[5].https = false; // use https = true / use http = false 
    get_Image[5].Sign  = "";


} // !!!Do not delete This    
//################################################################################
//################## Display Pin Config ##########################################
//################################################################################

// Connections for e.g. Waveshare ESP32 e-Paper Driver Board
// Numbers = Gipo Pins from ESP

#define EPD_BUSY    25 // to EPD BUSY
#define EPD_CS      15 // to EPD CS
#define EPD_RST     26 // to EPD RST
#define EPD_DC      27 // to EPD DC
#define EPD_SCK     13 // to EPD CLK
#define EPD_MISO    12 // Master-In Slave-Out not used, as no data from display
#define EPD_MOSI    14 // to EPD DIN
#define BATTERY_PIN 35 // if you will Use must add 2 Resistors an Example:comming soon

//################################################################################
//################## Touch config ################################################
//################################################################################

#define Enable_Touch // uncommited this for diable Touch

#ifdef Enable_Touch
    #define Threshold 25 // Greater the value, more the sensitivity 
    
    #define TouchT0 // Gipo  4
    //#define TouchT1 // Gipo  0
    //#define TouchT2   // Gipo  2
    //#define TouchT3 // Gipo 15 
    //#define TouchT4 // Gipo 13
    //#define TouchT5 // Gipo 12
    //#define TouchT6 // Gipo 14
    //#define TouchT7   // Gipo 27
    #define TouchT8   // Gipo 32
    #define TouchT9   // Gipo 33
#endif
// ###########################################################################
// build something complete no Changes need here!!!
#ifdef Apicast
String apicast_uri = apicast_uri_1 + apicast_uri_2;
#endif
String hiveeyes_uri = uri + "?include=" + ReadingTemperature_outside  + ","
                                        + ReadingHumidity_outside + ","
                                        + ReadingTemperature_inside_1 + ","
                                        + ReadingTemperature_inside_2 + ","
                                        + ReadingTemperature_inside_3 + ","
                                        + ReadingTemperature_inside_4 + ","
                                        + ReadingTemperature_inside_5 + ","
                                        + ReadingVoltage + ","
                                        + ReadingWeight + ","
                                        + ReadingRssi;

