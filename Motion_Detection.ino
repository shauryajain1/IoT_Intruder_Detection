#include <SoftwareSerial.h>

// Ultrasonic Sensor Pins
const int trigPin = 8;
const int echoPin = 9;

// Buzzer Pin
const int buzzerPin = 10;

// Variables
long duration;
int distance;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  
  // Initial state
  digitalWrite(buzzerPin, LOW);
  Serial.println("System Initialized. Waiting for RFID...");
}

void loop() {

  // Ultrasonic Sensor for Motion Detection
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2; // Convert to cm
  
  // If motion detected
  if (distance > 0 && distance < 10 ) {
    Serial.println("Intruder Detected! Distance: " + String(distance) + "cm");
    digitalWrite(buzzerPin, HIGH); // Turn on buzzer
    delay(1000);                   // Alarm for 5 seconds
    digitalWrite(buzzerPin, LOW);  // Turn off buzzer
  } else {
    digitalWrite(buzzerPin, LOW);
    Serial.println("Nothing");
    Serial.println(distance);
  }
  
  delay(100); // Delay to avoid excessive reads
}
