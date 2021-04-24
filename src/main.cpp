#include <Arduino.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SSD1306Wire.h>

#include "images.h"

// display config
SSD1306Wire display(0x3c, D2, D1, GEOMETRY_64_48 );

// WiFi config
const char* ssid = "YOUR_SSID";
const char* key  = "YOUR_KEY";

ESP8266WebServer server(80);

void write(String text) {
  display.clear();
  display.drawString(0, 0, text);
  display.display();
}

void drawCall() {
    display.clear();
    display.drawXbm(8, 0, Phone_width, Phone_height, Phone_bits);
    display.display();
}

void drawCode() {
    display.clear();
    display.drawXbm(8, 0, Keyboard_width, Keyboard_height, Keyboard_bits);
    display.display();
}

void drawFree() {
    display.clear();
    display.drawXbm(8, 0, Free_width, Free_height, Free_bits);
    display.display();
}

void handleCall() {
  drawCall();
  server.send(200, "text/html", "Call set");
}

void handleCode() {
  drawCode();
  server.send(200, "text/html", "Code set");
}

void handleFree() {
  drawFree();
  server.send(200, "text/html", "Free set");
}

void setup() {
  // Initialising the UI will init the display too.
  display.init();
  //display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  
  WiFi.begin(ssid, key);
  write("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    if (WiFi.status() == WL_CONNECT_FAILED) {
      write("Connection failed.");
    }
  }
  server.on("/call", handleCall);
  server.on("/code", handleCode);
  server.on("/free", handleFree);
  server.begin();

  // display ip address
  write(WiFi.localIP().toString().substring(12));
}

void loop() {
  server.handleClient();
}
