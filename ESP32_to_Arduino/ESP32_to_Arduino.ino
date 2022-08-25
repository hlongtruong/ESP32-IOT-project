#define INPUT0 A5
#define INPUT1 A4
#define INPUT2 A3
#define INPUT3 A2
#define INPUT4 A1
#define INPUT5 A0

#define MOTOR0 2
#define MOTOR1 3
#define MOTOR2 4
#define MOTOR3 5
#define MOTOR4 6
#define MOTOR5 7


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  pinMode(MOTOR0,OUTPUT);
  pinMode(MOTOR1,OUTPUT);
  pinMode(MOTOR2,OUTPUT);
  pinMode(MOTOR3,OUTPUT);
  pinMode(MOTOR4,OUTPUT);  
  pinMode(MOTOR5,OUTPUT);
  
  

}

void loop() {
  //since ESP32 only output 3.3V, when putting to Analog input of Arduino, the maximum value value is 675
  //mapping it to 8bit, 255, pwm output of arduino 
  
  uint16_t temp0 = map(analogRead(INPUT0),0,675, 0, 255);
  uint16_t temp1 = map(analogRead(INPUT1),0,675, 0, 255);
  uint16_t temp2 = map(analogRead(INPUT2),0,675, 0, 255);
  uint16_t temp3 = map(analogRead(INPUT3),0,675, 0, 255);
  uint16_t temp4 = map(analogRead(INPUT4),0,675, 0, 255);
  uint16_t temp5 = map(analogRead(INPUT5),0,675, 0, 255);
  
  //analogRead(A5)
  analogWrite(MOTOR0,temp0);
  analogWrite(MOTOR1,temp1);
  analogWrite(MOTOR2,temp2);
  analogWrite(MOTOR3,temp3);
  analogWrite(MOTOR4,temp4);
  analogWrite(MOTOR5,temp5);
  //Serial.println(temp);

}
