/*
  MSC USB Drive basic directory list example
 
 This example shows how to create and destroy an SD card file 	
 The circuit:
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11, pin 7 on Teensy with audio board
 ** MISO - pin 12
 ** CLK - pin 13, pin 14 on Teensy with audio board
 ** CS - pin 4, pin 10 on Teensy with audio board
 
 created   Nov 2010
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe
 modified 17 Nov 2020
 by Warren Watson
 
 This example code is in the public domain.
 	 
 */
#include <SD.h>

// Setup USBHost_t36 and as many HUB ports as needed.
USBHost myusb;
USBHub hub1(myusb);
USBHub hub2(myusb);
USBHub hub3(myusb);
USBHub hub4(myusb);
// Setup MSC for the number of USB Drives you are using. (Two for this example)
// Mutiple  USB drives can be used. Hot plugging is supported. There is a slight
// delay after a USB MSC device is plugged in. This is waiting for initialization
// but after it is initialized ther should be no delay.
msController msDrive(myusb);

// change this to match your SD shield or module;
// Teensy 2.0: pin 0
// Teensy++ 2.0: pin 20
// Wiz820+SD board: pin 4
// Teensy audio board: pin 10
// Teensy 3.5 & 3.6 & 4.1 on-board: BUILTIN_SDCARD
//const int chipSelect = 10;
const int chipSelect = BUILTIN_SDCARD;

void setup()
{
  //Uncomment these lines for Teensy 3.x Audio Shield (Rev C)
  //SPI.setMOSI(7);  // Audio shield has MOSI on pin 7
  //SPI.setSCK(14);  // Audio shield has SCK on pin 14  
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect.
  }

  // Start USBHost_t36, HUB(s) and USB devices.
  myusb.begin();

  Serial.print("\nInitializing USB MSC drive...");

//  if (!SD.begin(chipSelect)) {
  if (!SD.begin(&msDrive)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");

  File root = SD.open("/");
  
  printDirectory(root, 0);
  
  Serial.println("done!");
}

void loop()
{
  // nothing happens after setup finishes.
}

void printDirectory(File dir, int numSpaces) {
   while(true) {
     File entry = dir.openNextFile();
     if (! entry) {
       //Serial.println("** no more files **");
       break;
     }
     printSpaces(numSpaces);
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numSpaces+2);
     } else {
       // files have sizes, directories do not
       printSpaces(48 - numSpaces - strlen(entry.name()));
       Serial.print("  ");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}

void printSpaces(int num) {
  for (int i=0; i < num; i++) {
    Serial.print(" ");
  }
}

