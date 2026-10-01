#include <stdio.h>
#include <string.h>

// Khai bao cau truc SmartDevice
struct SmartDevice {
    int deviceID;
    char deviceName[50];
    int status;           // 1: Bat, 0: Tat
    float ratedPowerKW;   // Cong suat dinh muc (kW)
};

// Khai bao cau truc EnergySensor
struct EnergySensor {
    float voltageVolts;       // Dien ap (V)
    float currentAmperes;     // Dong dien (A)
    int unoccupancyMinutes;   // Thoi gian vang nguoi (phut)
};

int main() {
    struct SmartDevice dev;
    struct EnergySensor sen;
    float operatingHours;

    // 1. Nhap du lieu thiet bi va cam bien
    dev.deviceID = 201;
    strcpy(dev.deviceName, "Dieu Hoa Trung Tam");
    dev.status = 1;             // Mac dinh dang Bât
    dev.ratedPowerKW = 2.5f;     // Cong suat 2.5 kW

    sen.voltageVolts = 220.0f;
    sen.currentAmperes = 35.0f;  // Gia su dong dien 35A (Vuot nguong 30A)
    sen.unoccupancyMinutes = 20; // Gia su vang nguoi 20 phut

    operatingHours = 150.0f;     // So gio hoat dong trong thang

    // 2. Kiem tra Lỗi thường gặp dữ liệu 1: Thong so cam bien bat thuong
    if (sen.voltageVolts < 0.0f || sen.currentAmperes < 0.0f || sen.unoccupancyMinutes < 0) {
        printf("[LOI PHAN CUNG] Thong so cam bien khong hop le (chuoi gia tri am)!\n");
        return 1;
    }

    printf("=== HE THONG QUAN LY CUM THIET BI SMART HOME ===\n");
    printf("Thiet bi: %s (ID: %d)\n", dev.deviceName, dev.deviceID);

    // 3. Quy tac 1: Kiem soat An toan Dien (Qua tai dong dien > 30A)
    if (sen.currentAmperes > 30.0f) {
        dev.status = 0;
        printf("[CANH BAO NGUY HIEM] Dong dien phat hien: %.1fA (>30A)!\n", sen.currentAmperes);
        printf("-> Kich hoat NGAT DIEN KHAN CAP de phong chong chay no!\n");
    } 
    // 4. Quy tac 2: Tu dong hoa Tiet kiem Nang luong (Vang nguoi >= 15 phut)
    else if (dev.status == 1 && sen.unoccupancyMinutes >= 15) {
        dev.status = 0;
        printf("[TIET KIEM DIEN] Phong trong %d phut (>=15 phut).\n", sen.unoccupancyMinutes);
        printf("-> Tu dong TAT thiet bi de tiet kiem nang luong!\n");
    }

    printf("Trang thai thiet bi hien tai: %s\n", dev.status ? "BAT" : "TAT");
    printf("------------------------------------------------\n");

    // 5. Kiem tra Lỗi thường gặp dữ liệu 2: Thoi gian van hanh khong hop le
    if (operatingHours <= 0.0f || operatingHours > 744.0f) {
        printf("[LOI DU LIEU] So gio van hanh trong thang (%.1fh) khong hop le (1 - 744h)!\n", operatingHours);
        printf("-> Tu choi tinh toan hoa don dien EVN.\n");
        return 1;
    }

    // 6. Tinh toan Dien nang tieu thu va Hoa don EVN
    float totalKWh = dev.ratedPowerKW * operatingHours;
    float totalCost = 0.0f;
    float tempKWh = totalKWh;

    // Bac 1: 0 - 50 kWh (1.806 VNĐ/kWh)
    if (tempKWh > 50.0f) {
        totalCost += 50.0f * 1806.0f;
        tempKWh -= 50.0f;
    } else {
        totalCost += tempKWh * 1806.0f;
        tempKWh = 0.0f;
    }

    // Bac 2: 51 - 100 kWh (1.866 VNĐ/kWh)
    if (tempKWh > 0.0f) {
        if (tempKWh > 50.0f) {
            totalCost += 50.0f * 1866.0f;
            tempKWh -= 50.0f;
        } else {
            totalCost += tempKWh * 1866.0f;
            tempKWh = 0.0f;
        }
    }

    // Bac 3: 101 - 200 kWh (2.167 VNĐ/kWh)
    if (tempKWh > 0.0f) {
        if (tempKWh > 100.0f) {
            totalCost += 100.0f * 2167.0f;
            tempKWh -= 100.0f;
        } else {
            totalCost += tempKWh * 2167.0f;
            tempKWh = 0.0f;
        }
    }

    // Bac 4: Tu 201 kWh tro len (2.729 VNĐ/kWh)
    if (tempKWh > 0.0f) {
        totalCost += tempKWh * 2729.0f;
    }

    // In ket qua tinh toan
    printf("So gio hoat dong: %.1f gio\n", operatingHours);
    printf("Tong dien nang tieu thu: %.2f kWh\n", totalKWh);
    printf("Tong tien dien EVN (chua VAT): %.0f VNĐ\n", totalCost);
    printf("================================================\n");

    return 0;
}