

//--------------------------------------- Libraries

#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "lyRJXXFaMrvbTpBtYsPNW0I-QOzw3k8O";

char ssid[] = "SSID";
char pass[] = "Paswoord";



#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>


Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

// ------------------------------- BH1750 

#include <BH1750.h>

BH1750  lightMeter;


// ------------------------------------    OLED

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16
static const unsigned char PROGMEM logo_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };



void setup() {
  Serial.begin(9600);


  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("OLED HAS A BAD CONNECTION"));
    for(;;); // Don't proceed, loop forever
    }


    display.display();
    delay(1000);

    display.clearDisplay();

    display.drawPixel(10, 10, SSD1306_WHITE);

    display.display();
    delay(1000);

    display.clearDisplay();
    
  
  Serial.println(F("BMP280 Sensor event test"));

  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 Temperature sensor, check wiring!"));
    while (1) delay(10);
    }


  lightMeter.begin();
  Serial.println("Running lightMeter...");


 
  

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  bmp_temp->printSensorDetails();
}

void loop() {


//----------------------------------------------- BMP280 sensor read
  
  sensors_event_t temp_event, pressure_event;
  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);
  
  Serial.print(F("Temperature = "));
  Serial.print(temp_event.temperature);
  Serial.println(" *C");

  Serial.print(F("Pressure = "));
  Serial.print(pressure_event.pressure);
  Serial.println(" hPa");

  Serial.println();
  delay(2000);

//------------------------------------------------ BH1750 sensor read

  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.println(lux);


//------------------------------------------------ OLED screen
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left 
  
  display.println(F("Temperature = "));
  display.println(temp_event.temperature);
  display.println(" *C");
  
  display.display();
  delay(1000);
  
  display.clearDisplay();

  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left 

  display.println(F("Pressure = "));
  display.println(pressure_event.pressure);
  display.println(" hPa");
  
  display.display();
  delay(1000);
  
  display.clearDisplay();


  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);

  display.println(F("Light Intensity = "));
  display.println(lux);
  display.println("lightyears");
  delay(1000);

 
  
}
