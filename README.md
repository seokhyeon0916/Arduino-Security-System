# RFID 기반 보안 시스템 설계 및 구현
</br>

### 프로젝트 설명📝
* NFC 태그 카드를 사용하여 등록된 사용자와 미등록된 사용자를 분류하는 프로젝트
* 등록된 사용자가 카드를 태그 할 경우 LCD화면에 사용자의 정보를 출력하고, 서브모터를 활용하여 잠금이 해제되는 기능을 구현
* 승인된 기기 이외의 연결 시도 시 차단 및 경고음 발생 기능 구현


</br>

### 프로젝트 참여 인원 소개🧑‍🤝‍🧑
#### 팀명: 또봇(ToBot)🤖
* 팀원
> 이경민(팀장)👑</br>
> 운석현(팀원)</br>
> 김찬영(팀원)</br>

* 맡은역할: 아두이노 SW 구성

</br>

### 프로젝트 사용 언어💻
* Arduino
* 사용라이브러리
> MFRC522, SERVO, I2C


</br>

### 프로젝트 구성 회로도 모습💻
![image](https://github.com/user-attachments/assets/24de1f26-7c10-424b-917d-5b7e8a15e132)

</br>

### 기본 라이브러리 설치 및 설정
``` Arduino
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN   9
#define SS_PIN    10
LiquidCrystal_I2C lcd(0x27, 16, 2);

MFRC522 mfrc(SS_PIN, RST_PIN);
Servo servo;

int BLUE = 2;
int RED = 7;  
int GREEN = 4;
int buzzer = 6;
int motor = 8;  // 서보 모터 핀
```

### setup 부분
``` Arduino
void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc.PCD_Init(); 
  lcd.init();
  lcd.begin(16, 2);
  lcd.backlight();
  servo.attach(motor); // 서보 모터 초기화
  servo.write(0);      // 모터 기본 각도 0도로 설정

  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(buzzer, OUTPUT);
}
```

### loop 부분
``` Arduino
void loop() {
  lcd.setCursor(0, 0);
  lcd.print("waiting");
  delay(500);
  lcd.clear();

  if (!mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial()) {
    delay(100);
    return;
  }
  Serial.print("UID: ");
  for (byte i = 0; i < mfrc.uid.size; i++) {
    Serial.print(mfrc.uid.uidByte[i], HEX);
    Serial.print(" ");
  }
  Serial.println();

  if (mfrc.uid.uidByte[0] == 0xB4 && mfrc.uid.uidByte[1] == 0xD7 && 
      mfrc.uid.uidByte[2] == 0x9E && mfrc.uid.uidByte[3] == 0x1E) {
    // 허용된 태그
    digitalWrite(BLUE, HIGH);
    digitalWrite(RED, LOW);
    Serial.println("Hello, Eduino~");
    
    // 부저 및 LCD 메시지
    for (int i = 0; i < 1; i++) {
      tone(buzzer, 500);
      delay(400);
      tone(buzzer, 700);
      delay(400);
      tone(buzzer, 900);
      delay(400);
    }
    noTone(buzzer);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hello seok");

    // 모터 제어 (40도)
    servo.write(90);
    delay(2000);
    servo.write(0);
  } else {
    // 허용되지 않은 태그
    digitalWrite(BLUE, LOW);
    digitalWrite(RED, HIGH);
    Serial.println("Who are you?");
    
    // 부저 및 LCD 메시지
    for (int i = 0; i < 5; i++) {
      tone(buzzer, 1000);  // 높은 음
      delay(200);
      tone(buzzer, 500);   // 낮은 음
      delay(200);
    }
    noTone(buzzer);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Who are you");

    // 모터 제어 (정지, 0도)
    servo.write(0);
  }
}
```

