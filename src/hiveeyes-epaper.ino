/* ESP32 Weather Display using an EPD 4.2" Display, obtains data from Open Weather Map, decodes it and then displays it.
  ####################################################################################################################################
  This software, the ideas and concepts is Copyright (c) David Bird 2018. All rights to this software are reserved.

  Any redistribution or reproduction of any part or all of the contents in any form is prohibited other than the following:
  1. You may print or download to a local hard disk extracts for your personal and non-commercial use only.
  2. You may copy the content to individual third parties for their personal use, but only if you acknowledge the author David Bird as the source of the material.
  3. You may not, except with my express written permission, distribute or commercially exploit the content.
  4. You may not transmit it or store it in any other website or other form of electronic retrieval system for commercial purposes.

  The above copyright ('as annotated') notice and this permission notice shall be included in all copies or substantial portions of the Software and where the
  software use is visible to an end-user.

  THE SOFTWARE IS PROVIDED "AS IS" FOR PRIVATE USE ONLY, IT IS NOT FOR COMMERCIAL USE IN WHOLE OR PART OR CONCEPT. FOR PERSONAL USE IT IS SUPPLIED WITHOUT WARRANTY
  OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHOR OR COPYRIGHT HOLDER BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
  See more at http://www.dsbird.org.uk
*/
#include "owm_credentials.h"  // See 'owm_credentials' tab and enter your OWM API key and set the Wifi SSID and PASSWORD
#include "hiveeyes_config.h"
#include <ArduinoJson.h>       // https://github.com/bblanchon/ArduinoJson
#include <WiFi.h>              // Built-in
#include "time.h"              // Built-in
#include <SPI.h>               // Built-in
#include <upng.h>
#define  ENABLE_GxEPD2_display 0
#include <GxEPD2_BW.h>
#include <GxEPD2_3C.h>
#include <U8g2_for_Adafruit_GFX.h>
#include "epaper_fonts.h"
#include "forecast_record.h"
#include "hive_record.h"
#include "apicast_record.h"
#include <Adafruit_I2CDevice.h>

//#include "lang.h"
//#include "lang_cz.h"                // Localisation (Czech)
//#include "lang_fr.h"                // Localisation (French)
#include "lang_gr.h"                // Localisation (German)
//#include "lang_it.h"                // Localisation (Italian)
//#include "lang_nl.h"                // Localisation (Dutch)
//#include "lang_pl.h"                // Localisation (Polish)
#define SCREEN_WIDTH  400.0    // Set for landscape mode, don't remove the decimal place!
#define SCREEN_HEIGHT 300.0

enum alignment {LEFT, RIGHT, CENTER};


GxEPD2_BW<GxEPD2_420, GxEPD2_420::HEIGHT> display(GxEPD2_420(/*CS=D8*/ EPD_CS, /*DC=D3*/ EPD_DC, /*RST=D4*/ EPD_RST, /*BUSY=D2*/ EPD_BUSY));

U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;  // Select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall

// Using fonts:
// u8g2_font_helvB08_tf
// u8g2_font_helvB10_tf
// u8g2_font_helvB12_tf
// u8g2_font_helvB14_tf
// u8g2_font_helvB18_tf
// u8g2_font_helvB24_tf

//################  VERSION  ##########################
String version = "12.3";     // Version of this program
//################ VARIABLES ###########################

boolean LargeIcon = true, SmallIcon = false;
#define Large  8           // For icon drawing, needs to be odd number for best effect
#define Small  4            // For icon drawing, needs to be odd number for best effect
String  time_str, date_str; // strings to hold time and received weather data
int     wifi_signal, CurrentHour = 0, CurrentMin = 0, CurrentSec = 0;
long    StartTime = 0;

//################ PROGRAM VARIABLES and OBJECTS ################

#define max_readings 24
Forecast_record_type  WxConditions[1];
Forecast_record_type  WxForecast[max_readings];
#include <common.h>

#define max_readings_hiveeyes 24
int hive_readings;
Hive_record_type      hive_data[max_readings_hiveeyes];
#include "hiveeyes_client.h"
#ifdef Apicast
  Apicast_record_type   beeflight[10];
  #include "apicast_client.h"
#endif
String image_bitmap;
#include "image_client.h"

#define autoscale_on  true
#define autoscale_off false
#define barchart_on   true
#define barchart_off  false

float pressure_readings[max_readings]    = {0};
float temperature_readings[max_readings] = {0};
float humidity_readings[max_readings]    = {0};
float rain_readings[max_readings]        = {0};
float snow_readings[max_readings]        = {0};
float weight_readings[max_readings_hiveeyes]    = {0};

long SleepDuration = 30; // Sleep time in minutes, aligned to the nearest minute boundary, so if 30 will always update at 00 or 30 past the hour
int  WakeupTime    = 1;  // Don't wakeup until after 07:00 to save battery power
int  SleepTime     = 22; // Sleep after (23+1) 00:00 to save battery power

//#########################################################################################
void setup() {
  StartTime = millis();
  Serial.begin(115200);
  int wakeup_reason = esp_sleep_get_wakeup_cause();
  
  init_image();
  InitialiseDisplay(); // Give screen time to initialise by getting weather data!
  if (StartWiFi() == WL_CONNECTED && SetupTime() == true) {
    //if (CurrentHour >= WakeupTime && CurrentHour <= SleepTime ) {
    WiFiClient client;
    //Obtain weather and Hiveeyes information.
   // wifi client object 
  }
  #ifdef Enable_Touch
    //int wake_reason = print_wakeup_reason();
    touch_pad_t touchPin;
    touchPin = esp_sleep_get_touchpad_wakeup_status();
    //init_touch();
    switch(touchPin){
      case 0  : Serial.println("Touch 0 detected"); DisplayWeather();break;
      case 1  : Serial.println("Touch 1 detected"); BeginSleep();break;
      case 2  : Serial.println("Touch 2 detected"); BeginSleep();break;
      case 3  : Serial.println("Touch 3 detected"); BeginSleep();break;
      case 4  : Serial.println("Touch 4 detected"); BeginSleep();break;
      case 5  : Serial.println("Touch 5 detected"); BeginSleep();break;
      case 6  : Serial.println("Touch 6 detected"); BeginSleep();break;
      case 7  : Serial.println("Touch 7 detected"); DisplayWeather();break;
      case 8  : Serial.println("Touch 8 detected"); Draw_Image_from_http(0,0,"Test_PNG_400_300");break;
      case 9  : Serial.println("Touch 9 detected"); DisplaySlideShow(5000);break;
      default : Serial.println("Wakeup not by touchpad")   ;  DisplayWeather(); break;
    } 
  #endif // #ifdef Enable Touch 
  delay(100);
  BeginSleep(); 
}  
//#########################################################################################
void loop() {
   // this will never run!
}
//#########################################################################################
void BeginSleep() {
  init_touch();
  display.powerOff();
  long SleepTimer = (SleepDuration * 60 - ((CurrentMin % SleepDuration) * 60 + CurrentSec)); //Some ESP32 are too fast to maintain accurate time
  esp_sleep_enable_timer_wakeup((SleepTimer+20) * 1000000LL); // Added +20 seconnds to cover ESP32 RTC timer source inaccuracies
  //Configure Touchpad as wakeup source
  esp_sleep_enable_touchpad_wakeup();
  
  Serial.println("Entering " + String(SleepTimer) + "-secs of sleep time");
  Serial.println("Awake for : " + String((millis() - StartTime) / 1000.0, 3) + "-secs");
  Serial.println("Starting deep-sleep period...");
  delay(100);
  //Beginn Deep Sleep
  esp_deep_sleep_start();      // Sleep for e.g. 30 minutes
}
//#########################################################################################
void BeginlightSleep(long Sleep){
Serial.println();
Serial.println("Start Light Sleep");
touchAttachInterrupt(T9, callback , Threshold); //touch for next Image
esp_sleep_enable_timer_wakeup (Sleep* 1000);
esp_sleep_enable_touchpad_wakeup();
delay(100);
esp_light_sleep_start ();  
}

//#########################################################################################
void DisplaySlideShow(int wait) {
  //if (StartWiFi() == WL_CONNECTED && SetupTime() == true) {
    //if (CurrentHour >= WakeupTime && CurrentHour <= SleepTime ) {
      Serial.println("InitialiseDisplay");
      InitialiseDisplay(); // Give screen time to initialise by getting weather data!
        
      WiFiClient client;   // wifi client object
      Serial.println("Run Slideschow:");
      Draw_Image_from_http(0,0,"Test_PNG_200_150");
      BeginlightSleep(wait);
      Draw_Image_from_http(0,0,"Test_PNG_300_225‬");
      BeginlightSleep(wait);
      Draw_Image_from_http(0,0,"Test_PNG_400_300");
      BeginlightSleep(wait);
      Draw_Image_from_http(0,0,"Test2_PNG_160_270");
      BeginlightSleep(wait);
      Draw_Image_from_http(0,0,"Test_PNG_400_200");
      //StopWiFi(); // Reduces power consumption
      BeginlightSleep(wait*2);
      DisplayWeather();  
      BeginSleep();
    //}
  //}  
}
//#########################################################################################

void DisplayWeather() {                 // 4.2" e-paper display is 400x300 resolution
     
  //if (StartWiFi() == WL_CONNECTED && SetupTime() == true) {
    //if (CurrentHour >= WakeupTime && CurrentHour <= SleepTime ) {
      Serial.println("InitialiseDisplay");
      InitialiseDisplay(); // Give screen time to initialise by getting weather data!
      byte Attempts = 1;
      bool RxWeather = false, RxForecast = false, RxHiveeyes = false;
      WiFiClient client;   // wifi client object

      // Obtain weather & Hiveeyes information.
      while ((RxWeather == false || RxForecast == false || RxHiveeyes == false ) && Attempts <= 3) { // Try up-to 2 time for Weather and Forecast data
        if (RxWeather  == false) RxWeather  = obtain_wx_data(client, "weather");
        if (RxForecast == false) RxForecast = obtain_wx_data(client, "forecast");
        if (RxHiveeyes == false) RxHiveeyes = obtain_hiveeyes_data(client);
        Attempts++;
     // }  
      StopWiFi(); // Reduces power consumption
    //}
  DrawHeadingSection();                 // Top line of the display
  DrawMainWeatherSection(172, 70);      // Centre section of display for Location, temperature, Weather report, current Wx Symbol and wind direction
  DrawForecastSection(233, 15);         // 3hr forecast boxes
  DisplayPrecipitationSection(233, 45); // Precipitation sectio
  if (WxConditions[0].Visibility > 0) Visibility(335, 75, String(WxConditions[0].Visibility) + "M");
  if (WxConditions[0].Cloudcover > 0) CloudCover(350, 94, WxConditions[0].Cloudcover);
  //DrawAstronomySection(233, 74);        // Astronomy section Sun rise/set, Moon phase and Moon icon
  DrawHiveeyesSection(187); // DrawHiveeyesSection(y) Draw Hiveeyes Selection over full Screen size
  //DrawBeeflightSection(170, 233);
  //Draw_Image_from_http(0,200,"Test_PNG_400_200");
  display.display(false);
  BeginSleep();
  }
}
//#########################################################################################
void DrawHeadingSection() {
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  drawString(SCREEN_WIDTH / 2 + 40, 0, City, CENTER);
  drawString(SCREEN_WIDTH, 0, date_str, RIGHT);
  drawString(4, 0, time_str, LEFT);
  DrawBattery(34, 12);
  DrawRSSI(75,9, wifi_signal);
  //display.drawLine(0, 12, SCREEN_WIDTH, 12, GxEPD_BLACK);
}
//#########################################################################################
void DrawBeeflightSection(int x, int y) {
  #ifdef Apicast
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  drawString(x, y, "morgens" , RIGHT);
  drawString(x, y+10, beeflight[0].morning, RIGHT);
  drawString(x, y+20, beeflight[1].morning, RIGHT);
  drawString(x, y+30, beeflight[2].morning, RIGHT);
  drawString(x+45, y, "mittags" , RIGHT);
  drawString(x+45, y+10, beeflight[0].noon, RIGHT);
  drawString(x+45, y+20, beeflight[1].noon, RIGHT);
  drawString(x+45, y+30, beeflight[2].noon, RIGHT);
  drawString(x+90, y, "abends" , RIGHT);
  drawString(x+90, y+10, beeflight[0].evening, RIGHT);
  drawString(x+90, y+20, beeflight[1].evening, RIGHT);
  drawString(x+90, y+30, beeflight[2].evening, RIGHT);
  #endif
}
//#########################################################################################
void DrawMainWeatherSection(int x, int y) {
  DisplayDisplayWindSection(x - 132, y - 22, WxConditions[0].Winddir, WxConditions[0].Windspeed, 22);
  DisplayWXicon(x - 25, y - 30, WxConditions[0].Icon, LargeIcon);
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  DrawPressureAndTrend(x - 140, y + 20, WxConditions[0].Pressure, WxConditions[0].Trend);
  u8g2Fonts.setFont(u8g2_font_helvB12_tf);
  String Wx_Description = WxConditions[0].Forecast0;
  if (WxConditions[0].Forecast1 != "") Wx_Description += " & " +  WxConditions[0].Forecast1;
  if (WxConditions[0].Forecast2 != "" && WxConditions[0].Forecast1 != WxConditions[0].Forecast2) Wx_Description += " & " +  WxConditions[0].Forecast2;
  drawStringMaxWidth(x - 170, y + 50, 40, TitleCase(Wx_Description), LEFT);
  DrawMainWx(x-30, y + 24);
  display.drawRect(0, y + 32, SCREEN_WIDTH, 28, GxEPD_BLACK);
}
//#########################################################################################
void DrawForecastSection(int x, int y) {
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  DrawForecastWeather(x, y -2 , 0);
  DrawForecastWeather(x + 56, y- 2, 1);
  DrawForecastWeather(x + 112, y- 2, 2);
  //       (x,y,width,height,MinValue, MaxValue, Title, Data Array, AutoScale, ChartMode)
  for (int r = 1; r <= max_readings; r++) {
    if (Units == "I") {
      pressure_readings[r] = WxForecast[r].Pressure * 0.02953;
      rain_readings[r]     = WxForecast[r].Rainfall * 0.0393701;
    }
    else {
      pressure_readings[r] = WxForecast[r].Pressure;
      rain_readings[r]     = WxForecast[r].Rainfall;
    }
    temperature_readings[r] = WxForecast[r].Temperature;
  }
  //display.drawLine(0, y + 172, SCREEN_WIDTH, y + 172, GxEPD_BLACK);
  //u8g2Fonts.setFont(u8g2_font_helvB12_tf);
  //drawString(SCREEN_WIDTH / 2, y + 180, TXT_FORECAST_VALUES, CENTER);
  //u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  //DrawGraph(SCREEN_WIDTH / 400 * 30,  SCREEN_HEIGHT / 300 * 221, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 5, 900, 1050, Units == "M" ? TXT_PRESSURE_HPA : TXT_PRESSURE_IN, pressure_readings, max_readings, autoscale_on, barchart_off);
  //DrawGraph(SCREEN_WIDTH / 400 * 158, SCREEN_HEIGHT / 300 * 221, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 5, 10, 30, Units == "M" ? TXT_TEMPERATURE_C : TXT_TEMPERATURE_F, temperature_readings, max_readings, autoscale_on, barchart_off);
  //DrawGraph(SCREEN_WIDTH / 400 * 288, SCREEN_HEIGHT / 300 * 221, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 5, 0, 30, TXT_HUMIDITY_PERCENT, rain_readings, max_readings, autoscale_on, barchart_on);
}
//#########################################################################################
void DrawForecastWeather(int x, int y, int index) {
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  DisplayWXicon(x + 28, y + 30, WxForecast[index].Icon, SmallIcon);
  display.drawRect(x, y, 55, 50, GxEPD_BLACK);
  display.drawLine(x + 1, y + 13, x + 54, y + 13, GxEPD_BLACK);
  drawString(x + 31, y + 3, String(WxForecast[index].Period.substring(11, 16)), CENTER);
  drawString(x + 41, y + 39, String(WxForecast[index].High, 0) + "° / " + String(WxForecast[index].Low, 0) + "°", CENTER);
}
//#########################################################################################
void DrawHiveeyesSection( int y) {
  // Hiveeyes Heading
  display.drawLine(SCREEN_WIDTH/3, y , SCREEN_WIDTH/3, SCREEN_HEIGHT, GxEPD_BLACK);
  display.drawLine(SCREEN_WIDTH/3*2, y , SCREEN_WIDTH/3*2, SCREEN_HEIGHT, GxEPD_BLACK);
  display.drawLine(0, y +0, SCREEN_WIDTH, y + 0, GxEPD_BLACK);

  // Hiveeyes Hive1
  u8g2Fonts.setFont(u8g2_font_helvB12_tf);
  drawString(SCREEN_WIDTH / 6, y + 6, "Hive1", CENTER);
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  DrawBattery1(-9,y+16,  hive_data[hive_readings-1].voltage ,3.0 ,4.0 );
  DrawRSSI(106, y+14, hive_data[hive_readings-1].rssi);
  //Too Do! make void for Temp. Check all temp for Max and
  u8g2Fonts.setFont(FONT(u8g2_font_helvB10));
  drawString(120, y + 37, String(hive_data[2].temperature_inside_1, 1) + "°C", RIGHT);
  display.drawRect( 5, y + 40 , 10, -4, GxEPD_BLACK);
  display.fillRect( 17,y + 40 , 10, -6, GxEPD_BLACK);
  display.drawRect( 29,y + 40 , 10, -4, GxEPD_BLACK);
  display.drawRect( 41,y + 40 , 10, -2, GxEPD_BLACK);
  display.drawRect( 53,y + 40 , 10, -2, GxEPD_BLACK);
  //DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode)
  for (int r = 1; r <= hive_readings; r++) {
    weight_readings[r] = hive_data[r].weight;
  }
  DrawGraph(20,250,100,40,1,1.05,("weight =" + String(hive_data[hive_readings-1].weight)),weight_readings,hive_readings,false,false);
}
//#########################################################################################
void DrawRSSI(int x, int y, int rssi) {
  int WIFIsignal = 0;

  int xpos = 1;
  for (int _rssi = -100; _rssi <= rssi; _rssi = _rssi + 20) {
    if (_rssi <= -20)  WIFIsignal = 20; //            <-20dbm displays 5-bars
    if (_rssi <= -40)  WIFIsignal = 16; //  -40dbm to  -21dbm displays 4-bars
    if (_rssi <= -60)  WIFIsignal = 12; //  -60dbm to  -41dbm displays 3-bars
    if (_rssi <= -80)  WIFIsignal = 8;  //  -80dbm to  -61dbm displays 2-bars
    if (_rssi <= -100) WIFIsignal = 4;  // -100dbm to  -81dbm displays 1-bar
    display.fillRect(x + xpos * 6, y - WIFIsignal, 5, WIFIsignal, GxEPD_BLACK);
    xpos++;
  }
  display.fillRect(x, y - 1, 5, 1, GxEPD_BLACK);
  //drawString(x + 41,  y - 8, String(rssi) + "dBm", CENTER);
}
//#########################################################################################
void DrawBattery1(int x, int y, float voltage, float emptyV , float fullV) {
  uint8_t percentage = 100;
  percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
  if (voltage >= fullV) percentage = 100;
  if (voltage <= emptyV) percentage = 0;
  display.drawRect(x + 15, y - 12, 19, 10, GxEPD_BLACK);
  display.fillRect(x + 34, y - 10, 2, 5, GxEPD_BLACK);
  display.fillRect(x + 17, y - 10, 15 * percentage / 100.0, 6, GxEPD_BLACK);
  //drawString(x + 62, y - 11, String(percentage) + "%", RIGHT);
  //drawString(x + 52, y -11,  String(voltage, 2) + "V", CENTER);
}
//#########################################################################################
void DrawMainWx(int x, int y) {
  u8g2Fonts.setFont(u8g2_font_helvB14_tf);
  drawString(x - 25, y - 22, String(WxConditions[0].Temperature, 1) + "°" + (Units == "M" ? "C" : "F"), CENTER); // Show current Temperature
  u8g2Fonts.setFont(u8g2_font_helvB12_tf);
  drawString(x - 15, y - 3, String(WxConditions[0].High, 0) + "° | " + String(WxConditions[0].Low, 0) + "°", CENTER); // Show forecast high and Low
  drawString(x + 30, y - 22, String(WxConditions[0].Humidity, 0) + "%", CENTER);
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  drawString(x + 32, y - 3, "RH", CENTER);
}
//#########################################################################################
void DisplayDisplayWindSection(int x, int y, float angle, float windspeed, int Cradius) {
  arrow(x, y, Cradius - 7, angle, 12, 18); // Show wind direction on outer circle of width and length
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  int dxo, dyo, dxi, dyi;
  display.drawLine(0, 15, 0, y + Cradius + 30, GxEPD_BLACK);
  display.drawCircle(x, y, Cradius, GxEPD_BLACK);     // Draw compass circle
  display.drawCircle(x, y, Cradius - 3, GxEPD_BLACK); // Draw compass circle
  //display.drawCircle(x, y, Cradius * 0.7, GxEPD_BLACK); // Draw compass inner circle
  for (float a = 0; a < 360; a = a + 22.5) {
    dxo = Cradius * cos((a - 90) * PI / 180);
    dyo = Cradius * sin((a - 90) * PI / 180);
    //if (a == 45)  drawString(dxo + x + 10, dyo + y - 10, TXT_NE, CENTER);
    //if (a == 135) drawString(dxo + x + 7,  dyo + y + 5,  TXT_SE, CENTER);
    //if (a == 225) drawString(dxo + x - 15, dyo + y,      TXT_SW, CENTER);
    //if (a == 315) drawString(dxo + x - 15, dyo + y - 10, TXT_NW, CENTER);
    dxi = dxo * 0.9;
    dyi = dyo * 0.9;
    display.drawLine(dxo + x, dyo + y, dxi + x, dyi + y, GxEPD_BLACK);
    //dxo = dxo * 0.7;
    //dyo = dyo * 0.7;
    //dxi = dxo * 0.9;
    //dyi = dyo * 0.9;
    //display.drawLine(dxo + x, dyo + y, dxi + x, dyi + y, GxEPD_BLACK);
  }
  drawString(x, y - Cradius - 10,     TXT_N, CENTER);
  drawString(x, y + Cradius + 5,      TXT_S, CENTER);
  drawString(x - Cradius - 10, y - 3, TXT_W, CENTER);
  drawString(x + Cradius + 8,  y - 3, TXT_E, CENTER);
  //drawString(x - 2, y - 18, WindDegToDirection(angle), CENTER);
  //drawString(x + 3, y + 12, String(angle, 0) + "°", CENTER);
  drawString(x + 3, y - 3, String(windspeed, 1) + (Units == "M" ? "m/s" : "mph"), CENTER);
}
//#########################################################################################
String WindDegToDirection(float winddirection) {
  if (winddirection >= 348.75 || winddirection < 11.25)  return TXT_N;
  if (winddirection >=  11.25 && winddirection < 33.75)  return TXT_NNE;
  if (winddirection >=  33.75 && winddirection < 56.25)  return TXT_NE;
  if (winddirection >=  56.25 && winddirection < 78.75)  return TXT_ENE;
  if (winddirection >=  78.75 && winddirection < 101.25) return TXT_E;
  if (winddirection >= 101.25 && winddirection < 123.75) return TXT_ESE;
  if (winddirection >= 123.75 && winddirection < 146.25) return TXT_SE;
  if (winddirection >= 146.25 && winddirection < 168.75) return TXT_SSE;
  if (winddirection >= 168.75 && winddirection < 191.25) return TXT_S;
  if (winddirection >= 191.25 && winddirection < 213.75) return TXT_SSW;
  if (winddirection >= 213.75 && winddirection < 236.25) return TXT_SW;
  if (winddirection >= 236.25 && winddirection < 258.75) return TXT_WSW;
  if (winddirection >= 258.75 && winddirection < 281.25) return TXT_W;
  if (winddirection >= 281.25 && winddirection < 303.75) return TXT_WNW;
  if (winddirection >= 303.75 && winddirection < 326.25) return TXT_NW;
  if (winddirection >= 326.25 && winddirection < 348.75) return TXT_NNW;
  return "?";
}
//#########################################################################################
void DrawPressureAndTrend(int x, int y, float pressure, String slope) {
  drawString(x, y, String(pressure, (Units == "M" ? 0 : 1)) + (Units == "M" ? "mb" : "in"), CENTER);
  x = x + 40; y = y + 2;
  if      (slope == "+") {
    display.drawLine(x,  y, x + 4, y - 4, GxEPD_BLACK);
    display.drawLine(x + 4, y - 4, x + 8, y, GxEPD_BLACK);
  }
  else if (slope == "0") {
    display.drawLine(x + 4, y - 4, x + 8, y, GxEPD_BLACK);
    display.drawLine(x + 4, y + 4, x + 8, y, GxEPD_BLACK);
  }
  else if (slope == "-") {
    display.drawLine(x,  y, x + 4, y + 4, GxEPD_BLACK);
    display.drawLine(x + 4, y + 4, x + 8, y, GxEPD_BLACK);
  }
}
//#########################################################################################
void DisplayPrecipitationSection(int x, int y) {
  display.drawRect(x, y - 1, 167, 56, GxEPD_BLACK); // precipitation outline
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  if (WxForecast[1].Rainfall > 0.005) { // Ignore small amounts
    drawString(x + 5, y + 15, String(WxForecast[1].Rainfall, 2) + (Units == "M" ? "mm" : "in"), LEFT); // Only display rainfall total today if > 0
    addraindrop(x + 65 - (Units == "I" ? 10 : 0), y + 16, 7);
  }
  if (WxForecast[1].Snowfall > 0.005)  // Ignore small amounts
    drawString(x + 5, y + 35, String(WxForecast[1].Snowfall, 2) + (Units == "M" ? "mm" : "in") + " * *", LEFT); // Only display snowfall total today if > 0
}
//#########################################################################################
void DrawAstronomySection(int x, int y) {
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  display.drawRect(x, y + 64, 167, 48, GxEPD_BLACK);
  drawString(x + 7, y + 70, ConvertUnixTime(WxConditions[0].Sunrise).substring(0, (Units == "M" ? 5 : 7)) + " " + TXT_SUNRISE, LEFT);
  drawString(x + 7, y + 85, ConvertUnixTime(WxConditions[0].Sunset).substring(0, (Units == "M" ? 5 : 7)) + " " + TXT_SUNSET, LEFT);
  time_t now = time(NULL);
  struct tm * now_utc = gmtime(&now);
  const int day_utc   = now_utc->tm_mday;
  const int month_utc = now_utc->tm_mon + 1;
  const int year_utc  = now_utc->tm_year + 1900;
  drawString(x + 7, y + 100, MoonPhase(day_utc, month_utc, year_utc), LEFT);
  DrawMoon(x + 105, y + 50, day_utc, month_utc, year_utc, Hemisphere);
}
//#########################################################################################
void DrawMoon(int x, int y, int dd, int mm, int yy, String hemisphere) {
  const int diameter = 38;
  double Phase = NormalizedMoonPhase(dd, mm, yy);
  hemisphere.toLowerCase();
  if (hemisphere == "south") Phase = 1 - Phase;
  // Draw dark part of moon
  display.fillCircle(x + diameter - 1, y + diameter, diameter / 2 + 1, GxEPD_BLACK);
  const int number_of_lines = 90;
  for (double Ypos = 0; Ypos <= 45; Ypos++) {
    double Xpos = sqrt(45 * 45 - Ypos * Ypos);
    // Determine the edges of the lighted part of the moon
    double Rpos = 2 * Xpos;
    double Xpos1, Xpos2;
    if (Phase < 0.5) {
      Xpos1 = - Xpos;
      Xpos2 = (Rpos - 2 * Phase * Rpos - Xpos);
    }
    else {
      Xpos1 = Xpos;
      Xpos2 = (Xpos - 2 * Phase * Rpos + Rpos);
    }
    // Draw light part of moon
    double pW1x = (Xpos1 + number_of_lines) / number_of_lines * diameter + x;
    double pW1y = (number_of_lines - Ypos)  / number_of_lines * diameter + y;
    double pW2x = (Xpos2 + number_of_lines) / number_of_lines * diameter + x;
    double pW2y = (number_of_lines - Ypos)  / number_of_lines * diameter + y;
    double pW3x = (Xpos1 + number_of_lines) / number_of_lines * diameter + x;
    double pW3y = (Ypos + number_of_lines)  / number_of_lines * diameter + y;
    double pW4x = (Xpos2 + number_of_lines) / number_of_lines * diameter + x;
    double pW4y = (Ypos + number_of_lines)  / number_of_lines * diameter + y;
    display.drawLine(pW1x, pW1y, pW2x, pW2y, GxEPD_WHITE);
    display.drawLine(pW3x, pW3y, pW4x, pW4y, GxEPD_WHITE);
  }
  display.drawCircle(x + diameter - 1, y + diameter, diameter / 2 + 1, GxEPD_BLACK);
}
//#########################################################################################
String MoonPhase(int d, int m, int y) {
  int c, e;
  double jd;
  int b;
  if (m < 3) {
    y--;
    m += 12;
  }
  ++m;
  c   = 365.25 * y;
  e   = 30.6  * m;
  jd  = c + e + d - 694039.09;     /* jd is total days elapsed */
  jd /= 29.53059;                  /* divide by the moon cycle (29.53 days) */
  b   = jd;                        /* int(jd) -> b, take integer part of jd */
  jd -= b;                         /* subtract integer part to leave fractional part of original jd */
  b   = jd * 8 + 0.5;              /* scale fraction from 0-8 and round by adding 0.5 */
  b   = b & 7;                     /* 0 and 8 are the same phase so modulo 8 for 0 */
  Hemisphere.toLowerCase();
  if (Hemisphere == "south") b = 7 - b;
  if (b == 0) return TXT_MOON_NEW;              // New;              0%  illuminated
  if (b == 1) return TXT_MOON_WAXING_CRESCENT;  // Waxing crescent; 25%  illuminated
  if (b == 2) return TXT_MOON_FIRST_QUARTER;    // First quarter;   50%  illuminated
  if (b == 3) return TXT_MOON_WAXING_GIBBOUS;   // Waxing gibbous;  75%  illuminated
  if (b == 4) return TXT_MOON_FULL;             // Full;            100% illuminated
  if (b == 5) return TXT_MOON_WANING_GIBBOUS;   // Waning gibbous;  75%  illuminated
  if (b == 6) return TXT_MOON_THIRD_QUARTER;    // Third quarter;   50%  illuminated
  if (b == 7) return TXT_MOON_WANING_CRESCENT;  // Waning crescent; 25%  illuminated
  return "";
}
//#########################################################################################
void arrow(int x, int y, int asize, float aangle, int pwidth, int plength) {
  float dx = (asize - 10) * cos((aangle - 90) * PI / 180) + x; // calculate X position
  float dy = (asize - 10) * sin((aangle - 90) * PI / 180) + y; // calculate Y position
  float x1 = 0;           float y1 = plength;
  float x2 = pwidth / 2;  float y2 = pwidth / 2;
  float x3 = -pwidth / 2; float y3 = pwidth / 2;
  float angle = aangle * PI / 180 - 135;
  float xx1 = x1 * cos(angle) - y1 * sin(angle) + dx;
  float yy1 = y1 * cos(angle) + x1 * sin(angle) + dy;
  float xx2 = x2 * cos(angle) - y2 * sin(angle) + dx;
  float yy2 = y2 * cos(angle) + x2 * sin(angle) + dy;
  float xx3 = x3 * cos(angle) - y3 * sin(angle) + dx;
  float yy3 = y3 * cos(angle) + x3 * sin(angle) + dy;
  display.fillTriangle(xx1, yy1, xx3, yy3, xx2, yy2, GxEPD_BLACK);
}
//#########################################################################################
void DisplayWXicon(int x, int y, String IconName, bool IconSize) {
  Serial.println(IconName);
  if      (IconName == "01d" || IconName == "01n")  Sunny(x, y, IconSize, IconName);
  else if (IconName == "02d" || IconName == "02n")  MostlySunny(x, y, IconSize, IconName);
  else if (IconName == "03d" || IconName == "03n")  Cloudy(x, y, IconSize, IconName);
  else if (IconName == "04d" || IconName == "04n")  MostlySunny(x, y, IconSize, IconName);
  else if (IconName == "09d" || IconName == "09n")  ChanceRain(x, y, IconSize, IconName);
  else if (IconName == "10d" || IconName == "10n")  Rain(x, y, IconSize, IconName);
  else if (IconName == "11d" || IconName == "11n")  Tstorms(x, y, IconSize, IconName);
  else if (IconName == "13d" || IconName == "13n")  Snow(x, y, IconSize, IconName);
  else if (IconName == "50d")                       Haze(x, y, IconSize, IconName);
  else if (IconName == "50n")                       Fog(x, y, IconSize, IconName);
  else                                              Nodata(x, y, IconSize, IconName);
}
//#########################################################################################
uint8_t StartWiFi() {
  Serial.print("\r\nConnecting to: "); Serial.println(String(ssid));
  IPAddress dns(8, 8, 8, 8); // Google DNS
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); // switch off AP
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);
  WiFi.begin(ssid, password);
  unsigned long start = millis();
  uint8_t connectionStatus;
  bool AttemptConnection = true;
  while (AttemptConnection) {
    connectionStatus = WiFi.status();
    if (millis() > start + 15000) { // Wait 15-secs maximum
      AttemptConnection = false;
    }
    if (connectionStatus == WL_CONNECTED || connectionStatus == WL_CONNECT_FAILED) {
      AttemptConnection = false;
    }
    delay(50);
  }
  if (connectionStatus == WL_CONNECTED) {
    wifi_signal = WiFi.RSSI(); // Get Wifi Signal strength now, because the WiFi will be turned off to save power!
    Serial.println("WiFi connected at: " + WiFi.localIP().toString());
  }
  else Serial.println("WiFi connection *** FAILED ***");
  return connectionStatus;
}
//#########################################################################################
void StopWiFi() {
  WiFi.disconnect();
  WiFi.mode(WIFI_OFF);
}
//#########################################################################################
boolean SetupTime() {
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer, "time.nist.gov"); //(gmtOffset_sec, daylightOffset_sec, ntpServer)
  setenv("TZ", Timezone, 1);  //setenv()adds the "TZ" variable to the environment with a value TimeZone, only used if set to 1, 0 means no change
  tzset(); // Set the TZ environment variable
  delay(100);
  bool TimeStatus = UpdateLocalTime();
  return TimeStatus;
}
//#########################################################################################
boolean UpdateLocalTime() {
  struct tm timeinfo;
  char   time_output[30], day_output[30], update_time[30];
  while (!getLocalTime(&timeinfo, 10000)) { // Wait for 5-sec for time to synchronise
    Serial.println("Failed to obtain time");
    return false;
  }
  CurrentHour = timeinfo.tm_hour;
  CurrentMin  = timeinfo.tm_min;
  CurrentSec  = timeinfo.tm_sec;
  //See http://www.cplusplus.com/reference/ctime/strftime/
  //Serial.println(&timeinfo, "%a %b %d %Y   %H:%M:%S"); // Displays: Saturday, June 24 2017 14:05:49
  if (Units == "M") {
    if ((Language == "CZ") || (Language == "DE") || (Language == "NL") || (Language == "PL")) {
      sprintf(day_output, "%s, %02u. %s %04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900); // day_output >> So., 23. Juni 2019 <<
    }
    else
    {
      sprintf(day_output, "%s  %02u-%s-%04u", weekday_D[timeinfo.tm_wday], timeinfo.tm_mday, month_M[timeinfo.tm_mon], (timeinfo.tm_year) + 1900);
    }
    strftime(update_time, sizeof(update_time), "%H:%M:%S", &timeinfo);  // Creates: '@ 14:05:49'   and change from 30 to 8 <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    sprintf(time_output, "%s", update_time);
  }
  else
  {
    strftime(day_output, sizeof(day_output), "%a  %b-%d-%Y", &timeinfo); // Creates  'Sat May-31-2019'
    strftime(update_time, sizeof(update_time), "%r", &timeinfo);         // Creates: '@ 02:05:49pm'
    sprintf(time_output, "%s", update_time);
  }
  date_str = day_output;
  time_str = time_output;
  return true;
}
//#########################################################################################
// Symbols are drawn on a relative 10x10grid and 1 scale unit = 1 drawing unit
void addcloud(int x, int y, int scale, int linesize) {
  //Draw cloud outer
  display.fillCircle(x - scale * 3, y, scale, GxEPD_BLACK);                      // Left most circle
  display.fillCircle(x + scale * 3, y, scale, GxEPD_BLACK);                      // Right most circle
  display.fillCircle(x - scale, y - scale, scale * 1.4, GxEPD_BLACK);            // left middle upper circle
  display.fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75, GxEPD_BLACK); // Right middle upper circle
  display.fillRect(x - scale * 3 - 1, y - scale, scale * 6, scale * 2 + 1, GxEPD_BLACK); // Upper and lower lines
  //Clear cloud inner
  display.fillCircle(x - scale * 3, y, scale - linesize, GxEPD_WHITE);           // Clear left most circle
  display.fillCircle(x + scale * 3, y, scale - linesize, GxEPD_WHITE);           // Clear right most circle
  display.fillCircle(x - scale, y - scale, scale * 1.4 - linesize, GxEPD_WHITE); // left middle upper circle
  display.fillCircle(x + scale * 1.5, y - scale * 1.3, scale * 1.75 - linesize, GxEPD_WHITE); // Right middle upper circle
  display.fillRect(x - scale * 3 + 2, y - scale + linesize - 1, scale * 5.9, scale * 2 - linesize * 2 + 2, GxEPD_WHITE); // Upper and lower lines
}
//#########################################################################################
void addraindrop(int x, int y, int scale) {
  display.fillCircle(x, y, scale / 2, GxEPD_BLACK);
  display.fillTriangle(x - scale / 2, y, x, y - scale * 1.2, x + scale / 2, y , GxEPD_BLACK);
  x = x + scale * 1.6; y = y + scale / 3;
  display.fillCircle(x, y, scale / 2, GxEPD_BLACK);
  display.fillTriangle(x - scale / 2, y, x, y - scale * 1.2, x + scale / 2, y , GxEPD_BLACK);
}
//#########################################################################################
void addrain(int x, int y, int scale, bool IconSize) {
  if (IconSize == SmallIcon) scale *= 1.34;
  for (int d = 0; d < 4; d++) {
    addraindrop(x + scale * (7.8 - d * 1.95) - scale * 5.2, y + scale * 2.1 - scale / 6, scale / 1.6);
  }
}
//#########################################################################################
void addsnow(int x, int y, int scale, bool IconSize) {
  int dxo, dyo, dxi, dyi;
  for (int flakes = 0; flakes < 5; flakes++) {
    for (int i = 0; i < 360; i = i + 45) {
      dxo = 0.5 * scale * cos((i - 90) * 3.14 / 180); dxi = dxo * 0.1;
      dyo = 0.5 * scale * sin((i - 90) * 3.14 / 180); dyi = dyo * 0.1;
      display.drawLine(dxo + x + flakes * 1.5 * scale - scale * 3, dyo + y + scale * 2, dxi + x + 0 + flakes * 1.5 * scale - scale * 3, dyi + y + scale * 2, GxEPD_BLACK);
    }
  }
}
//#########################################################################################
void addtstorm(int x, int y, int scale) {
  y = y + scale / 2;
  for (int i = 0; i < 5; i++) {
    display.drawLine(x - scale * 4 + scale * i * 1.5 + 0, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 0, y + scale, GxEPD_BLACK);
    if (scale != Small) {
      display.drawLine(x - scale * 4 + scale * i * 1.5 + 1, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 1, y + scale, GxEPD_BLACK);
      display.drawLine(x - scale * 4 + scale * i * 1.5 + 2, y + scale * 1.5, x - scale * 3.5 + scale * i * 1.5 + 2, y + scale, GxEPD_BLACK);
    }
    display.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 0, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 0, GxEPD_BLACK);
    if (scale != Small) {
      display.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 1, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 1, GxEPD_BLACK);
      display.drawLine(x - scale * 4 + scale * i * 1.5, y + scale * 1.5 + 2, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5 + 2, GxEPD_BLACK);
    }
    display.drawLine(x - scale * 3.5 + scale * i * 1.4 + 0, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 0, y + scale * 1.5, GxEPD_BLACK);
    if (scale != Small) {
      display.drawLine(x - scale * 3.5 + scale * i * 1.4 + 1, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 1, y + scale * 1.5, GxEPD_BLACK);
      display.drawLine(x - scale * 3.5 + scale * i * 1.4 + 2, y + scale * 2.5, x - scale * 3 + scale * i * 1.5 + 2, y + scale * 1.5, GxEPD_BLACK);
    }
  }
}
//#########################################################################################
void addsun(int x, int y, int scale, bool IconSize) {
  int linesize = 3;
  if (IconSize == SmallIcon) linesize = 1;
  display.fillRect(x - scale * 2, y, scale * 4, linesize, GxEPD_BLACK);
  display.fillRect(x, y - scale * 2, linesize, scale * 4, GxEPD_BLACK);
  display.drawLine(x - scale * 1.3, y - scale * 1.3, x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
  display.drawLine(x - scale * 1.3, y + scale * 1.3, x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
  if (IconSize == LargeIcon) {
    display.drawLine(1 + x - scale * 1.3, y - scale * 1.3, 1 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    display.drawLine(2 + x - scale * 1.3, y - scale * 1.3, 2 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    display.drawLine(3 + x - scale * 1.3, y - scale * 1.3, 3 + x + scale * 1.3, y + scale * 1.3, GxEPD_BLACK);
    display.drawLine(1 + x - scale * 1.3, y + scale * 1.3, 1 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
    display.drawLine(2 + x - scale * 1.3, y + scale * 1.3, 2 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
    display.drawLine(3 + x - scale * 1.3, y + scale * 1.3, 3 + x + scale * 1.3, y - scale * 1.3, GxEPD_BLACK);
  }
  display.fillCircle(x, y, scale * 1.3, GxEPD_WHITE);
  display.fillCircle(x, y, scale, GxEPD_BLACK);
  display.fillCircle(x, y, scale - linesize, GxEPD_WHITE);
}
//#########################################################################################
void addfog(int x, int y, int scale, int linesize, bool IconSize) {
  if (IconSize == SmallIcon) {
    y -= 10;
    linesize = 1;
  }
  for (int i = 0; i < 6; i++) {
    display.fillRect(x - scale * 3, y + scale * 1.5, scale * 6, linesize, GxEPD_BLACK);
    display.fillRect(x - scale * 3, y + scale * 2.0, scale * 6, linesize, GxEPD_BLACK);
    display.fillRect(x - scale * 3, y + scale * 2.5, scale * 6, linesize, GxEPD_BLACK);
  }
}
//#########################################################################################
void Sunny(int x, int y, bool IconSize, String IconName) {
  int scale = Small, offset = 3;
  if (IconSize == LargeIcon) {
    scale = Large;
    y = y - 8;
    offset = 18;
  } else y = y - 3; // Shift up small sun icon
  if (IconName.endsWith("n")) addmoon(x, y + offset, scale, IconSize);
  scale = scale * 1.6;
  addsun(x, y, scale, IconSize);
}
//#########################################################################################
void MostlySunny(int x, int y, bool IconSize, String IconName) {
  int scale = Small, linesize = 3, offset = 3;
  if (IconSize == LargeIcon) {
    scale = Large;
    offset = 10;
  } else linesize = 1;
  if (IconName.endsWith("n")) addmoon(x, y + offset, scale, IconSize);
  addcloud(x, y + offset, scale, linesize);
  addsun(x - scale * 1.8, y - scale * 1.8 + offset, scale, IconSize);
}
//#########################################################################################
void MostlyCloudy(int x, int y, bool IconSize, String IconName) {
  int scale = Small, linesize = 3;
  if (IconSize == LargeIcon) {
    scale = Large;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addsun(x - scale * 1.8, y - scale * 1.8, scale, IconSize);
  addcloud(x, y, scale, linesize);
}
//#########################################################################################
void Cloudy(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
    linesize = 1;
    addcloud(x, y, scale, linesize);
  }
  else {
    y += 10;
    if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
    addcloud(x + 30, y - 35, 5, linesize); // Cloud top right
    addcloud(x - 20, y - 25, 7, linesize); // Cloud top left
    addcloud(x, y, scale, linesize);       // Main cloud
  }
}
//#########################################################################################
void Rain(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale, IconSize);
}
//#########################################################################################
void ExpectRain(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addsun(x - scale * 1.8, y - scale * 1.8, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale, IconSize);
}
//#########################################################################################
void ChanceRain(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addsun(x - scale * 1.8, y - scale * 1.8, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addrain(x, y, scale, IconSize);
}
//#########################################################################################
void Tstorms(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addtstorm(x, y, scale);
}
//#########################################################################################
void Snow(int x, int y, bool IconSize, String IconName) {
  int scale = Large, linesize = 3;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addcloud(x, y, scale, linesize);
  addsnow(x, y, scale, IconSize);
}
//#########################################################################################
void Fog(int x, int y, bool IconSize, String IconName) {
  int linesize = 3, scale = Large;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addcloud(x, y - 5, scale, linesize);
  addfog(x, y - 5, scale, linesize, IconSize);
}
//#########################################################################################
void Haze(int x, int y, bool IconSize, String IconName) {
  int linesize = 3, scale = Large;
  if (IconSize == SmallIcon) {
    scale = Small;
    linesize = 1;
  }
  if (IconName.endsWith("n")) addmoon(x, y, scale, IconSize);
  addsun(x, y - 5, scale * 1.4, IconSize);
  addfog(x, y - 5, scale * 1.4, linesize, IconSize);
}
//#########################################################################################
void CloudCover(int x, int y, int CCover) {
  addcloud(x - 9, y - 3, Small * 0.5, 2); // Cloud top left
  addcloud(x + 3, y - 3, Small * 0.5, 2); // Cloud top right
  addcloud(x, y,         Small * 0.5, 2); // Main cloud
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  drawString(x + 15, y - 5, String(CCover) + "%", LEFT);
}
//#########################################################################################
void Visibility(int x, int y, String Visi) {
  y = y - 3; //
  float start_angle = 0.52, end_angle = 2.61;
  int r = 10;
  for (float i = start_angle; i < end_angle; i = i + 0.05) {
    display.drawPixel(x + r * cos(i), y - r / 2 + r * sin(i), GxEPD_BLACK);
    display.drawPixel(x + r * cos(i), 1 + y - r / 2 + r * sin(i), GxEPD_BLACK);
  }
  start_angle = 3.61; end_angle = 5.78;
  for (float i = start_angle; i < end_angle; i = i + 0.05) {
    display.drawPixel(x + r * cos(i), y + r / 2 + r * sin(i), GxEPD_BLACK);
    display.drawPixel(x + r * cos(i), 1 + y + r / 2 + r * sin(i), GxEPD_BLACK);
  }
  display.fillCircle(x, y, r / 4, GxEPD_BLACK);
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  drawString(x + 12, y - 3, Visi, LEFT);
}
//#########################################################################################
void addmoon(int x, int y, int scale, bool IconSize) {
  if (IconSize == LargeIcon) {
    x = x + 12; y = y + 12;
    display.fillCircle(x - 50, y - 55, scale, GxEPD_BLACK);
    display.fillCircle(x - 35, y - 55, scale * 1.6, GxEPD_WHITE);
  }
  else
  {
    display.fillCircle(x - 20, y - 12, scale, GxEPD_BLACK);
    display.fillCircle(x - 15, y - 12, scale * 1.6, GxEPD_WHITE);
  }
}
//#########################################################################################
void Nodata(int x, int y, bool IconSize, String IconName) {
  if (IconSize == LargeIcon) u8g2Fonts.setFont(u8g2_font_helvB24_tf); else u8g2Fonts.setFont(u8g2_font_helvB10_tf);
  drawString(x - 3, y - 8, "?", CENTER);
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
}
//#########################################################################################
void DrawBattery(int x, int y) {
  uint8_t percentage = 100;
  float voltage = analogRead(BATTERY_PIN) / 4096.0 * 7.46;
  if (voltage > 1 ) { // Only display if there is a valid reading
    Serial.println("Voltage = " + String(voltage));
    percentage = 2836.9625 * pow(voltage, 4) - 43987.4889 * pow(voltage, 3) + 255233.8134 * pow(voltage, 2) - 656689.7123 * voltage + 632041.7303;
    if (voltage >= 4.20) percentage = 100;
    if (voltage <= 3.50) percentage = 0;
    display.drawRect(x + 15, y - 12, 19, 10, GxEPD_BLACK);
    display.fillRect(x + 34, y - 10, 2, 5, GxEPD_BLACK);
    display.fillRect(x + 17, y - 10, 15 * percentage / 100.0, 6, GxEPD_BLACK);
    drawString(x + 65, y - 11, String(percentage) + "%", RIGHT);
    //drawString(x + 13, y + 5,  String(voltage, 2) + "v", CENTER);
  }
}
//#########################################################################################
/* (C) D L BIRD
    This function will draw a graph on a ePaper/TFT/LCD display using data from an array containing data to be graphed.
    The variable 'max_readings' determines the maximum number of data elements for each array. Call it with the following parametric data:
    x_pos - the x axis top-left position of the graph
    y_pos - the y-axis top-left position of the graph, e.g. 100, 200 would draw the graph 100 pixels along and 200 pixels down from the top-left of the screen
    width - the width of the graph in pixels
    height - height of the graph in pixels
    Y1_Max - sets the scale of plotted data, for example 5000 would scale all data to a Y-axis of 5000 maximum
    data_array1 is parsed by value, externally they can be called anything else, e.g. within the routine it is called data_array1, but externally could be temperature_readings
    auto_scale - a logical value (TRUE or FALSE) that switches the Y-axis autoscale On or Off
    barchart_on - a logical value (TRUE or FALSE) that switches the drawing mode between bar and line graphs
    barchart_colour - a sets the title and graph plotting colour
    If called with Y!_Max value of 500 and the data never goes above 500, then autoscale will retain a 0-500 Y scale, if on, the scale increases/decreases to match the data.
    auto_scale_margin, e.g. if set to 1000 then autoscale increments the scale by 1000 steps.
*/
void DrawGraph(int x_pos, int y_pos, int gwidth, int gheight, float Y1Min, float Y1Max, String title, float DataArray[], int readings, boolean auto_scale, boolean barchart_mode) {
#define auto_scale_margin 0 // Sets the autoscale increment, so axis steps up in units of e.g. 3
#define y_minor_axis 5      // 5 y-axis division markers
  int maxYscale = -10000;
  int minYscale =  10000;
  int last_x, last_y;
  float x1, y1, x2, y2;
  if (auto_scale == true) {
    for (int i = 1; i < readings; i++ ) {
      if (DataArray[i] >= maxYscale) maxYscale = DataArray[i];
      if (DataArray[i] <= minYscale) minYscale = DataArray[i];
    }
    maxYscale = round(maxYscale + auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Max
    Y1Max = round(maxYscale + 0.5);
    if (minYscale != 0) minYscale = round(minYscale - auto_scale_margin); // Auto scale the graph and round to the nearest value defined, default was Y1Min
    Y1Min = round(minYscale);
  }
  // Draw the graph
  last_x = x_pos + 1;
  last_y = y_pos + (Y1Max - constrain(DataArray[1], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight;
  display.drawRect(x_pos, y_pos, gwidth + 3, gheight + 2, GxEPD_BLACK);
  u8g2Fonts.setFont(u8g2_font_helvB08_tf);
  drawString(x_pos + gwidth / 2, y_pos - 12, title, CENTER);
  // Draw the data
  for (int gx = 1; gx < readings; gx++) {
    x1 = last_x;
    y1 = last_y;
    x2 = x_pos + gx * gwidth / (readings - 1) - 1 ; // max_readings is the global variable that sets the maximum data that can be plotted
    y2 = y_pos + (Y1Max - constrain(DataArray[gx], Y1Min, Y1Max)) / (Y1Max - Y1Min) * gheight + 1;
    if (barchart_mode) {
      display.fillRect(x2, y2, (gwidth / readings) - 1, y_pos + gheight - y2 + 1, GxEPD_BLACK);
    } else {
      display.drawLine(last_x, last_y, x2, y2, GxEPD_BLACK);
    }
    last_x = x2;
    last_y = y2;
  }
  //Draw the Y-axis scale
#define number_of_dashes 15
  for (int spacing = 0; spacing <= y_minor_axis; spacing++) {
    for (int j = 0; j < number_of_dashes; j++) { // Draw dashed graph grid lines
      if (spacing < y_minor_axis) display.drawFastHLine((x_pos + 3 + j * gwidth / number_of_dashes), y_pos + (gheight * spacing / y_minor_axis), gwidth / (2 * number_of_dashes), GxEPD_BLACK);
    }
    if ((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing) < 5 || title == TXT_PRESSURE_IN) {
      drawString(x_pos, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
    }
    else
    {
      if (Y1Min < 1 && Y1Max < 10)
        drawString(x_pos - 3, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 1), RIGHT);
      else
        drawString(x_pos - 3, y_pos + gheight * spacing / y_minor_axis - 5, String((Y1Max - (float)(Y1Max - Y1Min) / y_minor_axis * spacing + 0.01), 0), RIGHT);
    }
  }
  for (int i = 0; i <= 2; i++) {
    drawString(15 + x_pos + gwidth / 3 * i, y_pos + gheight + 3, String(i), LEFT);
  }
  drawString(x_pos + gwidth / 2, y_pos + gheight + 10, TXT_DAYS, CENTER);
}
//#########################################################################################
void drawString(int x, int y, String text, alignment align) {
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  display.setTextWrap(false);
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (align == RIGHT)  x = x - w;
  if (align == CENTER) x = x - w / 2;
  u8g2Fonts.setCursor(x, y + h);
  u8g2Fonts.print(text);
}
//#########################################################################################
void drawStringMaxWidth(int x, int y, unsigned int text_width, String text, alignment align) {
  int16_t  x1, y1; //the bounds of x,y and w and h of the variable 'text' in pixels.
  uint16_t w, h;
  display.getTextBounds(text, x, y, &x1, &y1, &w, &h);
  if (align == RIGHT)  x = x - w;
  if (align == CENTER) x = x - w / 2;
  u8g2Fonts.setCursor(x, y);
  if (text.length() > text_width * 2) {
    u8g2Fonts.setFont(u8g2_font_helvB10_tf);
    text_width = 42;
    y = y - 3;
  }
  u8g2Fonts.println(text.substring(0, text_width));
  if (text.length() > text_width) {
    u8g2Fonts.setCursor(x, y + h + 15);
    String secondLine = text.substring(text_width);
    secondLine.trim(); // Remove any leading spaces
    u8g2Fonts.println(secondLine);
  }
}
//#########################################################################################
void DrawImagePNG(int x, int y, String payload) {
  
  // Convert format.
  const unsigned char* buffer = reinterpret_cast<const unsigned char*>(payload.c_str());
  upng_t* upng;
  upng = upng_new_from_bytes(buffer, payload.length());

  if (upng != NULL) {

    // Decode PNG image.
    upng_decode(upng);
    if (upng_get_error(upng) == UPNG_EOK) {

      // Get pointer to bitmap buffer.
      const uint8_t *bitmap = upng_get_buffer(upng);

      // Draw image.
      
      int width = upng_get_width(upng);
      int height = upng_get_height(upng);
      Serial.println("display Image");
      Serial.println(width);
      Serial.println(height);
      display.writeScreenBuffer();
      //display.drawBitmap(0,0,bitmap,width, height,true);
      display.writeImage(bitmap, x, y, width, height, false, false, true);
      display.refresh();
    }
    else
    {
      Serial.println();
      printf("error: %u %u\n", upng_get_error(upng), upng_get_error_line(upng));
      Serial.println();
    }
    Serial.println("Delete unpg Buffer");
    upng_free(upng);
    Serial.println("uPNG Buffer Deleted");
  }
  Serial.println("go Back");
}
//#########################################################################################
void Draw_Image_from_http( int x,int y,String name) {
  // get Image Counter from name of Image in config 
  int counter = 99;
  Serial.println(name);
  for (int i=0; i<=image_available; i++) {
    if (name == get_Image[i].name) {
    Serial.println("Find Image Data in Config");
    counter = i;
    break;
    }
  }
  if (counter == 99) {
    Serial.println("Image not Found in Config");
    return;
    }

  Serial.println("Get image data");
  WiFiClient client;
  image_bitmap = obtain_image(client, counter);
  
  if (get_Image[counter].typ == PNG) {
    Serial.println("Decode PNG");
    DrawImagePNG(x, y,image_bitmap);
  }
  else if (get_Image[counter].typ == BMP){
    Serial.println("Display BMP");
    
    //ToDo... Does not Work at the Moment writeImage need a Char[] array
    //char image_length[image_bitmap.length()];
    //char copy[image_length];
    //image_bitmap.toCharArray(copy, image_length);
    // display.writeImage(copy, x, y, 200, 200, false, false, true);
  } 
  else {Serial.println("Image Type not support");}  
}
//#########################################################################################
void InitialiseDisplay() {
  Serial.println("Initialise Display");
  display.init(0);
  SPI.end();
  SPI.begin(EPD_SCK, EPD_MISO, EPD_MOSI, EPD_CS);
  u8g2Fonts.begin(display); // connect u8g2 procedures to Adafruit GFX
  u8g2Fonts.setFontMode(1);                  // use u8g2 transparent mode (this is default)
  u8g2Fonts.setFontDirection(0);             // left to right (this is default)
  u8g2Fonts.setForegroundColor(GxEPD_BLACK); // apply Adafruit GFX color
  u8g2Fonts.setBackgroundColor(GxEPD_WHITE); // apply Adafruit GFX color
  u8g2Fonts.setFont(u8g2_font_helvB10_tf);   // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall
  display.fillScreen(GxEPD_WHITE);
  display.setFullWindow();
}
//#########################################################################################
void callback(){
   Serial.println("Call_Touch");
   //placeholder callback function
}
//#########################################################################################
void init_touch () {
  //Setup interrupt on Touch Pad 3 (GPIO15)
  #ifdef TouchT0 
    touchAttachInterrupt(T0, callback , Threshold); // T0 = Gipo4
  #endif
  #ifdef TouchT1
    touchAttachInterrupt(T1, callback , Threshold); // T1 = Gipo0 
  #endif
  #ifdef TouchT2
    touchAttachInterrupt(T2, callback , Threshold); // T2 = Gipo2 
  #endif
  #ifdef TouchT3
    touchAttachInterrupt(T3, callback , Threshold); // T3 = Gipo15! Used by Display default
  #endif
  #ifdef TouchT4
    touchAttachInterrupt(T4, callback , Threshold); // T4 = Gipo13! Used by Display default
  #endif
  #ifdef TouchT5
    touchAttachInterrupt(T5, callback , Threshold); // T5 = Gipo12! Used by Display default
  #endif
  #ifdef TouchT6
    touchAttachInterrupt(T6, callback , Threshold); // T6 = Gipo14! Used by Display default
  #endif
  #ifdef TouchT7
    touchAttachInterrupt(T7, callback , Threshold); // T7 = Gipo27
  #endif
  #ifdef TouchT8
    touchAttachInterrupt(T8, callback , Threshold); // T8 = Gipo33
  #endif
  #ifdef TouchT9
    touchAttachInterrupt(T9, callback , Threshold); // T9 = Gipo32
  #endif
}
//#########################################################################################
int print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
  return wakeup_reason;
}
//#########################################################################################
void print_GPIO_wake_up(){
  int GPIO_reason = esp_sleep_get_ext1_wakeup_status();
  Serial.print("GPIO that triggered the wake up: GPIO ");
  Serial.println((log(GPIO_reason))/log(2), 0);
}
//#########################################################################################
/*
  Version 12.0 reformatted to use u8g2 fonts
  1.  Screen layout revised
  2.  Made consitent with other versions specifically 7x5 variant
  3.  Introduced Visibility in Metres, Cloud cover in % and RH in %
  4.  Correct sunrise/sunset time when in imperial mode.

  Version 12.1 Clarified Waveshare ESP32 driver board connections

  Version 12.2 Changed GxEPD2 initialisation from 115200 to 0
  1.  display.init(115200); becomes display.init(0); to stop blank screen following update to GxEPD2

  Version 12.3
  1. Added 20-secs to allow for slow ESP32 RTC timers
*/
