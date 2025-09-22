#include <LiquidCrystal_I2C.h>

#define VERT A0
#define HORZ A1
#define SEL 2

LiquidCrystal_I2C lcd(0x27, 20, 3);

int top = 0;
int bottom = 0;
int row = 0;

void home_view();
int* set_cursor(int bottom, int top, int row);
void move_cursor(int bottom, int top, int row);
void logging(String text_log, String type_log);

struct TypeLog {
  String INFO = "INFO";
  String ERROR = "ERROR";
  String DEBUG = "DEBUG";
} TypeLog;

unsigned long startTime = 0;

void setup() {
  lcd.begin(20, 3); 
  Serial.begin(9600);

  lcd.setCursor(0, 0);
  lcd.backlight();
  home_view();

  logging("System initialized", "INFO");
}

void loop() {
  int* arr = xyz_cursor(bottom, top, row);
  top = arr[0];
  bottom = arr[1];
  row = arr[2];

  logging("Values: top: " + String(top) + " bottom: " + String(bottom) + " row: " + String(row), "DEBUG");

  move_cursor(bottom, top, row);

  delete [] arr;
  
  delay(1000);
}

void home_view() {
  lcd.clear();
  lcd.setCursor(15, 0);
  lcd.setCursor(0, 0);
  lcd.print(">");
  lcd.setCursor(6, 0);
  lcd.print("SETTINGS");
  lcd.setCursor(1, 1);
  lcd.print("1. Pipes");
  lcd.setCursor(1, 2);
  lcd.print("2. Timer");
}

void move_cursor(int bottom, int top, int row) {
  if (top == 1) {
    lcd.setCursor(0, row + 1);
    lcd.print(" ");
    lcd.setCursor(0, row);
    lcd.print(">");
    logging("Cursor wiil up", "INFO");
  }
  else if (bottom == 1) {
    lcd.setCursor(0, row - 1);
    lcd.print(" ");
    lcd.setCursor(0, row);
    lcd.print(">");
    logging("Cursor wiil down", "INFO");
  }
  else{
    logging("Cusor wiil stay", "INFO");
  }
}

int* xyz_cursor(int bottom, int top, int row) {
  int vertValue = analogRead(VERT);
  int horzValue = analogRead(HORZ);

  if ((vertValue > 512 && vertValue <= 1023) && horzValue == 512 && (row > 0 && row < 3)) {
    bottom = 0;
    top = 1;
    row--;

    String logg = "Cursor upping " + String(row);
    logging(logg, "INFO");
  }
  else if ((vertValue >= 0 && vertValue < 512) && horzValue == 512 && (row > 0 && row < 3)) {
    top = 0;
    bottom = 1;
    row++;

    String logg = "Cursor down " + String(row);
    logging(logg, "INFO");
  } else {
    if ((vertValue >= 0 && vertValue < 512) && horzValue == 512 && row == 0) {
      top = 0;
      bottom = 1;
      row++;
    } else if ((vertValue > 512 && vertValue <= 1023) && horzValue == 512 && row == 3){
      bottom = 0;
      top = 1;
      row--;
    }
  }
  int* arr = new int[3] {top, bottom, row};
  return arr;
}

void logging(String text_log, String type_log) {
  if (type_log == "INFO"){
    Serial.println(TypeLog.INFO + "| " + text_log);
  } else if (type_log == "ERROR") {
    Serial.println(TypeLog.ERROR + "| " + text_log);
  } else if (type_log == "DEBUG") {
    Serial.println(TypeLog.DEBUG + "| " + text_log);
  }
}