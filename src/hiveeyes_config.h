#include "image_record.h"
int PNG = 1;
int BMP = 2;
//##################################################################################
//############## Hiveeyes Config - to get Data from Hiveeyes Server ################
//##################################################################################
#define Hiveeyes
#if defined(Hiveeyes)
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
#define Apicast / uncomment with // if you will not use
#if defined(Apicast)
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

#define image_available 3                  // Number of Images you will use. 
Image_record_type get_Image[image_available]; //don't Change
void init_image() {                                          //The counter begin with 0. 3 Images_available means Image[0] to image[2]
// ******** Image[0] *********
    get_Image[0].name  = "TestPNG"; // Own name of Picture it must be different from the other Names!!! to use in future by draw_picture_from_Web
    get_Image[0].host  = "imagecast.hiveeyes.org";
    get_Image[0].uri   = "/?uri=https%3A%2F%2Fswarm.hiveeyes.org%2Fgrafana%2Frender%2Fd-solo%2Fstart%2Fwelcome%3ForgId%3D2%26panelId%3D9%26from%3D1555547678893%26to%3D1557998019523%26var-beekeeper%3DAll%26var-COMMON_CDC_NAME%3DBerlin-Tegel%26var-COMMON_MOSMIX_NAME%3DBERLIN-TEGEL%26var-STATION%3DBerlin-Tegel%26var-sensors%3Ddefault_1_sensors%26theme%3Dlight%26width%3D400%26height%3D300%26tz%3DEurope%252FBerlin&monochrome=200&crop=40,50,-50,-40&width=200&format=png";
    get_Image[0].typ   = PNG; // You can use PNG or BMP 
    get_Image[0].https = false; // use https = true / use http = false 
    get_Image[0].Sign  = "";

// ******** image1 *********
// uncomment (delete = //) and add next Picture here if you want to use. It wil begin with Image[1]
// for more Pictures Copy and edit Image[2]

    get_Image[1].name  = "TestBMP"; // to use in future by draw_any_picture_from_Web
    get_Image[1].host  = "imagecast.hiveeyes.org";
    get_Image[1].uri   = "/?uri=https%3A%2F%2Fswarm.hiveeyes.org%2Fgrafana%2Frender%2Fd-solo%2Fstart%2Fwelcome%3ForgId%3D2%26panelId%3D9%26from%3D1555547678893%26to%3D1557998019523%26var-beekeeper%3DAll%26var-COMMON_CDC_NAME%3DBerlin-Tegel%26var-COMMON_MOSMIX_NAME%3DBERLIN-TEGEL%26var-STATION%3DBerlin-Tegel%26var-sensors%3Ddefault_1_sensors%26theme%3Dlight%26width%3D400%26height%3D300%26tz%3DEurope%252FBerlin&monochrome=200&crop=40,50,-50,-40&width=200&format=bmp";
    get_Image[1].typ   = BMP;
    get_Image[1].https = false; // use https = true / use http = false 
    get_Image[1].Sign  = "";
}
//################################################################################
//################## Display Pin Config ##########################################
//################################################################################

// Connections for e.g. Waveshare ESP32 e-Paper Driver Board
// Numbers = Gipo Pins from ESP

static const uint8_t EPD_BUSY = 25; // to EPD BUSY
static const uint8_t EPD_CS   = 15; // to EPD CS
static const uint8_t EPD_RST  = 26; // to EPD RST
static const uint8_t EPD_DC   = 27; // to EPD DC
static const uint8_t EPD_SCK  = 13; // to EPD CLK
static const uint8_t EPD_MISO = 12; // Master-In Slave-Out not used, as no data from display
static const uint8_t EPD_MOSI = 14; // to EPD DIN
static const uint8_t BATTERY_PIN = 35; // if you will Use must add 2 Resistors an Example:comming soon

//################################################################################
//################## Button config comming soon, too##############################
//################################################################################


// ###########################################################################
// build something complete no Changes need here!!!

String apicast_uri = apicast_uri_1 + apicast_uri_2;
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

