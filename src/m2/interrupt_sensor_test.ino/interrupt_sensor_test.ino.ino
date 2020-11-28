#include <RTCZero.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//boolean pressed = digitalRead(3) == HIGH;

const byte ledPin = LED_BUILTIN;
const byte sensorPin1 = 11;
const byte sensorPin2 = 12;

boolean isHigh1 = false;
boolean isHigh2 = false;

//For States see attached pdf
//we only need to track exiting and entering states since the other possibilities do not change the count
//Entering State 1
boolean isEntering1 = false;
//Entering State 2
boolean isEntering2 = false;

//Exiting State 1
boolean isExiting1 = false;
//Exiting State 2
boolean isExiting2 = false;

//we assume at most 255 people fit in one room
uint8_t count;

RTCZero rtc;

void setup()   {            
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  Serial.begin(9600);
  
  pinMode(ledPin,OUTPUT);
  pinMode(sensorPin1,INPUT);
  pinMode(sensorPin2,INPUT);
  
  attachInterrupt(digitalPinToInterrupt(sensorPin1), onChange1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sensorPin2), onChange2, CHANGE);
  count = 0;

  //timer for updating the oled screen
  rtc.begin();
  rtc.setTime(00, 00, 00);
  rtc.setDate(00, 00, 00);
  rtc.setAlarmSeconds(1);
  rtc.enableAlarm(rtc.MATCH_SS);
  rtc.attachInterrupt(showRoomState);
}

void loop() {
  //showRoomState();
  //delay(2000);
  //rtc.standbyMode();
  }

void onChange1() {
  if(digitalRead(sensorPin1) == HIGH)
  {
    isHigh1 = true;
    Serial.println("Sensor 1 high");
    if(!isHigh2) {
      isEntering1 = true;
    }
  } else {
    isHigh1 = false;
    Serial.println("Sensor 1 low");
  }

    if(isHigh2 && isExiting1) {
    isExiting2 = isHigh1;
  }
  
  if(!isHigh1 && !isHigh2 && isExiting1 && isExiting2) {
    Serial.println("Person left");
    if(count > 0) {
      count--;
    }
  }
  
  if(!isHigh1 && !isHigh2) {
     resetState();
    }
  }

}


void onChange2() {
  if(digitalRead(sensorPin2) == HIGH)
  {
    Serial.println("Sensor 2 high");
    isHigh2 = true;
    if(!isHigh1 && !isEntering1) {
      isExiting1 = true;
    }
  } else {
    Serial.println("Sensor 2 low");
    isHigh2 = false;
  }
  if(isHigh1 && isEntering1) {
    isEntering2 = isHigh2;
  }
  if(!isHigh1 && !isHigh2 && isEntering1 && isEntering2) {
    Serial.println("Person entered");
    count++;
  }

  if(!isHigh1 && !isHigh2) {
    resetState();
  }
}

//reset to initial state (entered whenever !isHigh1 && !isHigh2)
void resetState() {
    Serial.println("Entered initial state again");
    isEntering1 = false;
    isEntering2 = false;
    isExiting1 = false;
    isExiting2 = false;
}

void showRoomState() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(35,5);
  display.println(count);
  display.display();
}
