#define BLYNK_TEMPLATE_ID   "TMPL39YAePhXT"
#define BLYNK_TEMPLATE_NAME "IOT BASED IV MONITOR"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include "HX711.h"
#define DOUT  23
#define BUZZER 25
#define CLK   19 
HX711 scale;
#define BLYNK_PRINT Serial
char auth[] = "po-ahucLZLBVnCyQ9SXxojDM_vcBW3-s";
char ssid[] = "wifi_name";
char pass[] = "wifi_password";
int liter;
int val;
float weight; 
float calibration_factor = 102500; 
void setup() {
  Serial.begin(115200);
  scale.begin(DOUT, CLK);
  lcd.init();
  lcd.backlight();
  pinMode(BUZZER ,OUTPUT);
  Serial.println("Remove all weight from scale");
  scale.set_scale();
  scale.tare(); 
  long zero_factor = scale.read_average(); 
  Serial.print("Zero factor: "); 
  Serial.println(zero_factor);
  Blynk.begin(auth, ssid, pass); 
}
 void loop() {
 Blynk.run();
 measureweight();
}
void measureweight(){
 scale.set_scale(calibration_factor); 
  weight = scale.get_units(5); 
    if(weight<0)
  {
    weight=0.00;
    }
  liter = weight*1000;
  val = liter;
  val = map(val, 0, 505, 0, 100);
  lcd.clear();
Serial.print("Kilogram: ");
Serial.print(weight); 
Serial.println(" Kg");
lcd.setCursor(0, 0); 
lcd.print("Volume = ");
lcd.print(liter);
lcd.print(" mL");
Serial.print("IV BOTTLE: ");
Serial.print(liter);
Serial.println(" mL");
lcd.setCursor(0, 1); 
lcd.print("Percent = ");
lcd.print(val);
lcd.print("%");
Serial.print("IV Bag Percent: ");
Serial.print(val);
Serial.println(" %");
Serial.println();
  delay(500);
  if (val <= 50 && val >= 40){
    Blynk.logEvent("iv_alert","IV Bottle is 50%");
    digitalWrite(BUZZER, HIGH);
    delay(50);
    digitalWrite(BUZZER, LOW);
    delay(50);
  }
  else if (val <= 20){
    Blynk.logEvent("iv_alert","IV Bottle is too LOW");
    digitalWrite(BUZZER, HIGH);
  }
  else{
    digitalWrite(BUZZER, LOW);
  }
  Blynk.virtualWrite(V0,liter);
  Blynk.virtualWrite(V1,val);
}
