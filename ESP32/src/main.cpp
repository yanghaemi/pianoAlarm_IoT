#include <WiFi.h>
#include <string>

const char *ssid = "";
const char *password = "";

const uint8_t R_LED = 2;
const uint8_t B_LED = 4;
const uint8_t G_LED = 17;

const int ledChannel = 0;
const int freq = 5000;
const int resolution = 8;

WiFiServer server(80);

void setup()
{
  Serial.begin(115200); // serial 통신 시작
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

  // -----------LED 세팅 ----------------------
}

void loop()
{
  WiFiClient client = server.available();

  if (!client)
  {
    return;
  }

  Serial.println("Client connected");
  String request = client.readStringUntil('\r');
  Serial.println(request);

  if (request.indexOf("/light/red") >= 0)
  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();                        // header 끝
    client.println("<h1>red light on</h1>"); // 본문 내용

    pinMode(R_LED, OUTPUT);

    digitalWrite(R_LED, HIGH);
    digitalWrite(B_LED, LOW);
    digitalWrite(G_LED, LOW);
  }

  else if (request.indexOf("/light/blue") >= 0)
  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();                         // header 끝
    client.println("<h1>blue light on</h1>"); // 본문 내용

    pinMode(B_LED, OUTPUT);

    digitalWrite(B_LED, HIGH);
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, LOW);
  }

  else if (request.indexOf("/light/green") >= 0)
  {
    client.println("HTTP/1.1 200 OK");
    client.println("Content-type:text/html");
    client.println();                          // header 끝
    client.println("<h1>green light on</h1>"); // 본문 내용

    pinMode(G_LED, OUTPUT);

    digitalWrite(G_LED, HIGH);
    digitalWrite(B_LED, LOW);
    digitalWrite(R_LED, LOW);
  }

  else if (request.indexOf("/light/pwm") >= 0)
  {

    // configure LED PWM functionalitites
    ledcSetup(ledChannel, freq, resolution);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(R_LED, ledChannel);
    ledcAttachPin(B_LED, ledChannel);
    ledcAttachPin(G_LED, ledChannel);

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