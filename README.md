# **Tài Liệu Báo Cáo: Phân Hệ Quản Lý Cụm Thiết Bị Smart Home Bằng Struct**

## **1. Phân Tích Bài Toán & Thiết Kế Giải Pháp**

### **Phân Tích Input / Output**
* **Dữ liệu đầu vào (Input):**
  * Struct `SmartDevice`: `deviceID` (int), `deviceName` (char[]), `status` (int: 0/1), `ratedPowerKW` (float).
  * Struct `EnergySensor`: `voltageVolts` (float), `currentAmperes` (float), `unoccupancyMinutes` (int).
  * Biến phụ trợ: `operatingHours` (float) - Số giờ hoạt động trong tháng.
* **Dữ liệu đầu ra (Output):**
  * Trạng thái hoạt động cuối cùng của thiết bị (BAT/TAT) và cảnh báo an toàn.
  * Tổng điện năng tiêu thụ ($kWh$).
  * Chi phí tiền điện tính theo biểu giá bậc thang EVN (chưa VAT).

### **Quy Trình Xử Lý Logic**
1. **Kiểm tra tính hợp lệ dữ liệu cảm biến:**
   * Nếu `voltageVolts < 0` hoặc `currentAmperes < 0` hoặc `unoccupancyMinutes < 0` $\rightarrow$ Thông báo lỗi cảm biến phần cứng và dừng chương trình.
2. **Kiểm soát An toàn Điện (Ưu tiên 1):**
   * Nếu `currentAmperes > 30.0A` $\rightarrow$ Tự động ngắt điện khẩn cấp (`status = 0`), bật cảnh báo quá tải/cháy nổ.
3. **Tự động hóa Tiết kiệm Điện (Ưu tiên 2):**
   * Nếu thiết bị đang bật (`status == 1`) và `unoccupancyMinutes >= 15` $\rightarrow$ Tự động tắt thiết bị (`status = 0`).
4. **Kiểm tra thời gian vận hành & Tính toán EVN:**
   * Nếu `operatingHours <= 0` hoặc `operatingHours > 744` $\rightarrow$ Từ chối tính toán hóa đơn.
   * Ngược lại, tính $kWh = \text{ratedPowerKW} \times \text{operatingHours}$ và áp biểu giá 4 bậc thang EVN:
     * Bậc 1 ($0 - 50 kWh$): 1.806 VNĐ/kWh
     * Bậc 2 ($51 - 100 kWh$): 1.866 VNĐ/kWh
     * Bậc 3 ($101 - 200 kWh$): 2.167 VNĐ/kWh
     * Bậc 4 ($> 200 kWh$): 2.729 VNĐ/kWh

---

## **2. Bảng Ma Trận Kiểm Thử (Test Cases)**

| Trường Hợp | Đầu Vào | Xử Lý Logic | Kết Quả Mong Đợi |
|---|---|---|---|
| **Test Case 1**<br>*(Lỗi cảm biến)* | `voltage = -220.0V`, `current = 10.0A` | Cảm biến báo thông số âm | ❌ Báo lỗi phần cứng cảm biến, hủy thao tác. |
| **Test Case 2**<br>*(Ngắt quá tải)* | `current = 35.0A` ($> 30A$), `status = 1` | Dòng vượt ngưỡng an toàn | ⚠️ Ngắt khẩn cấp (`status = 0`), báo nguy cơ cháy nổ. |
| **Test Case 3**<br>*(Tắt tiết kiệm)* | `current = 15.0A`, `unoccupancy = 20m` | Phòng trống $\ge 15$ phút | 🔌 Tự động tắt thiết bị (`status = 0`) để tiết kiệm điện. |
| **Test Case 4**<br>*(Tính tiền EVN)* | `power = 1.5kW`, `hours = 200h`<br>$\rightarrow kWh = 300` | Tính hóa đơn bậc thang EVN | 💰 Bậc 1: 90.300đ<br>💰 Bậc 2: 93.300đ<br>💰 Bậc 3: 216.700đ<br>💰 Bậc 4: 272.900đ<br>**Tổng:** 673.200 VNĐ |

---