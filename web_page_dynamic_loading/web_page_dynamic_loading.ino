#include <WiFi.h>
#include <WebServer.h>
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

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML>
  <html>
    <style>
        .float-left {
          float:inline-start;
          padding-left: 20px;
          padding: 5px;
        }

        .float-right{
          float:inline-end;
          padding-right: 20px;
          padding: 5px;
        }

        .float-container {
          margin-left: 30%;
          margin-right:30%;
          padding: 5px;
        }

        span * {
          height:10px;
          width:5px;
        }
    </style>
        
    <body>
     <center><h1>ESP32 Transfer to Arduino</h1></center> 
     <center><h2>Web Server</h2></center> 
     <center><h3>0: 0%, 255: 100%</h3></center> 
      <center>
      <form action ="/page_result">
        <p style="color:red;font-size:15px;"> Type PWM (not recommanded).</p>
          Motor0: <input type="number" min="50" max="245" id="r1" name ="input1"/><br>
          Motor1: <input type="number" min="50" max="250" id="r2" name ="input2"/><br>
          Motor2: <input type="number" min="50" max="255" id="r3" name ="input3"/><br>
          Motor3: <input type="number" min="50" max="210" id="r4" name ="input4"/><br>
          Motor4: <input type="number" min="50" max="255" id="r5" name ="input5"/><br>
          Motor5: <input type="number" min="55" max="135" id="r6" name ="input6"/><br>

          <input type="submit" value="Submit Total">
      </form>
      
      <div class="float-container">
        <div class="float-left">
          <p style="color:red;font-size:15px;"> Slider PWM (AP wifi mode).</p>
          Motor0: <input type="range" value="128" min="50" max="245" id="m1" name ="motor0" oninput="updateSlider(this.value,this.name,'motor0_PWM', 'motor0_PWM2')"/>
          <span id="motor0_PWM"></span><br>
          
          Motor1: <input type="range" value="128" min="50" max="250" id="m2" name ="motor1" oninput="updateSlider(this.value,this.name,'motor1_PWM', 'motor1_PWM2')"/>
          <span id="motor1_PWM"></span><br>
          
          Motor2: <input type="range"  value="128" min="50" max="255" id="m3" name ="motor2" oninput="updateSlider(this.value,this.name,'motor2_PWM', 'motor2_PWM2')"/>
          <span id="motor2_PWM"></span><br>
          
          Motor3: <input type="range" value="128" min="50" max="210" id="m4" name ="motor3" oninput="updateSlider(this.value,this.name,'motor3_PWM', 'motor3_PWM2')" />
          <span id="motor3_PWM"></span><br>
          
          Motor4: <input type="range" value="128" min="50" max="255" id="m5" name ="motor4" oninput="updateSlider(this.value,this.name,'motor4_PWM', 'motor4_PWM2')" />
          <span id="motor4_PWM"></span><br>
          
          Motor5: <input type="range" value="128" min="55" max="135" id="m6" name ="motor5" oninput="updateSlider(this.value,this.name,'motor5_PWM', 'motor5_PWM2')" />
          <span id="motor5_PWM"></span><br>
          
            <input type="button" value="Reset slider" onclick="resetFunc()">
        </div>
        

        <div class="float-right" >
          <p style="color:red;font-size:15px;"> button PWM (both wifi mode).</p>
          Motor0: <button type="button" name="input1_minus" onclick="updateButton(this.name,'motor0_PWM','motor0_PWM2','m1','minus')">go left </button><button type="button" name="input1_plus" onclick="updateButton(this.name,'motor0_PWM','motor0_PWM2','m1','add')">go right </button>
          <span id="motor0_PWM2"></span><br>
          
          Motor1: <button type="button"  name="input2_minus" onclick="updateButton(this.name,'motor1_PWM','motor1_PWM2','m2','minus')">go left </button><button type="button" name="input2_plus" onclick="updateButton(this.name,'motor1_PWM','motor1_PWM2','m2','add')">go right </button>
          <span id="motor1_PWM2"></span><br>
          
          Motor2: <button type="button" name="input3_minus" onclick="updateButton(this.name,'motor2_PWM','motor2_PWM2','m3','minus')">go left </button><button type="button" name="input3_plus" onclick="updateButton(this.name,'motor2_PWM','motor2_PWM2','m3','add')">go right </button>
          <span id="motor2_PWM2"></span><br>
          
          Motor3: <button type="button" name="input4_minus" onclick="updateButton(this.name,'motor3_PWM','motor3_PWM2','m4','minus')">go left </button><button type="button" name="input4_plus" onclick="updateButton(this.name,'motor3_PWM','motor3_PWM2','m4','add')">go right </button>
          <span id="motor3_PWM2"></span><br>
          
          Motor4: <button type="button" name="input5_minus" onclick="updateButton(this.name,'motor4_PWM','motor4_PWM2','m5','minus')">go left </button><button type="button" name="input5_plus" onclick="updateButton(this.name,'motor4_PWM','motor4_PWM2','m5','add')">go right </button>
          <span id="motor4_PWM2"></span><br>
          
          Motor5: <button type="button" name="input6_minus" onclick="updateButton(this.name,'motor5_PWM','motor5_PWM2','m6','minus')">go left </button><button type="button" name="input6_plus" onclick="updateButton(this.name,'motor5_PWM','motor5_PWM2','m6','add')">go right </button>
          <span id="motor5_PWM2"></span><br>
        </div>
      </div>
    </center>
    </body>

    <script type = "text/javascript">
        let xmlHttp=createXmlHttpObject();
      
        function resetFunc(){
          document.getElementById("m1").value = 128;
          updateSlider(document.getElementById("m1").value,document.getElementById("m1").name,'motor0_PWM','motor0_PWM2');
          
          document.getElementById("m2").value = 128;
         updateSlider(document.getElementById("m2").value,document.getElementById("m2").name,'motor1_PWM','motor1_PWM2');
          
          document.getElementById("m3").value = 128;
          updateSlider(document.getElementById("m3").value,document.getElementById("m3").name,'motor2_PWM','motor2_PWM2');
          
          document.getElementById("m4").value = 128;
          updateSlider(document.getElementById("m4").value,document.getElementById("m4").name,'motor3_PWM','motor3_PWM2');
          
          document.getElementById("m5").value = 128;
          updateSlider(document.getElementById("m5").value,document.getElementById("m5").name,'motor4_PWM','motor4_PWM2');
          
          document.getElementById("m6").value = 128;
          updateSlider(document.getElementById("m6").value,document.getElementById("m6").name,'motor5_PWM','motor5_PWM2');
       }

       function updateSlider(value,name_input, id, id2){
          //let web_address = address + "?"+ name_input + "="+value;
          let web_address = "handle_motor" + "?"+ name_input + "="+value;
          let xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
          // update the web based on reply from  ESP
            //console.log(this.value);
              document.getElementById(id).innerHTML = this.responseText;
              document.getElementById(id2).innerHTML = this.responseText;
            }
          };
          //document.getElementById(id).nextElementSibling.value = document.getElementById(id).value;
          xhttp.open("PUT", web_address, true);
          xhttp.send();
       }

       function updateUniqueSlider(slider_id, value){
          document.getElementById(slider_id).value = value;
       }

       function updateButton(button_name,id, id2,slider_id,preString){
          let xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              //id: motorX_PWM, id2: motorX_PWM2
              document.getElementById(id).innerHTML = this.responseText;
              document.getElementById(id2).innerHTML = this.responseText;
              updateUniqueSlider(slider_id, this.responseText);
              //console.log(document.getElementsByName(name_input).innerHTML);
            }
          };
          //console.log(preString+ "?" + button_name + "=" + 1);

          xhttp.open("PUT", preString+ "?" + button_name + "=" + 1, true);
          xhttp.send();
       }
      </script> 
   </html>)rawliteral";

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

void handleMainPage(){
  //Serial.println("sending web page");
  server.send(200, "text/html", index_html);
}

void handleMotor(){
  if(server.hasArg("motor0")) {
    String temp = server.arg("motor0");
    inputValue1 = temp.equals("") ? inputValue1 : temp;
  
//  uint32_t duty0 = (uint32_t)inputValue1.toInt();
//  ledcAnalogWrite(CHANNEL_0, duty0);
  
    server.send(200, "text/plain", inputValue1); 
    
  } else if(server.hasArg("motor1")){
      String temp = server.arg("motor1");
      inputValue2 = temp.equals("") ? inputValue2 : temp;
      
    //  uint32_t duty1 = (uint32_t)inputValue2.toInt();
    //  ledcAnalogWrite(CHANNEL_1, duty1);
    
      server.send(200, "text/plain", inputValue2); 
    
  } else if(server.hasArg("motor2")){
      String temp = server.arg("motor2");
      inputValue3 = temp.equals("") ? inputValue3 : temp;
    
    //  uint32_t duty2 = (uint32_t)inputValue3.toInt();
    //  ledcAnalogWrite(CHANNEL_2, duty2);
      
      server.send(200, "text/plain", inputValue3); 
    
  } else if(server.hasArg("motor3")){
      String temp = server.arg("motor3");
      inputValue4 = temp.equals("") ? inputValue4 : temp;
      
    //  uint32_t duty3 = (uint32_t)inputValue4.toInt();
    //  ledcAnalogWrite(CHANNEL_3, duty3);
      
      server.send(200, "text/plain", inputValue4); 
    
  } else if(server.hasArg("motor4")){
      String temp = server.arg("motor4");
      inputValue5 = temp.equals("") ? inputValue5 : temp;
      
    //  uint32_t duty4 = (uint32_t)inputValue5.toInt();
    //  ledcAnalogWrite(CHANNEL_4, duty4);
      
      server.send(200, "text/plain", inputValue5); 
   
  } else if(server.hasArg("motor5")){
      String temp = server.arg("motor5");
      inputValue6 = temp.equals("") ? inputValue6 : temp;
    
    //  uint32_t duty5 = (uint32_t)inputValue6.toInt();
    //  ledcAnalogWrite(CHANNEL_5, duty5);
      
      server.send(200, "text/plain", inputValue6); 
    
  }
  
}

//

void handleAllMotor() {
  //here is secured by form input 
  inputValue1 = server.arg(PARAM_INPUT_1).equals("") ? inputValue1 : server.arg(PARAM_INPUT_1);
  inputValue2 = server.arg(PARAM_INPUT_2).equals("") ? inputValue2 : server.arg(PARAM_INPUT_2);
  inputValue3 = server.arg(PARAM_INPUT_3).equals("") ? inputValue3 : server.arg(PARAM_INPUT_3);
  inputValue4 = server.arg(PARAM_INPUT_4).equals("") ? inputValue4 : server.arg(PARAM_INPUT_4);
  inputValue5 = server.arg(PARAM_INPUT_5).equals("") ? inputValue5 : server.arg(PARAM_INPUT_5);
  inputValue6 = server.arg(PARAM_INPUT_6).equals("") ? inputValue6 : server.arg(PARAM_INPUT_6);
  
  server.sendHeader("Location", "/",true);   //Redirect to our html web page  
  server.send(302, "text/plain", ""); 
}

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

  //server setup
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  //WiFi.mode(WIFI_AP);
  WiFi.begin(ssid, password);
  //WiFi.softAP("test","asdfghjkl");
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
