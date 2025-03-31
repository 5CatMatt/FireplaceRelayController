/*

Fireplace activation controller

Read an inbound PWM signal in order to activate a relay coil. The signal will
toggle between two frequencies at a 50% duty cycle once per second. If the 
pulsed signal is not detected, or is not toggling, the relay should be deactivated.

Inbound frequency will be low, less than 1kHz due to python limitations, no clock div needed.

This is part of the protection method for the fireplace control. The thermostat
functions are handled by python on an SBC. If the script fails or the SBC hangs
this controller shall deactivate the relay, turning off the fireplace.

TODO - Add format failed counter to frequecy detection. Using the noise version of
python PWM so there are some false triggers occuring. 

*/

#include <math.h>

#include "Defines.h"

void setup() {
  Serial.begin(115200);

  // Initialize relay
  pinMode(outputPin, OUTPUT);
  digitalWrite(outputPin, relayState);

  // PWM input pin attached to interrupt to calculate inbound frequency
  pinMode(inputPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(inputPin), pulseDetected, RISING);
}

void loop() {

  unsigned long currentMillis = millis();

  // Check frequency status every second
  if (currentMillis - lastCheckTime >= checkInterval) {
    lastCheckTime = currentMillis;

    // Serial.println(currentFrequency);

    bool newLowFrequencyDetected = (fabs(currentFrequency - targetFreq1) < tolerance1);
    bool newHighFrequencyDetected = (fabs(currentFrequency - targetFreq2) < tolerance2);

    // If state hasn't changed for a full second, deactivate relay
    if (newLowFrequencyDetected == lowFrequencyDetected && 
        newHighFrequencyDetected == highFrequencyDetected) {
      digitalWrite(outputPin, HIGH);
      relayState = false;
    } else {
      lowFrequencyDetected = newLowFrequencyDetected;
      highFrequencyDetected = newHighFrequencyDetected;
    }
  }

  // Control relay activation with 10-minute cooldown
  if ((lowFrequencyDetected || highFrequencyDetected) && !relayState) {
    if (millis() - lastActivationTime >= activationCooldown) {
      digitalWrite(outputPin, LOW);  // Activate relay
      relayState = true;
      lastActivationTime = millis();
    }
  }

  // The relay can be deactivated at any time
  if (!lowFrequencyDetected && !highFrequencyDetected) {
    digitalWrite(outputPin, HIGH);
    relayState = false;
  }

  Serial.println(relayState);

}

void pulseDetected() {
  unsigned long currentTime = micros();

  if (lastPulseTime > 0) {
    currentFrequency = microsecondsInSeconds / (currentTime - lastPulseTime);
  }

  lastPulseTime = currentTime;
}