#include <RTCZero.h>
#include <lmic.h>
#include <hal/hal.h>
#include <Time.h>
#include <Adafruit_SleepyDog.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const uint8_t ledPin = 13;
uint32_t value=3;
int taskSwitch = 0;
int delayBetweenSendingSec = 60;
bool failedSending = false;


RTCZero rtc;
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//Replace with your keys
//^^^^^^^^^^^^^^^^^^^^^
static  u1_t nwkSKey[16] = { 0x51, 0x23, 0xC7, 0xC8, 0x5D, 0xDB, 0x03, 0xC6, 0xEC, 0xFA, 0x30, 0x16, 0x25, 0x4C, 0xF1, 0xA4 };
static  u1_t  appSKey[16] = { 0x66, 0xCD, 0x82, 0xCC, 0x26, 0x2C, 0x05, 0x05, 0x75, 0xE4, 0x25, 0x80, 0x3A, 0x65, 0xC1, 0xDE };
static  u4_t devAddr = 0x26011780; // <-- Change this address for every node!
//^^^^^^^^^^^^^^^^^^^^


// These callbacks are only used in over-the-air activation, so they are
// left empty here (we cannot leave them out completely unless
// DISABLE_JOIN is set in arduino-lmic/project_config/lmic_project_config.h,
// otherwise the linker will complain).
void os_getArtEui (u1_t* buf) { }
void os_getDevEui (u1_t* buf) { }
void os_getDevKey (u1_t* buf) { }

// This EUI must be in little-endian format, so least-significant-byte
// first. When copying an EUI from ttnctl output, this means to reverse
// the bytes. For TTN issued EUIs the last bytes should be 0xD5, 0xB3,
// 0x70.
//static const u1_t PROGMEM APPEUI[8]={ 0xEF, 0xFE, 0x01, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };
//void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);}

// This should also be in little endian format, see above.
//static const u1_t PROGMEM DEVEUI[8]={ 0x94, 0x63, 0x42, 0x35, 0x29, 0x89, 0x73, 0x56 };
//void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);}

// This key should be in big endian format (or, since it is not really a
// number but a block of memory, endianness does not really apply). In
// practice, a key taken from ttnctl can be copied as-is.
// The key shown here is the semtech default key.
//static const u1_t PROGMEM APPKEY[16] = { 0x2F, 0x29, 0x46, 0x00, 0x95, 0x5E, 0x79, 0x41, 0x3C, 0x62, 0x3F, 0xA9, 0xF7, 0x52, 0xF7, 0x0F };
//void os_getDevKey (u1_t* buf) {  memcpy_P(buf, APPKEY, 16);}

static osjob_t sendjob;


// Pin mapping
// Adapted for Feather M0 per p.10 of [feather]
const lmic_pinmap lmic_pins = {
  .nss = 8,                       // chip select on feather (rf95module) CS
  .rxtx = LMIC_UNUSED_PIN,
  .rst = 4,                       // reset pin
  //    .dio = {6, 5, LMIC_UNUSED_PIN}, // assumes external jumpers [feather_lora_jumper]
  // DIO1 is on JP1-1: is io1 - we connect to GPO6
  // DIO1 is on JP5-3: is D2 - we connect to GPO5
  .dio = {3, 6, LMIC_UNUSED_PIN},
  .rxtx_rx_active = 0,
  .rssi_cal = 8,              // LBT cal for the Adafruit Feather M0 LoRa, in dB
  .spi_freq = 8000000,
};

void onEvent (ev_t ev) {
  Serial.print(os_getTime());
  Serial.print(": ");
  switch (ev) {
    case EV_SCAN_TIMEOUT:
      Serial.println(F("EV_SCAN_TIMEOUT"));
      break;
    case EV_BEACON_FOUND:
      Serial.println(F("EV_BEACON_FOUND"));
      break;
    case EV_BEACON_MISSED:
      Serial.println(F("EV_BEACON_MISSED"));
      break;
    case EV_BEACON_TRACKED:
      Serial.println(F("EV_BEACON_TRACKED"));
      break;
    case EV_JOINING:
      Serial.println(F("EV_JOINING"));
      break;
    case EV_JOINED:
      Serial.println(F("EV_JOINED"));
      break;
    /*
      || This event is defined but not used in the code. No
      || point in wasting codespace on it.
      ||
      || case EV_RFU1:
      ||     Serial.println(F("EV_RFU1"));
      ||     break;
    */
    case EV_JOIN_FAILED:
      Serial.println(F("EV_JOIN_FAILED"));
      break;
    case EV_REJOIN_FAILED:
      Serial.println(F("EV_REJOIN_FAILED"));
      break;
    case EV_TXCOMPLETE:
      Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
      if (LMIC.txrxFlags & TXRX_ACK)
        Serial.println(F("Received ack"));
      if (LMIC.dataLen) {
        Serial.println(F("Received "));
        Serial.println(LMIC.dataLen);
        Serial.println(F(" bytes of payload"));
      }
      // Reset watchdog with every loop to make sure the sketch keeps running.
      // If you comment out this call watch what happens in about 4 iterations!
      break;
    case EV_LOST_TSYNC:
      Serial.println(F("EV_LOST_TSYNC"));
      break;
    case EV_RESET:
      Serial.println(F("EV_RESET"));
      break;
    case EV_RXCOMPLETE:
      // data received in ping slot
      Serial.println(F("EV_RXCOMPLETE"));
      break;
    case EV_LINK_DEAD:
      Serial.println(F("EV_LINK_DEAD"));
      break;
    case EV_LINK_ALIVE:
      Serial.println(F("EV_LINK_ALIVE"));
      break;
    /*
      || This event is defined but not used in the code. No
      || point in wasting codespace on it.
      ||
      || case EV_SCAN_FOUND:
      ||    Serial.println(F("EV_SCAN_FOUND"));
      ||    break;
    */
    case EV_TXSTART:
      Serial.println(F("EV_TXSTART"));
      break;
    case EV_TXCANCELED:
      Serial.println(F("EV_TXCANCELED"));
      break;
    case EV_RXSTART:
      Serial.println(F("EV_RXSTART"));
      break;
    case EV_JOIN_TXCOMPLETE:
      Serial.println(F("EV_JOIN_TXCOMPLETE"));
      break;

    default:
      Serial.print(F("Unknown event: "));
      Serial.println((unsigned) ev);
      break;
  }
}


void setup() {
  
  //while (!Serial);
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  Serial.println("Starting ...");
  // initialize OLED
  initOled();
  delay(500);
  Serial.println("Initizalized Oled ");
  
  // initialize LMIC libary
  initLmic();
  delay(500);
  Serial.println("Initizalized LMIC ");

  int countdownMS = Watchdog.enable(16000);
  Serial.print("Enabled the watchdog with max countdown of ");
  Serial.print(countdownMS, DEC);
  Serial.println(" milliseconds!");
  digitalWrite(ledPin, LOW);

  // initialize RTC
  initRtc();
  delay(500);
  Serial.println("Initizalized RTC ");
}


void alarmMatch()
{
  uint32_t nextAlarm = getTimeInSeconds();
  printTime(nextAlarm);
  Serial.println();
  sendData();
  rtc.setSeconds(0);
}

void loop() {
  digitalWrite(ledPin, HIGH);
  delay(200);
  digitalWrite(ledPin, LOW);
  delay(200);
  Watchdog.reset();
  os_runloop_once();
}

// INIT FUNCTIONS
void initOled()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); 
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10,0);
  display.println("Hello,");
  display.setCursor(10,12);
  display.println("Starting to send");
  display.setCursor(10,24);
  display.println("anytime now...");
  display.display();
}

void initRtc()
{
  rtc.begin();
  rtc.setSeconds(0);
  rtc.setAlarmSeconds(10);// or any number between 0~59, I set 59 to ensure the setup is finished before the first interrupt
  rtc.enableAlarm(rtc.MATCH_SS);
  rtc.attachInterrupt(alarmMatch);
  // in alarmMatch()
  rtc.setSeconds(0);
}

void initLmic()
{
  os_init();
  // Reset the MAC state. Session and pending data transfers will be discarded.
  LMIC_reset();

  LMIC_setSession (0x13, devAddr, nwkSKey, appSKey);

  // Set up the channels used by the Things Network, which corresponds
  // to the defaults of most gateways. Without this, only three base
  // channels from the LoRaWAN specification are used, which certainly
  // works, so it is good for debugging, but can overload those
  // frequencies, so be sure to configure the full frequency range of
  // your network here (unless your network autoconfigures them).
  // Setting up channels should happen after LMIC_setSession, as that
  // configures the minimal channel set.
  LMIC_setupChannel(0, 868100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(1, 868300000, DR_RANGE_MAP(DR_SF12, DR_SF7B), BAND_CENTI);      // g-band
  LMIC_setupChannel(2, 868500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(3, 867100000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(4, 867300000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(5, 867500000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(6, 867700000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(7, 867900000, DR_RANGE_MAP(DR_SF12, DR_SF7),  BAND_CENTI);      // g-band
  LMIC_setupChannel(8, 868800000, DR_RANGE_MAP(DR_FSK,  DR_FSK),  BAND_MILLI);      // g2-band
  // TTN defines an additional channel at 869.525Mhz using SF9 for class B
  // devices' ping slots. LMIC does not have an easy way to define set this
  // frequency and support for class B is spotty and untested, so this
  // frequency is not configured here.
  // TTN uses SF9 for its RX2 window.
  LMIC.dn2Dr = DR_SF9;

  // Set data rate and transmit power for uplink (note: txpow seems to be ignored by the library)

  LMIC_setDrTxpow(DR_SF7, 14); 
  LMIC_setAdrMode (0);

// Enable/disable link check validation.
// LMIC sets the ADRACKREQ bit in UP frames if there were no DN frames
// for a while. It expects the network to provide a DN message to prove
// connectivity with a span of UP frames. If this no such prove is coming
// then the datarate is lowered and a LINK_DEAD event is generated.
// This mode can be disabled and no connectivity prove (ADRACKREQ) is requested
// nor is the datarate changed.
// This must be called only if a session is established (e.g. after EV_JOINED)
   LMIC_setLinkCheckMode(0);
}


//General Functions
void sendData()
{
  switch(taskSwitch) {
    case 0:
      Serial.println(F("Sending now with sf DR_SF7"));
      printToOled("Sending now with","sf:","DR_SF7");
      LMIC_setDrTxpow(DR_SF7, 14);
      taskSwitch++;
      break;
    case 1:
      Serial.println(F("Sending now with sf DR_SF8"));
      printToOled("Sending now with","sf:","DR_SF8");
      LMIC_setDrTxpow(DR_SF8, 14); 
      taskSwitch++;
      break;
    case 2:
      Serial.println(F("Sending now with sf DR_SF9"));
      printToOled("Sending now with","sf:","DR_SF9");
      LMIC_setDrTxpow(DR_SF9, 14);
      taskSwitch++;
      break;
    case 3:
      Serial.println(F("Sending now with sf DR_SF10"));
      printToOled("Sending now with","sf:","DR_SF10");
      LMIC_setDrTxpow(DR_SF10, 14);
      taskSwitch++;
      break;
     case 4:
      Serial.println(F("Sending now with sf DR_SF11"));
      printToOled("Sending now with","sf:","DR_SF11");     
      LMIC_setDrTxpow(DR_SF11, 14);
      taskSwitch++;
      break;
     case 5:
      Serial.println(F("Sending now with sf DR_SF12"));
      printToOled("Sending now with","sf:","DR_SF12");
      LMIC_setDrTxpow(DR_SF12, 14);
      taskSwitch++;
      break;
    default:
      Serial.println(F("All done now, starting again in a few seconds"));
      printToOled("All done now!","","");
      taskSwitch = 0;
      break;
  }

  if (LMIC.opmode & OP_TXRXPEND) {
    Serial.println(F("OP_TXRXPEND, not sending"));
    failedSending = true;
  } else {
    // Prepare upstream data transmission at the next possible time.
    LMIC_setTxData2(1, (uint8_t*) &value, 4, 0);
    if (value<20){
      value++;
    }else{
      value=0;
    }
  }
}

void printToOled(String line1, String line2, String line3)
{
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(10,0);
  display.println(line1);
  display.setCursor(10,12);
  display.println(line2);
  display.setCursor(10,24);
  display.println(line3);
  display.display();
}
