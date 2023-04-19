// Include Wire library for I2C communication
#include<Wire.h>
// Define constants and variables
#define PCF 0x20
int red1 = 2;
int yellow1 = 3;
int green1 = 4;
int red2 = 5;
int yellow2 = 6;
int green2 = 7;
int red3 = A3 ;
int yellow3 = A4;
int green3 = A5;
int trigger1 = 8;
int echo1 = 9;
int trigger2 = 10;
int echo2 = 11;
int trigger3 = A4;
int echo3 = A5;
int count = 0;
int num[3]={0,0,0};
int grtime1 = 10000;
int grtime2 = 10000;
int grtime3 = 10000;
// Function to manage traffic direction and count vehicles
void direction(int a, int b, int c, int d, int e, int f,int g,int h,int i,int num[3],int grtime )
{
  int count1 = 0,count2 = 0,count3 = 0;
  // Activate green light for direction A, red for directions B and C
  digitalWrite(a, LOW);
  digitalWrite(b, LOW);
  digitalWrite(c, HIGH);
  digitalWrite(d, HIGH);
  digitalWrite(e, LOW);
  digitalWrite(f, LOW);
  digitalWrite(g, HIGH);
  digitalWrite(h, LOW);
  digitalWrite(i, LOW);
  // Wait for green light duration
  delay(grtime);
  // Activate yellow light for direction A
  digitalWrite(c, LOW);
  digitalWrite(b, HIGH);
  // Measure distances for each direction five times
  for(int i=0;i<5;i++){
    // Measure distance for direction 1
    digitalWrite(8,HIGH);
    delayMicroseconds(10);
    digitalWrite(8,LOW);
    float duration1 = pulseIn(9, HIGH);
    float distance1 = 0.017 * duration1;
    Serial.println(distance1);
    // Measure distance for direction 2
    digitalWrite(10,HIGH);
    delayMicroseconds(10);
    digitalWrite(10,LOW);
    float duration2 = pulseIn(11, HIGH);
    float distance2 = 0.017 * duration2;
    Serial.println(distance2);
    // Measure distance for direction 3
    digitalWrite(12,HIGH);
    delayMicroseconds(10);
    digitalWrite(12,LOW);
    float duration3 = pulseIn(13, HIGH);
    float distance3 = 0.017 * duration3;
    Serial.println(distance3);
    // Count vehicles within a distance of 50 units for each direction
    if(distance1<=50){
      count1++;
    }
    if(distance2<=50){
      count2++;
    }
    if(distance3<=50){
      count3++;
    }
    delay(1000);
  }
  // Store vehicle count results for each direction  
  if(count1==5){
    num[0]=1;
  }
  if(count2==5){
    num[1]=1;
  }
  if(count3==5){
    num[2]=1;
  }
}
// Function to set up pins and communication
void setup()
{
  for(int i = 2; i<=7; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(A0,OUTPUT);
  pinMode(A1,OUTPUT);
  pinMode(A2,OUTPUT);
  Serial.begin(9600);
  Wire.begin();
  pinMode(trigger1,OUTPUT);
  pinMode(echo1,INPUT);
  pinMode(trigger2,OUTPUT);
  pinMode(echo2,INPUT);
  pinMode(trigger3,OUTPUT);
  pinMode(echo3,INPUT);
}
// Main loop function
void loop()
{ 
    Serial.println("came here");
    if(count==0){
      // Manage traffic for direction 1
      direction(red1, yellow1, green1, red2, yellow2, green2 , red3 , yellow3 , green3 , num , grtime1);
      // Update green light time for direction 1
      if(grtime1!=10000){
        grtime1=10000;
      }
      count++;
      Serial.println("array");
      // Print vehicle count array
      for(int i=0;i<3;i++){
        Serial.println(num[i]);
      }
      // Update green light times for directions 2 and 3
      if(num[2]==1){
        grtime2 = 5000;
        grtime3 = 15000;
      }
      num[1] = 0;
    }
    else if(count==1){
      Serial.println("lane2");
      // Manage traffic for direction 2
      direction(red2, yellow2, green2, red1, yellow1, green1 , red3 , yellow3 , green3 , num , grtime2 );
      // Update green light time for direction 2
      if(grtime2!=10000){
        grtime2=10000;
      }
      // Update green light times for directions 1 and 3
      if(num[0]==1){
        grtime3 = 5000;
        grtime1 = 15000;
      }
      num[2] = 0;
      count++;
      Serial.println("array");
      // Print vehicle count array
      for(int i=0;i<3;i++){
        Serial.println(num[i]);
      }
    }
    else if(count==2){
      Serial.println("lane3");
      // Manage traffic for direction 3
      direction(red3, yellow3, green3, red1, yellow1, green1 , red2 , yellow2 , green2 , num , grtime3 );
      // Update green light time for direction 3
      if(grtime3!=10000){
        grtime3=10000;
      }
      // Update green light times for directions 1 and 2
      if(num[1]==1){
        grtime1 = 5000;
        grtime2 = 15000;
      }
      num[0] = 0;
      count++;
      Serial.println("array");
      // Print vehicle count array
      for(int i=0;i<3;i++){
        Serial.println(num[i]);
      }
    }
    else{
    // Do nothing if count is not 0, 1, or 2
    }
    // Reset count after completing all directions
    count=count%3;
} 
