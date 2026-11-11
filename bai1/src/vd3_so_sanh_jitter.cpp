#include <Arduino.h>
#define PERIOD_MS   100
#define N_TICK      50
#define BUSY_MIN_MS 5
#define BUSY_MAX_MS 60
struct ThongKe {
  uint32_t last = 0;
  uint32_t n = 0;
  uint64_t tongJitter = 0;
  uint32_t maxJitter = 0;
  void ghiNhan(uint32_t nowUs) {
    if (last != 0) {
      uint32_t kc = nowUs - last;
      uint32_t lech = kc > PERIOD_MS * 1000UL ? kc - PERIOD_MS * 1000UL
                                              : PERIOD_MS * 1000UL - kc;
      tongJitter += lech;
      if (lech > maxJitter) maxJitter = lech;
      n++;
    }
    last = nowUs;
  }
  void in(const char* ten) {
    Serial.printf("[%s] khoang=%u  jitter_TB=%llu us  jitter_max=%u us\n",
                  ten, n, n ? tongJitter / n : 0ULL, maxJitter);
    Serial.printf(">jitterTB_%s:%llu\n", ten, n ? tongJitter / n : 0ULL);
    Serial.printf(">jitterMax_%s:%u\n", ten, maxJitter);
  }
};

ThongKe tkDelay, tkMillis;
volatile uint32_t hwLast = 0, hwN = 0, hwMax = 0;
volatile uint64_t hwTong = 0;

static void busyWork() {
  delay(random(BUSY_MIN_MS, BUSY_MAX_MS + 1));   // gia lap viec nang trong loop
}
void chayDelay() {
  for (int i = 0; i < N_TICK; i++) {
    busyWork();
    delay(PERIOD_MS);
    tkDelay.ghiNhan(micros());
  }
  tkDelay.in("delay");
}
void chayMillis() {
  uint32_t loopTick = millis();
  uint32_t dem = 0;
  while (dem < N_TICK) {
    busyWork(); 
    if (millis() - loopTick < PERIOD_MS) continue;
    loopTick += PERIOD_MS;
    if (millis() - loopTick >= PERIOD_MS) loopTick = millis();  // tre qua -> bat nhip
    tkMillis.ghiNhan(micros());
    dem++;
  }
  tkMillis.in("millis");
}
hw_timer_t* hwTimer = nullptr;
void IRAM_ATTR onTimer() {
  uint32_t now = micros();
  if (hwLast != 0) {
    uint32_t kc = now - hwLast;
    uint32_t lech = kc > PERIOD_MS * 1000UL ? kc - PERIOD_MS * 1000UL
                                            : PERIOD_MS * 1000UL - kc;
    hwTong += lech;
    if (lech > hwMax) hwMax = lech;
    hwN++;
  }
  hwLast = now;
}

void chayHwTimer() {
  hwTimer = timerBegin(0, 80, true);             // 80MHz/80 -> 1 tick = 1us
  timerAttachInterrupt(hwTimer, &onTimer, true);
  timerAlarmWrite(hwTimer, PERIOD_MS * 1000UL, true);
  timerAlarmEnable(hwTimer);
  while (hwN < N_TICK - 1) busyWork();           // loop van "ban" nhu cu
  timerAlarmDisable(hwTimer);
  timerEnd(hwTimer);
  Serial.printf("[hw_timer] khoang=%u  jitter_TB=%llu us  jitter_max=%u us\n",
                hwN, hwN ? hwTong / hwN : 0ULL, hwMax);
  Serial.printf(">jitterTB_hw:%llu\n", hwN ? hwTong / hwN : 0ULL);
  Serial.printf(">jitterMax_hw:%u\n", hwMax);
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.printf("Chu ky dat %d ms, %d nhip/cach, loop ban %d..%d ms\n",
                PERIOD_MS, N_TICK, BUSY_MIN_MS, BUSY_MAX_MS);
  chayDelay();
  chayMillis();
  chayHwTimer();
  Serial.println("Xong. Nhan RESET de chay lai.");
}

void loop() {}
