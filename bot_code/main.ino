

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiClientSecureAxTLS.h>
#include <WiFiClientSecureBearSSL.h>
#include <ESP8266HTTPClient.h>
#include <Arduino.h>
#include <TM1637Display.h>

#define CLK D2
#define DIO D3
#define BUZZER D5
#define TEST_DELAY   2000

auto eye_with_eye_broow = SEG_A | SEG_D | SEG_E | SEG_C | SEG_G;
auto mid_eye_b = SEG_G;
auto full_eye =  SEG_A | SEG_F | SEG_E | SEG_C | SEG_D | SEG_B;
auto mid_close = SEG_D;
const uint8_t middle_eye_D[] = {0x00, eye_with_eye_broow, eye_with_eye_broow, 0x00};
const uint8_t middle_eye_midb[] = {0x00, mid_eye_b, mid_eye_b, 0x00};
const uint8_t middle_open_eye[] = {0x00, full_eye, full_eye, 0x00};
const uint8_t middle_closed[] = {0x00, mid_close, mid_close, 0x00};
TM1637Display display(CLK, DIO);
int red_light_pin = D0;
int green_light_pin = D6;
int blue_light_pin = D7;

const char* ssid = "aniket";
const char* password = "xxx";
const char* API_ENDPOINT = "xxx";
const char* fingerprint = "xxx";

unsigned long lastTime = 0;

unsigned long timerDelay = 30000;

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
}

void connect_to_wifi() {
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}



void beep() {
  digitalWrite(BUZZER, HIGH) ; //Turn on active buzzer
  analogWrite(red_light_pin, LOW);
  delay (100);
  digitalWrite(BUZZER, LOW) ; //Turn off active buzzer
  delay (100);
  analogWrite(red_light_pin, 256);

}

void parseResult(String payload) {
  if (payload == "1") {
    beep();
    beep();
  } else {
    Serial.println("No Beep");
  }

}

void httpGETRequest(const char* serverName) {
  std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

  client->setFingerprint(fingerprint);

  HTTPClient https;

  Serial.print("[HTTPS] begin...\n");
  if (https.begin(*client, serverName)) {  // HTTPS

    Serial.print("[HTTPS] GET...\n");
    // start connection and send HTTP header
    int httpCode = https.GET();

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
        String payload = https.getString();
        parseResult(payload);
      }
    } else {
      Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
    }

    https.end();
  }

}


void setup()
{
  Serial.begin(115200);
  connect_to_wifi();

  pinMode(BUZZER, OUTPUT); // Set buzzer - pin 9 as an output
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);
}


void loop()
{


  display.setBrightness(0x0f);
  if ((millis() - lastTime) > timerDelay) {

    if (WiFi.status() == WL_CONNECTED) {
      httpGETRequest(API_ENDPOINT);
    }
    else {
      Serial.println("WiFi Disconnected");
    }
     lastTime = millis();
  }
  Serial.println((millis() - lastTime) > timerDelay);
  display.setSegments(middle_closed);
  delay(2000);
  display.setSegments(middle_open_eye);
  delay(100);
  display.setSegments(middle_eye_midb);
  delay(100);
  display.setSegments(middle_open_eye);
  delay(100);
  display.setSegments(middle_eye_midb);
  delay(100);
  display.setSegments(middle_open_eye);
  delay(100);
  display.setSegments(middle_eye_midb);
  delay(100);
  display.setSegments(middle_open_eye);
  delay(100);
  display.setSegments(middle_eye_midb);
  delay(100);
  display.setSegments(middle_open_eye);
  delay(1000);
  display.setSegments(middle_eye_D);
}