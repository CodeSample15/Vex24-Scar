#include "api.h"
#include "pros/motors.h"
#include "robot.h"
#include "HelperMethods.h"

void spinRightSlapperToPosition(int pos, int speed) {
    while(RightSlapperSensor.get_position() < pos) {
        RightSlapperMotor.move(speed);
    }
}

inline void SupremeBowling() {
    //start right against the bar
    static_imu.set_rotation(135);
    RightSlapperSensor.reset_position();
    RightSlapperMotor.set_brake_mode(pros::E_MOTOR_BRAKE_BRAKE);

    driveChassis.right->set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    driveChassis.left->set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);

    //swing backwards: position the bot correctly
    driveChassis.MovePid(-150, 1, 5);
    rightBackWingOut();

    //align the slapper
    spinRightSlapperToPosition(90, 127);
    RightSlapperSensor.reset_position();

    driveChassis.Turn(5, 127, 0);

    //start match loading
}