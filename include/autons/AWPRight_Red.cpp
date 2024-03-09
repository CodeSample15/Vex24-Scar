#include "api.h"
#include "robot.h"
#include "autons/HelperMethods.h"


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
inline void runRightAwpAuton() {

    

    leftBackWingOut();
    static_imu.set_rotation(-145);
    
    pros::delay(200);
    LeftSlapperMotor.tare_position();

    int times = 13;
    PID p(1, 0, 0);
    int endingPosition = 0;
    for(int i=0; i<times; i++) {
        spinLeftSlapperToPosition(300*(i+1), 127);
        pros::delay(1500);

        /*

        
        LeftSlapperMotor.tare_position();

        
        int error = 300;
        while(abs(error) > 10 || LeftSlapperMotor.get_actual_velocity() < 10) {
            error = 300 - LeftSlapperMotor.get_position();
            double scale = -0.4;
            if(error > 150){
                scale = -0.8;
            }
            
            LeftSlapperMotor.move_velocity(error * 0.4);
            pros::delay(10);
            pros::lcd::print(0, "Error: %d", error);

        }

        LeftSlapperMotor.move_velocity(0);
        LeftSlapperMotor.brake();


        if(i != times-1)
            pros::delay(1000);*/
    }

    leftBackWingIn();

    
    driveChassis.MovePid(-650, 1, 3, true);
    driveChassis.TurnPid(-180-static_imu.get_rotation(), 2);
    rightBackWingOut();
    pros::delay(300);
    driveChassis.MovePid(-3500, 0.6, 3, true);
    driveChassis.MovePid(-500, 0.5, 3, true);
    rightBackWingIn();
    leftBackWingOut();
    pros::delay(300);
    driveChassis.TurnPid(-210-static_imu.get_rotation(), 2);
    driveChassis.MovePid(-1000, 1, 3, true);
    driveChassis.TurnPid(-270-static_imu.get_rotation(), 2);
    driveChassis.MovePid(-1000, 1, 3, true);
    
}