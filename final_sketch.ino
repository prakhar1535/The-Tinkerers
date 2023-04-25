#include <Wire.h>
#include "max6675.h"
#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

SoftwareSerial SIM900A(3,4);

// FOR MPU // 
const int MPU = 0x68; // MPU6050 I2C address
float AccY;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY, gyroAngleZ;
float elapsedTime, currentTime, previousTime;
int flag = 1;
float first_X;
float first_Y ;
float first_Z;
float VelY = 0;
float currAccel;
float prevAccel = 0;
bool accident;

// FOR THERMOCOUPLE // 
int thermoDO = 7; 
int thermoCS = 5;
int thermoCLK = 8;
float temp ;
MAX6675 thermocouple(thermoCLK, thermoCS, thermoDO);

//FOR MQ3 //
#define MQ3 0
float initial = 0 ;
float sensorValue; 

// LED
int rla = 10;
int rlh = 11;
int b1 = 12;
int b2 = 13;




void setup() {

  // ===== MPU INITIALIZATIONS ===== // 
  Serial.begin(19200);
  Wire.begin();                      // Initialize comunication
  Wire.beginTransmission(MPU);       // Start communication with MPU6050 // MPU=0x68
  Wire.write(0x6B);                  // Talk to the register 6B
  Wire.write(0x00);                  // Make reset - place a 0 into the 6B register
  Wire.endTransmission(true);        //end the transmission
  delay(20);
  
  accident = false;
  

 // ======== MQ3 INITIALIZation ======
 initial = analogRead(MQ3);

 //GSM
  SIM900A.begin(9600);   // Setting the baud rate of GSM Module  
  //Serial.begin(9600);    // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println ("SIM900A Ready");
  delay(100);
 
  // LED
  pinMode(rla, OUTPUT);
  pinMode(rlh, OUTPUT);
  pinMode(b1, OUTPUT);
  pinMode(b2, INPUT);
}

 void Accident()
{
 
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+919643483623\"\r"); //Mobile phone number to send message
  delay(1000);
  //Serial.println ("Set SMS Content");
  SIM900A.println("Accident Detected!!!");// Messsage content
  delay(100);
  
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  
}

void Heat()
{
 
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+919643483623\"\r"); //Mobile phone number to send message
  delay(1000);
  //Serial.println ("Set SMS Content");
  SIM900A.println("Engine Overheat!!!");// Messsage content
  delay(100);
  
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  
}

void Daru()
{
 
  SIM900A.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(1000);
  //Serial.println ("Set SMS Number");
  SIM900A.println("AT+CMGS=\"+919643483623\"\r"); //Mobile phone number to send message
  delay(1000);
  //Serial.println ("Set SMS Content");
  SIM900A.println("Driver is Drunk!");// Messsage content
  delay(100);
  
  SIM900A.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  
}




void loop() {
  
  // === Read acceleromter data === //
  digitalWrite(rla, LOW);
  digitalWrite(rlh, LOW);
  digitalWrite(b1, HIGH);
  int counter;
  //int button = digitalRead(b2);
  //Serial.println(button);
  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // Start with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 6 registers total, each axis value is stored in 2 registers
  AccY = (Wire.read() << 8 | Wire.read()) / 16384.0; // Y-axis value
  previousTime = currentTime;        // Previous time is stored before the actual time read
  currentTime = millis();            // Current time actual time read
  elapsedTime = (currentTime - previousTime) / 1000; // Divide by 1000 to get seconds
  Wire.beginTransmission(MPU);
  Wire.write(0x43); // Gyro data first register address 0x43
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // Read 4 registers total, each axis value is stored in 2 registers
  GyroX = (Wire.read() << 8 | Wire.read()) / 131.0; // For a 250deg/s range we have to divide first the raw value by 131.0, according to the datasheet
  GyroY = (Wire.read() << 8 | Wire.read()) / 131.0;
  GyroZ = (Wire.read() << 8 | Wire.read()) / 131.0;
  // Correct the outputs with the calculated error values
  GyroX = GyroX + 0.56; // GyroErrorX ~(-0.56)
  GyroY = GyroY - 2; // GyroErrorY ~(2)
  GyroZ = GyroZ + 0.79; // GyroErrorZ ~ (-0.8)
  
  
// BUILDUNG THE ACCIDENT LOGIC // 
     if (flag == 1){
     first_X = GyroX;
     first_Y = GyroY;
     first_Z = GyroZ;
     } flag ++;

  if (GyroX - first_X > 50.0){
    accident = true;
  } if (GyroY - first_Y > 50.0){
    accident = true;
    } if (GyroZ - first_Z> 50.0){
      accident = true;
  }
  if (accident == true){
    Serial.println("Accident");
    digitalWrite(rla, HIGH);
    Accident();
    // for(int i = 0; i <= 10 ; i++){
    //   delay(1000);
    //   counter ++;
     // if(button == 1){
        //Serial.println("false alarm");
       // break;
    } else{
      digitalWrite(rla, LOW);

   // digitalWrite(acc, HIGH);
  } 
 accident = false;

// THERMOCOUPLE // 
  temp = thermocouple.readCelsius(); 
  //delay(2500);
  if(temp > 50){
    Serial.print("TEMPERATURE IS ");
    Serial.println(temp);
    digitalWrite(rlh, HIGH);
    Heat();
    //digitalWrite(heat, HIGH);
  }
// MQ3 //
  sensorValue = analogRead(MQ3);
    if (sensorValue - initial > 50) {
    Serial.println("DRIVER IS DRUNK");
    Daru();
   // digitalWrite(daru, HIGH);
  }

}
 // ADD SOS BUTTON, FALSE ALERT, LED, 
