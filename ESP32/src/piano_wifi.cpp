#include <WiFi.h>
#include <WebServer.h>
#include <ArduinoJson.h>
#include <Arduino.h>
#include <stdio.h>
#include <string.h>
#include "freertos/task.h"
#include "freertos/FreeRTOS.h" // Queue, Task 등 RTOS 사용 시
#include "freertos/queue.h"    // QueueHandle_t 정의

#include "task.h"

#define R_LED 2

static const char *TAG = "MAIN";

const char *ssid = "";
const char *password = "";

WebServer server(80);

String body;

int handleNotes()
{
    body = server.arg("notes");
    // String body = server.arg("notes"); 노트북 가져오면 notes -> plain로 바꾸기, get-> post로 바꾸기

    Serial.println("받은 JSON: " + body);

    JsonDocument doc;

    DeserializationError err = deserializeJson(doc, body);

    if (err)
    {
        Serial.print("JSON 파싱 실패: ");
        Serial.println(err.c_str());
        server.send(400, "application/json", "{\"code\":\"400\", \"data\":null, \"msg\": \"json 데이터 가져오기 실패\"}");
        return 0;
    }

    return 1;
}

void handlePlaySong()
{
    if (handleNotes())
    {
        currentSong = body.c_str();
        playSongFlag = 1;
        song_idx = 1;
        server.send(200, "application/json", "{\"code\":\"200\", \"data\":null, \"msg\": \"노래 재생 성공\"}");
    }
}

void handleSetSong()
{
    if (handleNotes())
    {
        currentSong = body.c_str();
        song_idx = 1;
        server.send(200, "application/json", "{\"code\":\"200\", \"data\":null, \"msg\": \"노래 세팅 성공\"}");
    }
}

void handleSetAlarmTimeSong()
{

    server.send(200, "application/json", "{\"code\":\"200\", \"data\":null, \"msg\": \"알람 세팅 성공\"}");
}

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
    Serial.print(String("WIFI connected\n IP : "));
    Serial.println(WiFi.localIP());

    server.on("/playsong", HTTP_GET, handlePlaySong);
    server.on("/setsong", HTTP_POST, handleSetSong);
    server.on("/setalarmtime", HTTP_POST, handleSetAlarmTimeSong);
    server.begin();
}

void wifi_loop()
{
    server.handleClient();
    // Serial.println("Client connected");
    // String request = client.readStringUntil('\r');
    // Serial.println(request);

    // if (request.indexOf("POST /playsong") >= 0)
    // {
    //     String body = server.arg("plain");
    //     Serial.println("받은 JSON: " + body);

    //     StaticJsonDocument<200> doc;

    //     DeserializationError err = deserializeJson(doc, body);

    //     if (err)
    //     {
    //         Serial.print("JSON 파싱 실패: ");
    //         Serial.println(err.c_str());
    //         server.send(400, "application/json", "{\"error\":\"Invalid JSON\"}");
    //         return;
    //     }

    //     String notes = doc["notes"]; // "C,D,E,F"
    //     String title = doc["title"]; // "테스트곡"

    //     Serial.println("notes: " + notes);
    //     Serial.println("title: " + title);

    //     pinMode(R_LED, OUTPUT);

    //     digitalWrite(R_LED, HIGH);
    // }

    // else if (request.indexOf("POST /setsong") >= 0)
    // {
    //     client.println("HTTP/1.1 200 OK");
    //     client.println("Content-type:text/html");
    //     client.println();                        // header 끝
    //     client.println("<h1>red light on</h1>"); // 본문 내용

    //     pinMode(R_LED, OUTPUT);

    //     digitalWrite(R_LED, LOW);
    // }

    // else if (request.indexOf("POST /setalarmtime") >= 0)
    // {
    //     client.println("HTTP/1.1 200 OK");
    //     client.println("Content-type:text/html");
    //     client.println();                        // header 끝
    //     client.println("<h1>red light on</h1>"); // 본문 내용

    //     pinMode(R_LED, OUTPUT);

    //     digitalWrite(R_LED, LOW);
    // }
}
