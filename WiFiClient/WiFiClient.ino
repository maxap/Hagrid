/*
 *  Dieser Sketch sendet Daten via HTTP GET Requests im Netzwerk.
 *  Sämtliche Server-zu-ESP-Kommunikation wird hier geregelt.
 */

#include <ESP8266WiFi.h>

// Festlegen der Netzwerkdaten
const char* ssid     = "jugend_hackt";
const char* password = "aegheex9ieTheine";
const char* host = "10.0.15.98";

void setup() {
  //Initialisieren des seriellen Monitors
  Serial.begin(115200);
  delay(10);

  // Verbinden mit dem WiFi-Netzwerk und Ausgabe im seriellen Monitor zum Debuggen
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // Verbinden mit WiFi
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Ausgabe zu Debuggingzwecken
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Holt sich die aufbereiteten Wetterdaten vom Raspberry Pi zur Weiterverarbeitung
  Serial.println(GET());
  delay(5456);
  
  // Schickt den Wert an unseren Webserver
  // POST-Bezeichnung eigentlich falsch, aber zur besseren Unterscheidung gewählt
  POST("888488488585858");
  delay(2345678);
  }

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

