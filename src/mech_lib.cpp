#include "main.h"

const double armHeights[] = {995, 1860, 2400};
double armTarg = armHeights[0], armKP = 0.5;
bool tiltState = LOW, armClampState = LOW;
double intakeTarg = 0;

void armControl(void*ignore) {
  Motor arm(armPort);
  ADIDigitalOut armClamp(armClampPort);
  ADIDigitalIn armLimit(armLimitPort);
  ADIAnalogIn potentiometer(potentiometerPort);

  while(true) {
    double armError = armTarg - potentiometer.get_value();
    arm.move(armError*armKP);

    if(armLimit.get_new_press()) armClampState = true;
    armClamp.set_value(armClampState);

    delay(2);
  }
}

void setArmPos(int pos) {
  armTarg = armHeights[pos];
}

void setArmClampState(bool state) {
  armClampState = state;
}

void toggleArmClampState() {
  armClampState = !armClampState;
}

void waitArmClamp(double cutoff) {
  waitUntil(armClampState, cutoff);
}

void tiltControl(void*ignore) {
  ADIDigitalOut tilt(tiltPort);
  ADIDigitalOut tiltClamp(tiltClampPort);
  ADIDigitalIn  tiltLimit(tiltLimitPort);

  while(true) {
    if(tiltLimit.get_new_press()) tiltState = true;
    if(tiltState) {
      tiltClamp.set_value(HIGH);
      delay(240);
      tilt.set_value(HIGH);
    } else{
      tilt.set_value(LOW);
      delay(300);
      tiltClamp.set_value(LOW);
    }
    delay(5);
  }
}

void setTiltState(bool state) {
  tiltState = state;
}

void toggleTiltState() {
  tiltState = !tiltState;
}

void waitTiltClamp(double cutoff) {
  waitUntil(tiltState, cutoff);
}

void intakeControl(void*ignore) {
  Motor intake(intakePort);
  while(true) {
    intake.move(intakeTarg);
    delay(5);
  }
}

void setIntake(double pow) {
  intakeTarg = pow;
}
