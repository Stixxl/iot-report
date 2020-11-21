#include <RTCZero.h>

RTCZero rtc;

const byte ledPin = 6;

void setup() {
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  rtc.begin();
  rtc.setTime(00, 00, 00);
  rtc.setDate(00, 00, 00);

  //set alarm timer to 5 seconds later and enable it
  rtc.setAlarmTime(00, 00, 05);
  rtc.enableAlarm(rtc.MATCH_HHMMSS);

  rtc.attachInterrupt(ledSwitch);
}

void loop()
{
  rtc.standbyMode();    // Sleep until next alarm match
}

//reset internal clock and changes state of LED
void ledSwitch()
{
  if(digitalRead(ledPin) == HIGH)
  {
    digitalWrite(ledPin, LOW);
  }
  else
  {
    digitalWrite(ledPin, HIGH);
  }
  // resets internal clock back to the date "00:00:00" 
  // (so that the alarm kicks in again in 5 seconds)
  rtc.setTime(00, 00, 00);
}
