//initialize robot components here
#include "pros/adi.hpp"
#include "pros/misc.h"
#include "robot.h"

pros::Controller master = pros::Controller(pros::E_CONTROLLER_MASTER);

#if ORANGE_BOT
    pros::Motor rightMotor1 = pros::Motor(RIGHT_MOTOR_1_PORT);
    pros::Motor rightMotor2 = pros::Motor(RIGHT_MOTOR_2_PORT, true);
    pros::Motor rightMotor3 = pros::Motor(RIGHT_MOTOR_3_PORT);
    pros::Motor rightMotor4 = pros::Motor(RIGHT_MOTOR_4_PORT, true);

    pros::Motor leftMotor1 = pros::Motor(LEFT_MOTOR_1_PORT, true);
    pros::Motor leftMotor2 = pros::Motor(LEFT_MOTOR_2_PORT);
    pros::Motor leftMotor3 = pros::Motor(LEFT_MOTOR_3_PORT, true);
    pros::Motor leftMotor4 = pros::Motor(LEFT_MOTOR_4_PORT);
#else
    pros::Motor rightMotor1 = pros::Motor(RIGHT_MOTOR_1_PORT, true);
    pros::Motor rightMotor2 = pros::Motor(RIGHT_MOTOR_2_PORT);
    pros::Motor rightMotor3 = pros::Motor(RIGHT_MOTOR_3_PORT, true);
    pros::Motor rightMotor4 = pros::Motor(RIGHT_MOTOR_4_PORT);

    pros::Motor leftMotor1 = pros::Motor(LEFT_MOTOR_1_PORT);
    pros::Motor leftMotor2 = pros::Motor(LEFT_MOTOR_2_PORT, true);
    pros::Motor leftMotor3 = pros::Motor(LEFT_MOTOR_3_PORT);
    pros::Motor leftMotor4 = pros::Motor(LEFT_MOTOR_4_PORT, true);
#endif

pros::Motor_Group leftMotors = pros::Motor_Group({leftMotor1, leftMotor2, leftMotor3, leftMotor4});
pros::Motor_Group rightMotors = pros::Motor_Group({rightMotor1, rightMotor2, rightMotor3, rightMotor4});

pros::Motor IntakeMotor = pros::Motor(INTAKE_PORT);
#if ORANGE_BOT
    pros::Motor climbMotor = pros::Motor(CLIMB_MOTOR_PORT);
#endif

pros::Imu imu = pros::Imu(INERTIAL_PORT);
pros::Imu static_imu = pros::Imu(STATIC_INERTIAL_PORT);

pros::ADIDigitalOut frontWings = pros::ADIDigitalOut(FRONT_WINGS_PORT);
pros::ADIDigitalOut backWings = pros::ADIDigitalOut(BACK_WINGS_PORT);

#if ORANGE_BOT
    PID drivePid = PID(0.5, 0.02, 1.5, 20, 20, 3, 30);
    PID turnPid = PID(1.4, 0.06, 0.85, 20, 15, 1, 20);
#else
    PID drivePid = PID(0.5, 0.02, 1.5, 20, 20, 3, 30);
    PID turnPid = PID(1.25, 0.02, 0.95, 20, 14, 2, 20);
#endif

Chassis driveChassis = Chassis(leftMotors, rightMotors, imu, drivePid, turnPid);