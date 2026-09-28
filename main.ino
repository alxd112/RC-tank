#include <Wire.h>
#include <Bluepad32.h>

// --- Hiwonder Encoder Motor Driver Configuration ---
#define MOTOR_ADDR              0x34
#define MOTOR_TYPE_ADDR         0x20
#define MOTOR_ENCODER_POLARITY  0x21
#define MOTOR_FIXED_SPEED_ADDR  0x33

// Bluepad32 Controller Storage
ControllerPtr myControllers[BP32_MAX_GAMEPADS];

void WireWriteDataArray(uint8_t reg, int8_t* data, uint8_t len) {
  Wire.beginTransmission(MOTOR_ADDR);
  Wire.write(reg);
  for (uint8_t i = 0; i < len; i++) {
    Wire.write(data[i]);
  }
  Wire.endTransmission();
}

// ================================================================
// BLUEPAD32 CALLBACKS
// ================================================================
void onConnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == nullptr) {
      Serial.printf("Controller connected at index %d\n", i);
      myControllers[i] = ctl;
      break;
    }
  }
}

void onDisconnectedController(ControllerPtr ctl) {
  for (int i = 0; i < BP32_MAX_GAMEPADS; i++) {
    if (myControllers[i] == ctl) {
      Serial.printf("Controller disconnected from index %d\n", i);
      myControllers[i] = nullptr;
      
      // Safety: Instantly kill motors if connection drops
      int8_t stopSpeeds[2] = {0, 0};
      WireWriteDataArray(MOTOR_FIXED_SPEED_ADDR, stopSpeeds, 2);
      break;
    }
  }
}

// ================================================================
// GAMEPAD PROCESSING (Tank Steering)
// ================================================================
void processGamepad(ControllerPtr ctl) {
  int32_t leftStickY = ctl->axisY();     // Left Stick Vertical
  int32_t rightStickY = ctl->axisRY();   // Right Stick Vertical

  if (abs(leftStickY) < 40) leftStickY = 0;
  if (abs(rightStickY) < 40) rightStickY = 0;

  int8_t motor1Speed = (int8_t)(leftStickY / -5.12f) * -1;
  int8_t motor2Speed = (int8_t)(rightStickY / -5.12f) * -1;

  int8_t speeds[2] = {motor1Speed, motor2Speed};
  WireWriteDataArray(MOTOR_FIXED_SPEED_ADDR, speeds, 2);

  Serial.printf("M1 (Left): %d | M2 (Right): %d\n", motor1Speed, 
motor2Speed);
}

// ================================================================
// SETUP & LOOP
// ================================================================
void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  Wire.setClock(100000);
  delay(100);

  int8_t motorType = 20; 
  WireWriteDataArray(MOTOR_TYPE_ADDR, &motorType, 1);
  delay(50);

  int8_t polarity = 0; 
  WireWriteDataArray(MOTOR_ENCODER_POLARITY, &polarity, 1);
  delay(100);


  BP32.setup(&onConnectedController, &onDisconnectedController);
  BP32.forgetBluetoothKeys();
  BP32.enableVirtualDevice(false);
    
  Serial.println("System Ready! Turn on your PS4 controller (PS + 
Share)");
}

void loop() {
  BP32.update();

  // Process data if a controller is linked
  bool controllerActive = false;
  for (auto myController : myControllers) {
    if (myController && myController->isConnected() && 
myController->hasData()) {
      processGamepad(myController);
      controllerActive = true;
    }
  }


  if (!controllerActive) {
    int8_t stopSpeeds[2] = {0, 0};
    WireWriteDataArray(MOTOR_FIXED_SPEED_ADDR, stopSpeeds, 2);
  }

  delay(10);
}
