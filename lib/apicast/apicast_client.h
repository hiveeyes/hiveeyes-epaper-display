#ifndef APICAST_CLIENT_H_
#define APICAST_CLIENT_H_

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

bool decode_beeflight(WiFiClient& json) {

  // Allocate the JsonDocument
  DynamicJsonDocument doc(5 * 1024);

  // Deserialize the JSON document.
  DeserializationError error = deserializeJson(doc, json);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return false;
  }
  
  Serial.println(" Decoding Apicast data");
  
  JsonArray data = doc["data"];
  int api_readings = data.size();
  
  for (int i = 0; i < api_readings; i++) {

    JsonObject reading = data[i];

    beeflight[i].date       = reading["Datum"].as<char*>();
    beeflight[i].morning    = reading["morgens"].as<char*>();
    beeflight[i].noon       = reading["mittags"].as<char*>();
    beeflight[i].evening    = reading["abends"].as<char*>(); 
  }

  return true;
}

bool obtain_apicast_data(WiFiClient& client) {

  // Define URI.
  HTTPClient http;
  http.useHTTP10(true);
  String server = apicast_server;

  String uri = apicast_uri;

  // Make HTTP request.
  //http.begin(uri, test_root_ca); // HTTPS example connection
  bool success;
  http.begin(client, server, 80, uri);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    if (!decode_beeflight(http.getStream())) return false;
    success = true;
    Serial.println("Connection Apicast OK");
  } else {
    Serial.printf("Connection failed, error: %s", http.errorToString(httpCode).c_str());
    success = false;
  }

  client.stop();
  http.end();
  return success;
}

#endif /* ifndef APCAST_CLIENT_H_ */
