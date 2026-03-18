#include "DeviceDriverSet_xxx0.h"

// ====================================================
//           ★★★ USER TUNED SETTINGS ★★★
// ====================================================

// --- SENSORS ---
const int LINE_THRESHOLD = 300; 
const int NOISE_FLOOR    = 50;  

// --- SPEEDS ---
const int CRUISE_SPEED   = 75;  
const int MANEUVER_SPEED = 150; 
const int BACKUP_SPEED   = 90;  
const int CENTER_SPEED   = 60;  // Very slow speed for final alignment

// --- DRIFT BIAS ---
const int LEFT_MOTOR_BIAS  = 0;
const int RIGHT_MOTOR_BIAS = 12; 

// --- SMOOTHNESS (PID) ---
const float Kp = 0.15; 

// --- SEQUENCE TIMINGS ---
const int WAIT_AT_END_MS   = 2000; // 1. Wait 2s at line end
const int FWD_PUSH_MS      = 300;  // 2. Forward 0.5s (Get off line)
const int TURN_90_MS       = 400;  // 3. Turn 90 deg 
const int BACK_TIME_MS     = 1000; // 4. Back 1.0s
const int FWD_RETURN_MS    = 1000; // 6. Return Forward
const int FWD_ONTO_LINE_MS = 400;  // 7. Blind push onto line
const int SPIN_180_MS      = 850;  // Final 180 spin time

// --- BATTERY & CHARGING ---
const float BATTERY_LOW_VOLTS  = 7.0; 
const float BATTERY_FULL_VOLTS = 8.3;    // Stop charging at this voltage
const float VOLTAGE_SCALAR     = 0.0125; // Conversion factor
const int   CHARGE_TIMEOUT_MS  = 60000;  // Safety timeout

// Filter Settings
const int   VOLT_BUFFER_SIZE   = 10;   
const int   LOW_BAT_CONFIRM_MS = 2000; 

// Safety Timers
const unsigned long STOP_DELAY_MS = 300; 

// ====================================================
//                OBJECTS & VARIABLES
// ====================================================
DeviceDriverSet_Motor       motor;
DeviceDriverSet_ITR20001    lineSensors;
DeviceDriverSet_Voltage     voltageSensor;
DeviceDriverSet_IRrecv      ir;
DeviceDriverSet_RBGLED      led;

// System Flags
bool systemActive = false;      
bool isTestMode = false;       
bool lowBatteryFlag = false;    
bool emergencyPaused = false; 
int  missionPhase = 0; 

// Logic Variables
unsigned long allBlackStartTime = 0;
bool seeingEndMarker = false;

// --- BATTERY VARIABLES ---
float voltageBuffer[VOLT_BUFFER_SIZE];
int   voltBufferIndex = 0;
float avgVoltage = 8.0; 
unsigned long lastVoltSampleTime = 0;
unsigned long lowBatDurationTimer = 0;
bool  lowBatTimerActive = false;

// ====================================================
//                HELPERS
// ====================================================

void setLED(uint8_t r, uint8_t g, uint8_t b) {
  led.DeviceDriverSet_RBGLED_Color(0, r, g, b);
}

void stopCar() {
  motor.DeviceDriverSet_Motor_control(true, 0, true, 0, false);
}

void drive(int leftSpeed, int rightSpeed) {
  if (leftSpeed > 0 && rightSpeed > 0) {
    leftSpeed  -= LEFT_MOTOR_BIAS;
    rightSpeed -= RIGHT_MOTOR_BIAS;
  }
  leftSpeed  = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  motor.DeviceDriverSet_Motor_control(leftSpeed>=0, abs(leftSpeed), rightSpeed>=0, abs(rightSpeed), true);
}

// ====================================================
//           BATTERY MANAGEMENT SYSTEM
// ====================================================

void updateBatteryVoltage() {
  if (millis() - lastVoltSampleTime > 50) { 
    lastVoltSampleTime = millis();
    float raw = voltageSensor.DeviceDriverSet_Voltage_getAnalogue();
    float sample = raw * VOLTAGE_SCALAR; 
    if (sample < 3.0) return; 

    voltageBuffer[voltBufferIndex] = sample;
    voltBufferIndex = (voltBufferIndex + 1) % VOLT_BUFFER_SIZE;

    float sum = 0;
    for (int i = 0; i < VOLT_BUFFER_SIZE; i++) sum += voltageBuffer[i];
    avgVoltage = sum / VOLT_BUFFER_SIZE;
  }
}

void checkLowBattery() {
  if (missionPhase == 1) return; 
  if (avgVoltage < BATTERY_LOW_VOLTS) {
    if (!lowBatTimerActive) {
      lowBatTimerActive = true;
      lowBatDurationTimer = millis();
    }
    else if (millis() - lowBatDurationTimer > LOW_BAT_CONFIRM_MS) {
      if (!lowBatteryFlag) lowBatteryFlag = true;
    }
  } else {
    lowBatTimerActive = false;
  }
}

// ====================================================
//           SMART WAIT & CHARGE LOGIC
// ====================================================

bool smartWait(unsigned long duration) {
  unsigned long start = millis();
  while (millis() - start < duration) {
    if (!isTestMode) updateBatteryVoltage(); 
    uint8_t key = 0;
    if (ir.DeviceDriverSet_IRrecv_Get(&key)) {
      if (key == 7) { 
        emergencyPaused = true;
        stopCar();
        setLED(255, 0, 0); 
        while(emergencyPaused) {
           if (ir.DeviceDriverSet_IRrecv_Get(&key)) {
             if (key == 8) { 
               emergencyPaused = false;
               systemActive = false; 
               return false; 
             }
           }
           delay(50);
        }
      }
      else if (key == 8) { 
         systemActive = false;
         return false; 
      }
    }
    delay(10);
  }
  return true; 
}

bool waitForSmartCharge() {
  Serial.println(">> MONITORING CHARGE...");
  unsigned long chargeStart = millis();
  unsigned long lastStableCheck = millis();
  float voltageSnapshot = avgVoltage;
  bool charging = true;

  while(charging) {
    if (!smartWait(100)) return false; 
    if ((millis() / 500) % 2 == 0) setLED(255, 255, 0);
    else setLED(0, 0, 0);

    if (avgVoltage >= BATTERY_FULL_VOLTS) break;

    if (millis() - lastStableCheck > 5000) {
      float diff = avgVoltage - voltageSnapshot;
      if (diff < 0.05) break;
      voltageSnapshot = avgVoltage;
      lastStableCheck = millis();
    }
    if (millis() - chargeStart > CHARGE_TIMEOUT_MS) break;
  }
  setLED(0, 255, 0); 
  return true;
}

// ====================================================
//           MANEUVER: PARK, CHARGE & RETURN
// ====================================================
void executeParkAndReturn() {
  stopCar(); 
  setLED(255, 0, 0); 
  if (!smartWait(WAIT_AT_END_MS)) return;

  setLED(0, 255, 0); 
  drive(MANEUVER_SPEED, MANEUVER_SPEED);
  if (!smartWait(FWD_PUSH_MS)) return; 
  stopCar(); smartWait(200);

  setLED(255, 165, 0); 
  drive(MANEUVER_SPEED, -MANEUVER_SPEED);
  if (!smartWait(TURN_90_MS)) return;
  stopCar(); smartWait(200);

  setLED(255, 0, 255); 
  drive(-BACKUP_SPEED, -BACKUP_SPEED); 
  if (!smartWait(BACK_TIME_MS)) return; 
  stopCar();

  if (isTestMode) {
    setLED(255, 255, 0);
    if (!smartWait(3000)) return;
  } else {
    if (!waitForSmartCharge()) return;
  }

  setLED(0, 255, 0); 
  drive(BACKUP_SPEED, BACKUP_SPEED); 
  if (!smartWait(FWD_RETURN_MS)) return; 
  stopCar(); smartWait(200);

  setLED(255, 165, 0);
  drive(MANEUVER_SPEED, -MANEUVER_SPEED);
  if (!smartWait(TURN_90_MS)) return;
  stopCar(); smartWait(200);

  setLED(0, 255, 255);
  drive(MANEUVER_SPEED, MANEUVER_SPEED);
  if (!smartWait(FWD_ONTO_LINE_MS)) return;

  missionPhase = 1; 
  setLED(0, 0, 255); 
}

// ====================================================
//           MANEUVER: FINAL SPIN & CENTER
// ====================================================
void executeFinalSpin() {
  stopCar();
  delay(500);
  
  // 1. Blind Spin 180
  setLED(255, 0, 0); 
  drive(MANEUVER_SPEED, -MANEUVER_SPEED);
  smartWait(SPIN_180_MS);
  
  stopCar();
  systemActive = false; 
}

// ====================================================
//           LOGIC
// ====================================================
void updateLogic() {
  if (!isTestMode) {
    updateBatteryVoltage();
    checkLowBattery();
  }

  long L = lineSensors.DeviceDriverSet_ITR20001_getAnaloguexxx_L();
  long M = lineSensors.DeviceDriverSet_ITR20001_getAnaloguexxx_M();
  long R = lineSensors.DeviceDriverSet_ITR20001_getAnaloguexxx_R();
  
  bool allBlack = (L > LINE_THRESHOLD) && (M > LINE_THRESHOLD) && (R > LINE_THRESHOLD);

  if (allBlack) {
    if (!seeingEndMarker) {
      allBlackStartTime = millis();
      seeingEndMarker = true;
    }
    drive(CRUISE_SPEED, CRUISE_SPEED);

    if (millis() - allBlackStartTime > STOP_DELAY_MS) {
      if (missionPhase == 0) executeParkAndReturn();
      else executeFinalSpin();
      return;
    }
    return;
  } else {
    seeingEndMarker = false;
  }

  long sumValues = L + M + R;
  if (sumValues < (NOISE_FLOOR * 3)) {
    stopCar();
    return;
  }

  if(lowBatteryFlag) setLED(255, 0, 0); else setLED(0, 0, 255); 

  long weightedSum = (L * -1000) + (M * 0) + (R * 1000);
  float position = (float)weightedSum / sumValues;

  int correction = position * Kp;
  int leftSpeed  = CRUISE_SPEED + correction;
  int rightSpeed = CRUISE_SPEED - correction;

  drive(leftSpeed, rightSpeed);
}

// ====================================================
//                SETUP & LOOP
// ====================================================
void setup() {
  Serial.begin(9600);
  motor.DeviceDriverSet_Motor_Init();
  lineSensors.DeviceDriverSet_ITR20001_Init();
  voltageSensor.DeviceDriverSet_Voltage_Init();
  ir.DeviceDriverSet_IRrecv_Init();
  led.DeviceDriverSet_RBGLED_Init(40);
  
  for(int i=0; i<VOLT_BUFFER_SIZE; i++) voltageBuffer[i] = 8.0;

  stopCar();
  setLED(255, 0, 0); 
  Serial.println("READY.");
}

void loop() {
  uint8_t key = 0;
  if (ir.DeviceDriverSet_IRrecv_Get(&key)) {
    if (key == 6) { // KEY 1: TEST MODE
      isTestMode = true;      
      lowBatteryFlag = true; 
      if (!systemActive) { systemActive = true; missionPhase = 0; }
    }
    else if (key == 7) { // KEY 2: EMERGENCY
      emergencyPaused = true;
      stopCar();
      setLED(255, 0, 0);
      while(emergencyPaused) {
        if (ir.DeviceDriverSet_IRrecv_Get(&key)) {
           if (key == 8) { 
             emergencyPaused = false;
             systemActive = false;
             lowBatteryFlag = false;
             missionPhase = 0; 
             setLED(0, 255, 0);
           }
        }
        delay(50);
      }
    }
    else if (key == 8) { // KEY 3: AUTO MODE
      isTestMode = false;     
      systemActive = true;
      lowBatteryFlag = false; 
      missionPhase = 0; 
      setLED(0, 0, 255);
    }
  }

  if (systemActive) {
    updateLogic();
  } 
  else {
    stopCar();
    if (!emergencyPaused) {
       int M = lineSensors.DeviceDriverSet_ITR20001_getAnaloguexxx_M();
       if (M > LINE_THRESHOLD) setLED(0, 255, 0); 
       else setLED(255, 0, 0); 
    }
  }
}