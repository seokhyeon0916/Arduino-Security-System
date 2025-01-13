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
