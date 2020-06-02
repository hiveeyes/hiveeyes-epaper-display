#ifndef APICAST_RECORD_H_
#define APICAST_RECORD_H_

#include <Arduino.h>

typedef struct {
  String date;
  String morning;
  String noon;
  String evening;   
} Apicast_record_type;

#endif /* ifndef APICAST_RECORD_H_ */
