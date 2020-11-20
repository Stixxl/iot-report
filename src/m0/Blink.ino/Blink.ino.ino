#include <RTCZero.h>

RTCZero rtc;

const byte ledPin = 13;

// Set to random time as we reset the clock every blink cycle

const byte seconds = 0;

const byte minutes = 00;

const byte hours = 17;

const byte day = 17;

const byte month = 11;

const byte year = 15;

// interval in seconds how long led is on/off
const byte INTERVAL = 1;

boolean isOn = false;


void setup() {
  while (!Serial); // wait for Serial to be initialized
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  rtc.begin();
  Serial.println("Starting...");
  setRTCAlarm();
  //rtc.standbyMode();
}

void loop() {
}


void blinkLED() {
   Serial.println("Blinking LED");
   if(!isOn)
   {
   digitalWrite(ledPin, HIGH);
   } else {
   digitalWrite(ledPin, LOW);
   }
   isOn = !isOn;
}

// Set rtc and attach interrupt, since we are resetting the clock too
// the alarm will trigger every INTERVAL seconds
void setRTCAlarm() {
  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);
  rtc.setAlarmSeconds(INTERVAL);
  rtc.enableAlarm(RTCZero::MATCH_SS);
  rtc.attachInterrupt(blinkInInterval);
  Serial.println("set rtc.");
}

void blinkInInterval() {
  blinkLED();
  setRTCAlarm();
}

void printTime() {
  Serial.print(rtc.getHours());
  Serial.print(":");
  Serial.print(rtc.getMinutes());
  Serial.print(":");
  Serial.print(rtc.getSeconds());
  Serial.println("");
}
