#include <Ezo_i2c.h> 
#include <Wire.h>    
#include <sequencer2.h> 
#include <Ezo_i2c_util.h> 

Ezo_board EC = Ezo_board(100, "EC");       // EC 센서의 주소 값 지정한다.

void step1();  
void step2();

Sequencer2 Seq(&step1, 1000, &step2, 0);  // step1, setp2 사이의 딜레이를 주기 위한 sequencer 함수

int EC_led = 12;                          // EC 센서의 달린 LED의 pin 번호를 정의함

void setup() {
  pinMode(EC_led, OUTPUT);                // EC led를 출력으로 사용하기 위한 설정

  Wire.begin();                           // I2C 통신 시작 함수
  Serial.begin(9600);                     // 컴퓨터와 보드 간의 시리얼 통신 시작 함수
  Seq.reset();                            // sequencer 함수 초기화
}

void loop() {
  Seq.run();                              // 함수 반복 시작
}

void step1(){
    EC.send_read_cmd();                   // 센서에 대한 데이터 읽기 모드 설정   
}

void step2(){
  
  char receive_buffer[32];                // 읽어올 데이터 저장소 생성
  EC.receive_cmd(receive_buffer, 32);     // receive_buffer에 값이 있다면 PH라는 변수에 센서 값을 저장

  receive_and_print_response(EC);
  
  if(EC.get_last_received_reading() > 10) {          // EC LED ON/OFF
    digitalWrite(EC_led,HIGH);                      
  }
  else{
    digitalWrite(EC_led,LOW);                       
  }
  Serial.println("  ");
}
