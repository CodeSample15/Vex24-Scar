#include "api.h"
#include "robot.h"
#include "autons/HelperMethods.h"

inline void run_skills() 
{
    leftBackWingOut();
    static_imu.set_rotation(-145);
    pros::delay(200);

    int times = 25;
    for(int i=0; i<times; i++) {
        LeftSlapperMotor.tare_position();

        while(LeftSlapperMotor.get_position() < 820) {
            LeftSlapperMotor.move(100);
            pros::delay(10);
        }

        LeftSlapperMotor.brake();

        if(i != times-1)
            pros::delay(1500);
    }

    driveChassis.TurnPid(-10, 2);
    driveChassis.MovePid(-900, 1, 3, true);
    driveChassis.TurnPid(-180-static_imu.get_rotation(), 2);

    //FLOOR IT
    driveChassis.MovePid(-3100, 0.7, 2, true);
}