#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Servo.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1  
#define TRIG_PIN      9
#define ECHO_PIN      10
#define SERVO_PIN     6

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Servo myServo;

void setup() {
    Serial.begin(9600);
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    myServo.attach(SERVO_PIN);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println("SSD1306 allocation failed");
        for (;;);
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(20, 10);
    display.println("Radar System Ready");
    display.display();
    delay(2000);
}

long getDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    long duration = pulseIn(ECHO_PIN, HIGH);
    if (duration == 0) return -1; // Error handling for timeout

    return duration * 0.034 / 2;  // Convert to cm
}

void loop() {
    for (int angle = 0; angle <= 360; angle += 5) {
        scanAndDisplay(angle);
    }
}

void scanAndDisplay(int angle) {
    myServo.write(angle % 180); // Convert 360° to two full 180° sweeps
    delay(200);
    long distance = getDistance();

    display.clearDisplay();
    
    // Draw full-circle radar base
    display.drawCircle(64, 32, 30, SSD1306_WHITE); // Outer circle
    display.drawCircle(64, 32, 20, SSD1306_WHITE); // Middle circle
    display.drawCircle(64, 32, 10, SSD1306_WHITE); // Inner circle
    display.drawLine(64, 32, 64 + cos(radians(angle)) * 30, 32 - sin(radians(angle)) * 30, SSD1306_WHITE); // Radar scanning line
    
    // Display detected object only if it's in range
    if (distance > 0 && distance < 30) { 
        int x = 64 + cos(radians(angle)) * distance;
        int y = 32 - sin(radians(angle)) * distance;
        display.fillCircle(x, y, 3, SSD1306_WHITE); // Object dot

        // Show angle and distance only when an object is detected
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.print("Angle: ");
        display.print(angle);
        display.print("  Dist: ");
        display.print(distance);
        display.print("cm");
    }

    display.display();

    Serial.print("Angle: ");
    Serial.print(angle);
    Serial.print(" | Distance: ");
    Serial.print(distance);
    Serial.println(" cm");
}
