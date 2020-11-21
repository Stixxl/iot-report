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

//we assume at most 255 people fit in one room
uint8_t count;


void setup()   {            
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  //Adressierung beachten, hier 0x3C!}
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(ledPin,OUTPUT);
  pinMode(sensorPin1,INPUT);
  pinMode(sensorPin2,INPUT);
  attachInterrupt(digitalPinToInterrupt(sensorPin1), onChange1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(sensorPin2), onChange2, CHANGE);
  //We probably also need to react to falling interrupts to catch cases where not both gates are triggered
  count = 0;
}

void loop() {
  showRoomState();
  delay(2000);
  }

void onChange1() {
  Serial.println("Sensor 1 changed");

  if(digitalRead(sensorPin1) == HIGH)
  {
    isHigh1 = true;
    Serial.println("Sensor 1 high");
  } else {
    isHigh1 = false;
    Serial.println("Sensor 1 low");
  }
  
  if(isHigh1 && isHigh2) {
    Serial.println("Person exiting");
    if(count > 0) {
    count--;
    }
  }

}


void onChange2() {
  Serial.println("Sensor 2 changed");
  if(digitalRead(sensorPin2) == HIGH)
  {
    isHigh2 = true;
    Serial.println("Sensor 2 high");
  } else {
    isHigh2 = false;
    Serial.println("Sensor 2 low");
  }

  if(isHigh1 && isHigh2) {
    Serial.println("Person entering");
    count++;
  }
}

void showRoomState() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(35,5);
  display.println(count);
  display.display();
}
