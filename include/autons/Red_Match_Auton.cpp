#include "api.h"
#include "robot.h"
#include "HelperMethods.h"

void spinLeftSlapperToPosition(int pos, int speed) {
    while(LeftSlapperSensor.get_position() < pos) {
        LeftSlapperMotor.move(speed);
        pros::delay(10);
    }

    LeftSlapperMotor.brake();
}

inline void KyleMatchAuton() {
    static_imu.set_rotation(-135);
    LeftSlapperSensor.reset_position();
    LeftSlapperMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

    driveChassis.right->set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    driveChassis.left->set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);

    //swing backwards: position the bot correctly
    driveChassis.MovePid(-150, 1, 5);
    leftBackWingOut();

    //align the slapper
    spinLeftSlapperToPosition(90, 127);
    LeftSlapperSensor.set_position(0);

    driveChassis.Turn(-5, 127, 1);
}