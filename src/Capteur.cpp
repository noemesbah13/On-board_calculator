#include <Adafruit_NeoPixel.h>
 
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
  // Affiche le buffer original vers l'écran
  // la librairie initialise le buffer avec un logo adafruit.
  display.display();

  return;
}
int tab[40]; // Tableau de valeur de temps

void lecture() {
  pinMode(2,OUTPUT);    // met la broche 2 en écriture
  double t=millis();
  digitalWrite(2,LOW);  // donne le start du signal
  delay(18);
  digitalWrite(2,HIGH);
  pinMode(2,INPUT_PULLUP);  // remet la broche 2 en lecture
  
  // lecture de la réponse
  // détection du signal
  while (digitalRead(2) == LOW) {}
  while (digitalRead(2) == HIGH) {}

  for (int i=0;i<40;i++) {
    while (digitalRead(2)==LOW){}
    t=micros();
    while (digitalRead(2)==HIGH){}
    tab[i]=micros()-t;
  }
  for (int i=0;i<40;i++) {
    Serial.println(tab[i]);
  }
}

int tabBit[40];     // Tableau de Bit

void calcul() {
    // Transformation des temps en 1 ou 0
    for (int i=0;i<40;i++){
        if (tab[i]>50){tabBit[i]=1;}
        else {tabBit[i]=0;}
    }
    
}

int tabVal[5];
void construction() {
  for (int i=0;i<40;i++) {
    int index = i/8 ;       // compte les octets de 1 à 5
    int bit = 7-i%8 ;       // récupère le bit
    tabVal[index] += tabBit[i]<<bit ;    // construit le nombre à la position index
  }
}


void loop() {
    delay(5000);
    lecture();
    delay(100);
    calcul();
    delay(100);
    construction();
    delay(500);
    Serial.println("\n");
    for (int i = 0;i<5;i++){Serial.println(tabVal[i]);tabVal[i]=0;}
    
}