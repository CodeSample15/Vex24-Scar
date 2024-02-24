#include "api.h"
#include "robot.h"
#include "autons/HelperMethods.h"

inline void BowlDownAlley() 
{
    leftBackWingOut();
    static_imu.set_rotation(-135);
    pros::delay(200);
    SlapperMotor.tare_position();

    while(SlapperMotor.get_position() < 900*10) {
        SlapperMotor.move(50);
        pros::delay(10);
    }
    SlapperMotor.brake();

    driveChassis.TurnPid(-45, 2);

    //FLOOR IT
    driveChassis.MovePid(-4000, 0.7, 2, true);
}