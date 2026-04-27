#include "arduino_secrets.h"

#include <Servo.h>
#include <math.h>
#include "coordinates.h"
#include "serial_commands.h"
#include "tasks.h"

Servo s1;  // Joint 1
Servo s2;  // Joint 2
Servo s3;  // gripper

const int US_AT_0   = 500;
const int US_AT_270 = 2500;

float x, y;

const int EE_UP_ANGLE    = 60;
const int EE_HOVER_ANGLE = 95;
const int EE_DOWN_ANGLE  = 130;
int currentEE = EE_HOVER_ANGLE;  

const float L1 = 110.0f;
const float L2 =  90.0f;

const float JOINT1_OFFSET_X = 0.0f;
const float JOINT1_OFFSET_Y = 140.0f;


int SERVO1_OFFSET = 48;   // degrees added to computed servo angle
int SERVO2_OFFSET = 158;
int SERVO1_DIR    = +1;  // +1 or -1 if reversed
int SERVO2_DIR    = -1;

const int STEP_DEG = 1;      
const int STEP_DELAY_MS = 30; 

int currentS1 = 0; 
int currentS2 = 0;

int angleToUs270(int a) {
  a = constrain(a, 0, 270);
  return map(a, 0, 270, US_AT_0, US_AT_270);
}

void writeServo270(Servo &s, int deg270) {
  s.writeMicroseconds(angleToUs270(deg270));
}

void endEffectorUp()    { moveEndEffectorTo(EE_UP_ANGLE); }
void endEffectorHover() { moveEndEffectorTo(EE_HOVER_ANGLE); }
void endEffectorDown()  { moveEndEffectorTo(EE_DOWN_ANGLE); }

void moveEndEffectorTo(int angleDeg) {
  angleDeg = constrain(angleDeg, 0, 160);   
  s3.write(angleDeg);                     
  currentEE = angleDeg;

  Serial.print("End effector (s3) angle = ");
  Serial.println(currentEE);
}

void moveServosSlow(int targetS1, int targetS2) {
  targetS1 = constrain(targetS1, 0, 270);
  targetS2 = constrain(targetS2, 0, 270);

  Serial.print("Servo angles (deg): s1=");
  Serial.print(targetS1);
  Serial.print(", s2=");
  Serial.println(targetS2);

  while (currentS1 != targetS1 || currentS2 != targetS2) {
    if (currentS1 < targetS1) currentS1 = min(currentS1 + STEP_DEG, targetS1);
    else if (currentS1 > targetS1) currentS1 = max(currentS1 - STEP_DEG, targetS1);

    if (currentS2 < targetS2) currentS2 = min(currentS2 + STEP_DEG, targetS2);
    else if (currentS2 > targetS2) currentS2 = max(currentS2 - STEP_DEG, targetS2);

    writeServo270(s1, currentS1);
    writeServo270(s2, currentS2);
    delay(STEP_DELAY_MS);

  }
}

bool solveIK(float x, float y, float &theta1_deg, float &theta2_deg, bool elbowUp) {
  float r2 = x*x + y*y;
  float r  = sqrt(r2);
  float theta2;   

  if (r > (L1 + L2) || r < fabs(L1 - L2)) return false;

  float c2 = (r2 - L1*L1 - L2*L2) / (2.0f * L1 * L2);
  c2 = constrain(c2, -1.0f, 1.0f);

  theta2 = acos(c2); 

  float s2 = sin(theta2);
  float k1 = L1 + L2 * c2;
  float k2 = L2 * s2;

  float theta1 = atan2(y, x) - atan2(k2, k1);

  theta1_deg = theta1 * 180.0f / M_PI;
  theta2_deg = theta2 * 180.0f / M_PI;

  Serial.print("IK joints (deg): theta1=");
  Serial.print(theta1_deg);
  Serial.print(", theta2=");
  Serial.println(theta2_deg);
  
  return true;
}

bool moveToBoardXY(float xb, float yb, bool elbowUp=true) {
  // Board -> Joint1 frame
  float x = xb - JOINT1_OFFSET_X;
  float y = yb + JOINT1_OFFSET_Y;

  float t1, t2;
  
  if (!solveIK(x, y, t1, t2, elbowUp)) {
    Serial.println("Invalid IK solution");
    return false;
  }

  int s1 = (int)round(SERVO1_OFFSET + SERVO1_DIR * t1);
  int s2 = (int)round(SERVO2_OFFSET + SERVO2_DIR * t2);

  s1 = constrain(s1, 0, 270);
  s2 = constrain(s2, 0, 270);

  moveServosSlow(s1, s2);
  return true;
}


void setup() {
  Serial.begin(115200);

  while (!Serial) { ; }

  s3.attach(10);
  s1.attach(9);
  s2.attach(8);
  
  endEffectorHover();

  Serial.println("Ready.");
  initBoxes();
}

void loop() {
  
  readSerialCellCommand();

}