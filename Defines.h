const int inputPin = 2;             // Interrupt-capable pin to read input frequency
const int outputPin = 4;            // Relay control (Active LOW)
const float targetFreq1 = 75.0;     // Expected frequency 1
const float targetFreq2 = 250.0;    // Expected frequency 2
const float tolerance1 = 20.0;      // ±20Hz tolerance
const float tolerance2 = 50.0;      // ±50Hz tolerance
const int toggleInterval = 1000;    // 1 second in ms

const unsigned long microsecondsInSeconds = 1000000.0;    // 1 second in microseconds 

volatile unsigned long lastPulseTime = 0;
volatile float currentFrequency = 0;
unsigned long lastStateChangeTime = 0;

bool lowFrequencyDetected = false;
bool highFrequencyDetected = false;

unsigned long lastCheckTime = 0;
unsigned long lastActivationTime = 0;
const unsigned long checkInterval = 1000; // 1 second
const unsigned long activationCooldown = 30000; // TODO DEBUG
// const unsigned long activationCooldown = 600000; // 10 minutes

bool signalValid = false;
bool relayState = HIGH;
