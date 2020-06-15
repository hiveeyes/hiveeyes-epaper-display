#ifndef IMAGE_CLIENT_H_
#define IMAGE_CLIENT_H_

#include <HTTPClient.h>


String obtain_image(WiFiClient& client, int counter) {
  
  Serial.println("conecting to "+get_Image[counter].host);
  HTTPClient http;
  delay(10);
  http.useHTTP10(true);
  http.begin(client, get_Image[counter].host, 80, get_Image[counter].uri);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    
    String payload = http.getString();
    Serial.println("Image loaded");
    return payload;
  }
  else
  {
    Serial.println("Immage load Error");
  }
  
  return "";

}

#endif /* ifndef IMAGE_CLIENT_H_ */
