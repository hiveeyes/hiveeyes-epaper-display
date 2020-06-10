#ifndef image_RECORD_H_
#define image_RECORD_H_

#include <Arduino.h>

typedef struct {
  String name;
  String host;
  String uri;
  int typ;
  bool https;
  String Sign;    
} Image_record_type;

#endif /* ifndef image_RECORD_H_ */