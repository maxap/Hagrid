#include <ESP8266WiFi.h>
/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/


#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

// For the Adafruit shield, these are the default.
#define TFT_DC 4
#define TFT_CS 5


#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0
#define WHITE    0xFFFF
#define ORANGE   0xFA00

const unsigned char s1 [] PROGMEM = {
0x9f,0x20,
0xe,0x0,
0x0,0x0,
0x0,0x0,
0x3f,0x80,
0x7f,0xc0,
0x5f,0x40,
0x75,0xc0,
0x31,0x80,
0x9f,0x20,
0xc0,0x60
};

const unsigned char s2 [] PROGMEM = {
0x9f,0x20,
0xe,0x0,
0x0,0x0,
0x0,0x0,
0x3f,0x80,
0x7f,0xc0,
0x5f,0x40,
0x7f,0xc0,
0x31,0x80,
0x9f,0x20,
0xc0,0x60
};

const unsigned char s3 [] PROGMEM = {
0x9f,0x20,
0xe,0x0,
0x0,0x0,
0x0,0x0,
0x3f,0x80,
0x7f,0xc0,
0x5f,0x40,
0x71,0xc0,
0x35,0x80,
0x9f,0x20,
0xc0,0x60
};


// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC);
// If using the breakout, change pins as desired
//Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

const char* ssid = "jugend_hackt";
const char* password = "aegheex9ieTheine";
const char* host = "10.0.15.98";
void setup()
{
  Serial.begin(115200);
  tft.begin();
  ESP.wdtDisable();
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)  {
    delay(50);

    
  }
  Serial.println(WiFi.localIP());
  
}
int currentSelection;
int recieved;

void loop() {
  String empfang = GET();
  recieved = empfang.toInt();
  Serial.println(recieved);
  if(recieved != currentSelection) {

    currentSelection = recieved;    
    Bildschirm(recieved);
  }
  else  {
    delay(20000);
  }
}

void Bildschirm(int auswahl)
{
  Serial.println(GET());
  delay(100);
  if(auswahl == 0){
    tft.fillScreen(GREEN);
    tft.drawBitmap(0, 0, s1, 240, 240, WHITE);
  }
  else if(auswahl == 1){
    tft.fillScreen(YELLOW);
    tft.drawBitmap(0, 0, s2, 240, 240, WHITE);
  }
  else if(auswahl == 3){
    tft.fillScreen(ORANGE);
    tft.drawBitmap(0, 0, s2, 240, 240, WHITE);
  }
  else if(auswahl == 4){
    tft.fillScreen(RED);
    tft.drawBitmap(0, 0, s3, 240, 240, WHITE);
  }

}

String GET() {
  delay(5000);


  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 8080;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
//    return;
  }
  
  // We now create a URI for the request
  String url = "/";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
//      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  String line;
  while(client.available()){
    line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  return line;
}


