#include <Arduino.h>
#define LED1 2
#define ADC  34
unsigned long lanTruocLed = 0, lanTruocAdc = 0, lanTruocHeart = 0;
bool ledState = false;
void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
}
void loop() {
  unsigned long now = millis();
  if (now - lanTruocLed >= 500) {
    lanTruocLed = now;
    ledState = !ledState;
    digitalWrite(LED1, ledState);
  }
  if (now - lanTruocAdc >= 2000) {
    lanTruocAdc = now;
    Serial.printf(">bienTro:%d\n", analogRead(ADC));
  }
  if (now - lanTruocHeart >= 5000) {
    lanTruocHeart = now;
    Serial.println(">heartbeat:1");
  }
}
