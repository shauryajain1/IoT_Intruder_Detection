#include <LiquidCrystal_I2C.h>

// Initialize LCD with I2C address 0x27 and 16x2 dimensions
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Ultrasonic Sensor Pins
const int trigPin = 8;
const int echoPin = 9;

// Buzzer Pin
const int buzzerPin = 10;

// Variables
long duration;
int distance;
bool isAuthorized = false;

// Authorized RFID Tag
String authorizedCard = "010046AAF21F"; // Replace with your RFID card ID

void setup() {
  // Initialize hardware serial communication
  Serial.begin(9600);

  // Initialize LCD
  lcd.init();
  lcd.backlight(); // Turn on backlight
  lcd.setCursor(0, 0); // Start at the first column, first row
  lcd.print("System Ready");

  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);

  // Initial state
  digitalWrite(buzzerPin, LOW);
  Serial.println("System Initialized. Waiting for RFID...");
}

void loop() {
  // Check RFID using hardware serial
  String readCard = "";
  while (Serial.available()) {
    char c = Serial.read();
    readCard += c; // Read the incoming data
  }

  // If an RFID tag is detected
  if (readCard.length() > 0) {
    Serial.println("Card Read: " + readCard);
    lcd.clear(); // Clear the LCD display
    
    // Verify RFID
    if (readCard.indexOf(authorizedCard) != -1) {
      Serial.println("Access Granted.");
      lcd.setCursor(0, 0);
      lcd.print("Access Granted"); // Print on LCD
      isAuthorized = true;
      delay(2000); // Display message for 2 seconds
    } else {
      Serial.println("Access Denied.");
      lcd.setCursor(0, 0);
      lcd.print("Access Denied"); // Print on LCD
      delay(2000); // Display message for 2 seconds
    }
    lcd.clear(); // Clear the LCD after displaying message
  }

  // Ultrasonic Sensor for Motion Detection
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Convert to cm

  // If motion detected and user is not authorized
  if (distance > 0 && distance < 100 && !isAuthorized) {
    Serial.println("Intruder Detected! Distance: " + String(distance) + "cm");
    lcd.setCursor(0, 0);
    lcd.print("Intruder Alert!"); // Print alert on LCD
    digitalWrite(buzzerPin, HIGH); // Turn on buzzer
    delay(1000);                   // Alarm for 1 second
    digitalWrite(buzzerPin, LOW);  // Turn off buzzer
    lcd.clear();                   // Clear LCD after alert
  } else {
    digitalWrite(buzzerPin, LOW);
  }

  delay(100); // Delay to avoid excessive reads
}
