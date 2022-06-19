#include <DHT.h>               // Khai báo thư viện 
#include<Servo.h> 
#include <LiquidCrystal.h>          // lcd
#include <TimerOne.h>               // timer
DHT dht(A0, DHT11);
LiquidCrystal lcd(31, 30, 53, 52, 50, 48); // rs, e, d4 ,d5 ,d6 d7
Servo SV1;
Servo SV2;        // khai báo tên các servo
Servo SV3;
Servo SV4;
int S1 = 90;
int S2 = 90;
int S3 = 90;
int S4 = 90;
int mo = 0;
int dong = 0;
int CCL, CCN = 0;
#define INA 3
#define INB 2
#define LED_NGU 11
#define LED_BEP 13
#define LED_KHACH 16
#define LED_SAN 17
#define FAN_NGU 19
#define FAN_KHACH 8
#define COI 22
#define BEP 23
#define LED_HL 9
#define NUTNGU_LED digitalRead(A6)
#define NUTNGU_CUA digitalRead(A8)          // Cấu hình tên cho các đầu vào- đầu ra
#define NUTNGU_FAN digitalRead(A10)
#define NUTBEP digitalRead(A12)
#define NUTBEP_LED digitalRead(42)
#define NUTKHACH_LED digitalRead(38)
#define NUTKHACH_CUA digitalRead(A14)
#define NUTKHACH_FAN digitalRead(36)
#define NUT_CUANHA digitalRead(40)
#define NUT_CUACUON digitalRead(34)
#define NUT_CUASAN digitalRead(32)
#define NUT_TAT digitalRead(44)
#define LDR analogRead(A5)         // quang trở chống trộm
#define LM35 analogRead(A1)        // đo nhiệt độ báo cháy
#define P1 analogRead(A3)          // cb chuyển động sân
#define P2 analogRead(A4)          //................hành lang

int TDCSN;      // biến trạng thái điều khiển cửa sổ ph ngủ
int TDFN;
int TDLN;
int TDB;        // bếp
int TDLB;
int TDCSK;
int TDFK;
int TDLK;
int TDCCK;
int TDLS;
int TDCS;
int TDCC;
int TDC;
int TDLHL;
int chay = 0;
int trom = 0;
unsigned long tg1;
unsigned long tg2;
unsigned long tgup;
unsigned long tg3 = 10000;
unsigned long tg4 = 10000;
unsigned long tgls = 5000;
unsigned long tglhl = 5000;
unsigned long tgcc = 400;
int S[15];
String U = "";
void setup() {
  Serial.begin(57600);       // khởi tạo cho cổng serial hoạt động  -  57600: baudrate  tốc độ truyền dữ liệu qua serial 
  lcd.begin(16, 2);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(13, OUTPUT);                     // cấu hình đầu ra
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(19, OUTPUT);
  pinMode(21, OUTPUT);
  pinMode(22, OUTPUT);
  pinMode(23, OUTPUT);
  digitalWrite(INA, 0);
  digitalWrite(INB, 1); delay(500);
  digitalWrite(INA, 0);
  digitalWrite(INB, 0);
  pinMode(32, INPUT_PULLUP);
  pinMode(34, INPUT_PULLUP);
  pinMode(36, INPUT_PULLUP);
  pinMode(38, INPUT_PULLUP);
  pinMode(40, INPUT_PULLUP);    // các nút nhấn 
  pinMode(42, INPUT_PULLUP);
  pinMode(44, INPUT_PULLUP);
  pinMode(46, INPUT_PULLUP);
  pinMode(A1, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT_PULLUP);
  pinMode(A8, INPUT_PULLUP);
  pinMode(A10, INPUT_PULLUP);
  pinMode(A12, INPUT_PULLUP);
  pinMode(A14, INPUT_PULLUP);
  SV1.attach(4);   // Cửa vào sân
  SV2.attach(5);   // Cửa phòng khách
  SV3.attach(6);   // Cửa sổ phòng khách
  SV4.attach(7);   // Cửa sổ phòng ngủ
  Timer1.initialize(100000);                //  hàm timer để đọc nút nhấn
  Timer1.attachInterrupt(NUT);
  digitalWrite(LED_HL, 1);
  lcd.setCursor(3, 0); lcd.print("DoAm:");
  lcd.setCursor(0, 1); lcd.print("NhietDo:");       
  lcd.setCursor(11, 0);
  lcd.print("%");
  lcd.setCursor(11, 1);
  lcd.print("*C");

}
void loop() {
  DIEUKHIEN();
  BAOCHAY();
  CHONGTROM();
  CUACUON();
    LCD();
  UP();
}
/////////////////////////Code đóng gói và gửi trạng thái sang NodeMCU///////////////////////
void UP() {                      
  S[0] = TDCSN;         // Cửa sổ phòng ngủ
  S[1] = TDFN;          // Quạt phòng ngủ
  S[2] = TDLN;          // Đèn phòng ngủ
  S[3] = TDB;           // Bếp
  S[4] = TDLB;         // Đèn bếp
  S[5] = TDCSK;          // Cửa sổ ph khách
  S[6] = TDFK;          // Quạt phòng khách
  S[7] = TDLK;         // Đèn phòng khách
  S[8] = TDCCK;          // Cửa nhà
  S[9] = TDLS;          // Đèn sân
  S[10] = TDCS;         // Cửa sân
  S[11] = TDCC;         // Cửa cuốn
  S[12] = TDLHL;         // đèn hl
  S[13] = chay;         // báo cháy
  S[14] = trom;         // chống trộm
  U = "";
  U += String(S[0]);
  U += String(S[1]);
  U += String(S[2]);
  U += String(S[3]);
  U += String(S[4]);
  U += String(S[5]);
  U += String(S[6]);
  U += String(S[7]);
  U += String(S[8]);
  U += String(S[9]);
  U += String(S[10]);
  U += String(S[11]);
  U += String(S[12]);
  U += String(S[13]);
  U += String(S[14]);
 if(millis() - tgup >2000){
  Serial.println(U);
  tgup = millis();
 }
}
///////////////////
////////////////////////Code đo nhiệt độ, độ ẩm và hiển thị lên LCD//////////////////
void LCD() {
 int h = dht.readHumidity();          // đọc độ ẩm từ cb dht11
  lcd.setCursor(8, 0);
  if (h < 10) {
    lcd.print(" ");
  }
  if (h < 100) {
    lcd.print(" ");
  }
  lcd.print(h);
   int t = dht.readTemperature();      // đọc nhiệt độ từ dht11
  lcd.setCursor(8, 1);
  if (t< 10) {
    lcd.print(" ");
  }
  if (t < 100) {
    lcd.print(" ");
  }
  lcd.print(t);
   
}
//////////////////////////////////////////////////////////////////////////////////
void NUT() {
  if (NUTNGU_CUA == 0 ) {
    if (TDCSN == 0) {
      Servo4(0, 5);
      TDCSN = 1;
    }
    else {
      Servo4(90, 5);
      TDCSN = 0;
    }
    while (NUTNGU_CUA == 0) {}    //   trong khi vẫn nhấn nút thì k làm gf cả
    delay(20);
  }
  if (NUTNGU_FAN == 0 ) {
    if (TDFN == 0) {
      digitalWrite(FAN_NGU, 1);
      TDFN = 1;
    }
    else {
      digitalWrite(FAN_NGU, 0);
      TDFN = 0;
    }
    while (NUTNGU_FAN == 0) {}
    delay(20);
  }
  if (NUTNGU_LED == 0 ) {
    if (TDLN == 0) {
      digitalWrite(LED_NGU, 1);
      TDLN = 1;
    }
    else {
      digitalWrite(LED_NGU, 0);
      TDLN = 0;
    }
    while (NUTNGU_LED == 0) {}
    delay(20);
  }
  if (NUTBEP == 0 ) {
    if (TDB == 0) {
      digitalWrite(BEP, 1);
      TDB = 1;
    }
    else {
      digitalWrite(BEP, 0);
      TDB = 0;
    }
    while (NUTBEP == 0) {}
    delay(20);
  }
  if (NUTBEP_LED == 0 ) {
    if (TDLB == 0) {
      digitalWrite(LED_BEP, 1);
      TDLB = 1;
    }
    else {
      digitalWrite(LED_BEP, 0);
      TDLB = 0;
    }
    while (NUTBEP_LED == 0) {}
    delay(20);
  }
  if (NUTKHACH_CUA == 0 ) {
    if (TDCSK == 0) {
      Servo3(0, 5);
      TDCSK = 1;
    }
    else {
      Servo3(90, 5);
      TDCSK = 0;
    }
    while (NUTKHACH_CUA == 0) {}
    delay(20);
  }
  if (NUTKHACH_FAN == 0 ) {
    if (TDFK == 0) {
      digitalWrite(FAN_KHACH, 1);
      TDFK = 1;
    }
    else {
      digitalWrite(FAN_KHACH, 0);
      TDFK = 0;
    }
    while (NUTKHACH_FAN == 0) {}
    delay(20);
  }
  if (NUTKHACH_LED == 0 ) {
    if (TDLK == 0) {
      digitalWrite(LED_KHACH, 1);
      TDLK = 1;
    }
    else {
      digitalWrite(LED_KHACH, 0);
      TDLK = 0;
    }
    while (NUTKHACH_LED == 0) {}
    delay(20);
  }
  if (NUT_CUANHA == 0 ) {
    if (TDCCK == 0) {
      Servo2(180, 5);
      TDCCK = 1;
    }
    else {
      Servo2(90, 5);
      TDCCK = 0;
    }
    while (NUT_CUANHA == 0) {}
    delay(20);
  }
  if (NUT_CUASAN == 0 ) {
    if (TDCS == 0) {
      Servo1(0, 5);
      TDCS = 1;
    }
    else {
      Servo1(90, 5);
      TDCS = 0;
    }
    while (NUT_CUASAN == 0) {}
    delay(20);
  }
  if (NUT_CUACUON == 0 ) {
    if (TDCC == 0) {
      TDCC = 1;
    }
    else {
      TDCC = 0;
    }
    while (NUT_CUACUON == 0) {}
    delay(20);
  }
  if (NUT_TAT == 0 ) {
    if (TDC == 0) {
      TDC = 1;
    }
    while (NUT_TAT == 0) {}
    delay(20);
  }
}
void DIEUKHIEN() {
  if (Serial.available()) {   // nếu có tín hiệu từ cổng serial thì...
    String N = "";             // khai báo 1 cái chuỗi để lưu chuỗi nhận đc
    N = Serial.readStringUntil('\r');      // lưu chuỗi nhận đc vào chuỗi N
    N.trim();
    if (N == "CSN" ) {
      if (TDCSN == 0) {        // cs phòng ngủ
        Servo4(0, 5);           // mở cửa
        TDCSN = 1;
      }
      else {
        Servo4(93, 5);      // đóng cửa
        TDCSN = 0;
      }
      //   Serial.flush();
    }
    if (N == "FN" ) {
      if (TDFN == 0) {                   // quạt phòng ngủ
        digitalWrite(FAN_NGU, 1);
        TDFN = 1;
      }
      else {
        digitalWrite(FAN_NGU, 0);
        TDFN = 0;
      }
      //   Serial.flush();
    }
    if (N == "LN" ) {
      if (TDLN == 0) {
        digitalWrite(LED_NGU, 1);     // đèn ph ngủ
        TDLN = 1;
      }
      else {
        digitalWrite(LED_NGU, 0);
        TDLN = 0;
      }
      //   Serial.flush();
    }
    if (N == "B" ) {
      if (TDB == 0) {
        digitalWrite(BEP, 1);
        TDB = 1;                  // bếp
      }
      else {
        digitalWrite(BEP, 0);
        TDB = 0;
      }
      //   Serial.flush();
    }
    if (N == "CH" ) {
      if (TDC == 0) {
        TDC = 1;           //   tắt còi báo động
      }
    }
    if (N == "CSK" ) {
      if (TDCSK == 0) {
        Servo3(0, 5);        // cửa sổ phòng khách
        TDCSK = 1;
      }
      else {
        Servo3(93, 5);
        TDCSK = 0;
      }
      //   Serial.flush();
    }
    if (N == "FK" ) {
      if (TDFK == 0) {
        digitalWrite(FAN_KHACH, 1);
        TDFK = 1;
      }
      else {                                   // quạt ph khách
        digitalWrite(FAN_KHACH, 0);
        TDFK = 0;
      }
      //   Serial.flush();
    }
    if (N == "LK" ) {
      if (TDLK == 0) {
        digitalWrite(LED_KHACH, 1);
        TDLK = 1;
      }                              // đèn ph khách
      else {
        digitalWrite(LED_KHACH, 0);
        TDLK = 0;
      }
      //   Serial.flush();
    }
    if (N == "CCK" ) {
      if (TDCCK == 0) {
        Servo2(180, 5);
        TDCCK = 1;                // cửa nhà
      }
      else {
        Servo2(95, 5);
        TDCCK = 0;
      }
      //   Serial.flush();
    }
    if (N == "CS" ) {
      if (TDCS == 0) {
        Servo1(0, 5);
        TDCS = 1;
      }                    // của vào sân
      else {
        Servo1(93, 5);
        TDCS = 0;
      }
      //   Serial.flush();
    }
    if (N == "CC" ) {
      if (TDCC == 0) {
        TDCC = 1;
      }                                 // cửa cuốn
      else {
        TDCC = 0;
      }
      //   Serial.flush();
    }
    if (N == "LB" ) {
      if (TDLB == 0) {
        digitalWrite(LED_BEP, 1);
        TDLB = 1;
      }
      else {
        digitalWrite(LED_BEP, 0);
        TDLB = 0;
      }
    }
  }
  //////////////////////////Code Bật tắt đèn sân và hành lang tự động/////////////////////
  if (PIR1() == 1) {
    tg3 = millis();
  }
  if (millis() - tg3 < tgls) {
    digitalWrite(LED_SAN, 1);
    TDLS = 1;
  }
  else {
    digitalWrite(LED_SAN, 0);
    TDLS = 0;
  }
  if (PIR2() == 1) {
    tg4 = millis();
  }
  if (millis() - tg4 < tglhl) {
    digitalWrite(LED_HL, 0);
    TDLHL = 1;
  }
  else {
    digitalWrite(LED_HL, 1);
    TDLHL = 0;
  }
}
////////////////////Code chống trộm/////////////////////////
void CHONGTROM() {
  if (LDR < 800 && trom == 0 && TDCS ==0 ) {
    delay(30);
    if (LDR < 800 && trom == 0 ) {
    digitalWrite(COI, 1);
    trom = 1;
  }
  }
  if (TDC == 1 && trom == 1) {
    digitalWrite(COI, 0);
    TDC = 0;
    trom = 0;
  }
}
////////////////////Code báo cháy/////////////////////////
void BAOCHAY() {
  if (NHIETDO1() > 50  ) {          // ct báo cháy khi to lớn hơn mức cho phép 
    digitalWrite(COI, 1);
    chay = 1;
  }
}
/////////////////////////////////////////////////////////
void CUACUON() {
  CCL = CCN;      // 
  CCN = TDCC;
  if (CCN > CCL) {
    tg1 = millis();
    mo = 1;
    digitalWrite(INA, 1);
    digitalWrite(INB, 0);
  }
  if (CCN < CCL) {
    tg2 = millis();
    dong = 1;
    digitalWrite(INA, 0);
    digitalWrite(INB, 1);
  }
  if (millis() - tg1 > tgcc && mo == 1) {
    digitalWrite(INA, 0);                            // delay mở cửa
    digitalWrite(INB, 0);
    mo = 0;
  }
  if (millis() - tg2 > tgcc && dong == 1) {
    digitalWrite(INA, 0);                             // delay đóng cửa
    digitalWrite(INB, 0);
    dong = 0;
  }

}
void Servo4(int goc, int dl) {    // ct điều khiển servo
  if (goc > S4) {
    for (S4; S4 <= goc; S4++) {
      SV4.write(S4);
      delay(dl);
    }
  }
  if (goc < S4) {
    for (S4; S4 >= goc; S4--) {
      SV4.write(S4);
      delay(dl);
    }
  }
}
void Servo3(int goc, int dl) {    // ct điều khiển servo
  if (goc > S3) {
    for (S3; S3 <= goc; S3++) {
      SV3.write(S3);
      delay(dl);
    }
  }
  if (goc < S3) {
    for (S3; S3 >= goc; S3--) {
      SV3.write(S3);
      delay(dl);
    }
  }
}
void Servo2(int goc, int dl) {    // ct điều khiển servo
  if (goc > S1) {
    for (S2; S2 <= goc; S2++) {
      SV2.write(S2);
      delay(dl);
    }
  }
  if (goc < S2) {
    for (S2; S2 >= goc; S2--) {
      SV2.write(S2);
      delay(dl);
    }
  }
}
void Servo1(int goc, int dl) {
  if (goc > S1) {
    for (S1; S1 <= goc; S1++) {
      SV1.write(S1);
      delay(dl);
    }
  }                                     //  ct điều khiển servo 
  if (goc < S1) {
    for (S1; S1 >= goc; S1--) {
      SV1.write(S1);
      delay(dl);
    }
  }
}
int PIR2() {
  if (P2 > 500) {
    return 1;             //  quy đổi mức logic cbcd ở sân
  }
  else {
    return 0;
  }
}
int PIR1() {
  if (P1 > 500) {
    return 1;
  }                       // quy đổi mức logic cbcd hành lang
  else {
    return 0;
  }
}
int NHIETDO1() {
  float b = (LM35 * 5.0) / 1023  * 100;    // đọc nhiệt độ từ cảm biến nhiệt độ lm35
  return b;
}
