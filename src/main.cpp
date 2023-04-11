#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>

static constexpr int PIN_LEFT = 12;
static constexpr int PIN_RIGHT = 4;
static constexpr int PIN_UP = 16;
static constexpr int PIN_DOWN = 17;

static constexpr int PIN_A = 5;
static constexpr int PIN_B = 15;
static constexpr int PIN_C = 14;
static constexpr int PIN_D = 13;


#define CHECK_PIN(PIN)         \
  if (PIN == 17)               \
  {                            \
    if (analogRead(PIN) > 800) \
      Serial.println(#PIN);    \
  }                            \
  else                         \
  {                            \
    if (digitalRead(PIN))      \
      Serial.println(#PIN);    \
  }

void setup()
{
  Serial.begin(115200);

  while (true)
  {
    // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    // wm.resetSettings();

    // Automatically connect using saved credentials,
    // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
    // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
    // then goes into a blocking loop awaiting configuration and will return success result

    // res = wm.autoConnect(); // auto generated AP name from chipid
    // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
    if (wm.autoConnect("HappyController"))
    {
      Serial.print("Wifi connected, IP address: ");
      Serial.println(WiFi.localIP());
      break;
    }
    else
    {
      Serial.print("Failed retrying.");
    }
  }
  /**
   * Enable OTA update
   */
  ArduinoOTA.begin();
}
void loop()
{
  // Check for over the air update request and (if present) flash it
  ArduinoOTA.handle();
  CHECK_PIN(PIN_A);
  CHECK_PIN(PIN_B);
  CHECK_PIN(PIN_C);
  CHECK_PIN(PIN_D);
  CHECK_PIN(PIN_LEFT);
  CHECK_PIN(PIN_RIGHT);
  CHECK_PIN(PIN_UP);
  CHECK_PIN(PIN_DOWN);
  delay(1000);
}
