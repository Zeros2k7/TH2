#include <Arduino.h>
#define LED_PIN 2
#define SLEEP_S 10
RTC_DATA_ATTR int soLanThuc = 0; 
int demThuong = 0;
void lamDeepSleep() {
  soLanThuc++; 
  demThuong++; 
  Serial.printf("Truoc khi ngu: soLanThuc(RTC)=%d  demThuong(RAM)=%d\n",
                soLanThuc, demThuong);
  Serial.printf(">soLanThuc:%d\n>demThuong:%d\n", soLanThuc, demThuong);
  Serial.printf("Ngu %d giay...\n", SLEEP_S);
  Serial.flush();
  digitalWrite(LED_PIN, LOW); 
  esp_sleep_enable_timer_wakeup((uint64_t)SLEEP_S * 1000000ULL);
  esp_deep_sleep_start();
}
void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  delay(500);
  Serial.println("\n=== Vua khoi dong / thuc day (chay lai tu setup) ===");
  Serial.printf("soLanThuc(RTC)=%d  demThuong(RAM)=%d\n", soLanThuc, demThuong);

  digitalWrite(LED_PIN, HIGH); 
  delay(3000);
  lamDeepSleep(); 
}
void loop() {}
