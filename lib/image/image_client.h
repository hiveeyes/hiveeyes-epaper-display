#ifndef IMAGE_CLIENT_H_
#define IMAGE_CLIENT_H_

#include <HTTPClient.h>

String obtain_image(WiFiClient& client) {

  // Define URI.
  String server = "swarm.hiveeyes.org";
  String uri = "/abc/def";

  HTTPClient http;
  http.begin(client, server, 80, uri);
  int httpCode = http.GET();
  if (httpCode == HTTP_CODE_OK) {
    String payload = http.getString();
    return payload;
  }

  return "";

}

#endif /* ifndef IMAGE_CLIENT_H_ */
