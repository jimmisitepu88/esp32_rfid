#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         13           // Configurable, see typical pin layout above
#define SS_PIN          5          // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.

String read_rfid;                   // Add how many you need and don't forget to include the UID.
String ok_rfid_1="e199312d";        // This is for my main RFID Card. aka. The one I will be using to turn on my PC. Can also be used to shut it down if you want to.
String ok_rfid_2="fbecb673";        // This is for the RFID Keyfob. aka. Shutdown Keyfob. Not advisable tho. Just shutdown your PC normally.
const byte buz = 4;
/*
 * Initialize.
 */
void setup() {
    Serial.begin(115200);         // Initialize serial communications with the PC
    while (!Serial);            // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
    SPI.begin();                // Init SPI bus
    mfrc522.PCD_Init();         // Init MFRC522 card
    pinMode(buz, OUTPUT);
}
/*
 * Helper routine to dump a byte array as hex values to Serial.
 */
void dump_byte_array(byte *buffer, byte bufferSize) {
    read_rfid="";
    for (byte i = 0; i < bufferSize; i++) {
        read_rfid=read_rfid + String(buffer[i], HEX);
    }
    digitalWrite(buz, HIGH);delay(100);
    digitalWrite(buz, LOW);delay(100);
    digitalWrite(buz, HIGH);delay(100);
    digitalWrite(buz, LOW);delay(250);
    delay(1000);
}

void loop() {

      // Look for new cards
    if ( ! mfrc522.PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522.PICC_ReadCardSerial())
        return;

    dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
    Serial.println(read_rfid);
    if (read_rfid==ok_rfid_1) {
      //ok, open the door.
      //open_lock();
    }
    if (read_rfid==ok_rfid_2) {
      //ok, open the door.
      //close_lock2();
    }
}
