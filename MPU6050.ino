#include <Wire.h>
#include <MPU6050.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

MPU6050 mpu;

void setup() {
    Serial.begin(115200);
    Wire.begin();

    // Initialize OLED Display
    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 OLED initialization failed"));
        while (1);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("Initializing...");
    display.display();

    // Initialize MPU6050
    Serial.println("Initializing MPU6050...");
    mpu.initialize();
    
    if (!mpu.testConnection()) {
        Serial.println("MPU6050 connection failed!");
        display.setCursor(0, 10);
        display.println("MPU6050 Error!");
        display.display();
        while (1);
    } else {
        Serial.println("MPU6050 connected successfully");
        display.setCursor(0, 10);
        display.println("MPU6050 OK!");
        display.display();
        delay(1000);
    }

    display.clearDisplay();
    display.display();
}

void loop() {
    int16_t ax, ay, az, gx, gy, gz;
    mpu.getAcceleration(&ax, &ay, &az);
    mpu.getRotation(&gx, &gy, &gz);

    // Convert raw values to readable format
    float accX = ax / 16384.0; // Convert to 'g'
    float accY = ay / 16384.0;
    float accZ = az / 16384.0;
    float gyroX = gx / 131.0;  // Convert to deg/sec
    float gyroY = gy / 131.0;
    float gyroZ = gz / 131.0;

    // Display Data on OLED
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    
    display.print("Acc X: "); display.println(accX, 2);
    display.print("Acc Y: "); display.println(accY, 2);
    display.print("Acc Z: "); display.println(accZ, 2);
    
    display.print("Gyro X: "); display.println(gyroX, 2);
    display.print("Gyro Y: "); display.println(gyroY, 2);
    display.print("Gyro Z: "); display.println(gyroZ, 2);

    display.display();

    // Print data to Serial Monitor for debugging
    Serial.print("Acc X: "); Serial.print(accX, 2);
    Serial.print(" Y: "); Serial.print(accY, 2);
    Serial.print(" Z: "); Serial.println(accZ, 2);

    Serial.print("Gyro X: "); Serial.print(gyroX, 2);
    Serial.print(" Y: "); Serial.print(gyroY, 2);
    Serial.print(" Z: "); Serial.println(gyroZ, 2);

    Serial.println("------------------------------");

    delay(500);
}
