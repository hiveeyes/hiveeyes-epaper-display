#ifndef HIVEEYES_CLIENT_H_
#define HIVEEYES_CLIENT_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

bool decode_data(WiFiClient& json) {

  // Allocate the JsonDocument
  DynamicJsonDocument doc(35 * 1024);

  // Deserialize the JSON document.
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }

  for (int i = 0; i < max_readings_hiveeyes; i++) {

    JsonObject reading = doc[i];

    hive_data[i].time                   = reading["time"].as<char*>();
    hive_data[i].temperature_outside    = reading["temperature.0x77.i2c:0"];
    hive_data[i].humidity_outside       = reading["humidity.0x77.i2c:0"];
    hive_data[i].temperature_inside_1   = reading["temperature.28ff641d8fc3944f.onewire:0"];
    hive_data[i].temperature_inside_2   = reading["temperature.28ff641d8fdf18c1.onewire:0"];
    hive_data[i].weight                 = reading["weight.0"];

  }

  return true;
}

bool obtain_hiveeyes_data(WiFiClient& client) {

  // Define URI.
  // https://getkotori.org/docs/handbook/export/
  HTTPClient http;
  String server = hiveeyes_server;

  // TODO: Improve Kotori by requesting only last reading.
  String uri = hiveeyes_uri + "?from=now-1m";

  // Make HTTP request.
  //http.begin(uri, test_root_ca); // HTTPS example connection
  bool success;
  http.begin(client, server, 80, uri);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    if (!decode_data(http.getStream())) return false;
    success = true;

  } else {
    Serial.printf("Connection failed, error: %s", http.errorToString(httpCode).c_str());
    success = false;
  }

  client.stop();
  http.end();
  return success;
}

#endif /* ifndef HIVEEYES_CLIENT_H_ */
