#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <FastLED.h>

#define NUM_LEDS 1
#define DATA_PIN 48

const char* ssid = "your_ssid";
const char* password = "your_pass";

CRGB leds[NUM_LEDS];
WebServer server(80);

String createHtml(String c) {
  String html =
"<!doctype html>\n" \
"<html>\n" \
"  <head>\n" \
"    <meta charset=\"utf-8\">\n" \
"    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n" \
"    <title>Web Server Test</title>\n" \
"    <style>body { font-size: 300%; }</style>\n" \
"  </head>\n" \
"  <body>\n";
  if (c.equals("黒")) {
    html += "    <p>消灯しました</p>\n";
  }
  else if (c.length() != 0) {
    html += "    <p>" + c + "で点灯しました</p>\n";
  }
  html +=
"    <ul>\n" \
"      <li><a href=\"/r\">赤</a></li>\n" \
"      <li><a href=\"/g\">緑</a></li>\n" \
"      <li><a href=\"/b\">青</a></li>\n" \
"      <li><a href=\"/k\">消灯</a></li>\n" \
"    </ul>\n" \
"  </body>\n" \
"</html>\n";
  return html;
}

void handleRoot() {
  leds[0] = CRGB::Black;
  FastLED.show();
  String html = createHtml("黒");
  server.send(200, "text/html", html);
}

void handle_r() {
  leds[0] = CRGB::Red;
  FastLED.show();
  String html = createHtml("赤");
  server.send(200, "text/html", html);
}

void handle_g() {
  leds[0] = CRGB::Green;
  FastLED.show();
  String html = createHtml("緑");
  server.send(200, "text/html", html);
}

void handle_b() {
  leds[0] = CRGB::Blue;
  FastLED.show();
  String html = createHtml("青");
  server.send(200, "text/html", html);
}


void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void) {
  Serial.begin(115200);
  FastLED.addLeds<SK6812, DATA_PIN, GRB>(leds, NUM_LEDS);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/r", handle_r);
  server.on("/g", handle_g);
  server.on("/b", handle_b);
  server.on("/k", handleRoot);
  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  delay(2);
}
