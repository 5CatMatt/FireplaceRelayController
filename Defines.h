const int inputPin = 2;             // Interrupt-capable pin to read input frequency
const int outputPin = 4;            // Relay control (Active LOW)
const float targetFreq1 = 75.0;     // Expected frequency 1
const float targetFreq2 = 250.0;    // Expected frequency 2
const float tolerance1 = 20.0;      // ±20Hz tolerance
const float tolerance2 = 50.0;      // ±50Hz tolerance
const unsigned long frequencyToggleTime = 1000000.0;    // 1 minute 

volatile unsigned long lastPulseTime = 0;
volatile float currentFrequency = 0;
unsigned long lastStateChangeTime = 0;

bool signalValid = false;
bool relayState = HIGH;
