#include "FS.h"
#include "SD.h"
#include "SPI.h"
SPIClass spiSD(HSPI);
#define SD_CS 27
#define SDSPEED 27000000
File root;

void setup(){
  Serial.begin(115200);
  spiSD.begin(14,2,15,27);//SCK,MISO,MOSI,ss // инициализируем HSPI1
  if(!SD.begin( SD_CS, spiSD, SDSPEED)){
  // if(!SD.begin()){
  Serial.println("Card Mount Failed");
  return;
  }
  Serial.println("found SD card");
  root = SD.open("/");
  printDirectory(root, 0);
  Serial.println("SD card Done");
  
}

void loop() {
  
}

void printDirectory(File dir, int numTabs) {
  while (true) {

    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}
