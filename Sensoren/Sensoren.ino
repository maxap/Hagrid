//DHT
#include <Wire.h>
#include <DHT.h>              // von ladyada: https://github.com/adafruit/DHT-sensor-library
#define DHTPIN 4              // benutzter ARDUINO-Pin
#define DHTTYPE DHT11         // Digitaler Pin, mit dem der Data-Pin vom DHT11 verbunden ist
DHT dht(DHTPIN, DHTTYPE);     // wichtig für DHT

//AQS
const int analogInPin = A0;   // Analoger Pin, mit dem der Data-Pin vom Luftqualitätssensor verbunden ist
int AirValue = 0;             // Luftqualitätswert

//Wifi Client
#include <ESP8266WiFi.h>
const char* ssid     = "jugend_hackt";      // Festlegen der Netzwerkdaten
const char* password = "aegheex9ieTheine";
const char* host = "10.0.15.98";
const char* streamId   = "....................";
const char* privateKey = "....................";


void setup() {
  Serial.begin(115200); //für den Serielen Monitor
  dht.begin();        //startet DHT

  // Verbinden mit dem WiFi-Netzwerk und Ausgabe im S.M zum Debuggen
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

//--------------------------------------HAUPTPROGRAMM--------------------------------------
void loop() {

  //--- LUFTQUALITÄT ---
    AirValue = analogRead(analogInPin);
    Serial.print("Luftqualitätswert = ");
    Serial.println(AirValue);

  //--- LUFTFEUCHTIGKEIT & TEMPERATUR ---
    int h = dht.readHumidity();
    int t = dht.readTemperature();
    if (isnan(t) || isnan(h)) { //prüft ob der DHT funktioniert (keine richtige Zahlen ausgibt)
      Serial.println("Fehler beim Lesen vom DHT-Sensor");
      delay(2000);
    }
    else { //schreibt Temp. und LF in Serielen Monitor
      Serial.print("Feuchtigkeit: "); 
      Serial.print(h);
      Serial.println(" %");
      Serial.print("Temperatur: "); 
      Serial.print(t);
      Serial.println(" Grad");
      Serial.println("");
    }

    delay(100);
}
//-----------------------------------------------------------------------------------------

//für Wifi-Verbindung
int value = 0;
void POST(String post)  {
  WiFiClient client;
  const int httpPort = 8080;
  if(client.connect(host, httpPort))  {
    Serial.println("connection failed");
  }
  String url = "/";
  url += "?sensor=" + post;
  // Ausgabe zu Debuggingzwecken
  Serial.print("Requesting URL: ");
  Serial.println(url);
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");   
}

String GET() {
  delay(5000);
  ++value;
  Serial.print("connecting to ");
  Serial.println(host);  
  // Benutzt WiFiClient-Klasse um eine TCP-Verbindung zu erstellen
  WiFiClient client;
  const int httpPort = 8080;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
  /*return;*/
  }
  // Erstellen einer URI für den Request
  String url = "/";
  Serial.print("Requesting URL: ");
  Serial.println(url);
  // Sendet den Request zum Server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      // return;
    }
}
  
  // Liesst alle Zeilen der Antwort aus und schreibt sie auf den seriellen Monitor
  String line;
  while(client.available()){
    line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  return line;
}

