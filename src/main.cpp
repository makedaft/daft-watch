#include <Arduino.h>

// Physical Pin 2 on the ATtiny84a is Arduino Digital Pin 0
#define LED_PIN 0

void setup() {
  // Initialize the digital pin as an output.
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  digitalWrite(LED_PIN, HIGH); // Turn the LED on
  delay(1000);                // Wait for a second
  digitalWrite(LED_PIN, LOW);  // Turn the LED off
  delay(1000);                // Wait for a second
}
