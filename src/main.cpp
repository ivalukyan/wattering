#include <RTClib.h>
#include <LiquidCrystal_I2C.h>

#define VERT A0
#define HORZ A1
#define SEL 2

LiquidCrystal_I2C lcd(0x27, 20, 2);
RTC_DS3231 rtc;

void home_view();
void set_cursor(int bottom, int top, int row);
void logging(String text_log, String type_log);

struct TypeLog {
  String INFO = "INFO";
  String ERROR = "ERROR";
  String DEBUG = "DEBUG";
} TypeLog;

unsigned long startTime = 0;

void setup() {
  lcd.begin(20, 2); 
  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.backlight();
  home_view();

  logging("System initialized", "INFO");
}

void loop() {
  bool top = 0; // Положение джойстика
  bool bottom = 0; // Положение джойстика
  int row = 0; // max = 4; min = 0

  set_cursor(bottom, top, row);

  if (top) {
    for (int i = 0; i < 4; i++){
      lcd.setCursor(0, i);
      lcd.print(" ");
    }
    lcd.setCursor(0, 1);
    lcd.print(">");
  }
  else if (bottom) {
    for (int i = 0; i < 4; i++){
      lcd.setCursor(0, i);
      lcd.print(" ");
    }
    lcd.setCursor(0, 2);
    lcd.print(">");
  }

  delay(1000);
}

void home_view() {
  lcd.clear();
  lcd.setCursor(15, 0);
  DateTime now = rtc.now();
  String timestamp = String(now.hour()) + ":" + String(now.minute());
  lcd.print(timestamp);
  lcd.setCursor(0, 0);
  lcd.print(">");
  lcd.setCursor(6, 0);
  lcd.print("SETTINGS");
  lcd.setCursor(1, 1);
  lcd.print("1. Pipes");
  lcd.setCursor(1, 2);
  lcd.print("2. Timer");
}

void set_cursor(int bottom, int top, int row) {
  int vertValue = analogRead(VERT);
  int horzValue = analogRead(HORZ);

  if (vertValue == 1023 && horzValue == 512 && (row >= 0 && row <= 3)) {
    bottom = 0;
    top = 1;
    row--;

    String logg = "Cursor upping";
    logging(logg, "INFO");
  }
  else if (vertValue == 0 && horzValue == 512 && (row >= 0 && row <= 3)) {
    top = 0;
    bottom = 1;
    row++;

    String logg = "Cursor down";
    logging(logg, "INFO");
  }
  delay(1000);
}

void logging(String text_log, String type_log) {
  DateTime now = rtc.now();
  String timestamp = String(now.day()) + "/" + 
                     String(now.month()) + "/" + 
                     String(now.year());

  if (type_log == "INFO"){
    Serial.println(timestamp + "\t|\t" + TypeLog.INFO + "\t|\t" + text_log);
  } else if (type_log == "ERROR") {
    Serial.println(timestamp + "\t|\t" + TypeLog.ERROR + "\t|\t" + text_log);
  } else if (type_log == "DEBUG") {
    Serial.println(timestamp + "\t|\t" + TypeLog.DEBUG + "\t|\t" + text_log);
  }
}