#include <Servo.h>

// ==================== Servos ====================
Servo hip1, knee1;
Servo hip2, knee2;
Servo hip3, knee3;
Servo hip4, knee4;
Servo UltraSonic;
// ==================== Pins ====================
#define HIP1_PIN  7 //4 -7
#define KNEE1_PIN 11 //5 -11
#define HIP2_PIN  5 //6 -5
#define KNEE2_PIN 8 //7 -8
#define HIP3_PIN  9 //8 - 9
#define KNEE3_PIN 4 //9 -4
#define HIP4_PIN  10 //10 -10
#define KNEE4_PIN 6 //11 -6
#define USPIN 12
// ==================== LEG CALIBRATION ====================
const int L1_HIP_FORWARD = 0;
const int L1_HIP_BACK    = 120;
const int L1_HIP_NEUTRAL = 60;

const int L2_HIP_FORWARD = 0;
const int L2_HIP_BACK    = 120;
const int L2_HIP_NEUTRAL = 60;

const int L3_HIP_FORWARD = 120;
const int L3_HIP_BACK    = 0;
const int L3_HIP_NEUTRAL = 60;

const int L4_HIP_FORWARD = 120;
const int L4_HIP_BACK    = 0;
const int L4_HIP_NEUTRAL = 60;

// Knees
const int L1_KNEE_DOWN = 0;
const int L2_KNEE_DOWN = 0;
const int L3_KNEE_DOWN = 180;
const int L4_KNEE_DOWN = 180;

const int L1_KNEE_UP = 60;
const int L2_KNEE_UP = 60;
const int L3_KNEE_UP = 130;
const int L4_KNEE_UP = 130;

bool forward = false;
bool right = false;
bool left = false;
bool back = false;

// ==================== Ultrasonic ====================
#define FRONT_TRIG  3
#define FRONT_ECHO  2
const int OBSTACLE_DIST = 20;
int lastDur=0;
// ==================== Smooth ====================
int speedDelay = 2;
int stepSize = 2;


long getDistance() {
  digitalWrite(FRONT_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(FRONT_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(FRONT_TRIG, LOW);

  long duration = pulseIn(FRONT_ECHO, HIGH, 30000);
  if (duration != 0) lastDur=duration;

  return lastDur * 0.034 / 2;
}

void smoothMove(Servo &servo, int target) {
  int current = servo.read();
  if (current < target) {
    for (int pos = current; pos <= target; pos += stepSize) {
      servo.write(pos);
      delay(speedDelay);
    }
  } else {
    for (int pos = current; pos >= target; pos -= stepSize) {
      servo.write(pos);
      delay(speedDelay);
    }
  }
  servo.write(target);
}

// Moves two servos simultaneously
void smoothMovePair(Servo &a, int targetA, Servo &b, int targetB) {
  int posA = a.read();
  int posB = b.read();
  int stepsA = abs(targetA - posA);
  int stepsB = abs(targetB - posB);
  int steps = max(stepsA, stepsB) / stepSize;
  if (steps == 0) return;

  for (int i = 0; i <= steps; i++) {
    a.write(posA + (targetA - posA) * i / steps);
    b.write(posB + (targetB - posB) * i / steps);
    delay(speedDelay);
  }
  a.write(targetA);
  b.write(targetB);
}

// Moves four servos simultaneously
void smoothMoveQuad(Servo &a, int tA, Servo &b, int tB, Servo &c, int tC, Servo &d, int tD) {
  int pA = a.read(), pB = b.read(), pC = c.read(), pD = d.read();
  int steps = max(max(abs(tA-pA), abs(tB-pB)), max(abs(tC-pC), abs(tD-pD))) / stepSize;
  if (steps == 0) return;

  for (int i = 0; i <= steps; i++) {
    a.write(pA + (tA - pA) * i / steps);
    b.write(pB + (tB - pB) * i / steps);
    c.write(pC + (tC - pC) * i / steps);
    d.write(pD + (tD - pD) * i / steps);
    delay(speedDelay);
  }
  a.write(tA); b.write(tB); c.write(tC); d.write(tD);
}

// ==================== Stand ====================
void stand() {
  knee1.write(L1_KNEE_DOWN);
  knee2.write(L2_KNEE_DOWN);
  knee3.write(L3_KNEE_DOWN);
  knee4.write(L4_KNEE_DOWN);

  hip1.write(L1_HIP_NEUTRAL);
  hip2.write(L2_HIP_NEUTRAL);
  hip3.write(L3_HIP_NEUTRAL);
  hip4.write(L4_HIP_NEUTRAL);

  delay(400);
}

// ==================== WALK ====================
bool walkForwardCycle() {

  // Fix 3: push grounded pair (2 & 4) back first before lifting pair 1 & 3
  smoothMovePair(hip2, L2_HIP_BACK, hip4, L4_HIP_NEUTRAL);

  // Pair 1: lift 1 & 3 simultaneously
  smoothMovePair(knee1, L1_KNEE_UP, knee3, L3_KNEE_UP);

  // Swing hips — keeping your original targets
  smoothMove(hip1, L1_HIP_NEUTRAL);
  smoothMove(hip3, L3_HIP_FORWARD);
  smoothMoveQuad(hip2, L2_HIP_BACK, hip4, L4_HIP_NEUTRAL, knee1, L1_KNEE_DOWN, knee3, L3_KNEE_DOWN);

  delay(60);

  // Fix 3: push grounded pair (1 & 3) back first before lifting pair 2 & 4
  smoothMovePair(hip1, L1_HIP_BACK, hip3, L3_HIP_NEUTRAL);

  // Pair 2: lift 2 & 4 simultaneously
  smoothMovePair(knee2, L2_KNEE_UP, knee4, L4_KNEE_UP);

  // Swing hips — keeping your original targets
  smoothMove(hip2, L2_HIP_NEUTRAL);
  smoothMove(hip4, L4_HIP_FORWARD);
  smoothMoveQuad(hip1, L1_HIP_BACK, hip3, L3_HIP_NEUTRAL, knee2, L2_KNEE_DOWN, knee4, L4_KNEE_DOWN);

  delay(60);

  return true;
}
bool walkBackCycle() {

  // Fix 3: push grounded pair (2 & 4) back first before lifting pair 1 & 3
  smoothMovePair(hip2, L2_HIP_BACK, hip4, L4_HIP_NEUTRAL);

  // Pair 1: lift 1 & 3 simultaneously
  smoothMovePair(knee1, L1_KNEE_UP, knee3, L3_KNEE_UP);

  // Swing hips — keeping your original targets
  smoothMove(hip1, L1_HIP_BACK);
  smoothMove(hip3, L3_HIP_NEUTRAL);
  smoothMoveQuad(hip2, L2_HIP_NEUTRAL, hip4, L4_HIP_FORWARD, knee1, L1_KNEE_DOWN, knee3, L3_KNEE_DOWN);

  delay(60);

  // Fix 3: push grounded pair (1 & 3) back first before lifting pair 2 & 4
  smoothMovePair(hip1, L1_HIP_NEUTRAL, hip3, L3_HIP_FORWARD);

  // Pair 2: lift 2 & 4 simultaneously
  smoothMovePair(knee2, L2_KNEE_UP, knee4, L4_KNEE_UP);

  // Swing hips — keeping your original targets
  smoothMove(hip2, L2_HIP_BACK);
  smoothMove(hip4, L4_HIP_NEUTRAL);
  smoothMoveQuad(hip1, L1_HIP_BACK, hip3, L3_HIP_NEUTRAL, knee2, L2_KNEE_DOWN, knee4, L4_KNEE_DOWN);

  delay(60);

  return true;
}
// ==================== left ====================
bool turnLeft() {

  // Fix 3: push grounded pair (2 & 4) back first before lifting pair 1 & 3
  smoothMovePair(hip2, L2_HIP_FORWARD, hip4, L4_HIP_NEUTRAL);

  // Pair 1: lift 1 & 3 simultaneously
  smoothMovePair(knee1, L1_KNEE_UP, knee3, L3_KNEE_UP);

  // Swing hips — keeping your original targets
  smoothMove(hip1, L1_HIP_NEUTRAL);
  smoothMove(hip3, L3_HIP_BACK);
  smoothMoveQuad(hip2, L2_HIP_BACK, hip4, L4_HIP_NEUTRAL, knee1, L1_KNEE_DOWN, knee3, L3_KNEE_DOWN);

  delay(60);

  // Fix 3: push grounded pair (1 & 3) back first before lifting pair 2 & 4
  smoothMovePair(hip1, L1_HIP_BACK, hip3, L3_HIP_NEUTRAL);

  // Pair 2: lift 2 & 4 simultaneously
  smoothMovePair(knee2, L2_KNEE_UP, knee4, L4_KNEE_UP);

  // Swing hips — keeping your original targets
  smoothMove(hip2, L2_HIP_BACK);
  smoothMove(hip4, L4_HIP_FORWARD);
  smoothMoveQuad(hip1, L1_HIP_BACK, hip3, L3_HIP_NEUTRAL, knee2, L2_KNEE_DOWN, knee4, L4_KNEE_DOWN);

  delay(60);

  return true;
}
// ==================== right ====================
bool turnRight() {

  // Fix 3: push grounded pair (2 & 4) back first before lifting pair 1 & 3
  smoothMovePair(hip2, L2_HIP_BACK, hip4, L4_HIP_FORWARD);

  // Pair 1: lift 1 & 3 simultaneously
  smoothMovePair(knee1, L1_KNEE_UP, knee3, L3_KNEE_UP);

  // Swing hips — keeping your original targets
  smoothMove(hip1, L1_HIP_BACK);
  smoothMove(hip3, L3_HIP_FORWARD);
  smoothMoveQuad(hip2, L2_HIP_BACK, hip4, L4_HIP_FORWARD, knee1, L1_KNEE_DOWN, knee3, L3_KNEE_DOWN);

  delay(60);

  // Fix 3: push grounded pair (1 & 3) back first before lifting pair 2 & 4
  smoothMovePair(hip1, L1_HIP_BACK, hip3, L3_HIP_NEUTRAL);

  // Pair 2: lift 2 & 4 simultaneously
  smoothMovePair(knee2, L2_KNEE_UP, knee4, L4_KNEE_UP);

  // Swing hips — keeping your original targets
  smoothMove(hip2, L2_HIP_NEUTRAL);
  smoothMove(hip4, L4_HIP_BACK);
  smoothMoveQuad(hip1, L1_HIP_FORWARD, hip3, L3_HIP_NEUTRAL, knee2, L2_KNEE_DOWN, knee4, L4_KNEE_DOWN);

  delay(60);

  return true;
}

// ==================== SETUP ====================
void setup() {
  Serial.begin(9600);

  hip1.attach(HIP1_PIN); knee1.attach(KNEE1_PIN);
  hip2.attach(HIP2_PIN); knee2.attach(KNEE2_PIN);
  hip3.attach(HIP3_PIN); knee3.attach(KNEE3_PIN);
  hip4.attach(HIP4_PIN); knee4.attach(KNEE4_PIN);
  UltraSonic.attach(USPIN);
  pinMode(FRONT_TRIG, OUTPUT);
  pinMode(FRONT_ECHO, INPUT);
  stand();
  delay(800);

  Serial.println("Robot Ready");
}

// ==================== LOOP ====================
void loop() {
  //walkForwardCycle();
  if (Serial.available()) {
    char c = Serial.read();

    if (c == 'f') {
      forward=true;
    }else if (c == 'r') {
      right = true;
    }else if (c=='l'){
      left = true;
    }else if(c=='b'){
      back=true;
    }else if(c=='s'){
      forward=false;
      right=false;
      left=false;
      back=false;
      stand();
    }
    if (forward){
      walkForwardCycle();
    }else if (right){
      turnRight();
    }else if (left){
      turnLeft();
    }else if (back){
      walkBackCycle();
    }
  }
}
