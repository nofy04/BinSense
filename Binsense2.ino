#include <Servo.h>

// ======= Pin Definitions =======
#define IR_PIN 5
#define PROXIMITY_PIN 6
#define RAIN_ANALOG A0
#define RAIN_DIGITAL 3
#define SERVO_PIN 7
#define SERVO2_PIN 10   // NEW SERVO (Rain only)

// ======= Config =======
bool INVERT_IR = true;

// ======= Globals =======
Servo myServo;           // Servo for metallic detection
Servo myServo2;          // Servo ONLY for rain
int irValue = 0;
int proximityValue = 0;
int rainAnalogValue = 0;
int rainDigitalValue = 0;

bool systemEnabled = false;   // NEW FLAG

void setup() {
  Serial.begin(9600);

  pinMode(IR_PIN, INPUT);
  pinMode(PROXIMITY_PIN, INPUT);
  pinMode(RAIN_DIGITAL, INPUT);

  myServo.attach(SERVO_PIN);
  myServo2.attach(SERVO2_PIN);

  myServo.write(0);  
  myServo2.write(0);

  Serial.println("✅ System started...");
  delay(500);
}

void loop() {
  // ======= Read Sensors =======
  irValue = digitalRead(IR_PIN);
  proximityValue = digitalRead(PROXIMITY_PIN);
  rainAnalogValue = analogRead(RAIN_ANALOG);
  rainDigitalValue = digitalRead(RAIN_DIGITAL);

  if (INVERT_IR) irValue = !irValue;

  Serial.println("---------------------------------------------");

  // Debug prints
  Serial.print("IR Value: "); Serial.println(irValue);
  Serial.print("Proximity Value: "); Serial.println(proximityValue);
  Serial.print("Rain Digital: "); Serial.println(rainDigitalValue);

  // ======= IR ENABLES SYSTEM =======
  if (irValue == HIGH && systemEnabled == false) {
    systemEnabled = true;
    Serial.println("🔓 System Enabled (IR detected object)");
  }

  // If IR didn't detect anything, do nothing
  if (!systemEnabled) {
    delay(500);
    return;
  }

  // ======= CONTROL LOGIC (only works when systemEnabled == true) =======

  // 1️⃣ RAIN DETECTED → Servo2 works + disable system after completion
  if (rainDigitalValue == LOW) {
    myServo2.write(90);
    Serial.println("☔ Rain detected → Servo2 = 90°");
    delay(10000);

    myServo2.write(0);
    Serial.println("☔ Servo2 returned to 0° after 10 sec");

    systemEnabled = false;     // Disable system until IR detects again
    Serial.println("🔒 System Disabled after action");
    delay(500);
    return;
  }

  // 2️⃣ METAL DETECTED → Servo1 works + disable system after completion
  if (proximityValue == HIGH) {
    myServo.write(90);
    Serial.println("🧲 Metal detected → Servo1 = 90°");
    delay(10000);

    myServo.write(0);
    Serial.println("🧲 Servo1 returned to 0° after 10 sec");

    systemEnabled = false;     // Disable system until IR detects again
    Serial.println("🔒 System Disabled after action");
    delay(500);
    return;
  }

  delay(500);
}
