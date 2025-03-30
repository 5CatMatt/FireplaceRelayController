/*

Fireplace activation controller

Read an inbound PWM signal in order to activate a relay coil. The signal will
toggle between two frequencies at a 50% duty cycle once per second. If the 
pulsed signal is not detected, or is not toggling, the relay should be deactivated.

This is part of the protection method for the fireplace control. The thermostat
functions are handled by python on an SBC. If the script fails or the SBC hangs
this controller will deactivate the relay, turning off the fireplace.

*/

#include <math.h>

#include <Defines.h>

void setup() {
  Serial.begin(115200);
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, relayState); // Initialize relay
  pinMode(inputPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(inputPin), pulseDetected, RISING);
}

void loop() {
  // Check frequency validity with tolerance
  bool newSignalValid = (fabs(currentFrequency - targetFreq1) < tolerance1) || 
                        (fabs(currentFrequency - targetFreq2) < tolerance2);

  bool desiredRelayState = newSignalValid ? HIGH : LOW;

  // Serial.print("signal valid: ");
  // Serial.print(newSignalValid);
  // Serial.print("  desired: ");
  // Serial.print(desiredRelayState);
  // Serial.print("  Relay state: ");
  // Serial.println(relayState);

  if (desiredRelayState != relayState) {
    if (millis() - lastStateChangeTime >= MIN_TOGGLE_INTERVAL) {
      relayState = desiredRelayState;
      digitalWrite(outputPin, relayState);
      lastStateChangeTime = millis();
      
      Serial.print("Relay changed to: ");
      Serial.println(relayState == LOW ? "ON" : "OFF");
    }
  }

  // Debug output
  static unsigned long lastPrintTime = 0;
  if (millis() - lastPrintTime >= 1000) {
    Serial.print("Freq: ");
    Serial.print(currentFrequency);
    Serial.print(" Hz | Valid: ");
    Serial.print(newSignalValid  ? "YES" : "NO");
    Serial.print(" | Relay: ");
    Serial.println(relayState == LOW ? "ON" : "OFF");
    lastPrintTime = millis();
  }
}

void pulseDetected() {
  unsigned long currentTime = micros();
  if (lastPulseTime > 0) {
    currentFrequency = frequencyToggleTime / (currentTime - lastPulseTime);
  }
  lastPulseTime = currentTime;
}