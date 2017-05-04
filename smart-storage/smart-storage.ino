#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//acc
#include <SparkFun_ADXL345.h>

ADXL345 adxl = ADXL345();

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ButtonRow.h>

#include <SmartStorageBox.h>

SmartStorageBox box("s002", "http://172.24.1.4:9000/api/");

#define OLED_RESET 0
Adafruit_SSD1306 display(OLED_RESET);
//ok, back, up, down, reset(up + down)
#define OK 0
#define BACK 1
#define UP 2
#define DOWN 3
#define RESET 4
int buttonValues[] = {645, 997, 745, 955, 988};
int size = sizeof(buttonValues) / sizeof(buttonValues[0]);
int threshold = 10;
ButtonRow buttonRow(A0, buttonValues, size, threshold);


static const unsigned char PROGMEM protoboxLogo[] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x38, 0x70, 0x8F, 0x3E, 0xDC, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x88, 0x84, 0x88, 0x49, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x88, 0xC4, 0x88, 0x6A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x89, 0x44, 0x88, 0x6A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x89, 0x44, 0x88, 0x5A, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x10, 0x89, 0xE4, 0x88, 0x5A, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x11, 0x89, 0x24, 0x88, 0x49, 0x26, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x3F, 0x73, 0x3F, 0x3E, 0xE8, 0xC6, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00,
0xBC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0xC0, 0x00, 0x00, 0x00, 0x00,
0x87, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x70, 0x00, 0x00, 0x00, 0x00,
0x81, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1C, 0x00, 0x00, 0x00, 0x00,
0x80, 0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x07, 0x00, 0x00, 0x00, 0x00,
0x98, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xC1, 0xC0, 0x00, 0x00, 0x00,
0xD6, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0D, 0x70, 0x70, 0x00, 0x00, 0x00,
0x71, 0x01, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x10, 0x1C, 0x00, 0x00, 0x00,
0x01, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0x07, 0x00, 0x00, 0x00,
0x01, 0x00, 0x1C, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xB0, 0x01, 0xC0, 0x00, 0x00,
0x01, 0x00, 0x07, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x50, 0x00, 0x70, 0x00, 0x00,
0x01, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1C, 0x8F, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x1F, 0x80, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x70, 0x00, 0x00, 0x20, 0x00, 0x18, 0xE0, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x18, 0x00, 0x00, 0x60, 0x00, 0x18, 0x30, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x18, 0x00, 0x00, 0x60, 0x00, 0x18, 0x30, 0x00, 0x00, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x1B, 0x61, 0xE0, 0xFC, 0x3C, 0x18, 0x30, 0x78, 0xC1, 0x88, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x1B, 0xC7, 0x38, 0x60, 0xE7, 0x18, 0x61, 0xCE, 0x63, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8C, 0x73, 0x06, 0x18, 0x60, 0xC3, 0x1F, 0x81, 0x86, 0x36, 0x08, 0x00, 0x00,
0x01, 0x00, 0x1E, 0x8F, 0xC3, 0x0C, 0x0C, 0x61, 0x81, 0x98, 0x63, 0x03, 0x14, 0x08, 0x00, 0x00,
0x01, 0x80, 0x1E, 0x8C, 0x03, 0x0C, 0x0C, 0x61, 0x81, 0x98, 0x33, 0x03, 0x1C, 0x08, 0x00, 0x00,
0x00, 0xC0, 0x1E, 0x8C, 0x03, 0x0C, 0x0C, 0x61, 0x81, 0x98, 0x33, 0x03, 0x1C, 0x08, 0x00, 0x00,
0x00, 0x60, 0x1E, 0x8C, 0x03, 0x0C, 0x0C, 0x61, 0x81, 0x98, 0x33, 0x03, 0x36, 0x08, 0x00, 0x00,
0x00, 0x38, 0x0E, 0x8C, 0x03, 0x06, 0x18, 0x60, 0xC3, 0x18, 0x31, 0x86, 0x36, 0x08, 0x00, 0x00,
0x00, 0x0C, 0x02, 0x8C, 0x03, 0x07, 0x38, 0x70, 0xE7, 0x18, 0xE1, 0xCE, 0x63, 0x08, 0x07, 0x0C,
0x00, 0x07, 0x00, 0x8C, 0x03, 0x01, 0xE0, 0x18, 0x3C, 0x1F, 0x80, 0x78, 0xC1, 0x88, 0x05, 0x12,
0x00, 0x01, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0xB1, 0x12,
0x00, 0x00, 0x60, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xA2, 0x12,
0x00, 0x00, 0x38, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0xA1, 0x12,
0x00, 0x00, 0x0C, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x45, 0x12,
0x00, 0x00, 0x07, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x47, 0x4C,
0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

int GREEN_LEDS = D6;
int RED_LEDS = D5;

ESP8266WiFiMulti WiFiMulti;

void setup() {

    WiFiMulti.addAP("smart-storage", "smart-storage");


    //AKSELEROMETER
    aks_init();

    //led lights
    pinMode(GREEN_LEDS, OUTPUT);
    pinMode(RED_LEDS, OUTPUT);

    // start serial communication
    Serial.begin(9600);
    Serial.println("ESP LOADING");
    Wire.begin(SDA, SCL);
    //oled
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
    display.clearDisplay();
    display.drawBitmap(1, 1, protoboxLogo, 128, 64, 1);
    display.display();
    delay(500);
    display.clearDisplay();
    display.display();


}


String menu_items[] = {
        "Info About Content", "Info About Use"};
int menu_length = 2;

int in = 0;
int last_button_pressed = -1;
int time_to_button_pressed_reset = 0;

uint32_t initial_weight = 0;

void main_menu() {
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    if (aks_get()) {
        if ((WiFiMulti.run() == WL_CONNECTED)) {
            box.postActivity();
            blink(RED_LEDS, 1, 100);
        }
        blink(GREEN_LEDS, 1, 100);
    }

    // todo move to function
    uint32_t weight, rfid;
    weight = Wire.read();
    weight = (weight << 8) | Wire.read();
    weight = (weight << 8) | Wire.read();
    weight = (weight << 8) | Wire.read();
    Serial.print("weight: ");
    Serial.print(weight);
    Serial.print("\n");
    int treshhold = 5;
    if ((weight < initial_weight - treshhold) || (weight > initial_weight + treshhold)) {

        if ((WiFiMulti.run() == WL_CONNECTED)) {
            initial_weight = weight;
            //box.postWeight(weight);
        }
    }

    rfid = Wire.read();
    rfid = (rfid << 8) | Wire.read();
    rfid = (rfid << 8) | Wire.read();
    rfid = (rfid << 8) | Wire.read();
    Serial.print("rfid: ");
    Serial.print(rfid);
    Serial.print("\n");

    if (rfid > 0) {

        if ((WiFiMulti.run() == WL_CONNECTED)) {
            //box.postRFID(String(rfid));
        }
    }

    //todo collect the rfid and send it to server

    Wire.requestFrom(0x11, 8);
    Serial.println("\n rfid: ");
    while (Wire.available()) { // slave may send less than requested
        char c = Wire.read(); // receive a byte as character
        Serial.print(c);         // print the character
    }
//TODO remeber last button clicked for some time to prevent dubble action on single click but keep responsveness
    int button_pressed = buttonRow.getPushedButton();
    if (button_pressed != last_button_pressed || time_to_button_pressed_reset < 1) {

        last_button_pressed = button_pressed;
        time_to_button_pressed_reset = 3;
        switch (button_pressed) {
            case OK:
                blink(GREEN_LEDS, 5, 100);

                break;
            case BACK:
                blink(RED_LEDS, 5, 300);
                break;
            case UP:
                if (in > 0) {
                    in--;
                }
                break;
            case DOWN:
              if(in < menu_length - 1){
                in++;
              }
                break;
            case RESET:
                //reset
                break;
        }
    }
    int line = 0;
    for (int i = in; i < menu_length; i++) {
        display.setCursor(0, 10 * line);
        display.print(menu_items[i]);
        line++;

    }
    if (time_to_button_pressed_reset > 0) {

        time_to_button_pressed_reset--;
    }
    display.display();
    delay(10);
}


void loop() {
main_menu();
}

boolean aks_get() {
    // getInterruptSource clears all triggered actions after returning value
    // Do not call again until you need to recheck for triggered actions

    byte interrupts = adxl.getInterruptSource();
/*
  // Free Fall Detection
  if (adxl.triggered(interrupts, ADXL345_FREE_FALL)) {
    //Serial.println("*** FREE FALL ***");
    //add code here to do when free fall is sensed
  }

  // Inactivity
  if (adxl.triggered(interrupts, ADXL345_INACTIVITY)) {
    //Serial.println("*** INACTIVITY ***");
    //add code here to do when inactivity is sensed
  }
*/
    // Activity
    if (adxl.triggered(interrupts, ADXL345_ACTIVITY)) {
        return true;
    }
/*
  // Double Tap Detection
  if (adxl.triggered(interrupts, ADXL345_DOUBLE_TAP)) {
    //Serial.println("*** DOUBLE TAP ***");
    //add code here to do when a 2X tap is sensed
  }

  // Tap Detection
  if (adxl.triggered(interrupts, ADXL345_SINGLE_TAP)) {
    //Serial.println("*** TAP ***");
    //add code here to do when a tap is sensed
  }*/
    return false;
}

//AKS
void aks_init() {
    adxl.powerOn();                     // Power on the ADXL345

    adxl.setRangeSetting(16);           // Give the range settings
    // Accepted values are 2g, 4g, 8g or 16g
    // Higher Values = Wider Measurement Range
    // Lower Values = Greater Sensitivity

    adxl.setSpiBit(
            0);                  // Configure the device to be in 4 wire SPI mode when set to '0' or 3 wire SPI mode when set to 1
    // Default: Set to 1
    // SPI pins on the ATMega328: 11, 12 and 13 as reference in SPI Library

    adxl.setActivityXYZ(1, 0,
                        0);       // Set to activate movement detection in the axes "adxl.setActivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
    adxl.setActivityThreshold(75);      // 62.5mg per increment   // Set activity   // Inactivity thresholds (0-255)

    adxl.setInactivityXYZ(1, 0,
                          0);     // Set to detect inactivity in all the axes "adxl.setInactivityXYZ(X, Y, Z);" (1 == ON, 0 == OFF)
    adxl.setInactivityThreshold(25);    // 62.5mg per increment   // Set inactivity // Inactivity thresholds (0-255)
    adxl.setTimeInactivity(10);         // How many seconds of no activity is inactive?

    adxl.setTapDetectionOnXYZ(0, 0,
                              1); // Detect taps in the directions turned ON "adxl.setTapDetectionOnX(X, Y, Z);" (1 == ON, 0 == OFF)

    // Set values for what is considered a TAP and what is a DOUBLE TAP (0-255)
    adxl.setTapThreshold(50);           // 62.5 mg per increment
    adxl.setTapDuration(15);            // 625 μs per increment
    adxl.setDoubleTapLatency(80);       // 1.25 ms per increment
    adxl.setDoubleTapWindow(200);       // 1.25 ms per increment

    // Set values for what is considered FREE FALL (0-255)
    adxl.setFreeFallThreshold(7);       // (5 - 9) recommended - 62.5mg per increment
    adxl.setFreeFallDuration(30);       // (20 - 70) recommended - 5ms per increment

    // Setting all interupts to take place on INT1 pin
    //adxl.setImportantInterruptMapping(1, 1, 1, 1, 1);     // Sets "adxl.setEveryInterruptMapping(single tap, double tap, free fall, activity, inactivity);"
    // Accepts only 1 or 2 values for pins INT1 and INT2. This chooses the pin on the ADXL345 to use for Interrupts.
    // This library may have a problem using INT2 pin. Default to INT1 pin.

    // Turn on Interrupts for each mode (1 == ON, 0 == OFF)
    adxl.InactivityINT(1);
    adxl.ActivityINT(1);
    adxl.FreeFallINT(1);
    adxl.doubleTapINT(1);
    adxl.singleTapINT(1);
}

void blink(int pin, int times, int d) {
    for (int j = 0; j < times; j++) {
        digitalWrite(pin, LOW);
        delay(d);
        digitalWrite(pin, HIGH);
        delay(d);
    }
    digitalWrite(pin, LOW);
}
