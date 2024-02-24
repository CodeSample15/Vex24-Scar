#include "robot.h"
#include "api.h"
#include "HelperMethods.h"

#if ORANGE_BOT
    void frontWingsOut() {
        frontWings.set_value(1);
    }

    void frontWingsIn() {
        frontWings.set_value(0);
    }
#endif

void leftBackWingOut() {
    leftBackWings.set_value(1);
}

void leftBackWingIn() {
    leftBackWings.set_value(0);
}

void rightBackWingOut() {
    rightBackWings.set_value(1);
}

void rightBackWingIn() {
    rightBackWings.set_value(0);
}

void backWingsIn() {
    rightBackWings.set_value(0);
    leftBackWings.set_value(0);
}

void backWingsOut() {
    rightBackWings.set_value(1);
    leftBackWings.set_value(1);
}

void startIntake(bool in) {
    IntakeMotor.move(in ? -127 : 127);
}

void stopIntake() {
    IntakeMotor.brake();
}