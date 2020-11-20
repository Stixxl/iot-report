const byte ledPin = 13;

// the setup function runs once when you press reset or power the board
void setup() {
  while (!Serial);
  Serial.begin(115200);
  delay(1000);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  Serial.println("Started");
}

void loop() {
  Serial.println("new iteration");
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(2000);
}
