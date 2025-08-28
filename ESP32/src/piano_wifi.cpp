
#include <Arduino.h>
#include <AsyncTCP.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include <AsyncJson.h>

#include "task.h"
#include "api.h"

#define R_LED 2

static const char *TAG = "MAIN";

const char *ssid = WIFI_SSID;
const char *password = WIFI_PASS;

static AsyncWebServer server(80);

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

    /*server.on("/playsong", HTTP_POST, handlePlaySong);
    server.on("/setsong", HTTP_POST, handleSetSong);
    server.on("/setalarmtime", HTTP_POST, handleSetAlarmTimeSong);
    server.begin();*/

    AsyncCallbackJsonWebHandler *handlerToPlaySong = new AsyncCallbackJsonWebHandler("/playsong",
                                                                                     [](AsyncWebServerRequest *request, JsonVariant &json)
                                                                                     {
                                                                                         JsonObject obj = json.as<JsonObject>();

                                                                                         AsyncJsonResponse *response = new AsyncJsonResponse();
                                                                                         JsonObject root = response->getRoot();
                                                                                         if (obj.containsKey("notes"))
                                                                                         {

                                                                                             currentSong = obj["notes"].as<String>();
                                                                                             Serial.printf("받은 값 : %s\n", currentSong);

                                                                                             playSong(currentSong);

                                                                                             root["code"] = 200;
                                                                                             root["data"] = "";
                                                                                             root["msg"] = "jkit 노래 재생 성공";
                                                                                         }
                                                                                         else
                                                                                         {
                                                                                             root["code"] = 400;
                                                                                             root["data"] = "";
                                                                                             root["msg"] = "jkit 노래 재생 실패";
                                                                                         }
                                                                                         response->setLength();

                                                                                         request->send(response);
                                                                                     });

    AsyncCallbackJsonWebHandler *handlerToSetSong = new AsyncCallbackJsonWebHandler("/setsong",
                                                                                    [](AsyncWebServerRequest *request, JsonVariant &json)
                                                                                    {
                                                                                        JsonObject obj = json.as<JsonObject>();

                                                                                        AsyncJsonResponse *response = new AsyncJsonResponse();
                                                                                        JsonObject root = response->getRoot();
                                                                                        if (obj.containsKey("notes"))
                                                                                        {
                                                                                            currentSong = obj["notes"].as<String>();

                                                                                            Serial.printf("받은 값 : %s\n", currentSong);

                                                                                            root["code"] = 200;
                                                                                            root["data"] = "";
                                                                                            root["msg"] = "jkit 알람 노래 설정 성공";
                                                                                        }
                                                                                        else
                                                                                        {
                                                                                            root["code"] = 400;
                                                                                            root["data"] = "";
                                                                                            root["msg"] = "jkit 알람 노래 설정 실패";
                                                                                        }
                                                                                        response->setLength();

                                                                                        request->send(response);
                                                                                    });

    AsyncCallbackJsonWebHandler *handlerToSetAlarmTime = new AsyncCallbackJsonWebHandler("/setalarmtime",
                                                                                         [](AsyncWebServerRequest *request, JsonVariant &json)
                                                                                         {
                                                                                             JsonObject obj = json.as<JsonObject>();

                                                                                             AsyncJsonResponse *response = new AsyncJsonResponse();
                                                                                             JsonObject root = response->getRoot();

                                                                                             if (obj.containsKey("hour") && obj.containsKey("min"))
                                                                                             {

                                                                                                 int hour = obj["hour"];
                                                                                                 int min = obj["min"];

                                                                                                 Serial.printf("받은 값 : 알람 시간 %d시 %d분 \n", hour, min);

                                                                                                 setAlarmTime(hour, min);

                                                                                                 root["code"] = 200;
                                                                                                 root["data"] = "";
                                                                                                 root["msg"] = "jkit 알람 시간 설정 성공";
                                                                                             }
                                                                                             else
                                                                                             {
                                                                                                 root["code"] = 400;
                                                                                                 root["data"] = "";
                                                                                                 root["msg"] = "jkit 알람 시간 설정 성공";
                                                                                             }
                                                                                             response->setLength();

                                                                                             request->send(response);
                                                                                         });

    AsyncCallbackJsonWebHandler *handlerToSetCurrentTime = new AsyncCallbackJsonWebHandler("/setcurrenttime",
                                                                                           [](AsyncWebServerRequest *request, JsonVariant &json)
                                                                                           {
                                                                                               JsonObject obj = json.as<JsonObject>();
                                                                                               AsyncJsonResponse *response = new AsyncJsonResponse();
                                                                                               JsonObject root = response->getRoot();

                                                                                               if (obj.containsKey("hour") && obj.containsKey("min"))
                                                                                               {
                                                                                                   int hour = obj["hour"];
                                                                                                   int min = obj["min"];

                                                                                                   Serial.printf("받은 값 : 현재 시간 %d시 %d분 \n", hour, min);

                                                                                                   setCurrentTime(hour, min);

                                                                                                   root["code"] = 200;
                                                                                                   root["data"] = "";
                                                                                                   root["msg"] = "jkit 현재 시간 설정 성공";
                                                                                               }
                                                                                               else
                                                                                               {
                                                                                                   root["code"] = 200;
                                                                                                   root["data"] = "";
                                                                                                   root["msg"] = "jkit 현재 시간 설정 실패";
                                                                                               }

                                                                                               response->setLength();

                                                                                               request->send(response);
                                                                                           });

    server.addHandler(handlerToPlaySong);
    server.addHandler(handlerToSetSong);
    server.addHandler(handlerToSetAlarmTime);
    server.addHandler(handlerToSetCurrentTime);

    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
    DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "Content-Type");

    // OPTIONS 요청 처리
    server.onNotFound([](AsyncWebServerRequest *request)
                      {
                          if (request->method() == HTTP_OPTIONS)
                          {
                              request->send(200); // 프리플라이트 요청에 OK 응답
                          }
                          else
                          {
                              request->send(404); // 나머지는 기존대로 404
                          } });

    server.begin();
}
