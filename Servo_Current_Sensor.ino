

// void loop () {}

//Global current sensor module
#define openLoc 10 
#define closeLoc 57
#define CURRENTREAD A0
#include <Servo.h>
#define Servo0 9


double posChip = 0; 
void moveGripper(int angle = posChip, bool doDetach = true);
bool isAttached = false;

Servo motor;

void setup() {
  Serial.begin(9600);
  pinMode(CURRENTREAD, INPUT);
  motor.attach(9);  // attaches the servo on pin 9
}

float vpp = 0.0048828125;
float sensitivity = 0.4; //mv
int ampOne = analogRead(CURRENTREAD);
float voltageOne = ampOne * vpp;
float amperageOne = voltageOne / sensitivity;
float amperageSum = 0;
float timeLimit = 4; //sec
float currentLimit = 1/1000;

void readCurrent() {

  for (int j = 1; j <= 400; j++) {
    ampOne = analogRead(CURRENTREAD);
    voltageOne = ampOne * vpp;
    voltageOne -= 2.5;
    amperageOne = voltageOne / sensitivity;
    amperageSum += amperageOne;
    if (j % 10 == 0 ) {
      Serial.print(amperageSum / 10); Serial.println(" Amps");
      amperageSum = 0;
    }
    delay(10);
  }
}


void moveServo(int angle, bool doDetach) {

  if (!isAttached) {
    motor.attach(Servo0);
    isAttached = true;
  }
  int pos = posChip;
  if (angle < pos) {
    while (angle < pos) {
      pos--;
      delay(8);
      motor.write(pos);
    }
  }
  else {
    while (angle >= pos) {
      pos++;
      delay(8);
      motor.write(pos);
    }
  }
  if (doDetach) {
    motor.detach();
    isAttached = false;
  }
  posChip = pos;
}

void loop()
{

  char serialChar = 0;

  if(Serial.available())
  {
    serialChar  = Serial.read();
  }

  switch (serialChar )
  {
    case 'A':
      motor(openLoc);
      Serial.println("Gripper Opened");
      break;
    case 'B':
      motor(closeLoc);
      Serial.println("Gripper Closed");
      break;
    case 'C':
      readCurrent();
      break;

  }

  serialChar = 0;

}
