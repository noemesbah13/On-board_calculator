/*#include <Adafruit_NeoPixel.h>
 
#define RGB_PIN     48      // Broche de la led RGB
#define NUM_PIXELS  1       // Une seule led présente
 
Adafruit_NeoPixel pixel(NUM_PIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128 // Largeur de l'écran OLED en pixels
#define SCREEN_HEIGHT 64 // Hauteur de l'écran OLED en pixels
// Déclaration pour l'écran SSD1306
#define OLED_RESET     -1 // Broche de réinitialisation
#define OLED_I2C_ADDRESS 0x3C  // Adresse I2C de l'écran SSD1306
#define OLED_I2C_SDA_PIN 42 //Numero de broche SDA
#define OLED_I2C_SCL_PIN 41 //Numero de broche SCL

//Instance de l'écran SSD1306
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  //Initialisation de la led
  pixel.begin();
  pixel.setBrightness(100); // Ajustement de la luminosité (0-255)
  Serial.begin(115200);

  Serial.println(F("Setup"));

  // ----------------------------------------------------------------
  // initialisation de la communication I2C
  Wire.begin(OLED_I2C_SDA_PIN, OLED_I2C_SCL_PIN);
  Serial.println(F("Wire.begin est termine"));
    // initialisation de l'écran OLED
  display.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS);
  display.clearDisplay();
  display.setCursor(5,5);
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(2);
  display.setTextWrap(true);
  display.print("ca depend du montant du cheque");
  double t=millis();
  while (millis()<t+18){digitalWrite(2,LOW);}
  digitalWrite(2,HIGH);
  int tab[40];
  // lecture de la réponse
  // détection du signal
  t=micros();
  while (micros()<t+80) {
    if (digitalRead(2) == HIGH) {
      Serial.print("kaput");
    }
  }
  t=micros();
  while (micros()<t+80) {
    if (digitalRead(2) == LOW) {
      Serial.print("kaput");
    }
  }

  for (int i=0;i<40;i++) {
    while (digitalRead(2)==LOW){}
    t=micros();
    while (digitalRead(2)==HIGH){}
    tab[0]=micros()-t;
  }
  for (int i=0;i<40;i++) {
    Serial.print(tab[i]);
  }
  display.print("Fini");


  // Affiche le buffer original vers l'écran
  // la librairie initialise le buffer avec un logo adafruit.
  display.display();

  return;
}
 
void loop() {
  static uint16_t hue = 0; // Hue varie de 0 à 65535 pour un cycle complet
 
  // Convertie HSV en RGB
  uint32_t color = pixel.gamma32(pixel.ColorHSV(hue));
  pixel.setPixelColor(0, color);
  
  
  pixel.show();
 
  hue += 256; // Pas d'incrémentation (plus petit plus fluide)
  delay(20);  // Ajuster la durée entre les pas
  Serial.println("Hello Everyone!");
}
*/