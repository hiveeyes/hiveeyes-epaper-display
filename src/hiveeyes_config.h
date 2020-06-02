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


//This Config is to get forecast beeflight Data from http://apicast.hiveeyes.org/ 

String apicast_server   = "apicast.hiveeyes.org";
String apicast_uri_1    = "/beeflight/forecast/germany/";
// federal-state and Station
String apicast_uri_2    = "berlin_brandenburg/berlin";                               




// build uri´s complete please no Changes here!!!
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

