/*
This is coworking door outside
rfid with arduino connected to the tablet via USB/Serial
*/

/*
 This is server part
 Read from SoftwareSerial, wich connect to local object and Write to Serial
 Read from Serial and print to SoftwareSerial

 Create in Atom - PlatformIO
 */

#include <Arduino.h>

#include <SoftwareSerial.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10 //SS for RFID
#define RST_PIN 9 // RST for RFID (not connect)
#define open_pin 8

void printDec(byte *buffer, byte bufferSize);
void CheckCard(); //test dor with 4 cards
//void serialEvent();

//SoftwareSerial mySerial(2, 3); // RX, TX
String ID = "";
String msg="";
int myTimeout = 50;
byte nuidPICC = 0;
static uint32_t last;
bool flagRaley = false;
#define del 2000

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the RFID class

void setup() {
  Serial.begin(9600);
  //mySerial.begin(9600);
  Serial.setTimeout(myTimeout);
  //mySerial.setTimeout(myTimeout);
  pinMode(open_pin, OUTPUT);
  digitalWrite(open_pin, HIGH);

  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522
}

void loop() {
  if (((millis()-last) > del) & flagRaley) {
    nuidPICC = 0;
    digitalWrite(open_pin, HIGH);
    flagRaley = false;
  }

  // while (Serial.available()) {
  //   String myString = Serial.readString();
  //
  //   if (myString == "yes")
  //   {
  //     digitalWrite(open_pin, LOW);
  //   }}
  // if (mySerial.available()) {
  //   msg = mySerial.readString();
  //   Serial.print(msg);
  // }
  //
  // if (Serial.available()) {
  //   msg = Serial.readString();
  //   mySerial.print(msg);
  // }
  //
  // msg = "";

  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);

  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  if (rfid.uid.uidByte[0] != nuidPICC)
    {
      nuidPICC = rfid.uid.uidByte[0];

      printDec(rfid.uid.uidByte, rfid.uid.size);
      //CheckCard();
      Serial.println(ID);
      last = millis();
      flagRaley = true;
    }
  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();

}

// void CheckCard()
// {
//   String Cards[] = {"14441160124",
//                     "150162859",
//                     "112249150124",
//                     "23351217172"};
//   for (int i = 0; i < 4; i ++)
//   {
//     if (ID == Cards[i])
//     {
//       digitalWrite(open_pin, LOW);
//       //Serial.println("OK");
//     }
//     //else {Serial.println("NOT");}
//   }
// }
/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize)
{
  ID = "";
  for (byte i = 0; i < bufferSize; i++)
  {
    ID.concat(String(buffer[i], DEC));
  }
}

void serialEvent() {
  while (Serial.available()) {
    String myString = Serial.readString();

    if (myString == "yes")
    {
      digitalWrite(open_pin, LOW);
    }

  }
}
