 
/*********************************************************************
 This is an example for our nRF52 based Bluefruit LE modules
 Pick one up today in the adafruit shop!
 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!
 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
*********************************************************************/
#include <bluefruit.h>

BLEDis bledis;
BLEHidAdafruit blehid;

bool hasKeyPressed = false;

// Declare the pins for the Button and the LED<br>int buttonPin = 12;
uint8_t b1 = 10; // Undo
uint8_t b2 = 12; // Redo
uint8_t b3 = A5; // Increase brush size
uint8_t b4 = 11; // Decrease brush size
uint8_t b5 = A7; // brush 
uint8_t b6 = A1; // eraser
uint8_t b7 = A2; // Quick menu
uint8_t b8 = A3; // Alt / color picker
uint8_t b9 = 6; // Colors
uint8_t b10 = 5; // Transform
uint8_t b11 = 9; // Layers
uint8_t b12 = A4; // Selection
uint8_t b13 = 13; // This pin keeps looping*** but works fine with diode

uint8_t keycodeZ[1] = { HID_KEY_Z };
uint8_t keycodeAlt[1] = { HID_KEY_NONE };
hid_keyboard_report_t keyReport = { 0, 0, { 0 } };

#define KEY_SPACEBAR   0x20
#define KEY_LEFT_CTRL   0x80
#define KEY_LEFT_SHIFT    0x81
#define KEY_LEFT_ALT    0x82
#define KEY_LEFT_GUI    0x83
#define KEY_RIGHT_CTRL    0x84
#define KEY_RIGHT_SHIFT   0x85
#define KEY_RIGHT_ALT   0x86
#define KEY_RIGHT_GUI   0x87

//default starting mode
int num = 1;
bool keyPressedPreviously = false;

void setup() {
   Serial.begin(115200);
   //while ( !Serial ) delay(10);   // for nrf52840 with native usb

   Bluefruit.begin();
   Bluefruit.setTxPower(4);    // Check bluefruit.h for supported values
   Bluefruit.setName("Limelight Pro");

   // Configure and Start Device Information Service
   bledis.setManufacturer("Limelight Studios");
   bledis.setModel("V1.1");
   bledis.begin();

   /* Start BLE HID
   * Note: Apple requires BLE device must have min connection interval >= 20m
   * ( The smaller the connection interval the faster we could send data).
   * However for HID and MIDI device, Apple could accept min connection interval 
   * up to 11.25 ms. Therefore BLEHidAdafruit::begin() will try to set the min and max
   * connection interval to 11.25  ms and 15 ms respectively for best performance.
   */
   blehid.begin();

   /* Set connection interval (min, max) to your perferred value.
    * Note: It is already set by BLEHidAdafruit::begin() to 11.25ms - 15ms
    * min = 9*1.25=11.25 ms, max = 12*1.25= 15 ms 
    */
   /* Bluefruit.Periph.setConnInterval(9, 12); */

   // Set up and start advertising
   startAdv();
  
   // Connect button pins
   pinMode(b1, INPUT_PULLUP);
   pinMode(b2, INPUT_PULLUP);
   pinMode(b3, INPUT_PULLUP);
   pinMode(b4, INPUT_PULLUP);
   pinMode(b5, INPUT_PULLUP);
   pinMode(b6, INPUT_PULLUP);
   pinMode(b7, INPUT_PULLUP);
   pinMode(b8, INPUT_PULLUP);
   pinMode(b9, INPUT_PULLUP);
   pinMode(b10, INPUT_PULLUP);
   pinMode(b11, INPUT_PULLUP);
   pinMode(b12, INPUT_PULLUP);
   pinMode(b13, INPUT);
}

void startAdv(void)
{  
   // Advertising packet
   Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
   Bluefruit.Advertising.addTxPower();
   Bluefruit.Advertising.addAppearance(BLE_APPEARANCE_HID_KEYBOARD);
  
   // Include BLE HID service
   Bluefruit.Advertising.addService(blehid);

   // There is enough room for the dev name in the advertising packet
   Bluefruit.Advertising.addName();
  
   /* Start Advertising
    * - Enable auto advertising if disconnected
    * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
    * - Timeout for fast mode is 30 seconds
    * - Start(timeout) with timeout = 0 will advertise forever (until connected)
    * 
    * For recommended advertising interval
    * https://developer.apple.com/library/content/qa/qa1931/_index.html   
    */
   Bluefruit.Advertising.restartOnDisconnect(true);
   Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
   Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
   Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

void loop()
{
  bool anyKeyPressed = false;
  int keyIndex = 0;
  keyReport.modifier=0;

  for(int i=0;i<1;i++)
    {
      keyReport.keycode[i] = 0;
    }

  // Undo
  if ( 0 == digitalRead(b1) )
  {
     Serial.println("b1");
     blehid.keyboardReport( KEYBOARD_MODIFIER_LEFTGUI , keycodeZ );
     anyKeyPressed = true;
     keyPressedPreviously = true;
  }

  // Redo
  if ( 0 == digitalRead(b2) )
  {
     Serial.println("b2");
     blehid.keyboardReport( KEYBOARD_MODIFIER_LEFTSHIFT + KEYBOARD_MODIFIER_LEFTGUI, keycodeZ );
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }
  // Increase brush size
  if ( 0 == digitalRead(b3) )
  {
     Serial.println("b3"); 
     blehid.keyPress(']'); 
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }

  // Decrease brush size
  if ( 0 == digitalRead(b4) )
  {
     Serial.println("b4"); 
     blehid.keyPress('['); 
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }

  // Select Brush
  if ( 0 == digitalRead(b5) )
  {
     Serial.println("b5"); 
     blehid.keyPress('b'); 
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }

  // Select Erase
  if ( 0 == digitalRead(b6) )
  {
     Serial.println("b6"); 
     blehid.keyPress('e'); 
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }

  // Quick Menu (Spacebar) *****
  if ( 0 == digitalRead(b7) )
  {
     Serial.println("b7"); 
     blehid.keyPress(KEY_SPACEBAR); 
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }

  // Color picker (Option / Alt) *****
  if ( 0 == digitalRead(b8) )
  {
     Serial.println("b8"); 
     blehid.keyboardReport(KEYBOARD_MODIFIER_LEFTALT, keycodeAlt); 
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }

  // Open color
  if ( 0 == digitalRead(b9) )
  {
     Serial.println("b9"); 
     blehid.keyPress('c'); 
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }

  // Transform
  if ( 0 == digitalRead(b10) )
  {
     Serial.println("b10"); 
     blehid.keyPress('v'); 
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }

  // Layers
  if ( 0 == digitalRead(b11) )
  {
     Serial.println("b11"); 
     blehid.keyPress('l');
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }

  // Selection
  if ( 0 == digitalRead(b12) )
  {
     Serial.println("b12"); 
     blehid.keyPress('s'); 
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }

  // ---- *****
  int button13State = digitalRead(b13);
  if ( button13State == 1 )
  {
     Serial.println("b13"); 
     blehid.keyPress('l'); 
     anyKeyPressed = true;
     keyPressedPreviously = true;   
  }

  // Send All-zero report to indicate there is no keys pressed
  // Most of the time, it is, though we don't need to send zero report
  // every loop(), only a key is pressed in previous loop() 
  if ( !anyKeyPressed && keyPressedPreviously )
  {
    keyPressedPreviously = false;
    blehid.keyRelease();
  }
  
  // Poll interval if you have double clicks increase value default is 50
  delay(90);
   
}
