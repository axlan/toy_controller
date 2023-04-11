#include <array>

#include <Arduino.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <WiFiManager.h>

static constexpr int PIN_LEFT = 12;
static constexpr int PIN_RIGHT = 4;
static constexpr int PIN_UP = 16;
static constexpr int PIN_DOWN = 17;

static constexpr int PIN_A = 5;
static constexpr int PIN_B = 15;
static constexpr int PIN_C = 14;
static constexpr int PIN_D = 13;

struct ControllerButton {
  const char* name = "";
  int pin = -1;
};

static constexpr std::array<ControllerButton, 8> BUTTONS = {{
  {"LEFT", 12},
  {"RIGHT", 4},
  {"UP", 16},
  {"DOWN", 17},
  {"A", 5},
  {"B", 15},
  {"C", 14},
  {"D", 13},
}};

static constexpr const char* DEVICE_NAME = "HappyController";
static constexpr const char* MQTT_SERVER = "192.168.1.110";
static constexpr const char* MQTT_TOPIC = "buttons_pressed";


bool CheckPin(int pin) {
  if (pin == 17) {
    return analogRead(pin) > 800;
  }
  else {
    return digitalRead(pin);
  }
}

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

bool reconnect() {
  static long long next_reconnect = 0;
  if (next_reconnect > millis() || !WiFi.isConnected()) {
    return false;
  }

  mqtt_client.setServer(MQTT_SERVER, 1883);
  Serial.print("Attempting MQTT connection...");
  // Attempt to connect
  if (mqtt_client.connect(DEVICE_NAME)) {
    Serial.println("connected");
    return true;
  } else {
    Serial.print("failed, rc=");
    Serial.print(mqtt_client.state());
    Serial.println(" try again in 5 seconds");
    // Wait 5 seconds before retrying
    next_reconnect = millis() + 5000;
  }
  
  return false;
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
    if (wm.autoConnect(DEVICE_NAME))
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

  // client.setSocketTimeout(0xFFFF);
  mqtt_client.setKeepAlive(0xFFFF);
}
void loop()
{
  // Check for over the air update request and (if present) flash it
  ArduinoOTA.handle();
  if (!mqtt_client.connected()) {
    reconnect();
  }

  static uint32_t last_button_mask = 0;

  uint32_t new_button_mask = 0;
  for(size_t i = 0; i < BUTTONS.size(); i++) {
    if(CheckPin(BUTTONS[i].pin)) {
      new_button_mask |= 1 << i;
    }
  }

  if (new_button_mask != last_button_mask) {
    String msg = "[";
    bool first = true;
    for(size_t i = 0; i < BUTTONS.size(); i++) {
      if(new_button_mask & (1 << i)) {
        if (!first) {
          msg += ',';
        }
        first = false;
        msg += BUTTONS[i].name;
      }
    }
    msg += ']';
    Serial.println(msg);
    // mosquitto_sub -t "buttons_pressed"
    if (!mqtt_client.publish(MQTT_TOPIC, msg.c_str(), true)) {
      Serial.println("Json pub failed.");
    }
    last_button_mask = new_button_mask;
  }

  mqtt_client.loop();
  delay(100);
}
