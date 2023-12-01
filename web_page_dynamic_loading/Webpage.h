#ifndef WEBPAGE
#define WEBPAGE

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
          Motor0: <input type="range" value="128" min="0" max="245" id="m1" name ="motor0" oninput="updateSlider(this.value,this.name,'motor0_PWM', 'motor0_PWM2')"/>
          <span id="motor0_PWM"></span><br>
          
          Motor1: <input type="range" value="128" min="0" max="1023" id="m2" name ="motor1" oninput="updateSlider(this.value,this.name,'motor1_PWM', 'motor1_PWM2')"/>
          <span id="motor1_PWM"></span><br>
          
          Motor2: <input type="range"  value="128" min="0" max="1023" id="m3" name ="motor2" oninput="updateSlider(this.value,this.name,'motor2_PWM', 'motor2_PWM2')"/>
          <span id="motor2_PWM"></span><br>
          
          Motor3: <input type="range" value="128" min="0" max="1023" id="m4" name ="motor3" oninput="updateSlider(this.value,this.name,'motor3_PWM', 'motor3_PWM2')" />
          <span id="motor3_PWM"></span><br>
          
          Motor4: <input type="range" value="128" min="0" max="1023" id="m5" name ="motor4" oninput="updateSlider(this.value,this.name,'motor4_PWM', 'motor4_PWM2')" />
          <span id="motor4_PWM"></span><br>
          
          Motor5: <input type="range" value="128" min="0" max="1023" id="m6" name ="motor5" oninput="updateSlider(this.value,this.name,'motor5_PWM', 'motor5_PWM2')" />
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

   #endif
