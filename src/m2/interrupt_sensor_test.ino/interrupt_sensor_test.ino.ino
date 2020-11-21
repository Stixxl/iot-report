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
  attachInterrupt(digitalPinToInterrupt(sensorPin1), onRising1, RISING);
  attachInterrupt(digitalPinToInterrupt(sensorPin2), onRising2, RISING);
  attachInterrupt(digitalPinToInterrupt(sensorPin1), onFalling1, LOW);
  attachInterrupt(digitalPinToInterrupt(sensorPin2), onFalling2, LOW);
  //We probably also need to react to falling interrupts to catch cases where not both gates are triggered
  count = 0;
}

void loop() {
  showRoomState();
  delay(2000);
  }

void onRising1() {
  Serial.println("Sensor 1 high");
  isHigh1 = true;
  
  if(isHigh2) {
    Serial.println("Person exiting");
    if(count > 0) {
    count--;
    }
  }

}

void onFalling1() {
  Serial.println("Sensor 1 low");
  isHigh1 = false;
}

void onRising2() {
  Serial.println("Sensor 2 high");
  isHigh2 = true;

  if(isHigh1) {
    Serial.println("Person entering");
    count++;
  }
}

void onFalling2() {
 Serial.println("Sensor 2 low"); 
 
 isHigh2 = false;
}

void showRoomState() {
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(35,5);
  display.println(count);
  display.display();
}
