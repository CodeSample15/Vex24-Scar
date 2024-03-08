#include "api.h"
#include "robot.h"
#include "HelperMethods.h"
#include <iostream>

inline void spinLeftSlapperToPosition(int pos, int speed) {
    while(LeftSlapperMotor.get_position() < pos) {
        LeftSlapperMotor.move(speed);
        pros::delay(5);
    }

    LeftSlapperMotor.brake();

    while(LeftSlapperMotor.get_position() > pos) {
        LeftSlapperMotor.move(-20);
        pros::delay(5);
    }

    LeftSlapperMotor.brake();
}

inline void KyleMatchAuton() {
    static_imu.set_rotation(-135);
    LeftSlapperMotor.tare_position();
    LeftSlapperMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

    driveChassis.right->set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    driveChassis.left->set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);

    //swing backwards: position the bot correctly
    driveChassis.MovePid(-160, 1, 5);
    leftBackWingOut();

    //align the slapper
    spinLeftSlapperToPosition(30, 127);
    LeftSlapperSensor.set_position(0);
    pros::delay(100);

    driveChassis.Turn(-3, 127, 1);

    //matchloads
    /*
    for(int i=0; i<13; i++) {
        spinLeftSlapperToPosition(300*(i+1), 127);
        pros::delay(1500);
    }
    */

   driveChassis.MovePid(-400, 1, 5);
   driveChassis.TurnPid(-180-static_imu.get_rotation(), 1.5);

    driveChassis.MovePid(-3000, 1, 5, true);
/*
   leftBackWingIn();
   pros::delay(200);
   driveChassis.MovePid(-950, 1, 5, true);
   driveChassis.TurnPid(-180-static_imu.get_rotation(), 1.5);
   rightBackWingOut();
   pros::delay(200);

    driveChassis.MovePid(-3000, 1, 4, true);
    */
}