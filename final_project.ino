#include <Stepper.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// all measurements are in mm
#define WATER_SENSOR_HEIGHT 42
#define POND_DIAMETER 183
#define STEM_DIAMETER 48
#define POND_DEPTH 67
#define WATER_LEVEL_MAX 1023

//Stepper pins
#define STEPPER_IN1 26
#define STEPPER_IN2 33 // for some reason, In2 and In3 must be switched for stepper to work. IN2 is actually 33, IN3 is actually 25.
#define STEPPER_IN3 25
#define STEPPER_IN4 32

//touch sensor pins
#define TOUCH_SIG 36

//water sensor
#define WATER_READ 15
#define WATER_POWER 0

//declare variables
const int stepsPerRevolution = 32*64;  // change this to fit the number of steps per revolution for your motor
bool touchSensor = 0; //boolean value measures if touch sensor is touched/activated
bool bloomieBloomin = 0; //boolean measures if all environmental variables have been met so bloomie will bloom

extern const unsigned char smileyy[128];
extern const unsigned char waterSad[128];
extern const unsigned char sunSad[128];
extern const unsigned char thermSad[128];
extern const unsigned char touchSad[128];


// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, STEPPER_IN1, STEPPER_IN2, STEPPER_IN3, STEPPER_IN4);

void setup() {
  Serial.begin(115200);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  pinMode(WATER_POWER, OUTPUT);
  pinMode(WATER_READ, INPUT);
  digitalWrite(WATER_POWER, LOW);
  // Clear the buffer
  display.clearDisplay();
  //set up stepper speed
  myStepper.setSpeed(5);
}

void loop() {
  //display OLED icons
  showBitmap(waterSad, 128-32, 0);
  showBitmap(smileyy, 128-32, 0+32);
  showBitmap(sunSad, 128-32-32, 0);
  showBitmap(thermSad, 128-32-32, 0+32);
  showBitmap(touchSad, 128-32-32-32, 0);
  //grab water percentage and print to serial monitor
  int water_fill_percentage = getWaterPercentage();
  Serial.println(water_fill_percentage);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(water_fill_percentage);
  
  display.display();
  // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
  delay(500);

  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
  delay(2000);
  display.clearDisplay();
  display.display();
}

int getWaterPercentage() {
  digitalWrite(WATER_POWER, HIGH);
  delay(10);
  int raw_value = analogRead(WATER_READ);
  Serial.print("Raw: ");
  Serial.println(raw_value);
  digitalWrite(WATER_POWER, LOW);
  return map(raw_value, 0, WATER_LEVEL_MAX, 0, 100);
  //return raw_value;
}

bool getTouchBool(){

}