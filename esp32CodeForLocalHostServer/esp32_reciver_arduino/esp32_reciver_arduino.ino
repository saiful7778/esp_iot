#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WebServer.h>
#include <IRremote.h>
#include <ESPmDNS.h>
#include <WebSocketsServer.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <Ticker.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// set all relay pin
const int relay_1 = 18;
const int relay_2 = 5; // problem
const int relay_3 = 13;
const int relay_4 = 2;

void sendData();
void sendRelayData(int relayNumber, int Status);

// Set wifi
const char* ssid = "DTR - Dream To Real";          // Your WiFi SSID
const char* password = "#8426#DoNoT";  // Your WiFi Password

AsyncWebServer server(80); // server port 80
WebSocketsServer webSocket = WebSocketsServer(81);

#include "webpage.h"
#include "handlefunctions.h"

//set ir remote
const int RECV_PIN = 15;
//IRsend irsend;
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup() {
  Serial.begin(115200);
  pinMode(relay_1, OUTPUT);
  pinMode(relay_2, OUTPUT);
  pinMode(relay_3, OUTPUT);
  pinMode(relay_4, OUTPUT);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.localIP());
  if (MDNS.begin("DTRHOME")) { //dtr_home.local/
    Serial.println("MDNS responder started");
  }
  //-----------------------------------------------
  server.on("/", [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", webpageCode);
    sendData();
  });
  server.on("/mobile", [](AsyncWebServerRequest * request) {
    request->send_P(200, "text/html", webpageCode);
  });
  server.onNotFound(notFound);
  //-----------------------------------------------
  server.begin(); webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  lcd.begin();
  irrecv.enableIRIn();
  lcd.setCursor(0, 0);
  lcd.print(WiFi.localIP());
}
void loop() {
  webSocket.loop();
}
