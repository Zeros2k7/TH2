#include <Arduino.h>
#define LED1 2
#define ADC  34

void setup() {
  Serial.begin(115200);
  pinMode(LED1, OUTPUT);
}

void loop() {
  digitalWrite(LED1, HIGH); delay(500);
  digitalWrite(LED1, LOW);  delay(500);

  int tho = analogRead(ADC);
  Serial.printf(">bienTro:%d\n", tho);
  delay(2000);
  Serial.println(">heartbeat:1");
  delay(5000);

}
