#include <Arduino.h>
#include <WiFi.h>
#include <esp_now.h>

#include "codes.h"

uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
esp_now_peer_info_t peerInfo;
uint32_t counter = 0;

void data_sent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup()
{
    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    if (esp_now_init() != ESP_OK)
    {
        Serial.println("Error initializing ESP-NOW");
        return;
    }

    esp_now_register_send_cb(data_sent);

    memcpy(peerInfo.peer_addr, broadcastAddress, 6);
    peerInfo.channel = 0;
    peerInfo.encrypt = false;

    // Add peer
    if (esp_now_add_peer(&peerInfo) != ESP_OK)
    {
        Serial.println("Failed to add peer");
        return;
    }
}

void send_code(uint32_t response)
{
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *)&response, sizeof(response));
    if (result == ESP_OK)
        Serial.println("Sent with success");
    else
        Serial.println("Error sending the data");
}

void loop()
{
    send_code(codes[counter++ % (sizeof(codes) / sizeof(uint32_t))]);
    delay(50);
}
