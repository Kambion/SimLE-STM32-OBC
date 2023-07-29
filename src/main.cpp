#include <Arduino.h>
#include <TinyGPSPlus.h>

static constexpr int EVENTS_NUM = 2;

enum EventNames{
  TOGGLE_LED,
  PRINT_GPS
};

unsigned long events_time[EVENTS_NUM];
unsigned long events_delays[EVENTS_NUM];
bool events[EVENTS_NUM];
unsigned long current_time;

HardwareSerial SerialGPS(PA10, PA15);
TinyGPSPlus gps;

void setup() {
  pinMode(PC13, OUTPUT);
  Serial.begin(9600);
  SerialGPS.begin(9600);
  events_delays[TOGGLE_LED] = 1000;
  events_delays[PRINT_GPS] = 5000;
}

void loop() {
  current_time = millis();
  for(int i = 0; i < EVENTS_NUM; i++){
    events[i] = (current_time - events_time[i] > events_delays[i]);
  }

  while (SerialGPS.available() > 0){
    gps.encode(SerialGPS.read());
  }

  if(events[TOGGLE_LED]){
    digitalWrite(PC13, !digitalRead(PC13));
    events_time[TOGGLE_LED] = current_time;
  }

  if(events[PRINT_GPS]){
    Serial.print("Position: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(' ');
    Serial.println(gps.location.lng(), 6);
    events_time[PRINT_GPS] = current_time;
  }
}