#include <Adafruit_NeoPixel.h>
 
#define RGB_PIN     48      // Broche de la led RGB
#define NUM_PIXELS  1       // Une seule led présente
 
Adafruit_NeoPixel pixel(NUM_PIXELS, RGB_PIN, NEO_GRB + NEO_KHZ800);
 
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <tinyxml2.h>
#include <HTTPClient.h>

using namespace tinyxml2;

#define SCREEN_WIDTH 128 // Largeur de l'écran OLED en pixels
#define SCREEN_HEIGHT 64 // Hauteur de l'écran OLED en pixels
// Déclaration pour l'écran SSD1306
#define OLED_RESET     -1 // Broche de réinitialisation
#define OLED_I2C_ADDRESS 0x3C  // Adresse I2C de l'écran SSD1306
#define OLED_I2C_SDA_PIN 42 //Numero de broche SDA
#define OLED_I2C_SCL_PIN 41 //Numero de broche SCL

//Instance de l'écran SSD1306
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
bool wifiConnect=false;

// Replace with your own network credentials
const char* ssid = "TP_INSA";
const char* password = "TP_INSA_3A";
const char* serverName = "http://192.168.12.100:8080/SendData";

void connecteHandler(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  wifiConnect=true;
  
}

void deconnecteHandler(WiFiEvent_t wifi_event, WiFiEventInfo_t wifi_info) {
  wifiConnect=false;
}

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
  display.print("Bonjour !");
  display.display();

  // ----------- configuration du WI-FI ------------------------------
  IPAddress local_IP(192, 168, 12, 6); // Your Desired Static IP Address
  IPAddress subnet(255, 255, 255, 0);
  IPAddress gateway(192, 168, 12, 254);
     // Configures Static IP Address
   if (!WiFi.config(local_IP, gateway, subnet))
   {
       Serial.println("Configuration Failed!");
   }
    WiFi.mode(WIFI_STA);
    WiFi.onEvent(connecteHandler,ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(deconnecteHandler,ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    WiFi.begin(ssid, password);
    Serial.println("\nConnecting to WiFi Network ..");
    // Print network settings assigned by the DHCP
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
    Serial.print("Subnet Mask: " );
    Serial.println(WiFi.subnetMask());
    Serial.print("Gateway IP: ");
    Serial.println(WiFi.gatewayIP());
    Serial.print("DNS 1: ");
    Serial.println(WiFi.dnsIP(0));
    Serial.print("DNS 2: ");
    Serial.println(WiFi.dnsIP(1));
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
    //.println(tab[i]);
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

void statutWifi(bool connecte) {
  if (connecte){
    display.setCursor(5,0);
    display.setTextSize(1);
    display.print("Online");
    display.setTextSize(2);
  }
  if (!connecte){
    display.setCursor(5,0);
    display.setTextSize(1);
    display.print("Offline");
    display.setTextSize(2);
  }
}

void loop() {
  static uint16_t hue = 0;
    tabVal[0]=0;
    tabVal[1]=0;
    tabVal[2]=0;
    tabVal[3]=0;
    tabVal[4]=0;
    delay(5000);
    lecture();
    delay(100);
    calcul();
    delay(100);
    //---------------------Création du fichier XML----------------------------
    construction();
    XMLDocument doc;
    XMLElement* root = doc.NewElement("soapenv:Envelope");
    doc.InsertFirstChild(root);
    root->SetAttribute("xmlns:soapenv", "http://schemas.xmlsoap.org/soap/envelope/");
    root->SetAttribute("xmlns:tp", "http://tp.insa");
    XMLElement* child1 = doc.NewElement("soapenv:Header");
    root->InsertEndChild(child1);
    XMLElement* child2 = doc.NewElement("soapenv:Body");
    root->InsertEndChild(child2);
    XMLElement* child3 = doc.NewElement("tp:Request1");
    child2->InsertEndChild(child3);
    child3->SetAttribute("id","6");
    XMLElement* child4 = doc.NewElement("Data");
    child3->InsertEndChild(child4);
    child4->SetAttribute("temperature",tabVal[2]+(tabVal[3]/10.0));
    child4->SetAttribute("humidity",tabVal[0]+(tabVal[1]/10.0));

    XMLPrinter printer;
    doc.Print(&printer);
    WiFiClient client;
    HTTPClient http;
    http.begin(client, serverName);
    String httpRequestData = printer.CStr();
    http.addHeader("Content-Type", "text/xml;charset=UTF-8");
    http.addHeader("SOAPAction", "SendDataAction");
    // Envoi de la requête HTTP POST
    int httpResponseCode = http.POST(httpRequestData);
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    String payload = http.getString();
    Serial.println(payload);
    XMLDocument receptionDoc;
    auto blue = 0;
    auto green = 0;
    auto red = 0;
    if (httpResponseCode == 200)
    {
      receptionDoc.Parse(payload.c_str());
      Serial.println("XML parsed successfully");
      XMLElement *pEnveloppe, *pBody, *pResponse1, *pData;
      pEnveloppe = receptionDoc.FirstChildElement("soapenv:Envelope");
      if (pEnveloppe)
      {
        pBody = pEnveloppe->FirstChildElement("soapenv:Body");
      }
      else
      {
        Serial.println("kaput 1");
      }
      if (pBody)
      {
        pResponse1 = pBody->FirstChildElement("tp:Response1");
      }
      else
      {
        Serial.println("kaput 2");
      }
      if (pResponse1)
      {
        pData = pResponse1->FirstChildElement("Data");
      }
      else
      {
        Serial.println("kaput 3");
      }
      if (pData)
      {
        red = pData->UnsignedAttribute("red");
        green = pData->UnsignedAttribute("green");
        blue = pData->UnsignedAttribute("blue");
      }
      else
      {
        Serial.println("Pas de Data");
      }
    }

    Serial.println(red);
    Serial.println(green);
    Serial.println(blue);
    http.end();

    //--------------------------------------------------------------------------



    delay(500);
    display.clearDisplay();
    delay(500);
    statutWifi(wifiConnect);
    display.setTextSize(2);
    if (tabVal[4] == tabVal[0]+tabVal[1]+tabVal[2]+tabVal[3]) {
        display.setCursor(5,25);
        display.print("T : ");
        display.print(tabVal[2]);
        display.print(",");
        display.print(tabVal[3]);
        display.print(" C");
        display.setCursor(5,45);
        display.print("H : ");
        display.print(tabVal[0]);
        display.print(",");
        display.print(tabVal[1]);
        display.print(" %");
        display.display();
        pixel.show();
        pixel.setBrightness(100);
        display.display();
        pixel.setPixelColor(0, pixel.Color(red, green, blue));
    } else {
        Serial.println("Checksum ERREUR");
        pixel.setPixelColor(0, pixel.Color(255, 0, 0));
        pixel.setBrightness(100);
    }
    
    
}