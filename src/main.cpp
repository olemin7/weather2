#include "main.h"
using namespace std;

constexpr auto pinCS = D6;
constexpr auto numberOfHorizontalDisplays = 4;
constexpr auto numberOfVerticalDisplays = 1;

constexpr auto DHTPin = D4;

const char *update_path = "/firmware";
bool is_safe_mobe = false;

void mqtt_send();

DHTesp dht;

ESP8266WebServer serverWeb(SERVER_PORT_WEB);
CMQTT mqtt;
ESP8266HTTPUpdateServer otaUpdater;
CWifiStateSignal wifiStateSignal;

te_ret get_about(ostream &out) {
    out << "{";
    out << "\"firmware\":\"" << DEVICE_VERSION << "\"";
    out << ",\"deviceName\":\"" << config.getDeviceName() << "\"";
    out << ",\"resetInfo\":" << system_get_rst_info()->reason;
    out << "}";
    return er_ok;
}

te_ret get_status(ostream &out) {
    out << "{\"temperature\":";
    toJson(out, dht.getTemperature());
    out << ",\"humidity\":";
    toJson(out, dht.getHumidity());
    out << "}";
    return er_ok;
}

void setup_WebPages() {
    otaUpdater.setup(&serverWeb, update_path, config.getOtaUsername(), config.getOtaPassword());

    serverWeb.on("/restart", []() {
        webRetResult(serverWeb, er_ok);
        delay(1000);
        ESP.restart();
    });

    serverWeb.on("/about", [] {
        wifiHandle_send_content_json(serverWeb, get_about);
    });

    serverWeb.on("/status", [] {
        wifiHandle_send_content_json(serverWeb, get_status);
    });

    serverWeb.on("/filesave", []() {
        DBG_FUNK();
        if (!serverWeb.hasArg("path") || !serverWeb.hasArg("payload")) {
            webRetResult(serverWeb, er_no_parameters);
            return;
        }
        const auto path = string("/www/") + serverWeb.arg("path").c_str();
        cout << path << endl;
        auto file = LittleFS.open(path.c_str(), "w");
        if (!file) {
            webRetResult(serverWeb, er_createFile);
            return;
        }
        if (!file.print(serverWeb.arg("payload"))) {
            webRetResult(serverWeb, er_FileIO);
            return;
        }
        file.close();
        webRetResult(serverWeb, er_ok);
    });

    serverWeb.on("/scanwifi", HTTP_ANY,
            [&]() {
                wifiHandle_sendlist(serverWeb);
            });
    serverWeb.on("/connectwifi", HTTP_ANY,
            [&]() {
                wifiHandle_connect(serverWeb);
            });

    serverWeb.on("/getlogs", HTTP_ANY,
            [&]() {
                serverWeb.send(200, "text/plain", log_buffer.c_str());
                log_buffer = "";
            });

    serverWeb.serveStatic("/", LittleFS, "/www/");

    serverWeb.onNotFound([] {
        Serial.println("Error no handler");
        Serial.println(serverWeb.uri());
        webRetResult(serverWeb, er_fileNotFound);
    });
    serverWeb.begin();
}

void setup_WIFIConnect() {
    WiFi.begin();
    wifiStateSignal.onSignal([](const wl_status_t &status) {
        wifi_status(cout);
    }
    );
    wifiStateSignal.begin();
    if (is_safe_mobe) {
        WiFi.persistent(false);
        WiFi.mode(WIFI_AP);
        WiFi.softAP(config.getDeviceName(), DEF_AP_PWD);
        DBG_OUT << "safemode AP " << config.getDeviceName() << ",pwd: " << DEF_AP_PWD << ",ip:" << WiFi.softAPIP().toString() << std::endl;
    } else if (WIFI_STA == WiFi.getMode()) {
        DBG_OUT << "connecting <" << WiFi.SSID() << "> " << endl;
    }
}

void setup() {
    // is_safe_mobe = isSafeMode(GPIO_PIN_WALL_SWITCH, 3000);

    Serial.begin(SERIAL_BAUND);
    logs_begin();
    DBG_FUNK();
    DBG_OUT << "is_safe_mobe=" << is_safe_mobe << endl;
    hw_info(cout);
    LittleFS.begin();
    if (!config.setup() || is_safe_mobe) {
        config.setDefault();
    }
    MDNS.addService("http", "tcp", SERVER_PORT_WEB);
    MDNS.begin(config.getDeviceName());
    setup_WebPages();

    LittleFS_info(cout);

    mqtt.setup(config.getMqttServer(), config.getMqttPort(), config.getDeviceName());
    string topic = "cmd/";
    topic += config.getDeviceName();

    mqtt.callback(topic, [](char *topic, byte *payload, unsigned int length) {
        DBG_OUT << "MQTT>>[" << topic << "]:";
        auto tt = reinterpret_cast<const char*>(payload);
        auto i = length;
        while (i--) {
            DBG_OUT << *tt;
            tt++;
        };
        DBG_OUT << endl;
    });

//------------------
    dht.setup(DHTPin, DHTesp::DHT22);
//-----------------

    setup_WIFIConnect();
    DBG_OUT << "Setup done" << endl;
}

static long nextMsgMQTT = 0;
void mqtt_send() {
    nextMsgMQTT = millis() + config.getMqttPeriod();

    string topic = "stat/";
    topic += config.getDeviceName();
    ostringstream payload;
    get_status(payload);
    DBG_OUT << "MQTT<<[" << topic << "]:" << payload.str() << endl;
    mqtt.publish(topic, payload.str());
}

void mqtt_loop() {
    if (WL_CONNECTED != WiFi.status()) {
        return;
    }
    mqtt.loop();

    if (millis() >= nextMsgMQTT) {
        mqtt_send();
    }
}

void loop() {
    wifiStateSignal.loop();
    mqtt_loop();
    serverWeb.handleClient();
}
