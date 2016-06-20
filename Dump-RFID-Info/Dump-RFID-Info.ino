/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the ID/UID, type and any data blocks it can read.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <Adafruit_NeoPixel.h>

#define RST_PIN         9          // Configurable, see typical pin layout above
#define SS_PIN          10         // Configurable, see typical pin layout above
#define PIN            6
#define NUM_LEDS      16

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(9600);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  strip.setBrightness(10);
  strip.begin(); // This initializes the NeoPixel library.
  strip.show();
}

void loop() {

  String uidString = "";

  for(int i=0;i<NUM_LEDS;i++){
    strip.setPixelColor(i, strip.Color(255, 255, 255)); //change RGB color value here
    strip.show();
  }
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
  Serial.print("UID in Hex: ");
  uidString = readUID(uidString);
  Serial.println();
  Serial.print("UID in decimal?: ");
  Serial.println(uidString);
  
  if(uidString.equals(" 04 92 51 226 107 54 128")){
    //pink
    colorRing(255, 5, 180);
  }
  if(uidString.equals(" 04 117 99 178 239 50 128")){
    //red
    colorRing(255, 0, 0);
  }
  if(uidString.equals(" 04 97 99 18 223 57 128")){
    //orange
    colorRing(255, 150, 0);
  }
  if(uidString.equals(" 04 109 142 242 17 53 128")){
    //yellow
    colorRing(255, 255, 5);
  }
  if(uidString.equals(" 04 100 84 122 222 57 128")){
    //green
    colorRing(0, 255, 0);
  }
  if(uidString.equals(" 04 29 37 202 239 50 128")){
    //blue
    colorRing(0, 0, 255);
  }
  if(uidString.equals(" 04 69 37 202 28 68 128")){
    //purple
    colorRing(135, 10, 215);
  }
  if(uidString.equals(" 04 25 55 18 160 61 128")){
    //rainbow
    rainbowRing();
  }

  delay(200);
}

String readUID(String uidString){
  for (byte i = 0; i < (&(mfrc522.uid))->size; i++) {
    if((&(mfrc522.uid))->uidByte[i] < 0x10) {
      Serial.print(F(" 0"));
      uidString = uidString + F(" 0");
    }
    else {
      Serial.print(F(" "));
      uidString = uidString + F(" ");
    }
    Serial.print((&(mfrc522.uid))->uidByte[i], HEX);
    uidString = uidString + (&(mfrc522.uid))->uidByte[i];
  } 
  //Serial.println();
  return uidString;
}

void colorRing(int r, int g, int b) {
  for(int i=0;i<NUM_LEDS;i++){
      strip.setPixelColor(i, strip.Color(r, g, b)); //change RGB color value here
      strip.show();
      delay(25);
    }
}

void rainbowRing() {
  
  int i = 0;
  while(i<NUM_LEDS*4) {     
    strip.setPixelColor(i%NUM_LEDS, strip.Color(255, 255, 255)); //change RGB color value here
    strip.setPixelColor((i+1)%NUM_LEDS, strip.Color(255, 5, 180)); //change RGB color value here
    strip.setPixelColor((i+2)%NUM_LEDS, strip.Color(255, 0, 0)); //change RGB color value here
    strip.setPixelColor((i+3)%NUM_LEDS, strip.Color(255, 150, 0)); //change RGB color value here
    strip.setPixelColor((i+4)%NUM_LEDS, strip.Color(255, 255, 5)); //change RGB color value here
    strip.setPixelColor((i+5)%NUM_LEDS, strip.Color(0, 255, 0)); //change RGB color value here
    strip.setPixelColor((i+6)%NUM_LEDS, strip.Color(0, 0, 255)); //change RGB color value here
    strip.setPixelColor((i+7)%NUM_LEDS, strip.Color(135, 10, 215)); //change RGB color value here
    strip.setPixelColor((i+8)%NUM_LEDS, strip.Color(255, 255, 255)); //change RGB color value here
    strip.setPixelColor((i+9)%NUM_LEDS, strip.Color(255, 5, 180)); //change RGB color value here
    strip.setPixelColor((i+10)%NUM_LEDS, strip.Color(255, 0, 0)); //change RGB color value here
    strip.setPixelColor((i+11)%NUM_LEDS, strip.Color(255, 150, 0)); //change RGB color value here
    strip.setPixelColor((i+12)%NUM_LEDS, strip.Color(255, 255, 5)); //change RGB color value here
    strip.setPixelColor((i+13)%NUM_LEDS, strip.Color(0, 255, 0)); //change RGB color value here
    strip.setPixelColor((i+14)%NUM_LEDS, strip.Color(0, 0, 255)); //change RGB color value here
    strip.setPixelColor((i+15)%NUM_LEDS, strip.Color(135, 10, 215)); //change RGB color value here
    i++;
    strip.show();
    delay(75);
  }
}

