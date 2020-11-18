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

bool state[4] = {false, false, false, false};

uint8_t count;


void setup()   {            
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  //Adressierung beachten, hier 0x3C!}
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  pinMode(ledPin,OUTPUT);
  pinMode(sensorPin1,INPUT);
  pinMode(sensorPin2,INPUT);

  count = 0;
}

int i;

void loop() {


  readSensor(state);

  if(state[0] == true || state[1] == true)
  {
    digitalWrite(ledPin,HIGH);
  }
  else
  {
    digitalWrite(ledPin,LOW);
  }
  
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(3);
  display.setCursor(35,5);
  display.println(int(state[0]));
  display.display();delay(50);
  }

void readSensor(bool state[]){

  state[2] = state[0];
  state[3] = state[1];
  state[0] =digitalRead(sensorPin1);
  state[1] =digitalRead(sensorPin2);

  return state;
}
