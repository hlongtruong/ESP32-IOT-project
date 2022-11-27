#include <WiFi.h>
#include <WebServer.h>
#include <AsyncTCP.h>
#include <Webpage.h>

// #include <ESPAsyncWebServer.h>


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

WebServer server(80);//server on 80

const char* ssid = "myLapTopWifi";
const char* password = "asdfghjkl";

//for obtaining the parameter's value
const char* PARAM_INPUT_1 = "input1";
const char* PARAM_INPUT_2 = "input2";
const char* PARAM_INPUT_3 = "input3";
const char* PARAM_INPUT_4 = "input4";
const char* PARAM_INPUT_5 = "input5";
const char* PARAM_INPUT_6 = "input6";


String inputValue1 = "128";
String inputValue2 = "128";
String inputValue3 = "128";
String inputValue4 = "128";
String inputValue5 = "128";
String inputValue6 = "128";

//for live update
char xml[100]; 

char buff[32];

void ledcAnalogWrite(uint8_t channel, uint32_t value, uint32_t valueMax = 255) {
  // calculate duty, 4095 from 2 ^ 12 - 1
  uint32_t duty = (4095 / valueMax) * min(value, valueMax);

  // write duty to LEDC
  ledcWrite(channel, duty);
} 


//all web application
void notFound() {
  server.send(404, "text/plain","404 Not Found!");
  
}

////http://xxx.xxx.xxx.xxx
void handleMainPage(){
  server.send(200, "text/html", index_html);
}

//http://xxx.xxx.xxx.xxx/handleMotor?motor0=65
//http://xxx.xxx.xxx.xxx/handleMotor?motor1=65
void handleMotor(){
  //slider on webpage
  if(server.hasArg("motor0")) {
    String temp = server.arg("motor0");
    inputValue1 = temp.equals("")||temp.toInt()<50 || temp.toInt() > 245 ? inputValue1 : temp;
  
    server.send(200, "text/plain", inputValue1); 
    
  } else if(server.hasArg("motor1")){
      String temp = server.arg("motor1");
      inputValue2 = temp.equals("")||temp.toInt()<50 || temp.toInt() > 250 ? inputValue2 : temp;
    
      server.send(200, "text/plain", inputValue2); 
    
  } else if(server.hasArg("motor2")){
      String temp = server.arg("motor2");
      inputValue3 = temp.equals("")||temp.toInt()<50 || temp.toInt() > 255 ? inputValue3 : temp;
      
      server.send(200, "text/plain", inputValue3); 
    
  } else if(server.hasArg("motor3")){
      String temp = server.arg("motor3");
      inputValue4 = temp.equals("")||temp.toInt()<50 || temp.toInt() > 210 ? inputValue4 : temp;
      
      server.send(200, "text/plain", inputValue4); 
    
  } else if(server.hasArg("motor4")){
      String temp = server.arg("motor4");
      inputValue5 = temp.equals("")||temp.toInt()<50 || temp.toInt() > 255 ? inputValue5 : temp;
      
      server.send(200, "text/plain", inputValue5); 
   
  } else if(server.hasArg("motor5")){
      String temp = server.arg("motor5");
      inputValue6 = temp.equals("")||temp.toInt()<55 || temp.toInt() > 135 ? inputValue6 : temp;
      
      server.send(200, "text/plain", inputValue6); 
    
  }
  
}

//http://xxx.xxx.xxx.xxx/page_result?input1=yy&input2=yy....
void handleAllMotor() {
  //form on webpages: need to add more limits 
  inputValue1 = server.arg(PARAM_INPUT_1).equals("") ? inputValue1 : server.arg(PARAM_INPUT_1);
  inputValue2 = server.arg(PARAM_INPUT_2).equals("") ? inputValue2 : server.arg(PARAM_INPUT_2);
  inputValue3 = server.arg(PARAM_INPUT_3).equals("") ? inputValue3 : server.arg(PARAM_INPUT_3);
  inputValue4 = server.arg(PARAM_INPUT_4).equals("") ? inputValue4 : server.arg(PARAM_INPUT_4);
  inputValue5 = server.arg(PARAM_INPUT_5).equals("") ? inputValue5 : server.arg(PARAM_INPUT_5);
  inputValue6 = server.arg(PARAM_INPUT_6).equals("") ? inputValue6 : server.arg(PARAM_INPUT_6);
  
  server.sendHeader("Location", "/",true);   //Redirect to our html web page  
  server.send(302, "text/plain", ""); 
}

//http://xxx.xxx.xxx.xxx/add?input1_plus=whatever number you want, this api does not read the argument's value, just to detect the existence of the argument
void handleButtonPlus() {
  if(server.hasArg("input1_plus")) {
    inputValue1 = inputValue1.equals("245")? inputValue1 : String(inputValue1.toInt()+1);
    server.send(200, "text/plain", inputValue1);
    
  } else if(server.hasArg("input2_plus")){
    inputValue2 = inputValue2.equals("250")? inputValue2 : String(inputValue2.toInt()+1);
     server.send(200, "text/plain", inputValue2);
    
  } else if(server.hasArg("input3_plus")){
    inputValue3 = inputValue3.equals("255")? inputValue3 : String(inputValue3.toInt()+1);
    server.send(200, "text/plain", inputValue3);
    
  } else if(server.hasArg("input4_plus")){
    inputValue4 = inputValue4.equals("210")? inputValue4 : String(inputValue4.toInt()+1);
     server.send(200, "text/plain", inputValue4);
    
  } else if(server.hasArg("input5_plus")){
    inputValue5 = inputValue3.equals("255")? inputValue5 : String(inputValue5.toInt()+1);
     server.send(200, "text/plain", inputValue5);
   
  } else if(server.hasArg("input6_plus")){
    inputValue6 = inputValue6.equals("135")? inputValue6 : String(inputValue6.toInt()+1);
    server.send(200, "text/plain", inputValue6);
  }
}

//http://xxx.xxx.xxx.xxx/minus?input1_plus=whatever number you want, same as handleButtonPlus
void handleButtonMinus() {    
  if(server.hasArg("input1_minus")) {
    inputValue1 = inputValue1.equals("50")? inputValue1 : String(inputValue1.toInt()-1);
    server.send(200, "text/plain", inputValue1);
    
  } else if(server.hasArg("input2_minus")){
    inputValue2 = inputValue2.equals("50")? inputValue2 : String(inputValue2.toInt()-1);
    server.send(200, "text/plain", inputValue2);
   
  } else if(server.hasArg("input3_minus")){
    inputValue3 = inputValue3.equals("50")? inputValue3 : String(inputValue3.toInt()-1);
    server.send(200, "text/plain", inputValue3);
    
  } else if(server.hasArg("input4_minus")){
    inputValue4 = inputValue4.equals("50")? inputValue4 : String(inputValue4.toInt()-1);
     server.send(200, "text/plain", inputValue4);
    
  } else if(server.hasArg("input5_minus")){
    inputValue5 = inputValue3.equals("50")? inputValue5 : String(inputValue5.toInt()-1);
     server.send(200, "text/plain", inputValue5);
   
  } else if(server.hasArg("input6_minus")){
    inputValue6 = inputValue6.equals("55")? inputValue6 : String(inputValue6.toInt()-1);
    server.send(200, "text/plain", inputValue6); 
  }

}


void setup() {
  // put your setup code here, to run once:
  pinMode(19, INPUT); //pushbutton, need putdown resistor
  
  //pwm SETUP
  ledcSetup(CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcSetup(CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcSetup(CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcSetup(CHANNEL_3, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcSetup(CHANNEL_4, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  ledcSetup(CHANNEL_5, LEDC_BASE_FREQ, LEDC_TIMER_12_BIT);
  
  ledcAttachPin(MOTOR0, CHANNEL_0);
  ledcAttachPin(MOTOR1, CHANNEL_1);
  ledcAttachPin(MOTOR2, CHANNEL_2);
  ledcAttachPin(MOTOR3, CHANNEL_3);
  ledcAttachPin(MOTOR4, CHANNEL_4);
  ledcAttachPin(MOTOR5, CHANNEL_5);

  //in case of rebooting
  disableCore1WDT();
  Serial.begin(115200);
  //server setup
  //both wifi mode: you can access ESP32 server from its inner ip address if you connect to softAP wifi(ESP32 act as wifi)
  //Meanwhile, you can also access ESP32 server from your local network
  //AP_STA: ESP32 has both connection methods
  WiFi.mode(WIFI_AP_STA);
  WiFi.begin(ssid, password);
  WiFi.softAP("test","asdfghjkl");
  
  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Failed!");
    //return;
  }

  
  Serial.println();
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());

  server.on("/", handleMainPage);
  server.on("/page_result", handleAllMotor);
  server.on("/handle_motor", handleMotor);
  server.on("/add", handleButtonPlus);
  server.on("/minus",handleButtonMinus);

  server.onNotFound(notFound);
  server.begin();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  

  uint32_t duty0 = (uint32_t)inputValue1.toInt();
  ledcAnalogWrite(CHANNEL_0, duty0);
  
  uint32_t duty1 = (uint32_t)inputValue2.toInt();
  ledcAnalogWrite(CHANNEL_1, duty1);
  
  uint32_t duty2 = (uint32_t)inputValue3.toInt();
  ledcAnalogWrite(CHANNEL_2, duty2);
  
  uint32_t duty3 = (uint32_t)inputValue4.toInt();
  ledcAnalogWrite(CHANNEL_3, duty3);
  
  uint32_t duty4 = (uint32_t)inputValue5.toInt();
  ledcAnalogWrite(CHANNEL_4, duty4);
  
  uint32_t duty5 = (uint32_t)inputValue6.toInt();
  ledcAnalogWrite(CHANNEL_5, duty5);


//  if(digitalRead(19)== HIGH) {
//    Serial.print("MOTOR0: ");
//    Serial.println(inputValue1);
//    Serial.print("MOTOR1: ");
//    Serial.println(inputValue2);
//    Serial.print("MOTOR2: ");
//    Serial.println(inputValue3);
//    Serial.print("MOTOR3: ");
//    Serial.println(inputValue4);
//    Serial.print("MOTOR4: ");
//    Serial.println(inputValue5);
//    Serial.print("MOTOR5: ");
//    Serial.println(inputValue6);
//    Serial.println();
//
//    delay(500);
//  }

  server.handleClient();

}






//Obselete
//void handleMotor0() {
//  String temp = server.arg("motor0");
//  inputValue1 = temp.equals("") ? inputValue1 : temp;
//  
////  uint32_t duty0 = (uint32_t)inputValue1.toInt();
////  ledcAnalogWrite(CHANNEL_0, duty0);
//  
//  server.send(200, "text/plain", inputValue1); 
//
//  
//}
//
//void handleMotor1() {
//  String temp = server.arg("motor1");
//  inputValue2 = temp.equals("") ? inputValue2 : temp;
//  
////  uint32_t duty1 = (uint32_t)inputValue2.toInt();
////  ledcAnalogWrite(CHANNEL_1, duty1);
//
//  server.send(200, "text/plain", inputValue2); 
//}  
//
//void handleMotor2() {
//  String temp = server.arg("motor2");
//  inputValue3 = temp.equals("") ? inputValue3 : temp;
//
////  uint32_t duty2 = (uint32_t)inputValue3.toInt();
////  ledcAnalogWrite(CHANNEL_2, duty2);
//  
//  server.send(200, "text/plain", inputValue3); 
//}  
//
//void handleMotor3() {
//  String temp = server.arg("motor3");
//  inputValue4 = temp.equals("") ? inputValue4 : temp;
//  
////  uint32_t duty3 = (uint32_t)inputValue4.toInt();
////  ledcAnalogWrite(CHANNEL_3, duty3);
//  
//  server.send(200, "text/plain", inputValue4); 
//}  
//
//void handleMotor4() {
//  String temp = server.arg("motor4");
//  inputValue5 = temp.equals("") ? inputValue5 : temp;
//  
////  uint32_t duty4 = (uint32_t)inputValue5.toInt();
////  ledcAnalogWrite(CHANNEL_4, duty4);
//  
//  server.send(200, "text/plain", inputValue5); 
//}  
//
//void handleMotor5() {
//  String temp = server.arg("motor5");
//  inputValue6 = temp.equals("") ? inputValue6 : temp;
//
////  uint32_t duty5 = (uint32_t)inputValue6.toInt();
////  ledcAnalogWrite(CHANNEL_5, duty5);
//  
//  server.send(200, "text/plain", inputValue6); 
//} 


//  server.on("/input1", handleMotor0);
//  server.on("/input2", handleMotor1);
//  server.on("/input3", handleMotor2);
//  server.on("/input4", handleMotor3);
//  server.on("/input5", handleMotor4);
//  server.on("/input6", handleMotor5);
