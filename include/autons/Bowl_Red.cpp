#include "api.h"
#include "pros/motors.h"
#include "robot.h"
#include "HelperMethods.h"

void spinRightSlapperToPosition(int pos, int speed) {
    while(std::abs(RightSlapperMotor.get_position()) < std::abs(pos)) {
        
    }
}

inline void SupremeBowling() {
    //start right against the bar
    static_imu.set_rotation(135);
    RightSlapperMotor.tare_position();

    driveChassis.right->set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);
    driveChassis.left->set_brake_modes(pros::E_MOTOR_BRAKE_BRAKE);

    //swing backwards: position the bot correctly
    driveChassis.MovePid(-150, 1, 5);
    rightBackWingOut();
    driveChassis.Turn(5, 127, 0);
    


    //start match loading
}