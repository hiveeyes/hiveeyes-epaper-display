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
  
  if (max_readings_hiveeyes  > doc.size()) {
    hive_readings = doc.size();
  }
  else
  {
    hive_readings = max_readings_hiveeyes;
  }
  Serial.println(" Decoding hiveeyes data");
  for (int i = 0; i < hive_readings; i++) {

    JsonObject reading = doc[i];

    hive_data[i].time                   = reading["time"].as<char*>();
    hive_data[i].temperature_outside    = reading[ReadingTemperature_outside];
    hive_data[i].humidity_outside       = reading[ReadingHumidity_outside];
    hive_data[i].temperature_inside_1   = reading[ReadingTemperature_inside_1];
    hive_data[i].temperature_inside_2   = reading[ReadingTemperature_inside_2];
    hive_data[i].temperature_inside_3   = reading[ReadingTemperature_inside_3];
    hive_data[i].temperature_inside_4   = reading[ReadingTemperature_inside_4];
    hive_data[i].temperature_inside_5   = reading[ReadingTemperature_inside_5];
    hive_data[i].weight                 = reading[ReadingWeight];
    hive_data[i].voltage                = reading[ReadingVoltage];
    hive_data[i].rssi                   = reading[ReadingRssi];
  }

  return true;
}

bool obtain_hiveeyes_data(WiFiClient& client) {

  // Define URI.
  // https://getkotori.org/docs/handbook/export/
  HTTPClient http;
  http.useHTTP10(true);
  String server = hiveeyes_server;

  // TODO: Improve Kotori by requesting only last reading.
  String uri = hiveeyes_uri + hiveeyes_readTime;

  // Make HTTP request.
  //http.begin(uri, test_root_ca); // HTTPS example connection
  bool success;
  http.begin(client, server, 80, uri);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    if (!decode_data(http.getStream())) return false;
    success = true;
    Serial.println("Connection Hiveeyes OK");
  } else {
    Serial.printf("Connection failed, error: %s", http.errorToString(httpCode).c_str());
    success = false;
  }

  client.stop();
  http.end();
  return success;
}

#endif /* ifndef HIVEEYES_CLIENT_H_ */
