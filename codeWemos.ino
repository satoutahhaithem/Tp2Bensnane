#include <ESP8266WiFi.h>
#include "Thing.CoAP.h"
#include "Thing.CoAP/Server.h"
#include "Thing.CoAP/ESP/UDPPacketProvider.h"

Thing::CoAP::Server server;
Thing::CoAP::ESP::UDPPacketProvider udpProvider;

char* ssid = "Aa1";
char* password = "Haithem2023";
#define LED 2
#define BUTTON 0
float temp = 0.0;

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing");

  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to the WiFi network");
  Serial.println("My IP: ");
  Serial.println(WiFi.localIP());

  server.SetPacketProvider(udpProvider);

  server.CreateResource("LED", Thing::CoAP::ContentFormat::TextPlain, true)
    .OnGet([](Thing::CoAP::Request & request) {
      Serial.println("GET Request received for endpoint 'LED'");
      Serial.println(LED);

      std::string result;
      if (digitalRead(LED) == HIGH)
        result = "On";
      else
        result = "Off";

      return Thing::CoAP::Status::Content(result);
    })
    .OnPost([](Thing::CoAP::Request& request) {
      Serial.println("POST Request received for endpoint 'LED'");
      auto payload = request.GetPayload();
      std::string message(payload.begin(), payload.end());
      Serial.print("The client sent the message: ");
      Serial.println(message.c_str());
      if (message == "On") {
        digitalWrite(LED, HIGH);
      } else if (message == "Off") {
        digitalWrite(LED, LOW);
      } else {
        return Thing::CoAP::Status::BadRequest();
      }
      return Thing::CoAP::Status::Created("ok merci");
    });

  server.CreateResource("temp", Thing::CoAP::ContentFormat::TextPlain, false)
    .OnGet([](Thing::CoAP::Request & request) {
      Serial.println("GET Request received for endpoint 'temp'");
      std::string result;
      result = String(temp).c_str();
      return Thing::CoAP::Status::Content(result);
    });

  server.Start();
}

void loop() {
  temp += 1;
  Serial.println(temp);
  server.Process();
  delay(1000);
}
  