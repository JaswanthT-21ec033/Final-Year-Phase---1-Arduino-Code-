#include <HX711.h>

// Pin Definitions
const int LOADCELL_DOUT_PIN = 2; // HX711 DOUT pin
const int LOADCELL_SCK_PIN = 3;  // HX711 SCK pin
const int MOTOR_PIN = 9;         // DC Motor control pin
const int LED_GREEN_PIN = 12;    // Green LED pin
const int LED_RED_PIN = 11;      // Red LED pin
const int SWITCH_PIN = 4;        // Switch input pin (with internal pull-up)

HX711 scale;

// Weight threshold in grams
const float weightThreshold = 1000.0;

void setup() {
  Serial.begin(9600);

  // Initialize HX711
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();  // Adjust for calibration factor
  scale.tare();       // Reset scale to 0

  // Pin Modes
  pinMode(MOTOR_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(SWITCH_PIN, INPUT_PULLUP); // Enable internal pull-up resistor

  // Initial States
  digitalWrite(MOTOR_PIN, LOW);      // Motor OFF
  digitalWrite(LED_GREEN_PIN, LOW); // Green LED OFF
  digitalWrite(LED_RED_PIN, HIGH);  // Red LED ON
}

void loop() {
  // Read weight from HX711
  float weight = scale.get_units(10); // Average over 10 readings

  // Read the switch state
  bool switchState = digitalRead(SWITCH_PIN); // LOW = closed, HIGH = open

  // Debugging Info
  Serial.print("Weight: ");
  Serial.print(weight);
  Serial.print(" g | Switch: ");
  Serial.println(switchState ? "Open" : "Closed");

  // Control Logic
  if (weight < weightThreshold && switchState == LOW) {
    // Weight < 1kg AND switch is closed
    digitalWrite(MOTOR_PIN, HIGH);       // Motor ON
    digitalWrite(LED_GREEN_PIN, HIGH);  // Green LED ON
    digitalWrite(LED_RED_PIN, LOW);     // Red LED OFF
    Serial.println("Motor Running, Green LED ON");
  } else {
    // Weight >= 1kg OR switch is open
    digitalWrite(MOTOR_PIN, LOW);       // Motor OFF
    digitalWrite(LED_GREEN_PIN, LOW);  // Green LED OFF
    digitalWrite(LED_RED_PIN, HIGH);   // Red LED ON
    Serial.println("Motor Stopped, Red LED ON");
  }

  delay(500); // Short delay for stability
}
