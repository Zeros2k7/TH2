- Họ tên: Phạm Tuấn Minh
- MSSV 25112081
- Board: ESP32
Bài 1 – delay() vs millis() vs timer phần cứng
**Kết luận.**
- "delay()" cộng thời gian chờ vào thời gian làm việc, nên chu kỳ thực = chu kỳ đặt + thời gian xử lý → sai nhịp và chặn mọi việc khác.
- "millis()" bám nhịp tốt khi loop nhẹ, nhưng chỉ được kiểm tra khi "loop()" quay lại: nếu một vòng bận lâu hơn chu kỳ thì nhịp bị trễ (jitter tăng).
- Timer phần cứng tạo ngắt đúng chu kỳ, độc lập với "loop()" bận hay không → jitter thấp nhất khi CPU bận. ISR phải ngắn gọn ("IRAM_ATTR", không "Serial.print"/"delay").
Bài 2 – Deep sleep định giờ
**Kết luận.**
- Deep sleep dừng CPU hoàn toàn và mất RAM thường; chỉ vùng RTC giữ giá trị.
- Biến cần nhớ qua các lần ngủ phải khai báo RTC_DATA_ATTR.
- Không đo dòng bằng ampere được vì thiếu thiết bị đo dòng cỡ µA–mA và dòng nguồn ngủ rất nhỏ so với dòng trên board (chip USB-UART, LED nguồn) — nên chỉ quan sát gián tiếp qua hành vi.
