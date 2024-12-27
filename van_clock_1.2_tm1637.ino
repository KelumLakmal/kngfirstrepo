#include <Arduino.h>
#include <TM1637Display.h>

#include "RTClib.h"

// Define the connections pins
#define CLK 7
#define DIO 8
TM1637Display display(CLK, DIO);  // Create an instance of the TM1637Display

RTC_DS3231 rtc;

const byte brigtPin = 9;

int hour = 6;
int minute = 27;
int lastMin = 0;
int k = 0;

int maxbrigtVal = 7;
int minbrigtVal = 2;

boolean colon = true;

#define TEST_DELAY 200

const uint8_t hiace[] = {

  0x00,
  0x00,
  0x00,
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,          // H
  SEG_F | SEG_E,                                  // I
  SEG_A | SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,  //A
  SEG_A | SEG_D | SEG_E | SEG_F,                  //C
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,          //E
  0x00,
  0x00,
  0x00,
  0x00
};

// const uint8_t SEG_DONE[] = {
//   SEG_E | SEG_C | SEG_D | SEG_B | SEG_G,          // d
//   SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,  // O
//   SEG_C | SEG_E | SEG_G,                          // n
//   SEG_A | SEG_D | SEG_E | SEG_F | SEG_G           // E
// };

void setup() {
  Serial.begin(9600);
  pinMode(brigtPin, INPUT);
  display.setBrightness(maxbrigtVal);  // Set the display brightness (0-7)


  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    //    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
     rtc.adjust(DateTime(2020, 1, 21, 10, 40, 0));
  }



  for (int i = 0; i < (sizeof(hiace) - 3); i++) {
    display.setSegments(hiace + i);  // Display the data
    delay(120);
  }

  display.clear();
}


void loop() {

  DateTime now = rtc.now();

  hour = now.hour();
  minute = now.minute();

  //  Serial.print(hour);
  //  Serial.println(minute);
  delay(100);

  int brigtRead = digitalRead(brigtPin);


  if (brigtRead == 0) {
    // parklight on
    display.setBrightness(minbrigtVal);
  } else {
    display.setBrightness(maxbrigtVal);
  }

  // display.showNumberDec(hour, true, 2, 0);
  if (colon) {
    display.showNumberDecEx(hour, 0b01000000, true, 2, 0);
    colon = !colon;
    // delay(TEST_DELAY * 3);
  }

  else {
    display.showNumberDec(hour, true, 2, 0);
    colon = !colon;
    // delay(TEST_DELAY * 2);
  }

  if (minute != lastMin) {
    display.showNumberDec(minute, true, 2, 2);
    lastMin = minute;
  }

  delay(TEST_DELAY * 3);  //dot blinking timing

}
