#include <WiFi.h>
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include "freertos/task.h"
#include "freertos/FreeRTOS.h" // Queue, Task 등 RTOS 사용 시
#include "freertos/queue.h"    // QueueHandle_t 정의
#include "esp_err.h"           // ESP_ERROR_CHECK 등 매크로

#define R_LED 2

static const char *TAG = "MAIN";

const char *ssid = "LED-wifi-2.4G";
const char *password = "led12345!";

const int ledChannel = 0;
const int freq = 5000;
const int resolution = 8;

WiFiServer server(80);

void wifi_init()
{
    Serial.println(String("Connect try : ") + ssid);

    WiFi.mode(WIFI_STA); // Wifi mode : STA mode ( 공유기에 접속할 수 있음 )
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) // WL_CONNECTED : Wifi network에 연결됨.
    {
        delay(500);
        Serial.println(".");
    }
    Serial.print(String("WIFI conneted\n IP : "));
    Serial.println(WiFi.localIP());
    server.begin();
}

void wifi_loop()
{

    WiFiClient client = server.available();

    if (!client)
    {
        return;
    }

    Serial.println("Client connected");
    String request = client.readStringUntil('\r');
    Serial.println(request);

    if (request.indexOf("/esp/playsong") >= 0)
    {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();                        // header 끝
        client.println("<h1>red light on</h1>"); // 본문 내용

        pinMode(R_LED, OUTPUT);

        digitalWrite(R_LED, HIGH);
    }

    else if (request.indexOf("/esp/setsong") >= 0)
    {
        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();                        // header 끝
        client.println("<h1>red light on</h1>"); // 본문 내용

        pinMode(R_LED, OUTPUT);

        digitalWrite(R_LED, LOW);
    }

    else if (request.indexOf("/light/pwm") >= 0)
    {

        // LED 채널과 주파수, 듀티 셋업
        ledcSetup(ledChannel, freq, resolution);

        // 각 led를 채널에 attach
        ledcAttachPin(R_LED, ledChannel);

        client.println("HTTP/1.1 200 OK");
        client.println("Content-type:text/html");
        client.println();               // header 끝
        client.println("<h1>pwm</h1>"); // 본문 내용

        for (int i = 0; i < 256; i++)
        {
            ledcWrite(ledChannel, i);
            delay(10);
        }
        for (int i = 255; i >= 0; i--)
        {
            ledcWrite(ledChannel, i);
            delay(10);
        }
    }

    delay(1); // 보내는 시간 대기
}