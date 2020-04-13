  #include <Wire.h>
  #include "RTClib.h"
  #include <Adafruit_GFX.h>
  #include <Adafruit_SSD1306.h>
//
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     13 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
  
//
// Setup RTC
  RTC_DS3231 RTC;
  char monthString[37]= {"JanFebMarAprMayJunJulAugSepOctNovDec"};
  int  monthIndex[122] ={0,3,6,9,12,15,18,21,24,27,30,33};
  float tempReading; // the analog reading from the sensor
//

void setup() {
  Serial.begin(115200);
  // If you want to set the aref to something other than 5v
  //analogReference(EXTERNAL);
   if (! RTC.begin()) {
    Serial.println("Couldn't find RTC");
    //while (1);
  }
  if (RTC.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // If the RTC have lost power it will sets the RTC to the date & time this sketch was compiled in the following line
    RTC.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  // Use I2C Scanner to check the address, if necessary change the 0x3C in the line below
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x64)
  // init done
  // set font size
  display.setTextSize(1);  // small font size
  display.setTextColor(WHITE);
  display.clearDisplay();
  
}

void loop() {
 //***** RTC **********
  DateTime now = RTC.now();
  display.setCursor(32,57);  
  display.print(now.day(), DEC);
  display.print('/');
  for (int i=0; i<=2; i++){
    display.print(monthString[monthIndex[now.month()-1]+i]);
  }
  display.print('/');
  display.print(now.year(), DEC);   
 // ********************* 

  //
  // display time in digital format
  display.setCursor(10,2); 
  display.print(now.hour(), DEC);
  printDigits(now.minute());
  printDigits(now.second());
  //
  // Now draw the clock face
  display.drawCircle(display.width()/2, display.height()/2, 20, WHITE);
  display.drawCircle(display.width()/2, display.height()/2, 2, WHITE);
  //
  //hour ticks
  for( int z=0; z < 360;z= z + 30 ){
  //Begin at 0° and stop at 360°
    float angle = z ;
    angle=(angle/57.29577951) ; //Convert degrees to radians
    int x2=(64+(sin(angle)*20));
    int y2=(32-(cos(angle)*20));
    int x3=(64+(sin(angle)*(20-5)));
    int y3=(32-(cos(angle)*(20-5)));
    display.drawLine(x2,y2,x3,y3,WHITE);
  }
  // display second hand
  float angle = now.second()*6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  int x3=(64+(sin(angle)*(20)));
  int y3=(32-(cos(angle)*(20)));
  display.drawLine(64,32,x3,y3,WHITE);
  //
  // display minute hand
  angle = now.minute() * 6 ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(64+(sin(angle)*(20-3)));
  y3=(32-(cos(angle)*(20-3)));
  display.drawLine(64,32,x3,y3,WHITE);
  //
  // display hour hand
  angle = now.hour() * 30 + int((now.minute() / 12) * 6 )   ;
  angle=(angle/57.29577951) ; //Convert degrees to radians  
  x3=(64+(sin(angle)*(20-11)));
  y3=(32-(cos(angle)*(20-11)));
  display.drawLine(64,32,x3,y3,WHITE);
   
   tempReading = RTC.getTemperature();   
   display.setCursor(90,2);
   display.print(tempReading); 
   display.print("C");

  
  // update display with all data
  display.display();
  delay(100);
  display.clearDisplay();

}

// **************** End Main Loop *****************

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  display.print(":");
  if(digits < 10)
    display.print('0');
    display.print(digits);
  }
