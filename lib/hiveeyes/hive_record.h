#ifndef HIVE_RECORD_H_
#define HIVE_RECORD_H_

#include <Arduino.h>

typedef struct {
  String time;
  float temperature_outside;
  float humidity_outside;
  float temperature_inside_1;
  float temperature_inside_2;
  float temperature_inside_3;
  float temperature_inside_4;
  float temperature_inside_5;
  float weight;
  float voltage;                
  float rssi; 
} Hive_record_type;

#endif /* ifndef HIVE_RECORD_H_ */
