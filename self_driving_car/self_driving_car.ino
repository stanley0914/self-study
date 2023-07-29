#include<Servo.h>
Servo myservo;

const int In1 = 4; //定義In1為4，表示In1接在數位接腳4
const int In2 = 5; //定義In2為5，表示In2接在數位接腳5
const int In3 = 6; //定義In3為6，表示In3接在數位接腳6   
const int In4 = 7; //定義In4為7，表示In4接在數位接腳7

float distance;    //設超音波偵測距離為浮點數distance
int d,r,l;         //d:將浮點數轉為整數;r:右邊距離；l:左邊距離


void setup(){
  
  Serial.begin(9600);     //超音波
  pinMode(10, OUTPUT);    //Trig腳位10
  pinMode(11, INPUT);     //Echo腳位11

  myservo.attach(9);      //伺服馬達訊號線接在腳位9
  
  pinMode(In1, OUTPUT);   //L298N腳位皆設為輸出
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);
  
  myservo.write(0);       //伺服馬達角度歸0
  delay(500);
  myservo.write(80);      //伺服馬達角度轉至前方(微調為80度)
  delay(500);
  
  int i;
  while(1){               //將超音波測試十次(因為前幾次都不準)
  readdistance();
  int i=i+1;
  Serial.print(i);
  if(i>10){
    break;
  }
  }
  Serial.print("start");
}


void loop(){
  readdistance();         //偵測距離
  
  if(d>=40){              //判斷距離是否小於40cm，若無就直走
    mfront();
  }
  else{                   //若小於40cm，就停下
    mstop();
    Serial.print("stop");
    dturn();              //偵測左、右的距離
    if(l>r){              //若左大於右，就左轉
      mleft();
      delay(470);
    }
    else{                 //若右大於左，就右轉
      mright();
      delay(530);
    }
    mstop();
    myservo.write(80);
    delay(1000);
  }
}

void mstop()//煞車程式
{
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW); //In1、In2、In3與In4接腳都輸入LOW電壓，車子將停止在原地。
}
void mfront()//前進程式
{
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH); //In2與In4輸入HIGH電壓，In1與In3接腳輸入LOW電壓。
}
void mright()//右轉程式
{
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW); //In2與In3與In4輸入LOW電壓，In1接腳輸入HIGH電壓。
}
void mleft()//左轉程式
{
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, HIGH);
  digitalWrite(In4, LOW); //In1與In2與In4輸入LOW電壓，In3接腳輸入HIGH電壓。
}
void readdistance()//超音波程式
{
  digitalWrite(10, HIGH);
  delayMicroseconds(10);  
  digitalWrite(10, LOW);
  float distance=(pulseIn(11, HIGH)*0.017);
  //Serial.println(pulseIn(11, HIGH)*0.017);
  //Serial.print(distance);
  //Serial.print("\n");
  d=round(distance);
  Serial.print(d);
  Serial.print("\n");
  delay(300);
}
void dturn()//轉彎判斷程式(測左右距離)
{
  myservo.write(0);
  delay(500);
  readdistance();
  r=d;
  myservo.write(180);
  delay(500);
  readdistance();
  l=d;
}
