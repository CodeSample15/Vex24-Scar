#include "api.h"
#include "robot.h"
#include "autons/HelperMethods.h"

inline void BowlDownAlley() 
{
    backWingsOut();
    static_imu.set_rotation(-135);
    pros::delay(200);

    //shimmy shimmy (start facing backwards)
    for(int i=0; i<5; i++) {
        driveChassis.TurnPid(-45, 2, 1);

        if(i != 4)
            driveChassis.TurnPid(45, 2, 1);

        pros::delay(200);
    }

    //FLOOR IT
    driveChassis.MovePid(-4000, 0.7, 2, true);
}