#include <ESP8266WiFi.h>
#include "Thing.CoAP.h"
#include "Thing.CoAP/Server.h"
#include "Thing.CoAP/ESP/UDPPacketProvider.h"

Thing::CoAP::Server server;
Thing::CoAP::ESP::UDPPacketProvider udpProvider;

const char* ssid = "Aa1";
const char* password = "Haithem2023";

#define LED_PIN 2
#define BUTTON_PIN 0
float temp = 0.0;

void setup() {
  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.print("My IP: ");
  Serial.println(WiFi.localIP());

  server.SetPacketProvider(udpProvider);

  server.CreateResource("LED", Thing::CoAP::ContentFormat::TextPlain, true)
    .OnGet([](Thing::CoAP::Request & request) {
      return Thing::CoAP::Status::Content(digitalRead(LED_PIN) == HIGH ? "On" : "Off");
    })
    .OnPost([](Thing::CoAP::Request & request) {
      auto payload = request.GetPayload();
      String message = String(payload.c_str());

      if (message == "On") {
        digitalWrite(LED_PIN, HIGH);
      } else if (message == "Off") {
        digitalWrite(LED_PIN, LOW);
      } else {
        return Thing::CoAP::Status::BadRequest();
      }

      return Thing::CoAP::Status::Created("Command Executed");
    });

  server.CreateResource("temp", Thing::CoAP::ContentFormat::TextPlain, false)
    .OnGet([](Thing::CoAP::Request & request) {
      return Thing::CoAP::Status::Content(String(temp).c_str());
    });

  server.Start();
}

void loop() {
  temp += 1;
  Serial.println(temp);
  server.Process();
  delay(1000);
}
