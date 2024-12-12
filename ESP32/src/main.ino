#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>


// TODO: configure those for your setup
const char* ssid = "SSID"; // Wifi name
const char* password = "PASSWORD"; // Wifi password
const uint8_t motors_pins[] = { 22, 21, 33, 32, 31 }; // Pins numbers for each motors or use GPIOXX
//#define LED_BUILTIN 24 // should be defined already, depends on your board pin number


#define MOTORS_COUNT (sizeof(motors_pins)/sizeof(motors_pins[0]))
WiFiUDP udp;
bool blink = false;

void setup() {
    Serial.begin(115200);
    Serial.println("DakyFirmware starting");

    // setup pins
    pinMode(LED_BUILTIN, OUTPUT);
    analogWriteFrequency(8000);
    analogWriteResolution(8); // max 255
    for (int i = 0; i < MOTORS_COUNT; ++i) {
        pinMode(motors_pins[i], OUTPUT);
    }

    WiFi.mode(WIFI_STA);
    WiFi.setSleep(false); // should fix high latency
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.println("connecting to wifi");
        digitalWrite(LED_BUILTIN, (blink = !blink));
        delay(500);
    }
    digitalWrite(LED_BUILTIN, (blink = 0));

    // 3 quick blinks = success
    digitalWrite(LED_BUILTIN, 1); delay(100);
    digitalWrite(LED_BUILTIN, 0); delay(100);
    digitalWrite(LED_BUILTIN, 1); delay(100);
    digitalWrite(LED_BUILTIN, 0); delay(100);
    digitalWrite(LED_BUILTIN, 1); delay(100);
    digitalWrite(LED_BUILTIN, 0); delay(100);

    // server
    udp.begin(1234);

    Serial.println("started and ready");
}

void loop() {
    int size = udp.parsePacket();
    if (size == 0) return;

    byte raw_[size];
    byte *raw = raw_;
    size = udp.read(raw, size);
    size -= 1;
    char command = raw[0];
    raw++; // skip size byte

    switch (command) {

    case '/': { // blink test
        digitalWrite(LED_BUILTIN, (blink = !blink));
        Serial.println("blink");
        break;
    }

    case 'm': { // send motor value
        struct __attribute__((packed)) { uint8_t motor_id; uint8_t strength; }* data;
        if (size != sizeof(*data)) goto signal_error;
        data = (typeof(data))raw;
        if (data->motor_id >= MOTORS_COUNT) goto signal_error;
        analogWrite(motors_pins[data->motor_id], data->strength);
        break;
    }


    default: {
        goto signal_error;
    }
    }

    return;

signal_error:
    printf("error command: %c\n", raw[0]);
}
