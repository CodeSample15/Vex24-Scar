#include "api.h"
#include "robot.h"
#include "autons/HelperMethods.h"

inline void BowlDownAlley() 
{
    leftBackWingOut();
    static_imu.set_rotation(-145);
    pros::delay(200);

    for(int i=0; i<10; i++) {
        SlapperMotor.tare_position();

        while(SlapperMotor.get_position() < 820) {
            SlapperMotor.move(100);
            pros::delay(10);
        }

        SlapperMotor.brake();

        if(i != 9)
            pros::delay(1500);
    }

    driveChassis.TurnPid(-10, 2);
    driveChassis.MovePid(-900, 1, 3, true);
    driveChassis.TurnPid(-180-static_imu.get_rotation(), 2);

    //FLOOR IT
    driveChassis.MovePid(-3000, 0.7, 2, true);

    driveChassis.TurnPid(-45, 2, 0);
    driveChassis.MovePid(-1300, 1, 5, true);
    driveChassis.TurnPid(-45, 2, 0);

    leftBackWingIn();
}