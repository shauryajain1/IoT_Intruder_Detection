// Ultrasonic Sensor Pins
const int trigPin = 8;
const int echoPin = 9;

// Buzzer Pin
const int buzzerPin = 10;

//Grant Access
const int grant_access = 2000;

// Variables
long duration;
int distance;
bool isAuthorized = false;

// Authorized RFID Tag
String authorizedCard = "010046AAF21F";

void setup() {
  // Initialize hardware serial communication
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
  // Check RFID using hardware serial
  String readCard = "";
  while (Serial.available()) {
    char c = Serial.read();
    readCard += c; // Read the incoming data
  }

  // If an RFID tag is detected
  if (readCard.length() > 0) {
    Serial.println("Card Read: " + readCard);
    
    // Verify RFID
    if (readCard.indexOf(authorizedCard) != -1) {
      Serial.println("Access Granted.");
      isAuthorized = true;
      delay(grant_access); 
    } else {
      Serial.println("Access Denied.");
    }
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
  if (distance > 0 && distance < 10 && !isAuthorized) {
    Serial.println("Intruder Detected! Distance: " + String(distance) + "cm");
    digitalWrite(buzzerPin, HIGH); // Turn on buzzer
    delay(1000);                   // Alarm for 1 second
    digitalWrite(buzzerPin, LOW);  // Turn off buzzer
  } else {
    digitalWrite(buzzerPin, LOW);
  }
  
  delay(100); // Delay to avoid excessive reads
  isAuthorized = false; // Making Authorization False
}
