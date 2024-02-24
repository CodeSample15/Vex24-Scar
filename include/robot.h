// define robot ports here
#pragma once

//what are we compiling to?
#define ORANGE_BOT false //false for red bot

#include "api.h"
#include "library/ChassisController.h"
#include "pros/adi.hpp"
#include "pros/misc.h"

#if ORANGE_BOT
    #define RIGHT_MOTOR_1_PORT 1
    #define RIGHT_MOTOR_2_PORT 2
    #define RIGHT_MOTOR_3_PORT 3
    #define RIGHT_MOTOR_4_PORT 4

    #define INERTIAL_PORT 5
    #define STATIC_INERTIAL_PORT 17 //named static because it never get's reset once the bot starts running

    #define INTAKE_PORT 6

    #define LEFT_MOTOR_1_PORT 7
    #define LEFT_MOTOR_2_PORT 8
    #define LEFT_MOTOR_3_PORT 9
    #define LEFT_MOTOR_4_PORT 10

    #define CLIMB_MOTOR_PORT 20

    #define SLAPPER_MOTOR_PORT 18

    #define FRONT_WINGS_PORT 'C'

    #define LEFT_BACK_WINGS_PORT 'B'
    #define RIGHT_BACK_WINGS_PORT 'A'
#else
    #define RIGHT_MOTOR_1_PORT 16
    #define RIGHT_MOTOR_2_PORT 17
    #define RIGHT_MOTOR_3_PORT 18
    #define RIGHT_MOTOR_4_PORT 19

    #define INERTIAL_PORT 19
    #define STATIC_INERTIAL_PORT 20

    #define INTAKE_PORT 10

    #define LEFT_MOTOR_1_PORT 11
    #define LEFT_MOTOR_2_PORT 12
    #define LEFT_MOTOR_3_PORT 13
    #define LEFT_MOTOR_4_PORT 14

    #define SLAPPER_MOTOR_PORT 21

    #define LEFT_BACK_WINGS_PORT 'G'
    #define RIGHT_BACK_WINGS_PORT 'H'
#endif


extern pros::Controller master;

extern pros::Motor rightMotor1;
extern pros::Motor rightMotor2;
extern pros::Motor rightMotor3;
extern pros::Motor rightMotor4;

extern pros::Motor leftMotor1;
extern pros::Motor leftMotor2;
extern pros::Motor leftMotor3;
extern pros::Motor leftMotor4;

extern pros::Motor IntakeMotor;
extern pros::Motor SlapperMotor;

extern pros::Motor_Group leftMotors;
extern pros::Motor_Group rightMotors;

#if ORANGE_BOT
    extern pros::Motor climbMotor;
#endif

extern pros::Imu imu;
extern pros::Imu static_imu;

#if ORANGE_BOT
    extern pros::ADIDigitalOut frontWings;
#endif

extern pros::ADIDigitalOut leftBackWings;
extern pros::ADIDigitalOut rightBackWings;

extern PID drivePid;
extern PID turnPid;
extern Chassis driveChassis;