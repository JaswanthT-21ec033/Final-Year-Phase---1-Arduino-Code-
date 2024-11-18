#include <HX711.h>

// Pin Definitions
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
const int MOTOR_PIN = 9;   // Control pin for the transistor or motor driver
const int LED1_PIN = 12;
const int LED2_PIN = 11;

HX711 scale;

// Weight Threshold in grams (1kg = 1000g)
const float weightThreshold = 1000.0;
float currentWeight = 0;

void setup() {
  Serial.begin(9600);
  
  // Initialize the weight sensor
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();  // Set your calibration factor here
  scale.tare();       // Reset the scale to 0

  // Initialize motor control and LED pins
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  // Start motor and LED1 initially
  digitalWrite(MOTOR_PIN, HIGH);   // Motor ON (rotation)
  digitalWrite(LED1_PIN, HIGH);    // LED1 ON (under 1kg)
  digitalWrite(LED2_PIN, LOW);     // LED2 OFF
}

void loop() {
  // Read the weight from the sensor
  currentWeight = scale.get_units(10);

  // Print current weight for debugging
  Serial.print("Current Weight: ");
  Serial.println(currentWeight);

  // If the weight exceeds 1kg, stop the motor
  if (currentWeight >= weightThreshold) {
    digitalWrite(MOTOR_PIN, LOW);   // Stop the motor
    digitalWrite(LED1_PIN, LOW);    // LED1 OFF
    digitalWrite(LED2_PIN, HIGH);   // LED2 ON (overloaded)
  } 
  // If the weight is below 1kg, keep the motor rotating
  else {
    digitalWrite(MOTOR_PIN, HIGH);  // Motor ON
    digitalWrite(LED1_PIN, HIGH);   // LED1 ON (under 1kg)
    digitalWrite(LED2_PIN, LOW);    // LED2 OFF
  }

  delay(500);  // Add a small delay to reduce sensor jitter
}
