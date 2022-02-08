//
// Created by Magnus Nordlander on 2022-01-30.
//

#ifndef FIRMWARE_ARDUINO_NETWORKCONTROLLER_H
#define FIRMWARE_ARDUINO_NETWORKCONTROLLER_H

#include <WiFiNINA_Pinout_Generic.h>
#include <WiFi_Generic.h>
#include <ArduinoOTA.h>
#include <PubSubClient.h>
#include <FS.h>
#include <LittleFS.h>
#include <string>
#include "optional.hpp"
#include "types.h"
#include "SystemStatus.h"

// Because these libraries don't use .cpp files, we have to forward declare the class instead to linking errors.
class WiFiWebServer;

#define SSID_MAX_LEN      32
#define PASS_MAX_LEN      64
#define MQTT_SERVER_LEN       20
#define MQTT_PORT_LEN         5
#define MQTT_USERNAME_LEN     20
#define MQTT_PASS_LEN         40
#define MQTT_PREFIX_LEN       20
#define TOPIC_LENGTH 49

typedef struct
{
    char wifi_ssid[SSID_MAX_LEN];
    char wifi_pw  [PASS_MAX_LEN];
}  WiFi_Credentials;

typedef struct
{
    char server[MQTT_SERVER_LEN + 1] = "test.mosquitto.org";
    char port[MQTT_PORT_LEN + 1] = "1883";
    char username[MQTT_USERNAME_LEN + 1] = "";
    char password[MQTT_PASS_LEN + 1] = "";
    char prefix[MQTT_PREFIX_LEN + 1] = "lcc";
}  MQTT_Configuration;

typedef struct
{
    WiFi_Credentials wiFiCredentials;
    MQTT_Configuration mqttConfig;
} WiFiNINA_Configuration;

class NetworkController {
public:
    explicit NetworkController(FS* _fileSystem, SystemStatus* _status, SystemSettings* _settings);

    void init(NetworkControllerMode mode);

    bool hasConfiguration();
    bool isConnectedToWifi();
    bool isConnectedToMqtt();
    NetworkControllerMode getMode() {
        return mode;
    }

    void loop();
private:
    NetworkControllerMode mode;
    FS* fileSystem;
    SystemStatus* status;
    SystemSettings* settings;

    uint8_t previousWifiStatus = 0;

    nonstd::optional<WiFiNINA_Configuration> config;
    nonstd::optional<absolute_time_t> wifiConnectTimeoutTime;
    nonstd::optional<absolute_time_t> mqttConnectTimeoutTime;
    nonstd::optional<absolute_time_t> mqttNextPublishTime;

    ArduinoOTAMdnsClass <WiFiServer, WiFiClient, WiFiUDP> ArduinoOTA;
    WiFiClient client = WiFiClient();
    PubSubClient mqtt = PubSubClient(client);

    WiFiWebServer* server = nullptr;

    bool otaInited = false;
    bool _isConnectedToWifi = false;
    bool topicsFormatted = false;

    char identifier[24];
    std::string stdIdentifier;

    void initConfigMode();
    void initOTA();
    void loopNormal();
    void loopConfig();
    void loopOta();
    void attemptWifiConnection();
    void attemptReadConfig();
    void writeConfig(WiFiNINA_Configuration newConfig);

    void ensureMqttClient();
    void ensureTopicsFormatted();

    void publishMqtt();

    void handleConfigHTTPRequest();
    void sendHTTPHeaders();
    void createHTML(String& root_html_template);

    char TOPIC_LWT[TOPIC_LENGTH];
    char TOPIC_STATE[TOPIC_LENGTH];
    char TOPIC_COMMAND[TOPIC_LENGTH];

    char TOPIC_AUTOCONF_STATE_SENSOR[128];
    char TOPIC_AUTOCONF_BREW_BOILER_SENSOR[128];
    char TOPIC_AUTOCONF_SERVICE_BOILER_SENSOR[128];
    char TOPIC_AUTOCONF_ECO_MODE_SWITCH[128];
    char TOPIC_AUTOCONF_SLEEP_MODE_SWITCH[128];
    char TOPIC_AUTOCONF_BREW_TEMPERATURE_TARGET_NUMBER[128];
    char TOPIC_AUTOCONF_SERVICE_TEMPERATURE_TARGET_NUMBER[128];
    char TOPIC_AUTOCONF_WATER_TANK_LOW_BINARY_SENSOR[128];
    char TOPIC_AUTOCONF_WIFI_SENSOR[128];

    void callback(char *topic, byte *payload, unsigned int length);

    void publishAutoconfigure();
};


#endif //FIRMWARE_ARDUINO_NETWORKCONTROLLER_H