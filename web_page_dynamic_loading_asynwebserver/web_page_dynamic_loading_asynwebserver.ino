#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

//channel
#define CHANNEL_0 0
#define CHANNEL_1 1
#define CHANNEL_2 2
#define CHANNEL_3 3
#define CHANNEL_4 4
#define CHANNEL_5 5

//pin
#define MOTOR0 15
#define MOTOR1 2
#define MOTOR2 4
#define MOTOR3 5
#define MOTOR4 22
#define MOTOR5 23

//resolution
#define LEDC_TIMER_12_BIT  12
#define LEDC_BASE_FREQ     5000


AsyncWebServer server(8080);//server on 8080

const char* ssid = "NETGEAR59";
const char* password = "purpletrumpet839";

//for obtaining the parameter's value
const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";
const char* PARAM_INPUT_4 = "input4";
const char* PARAM_INPUT_5 = "input5";
const char* PARAM_INPUT_6 = "input6";


String inputValue1 = "0";
String inputValue2 = "0";
String inputValue3 = "0";
String inputValue4 = "0";
String inputValue5 = "0";
String inputValue6 = "0";


void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
