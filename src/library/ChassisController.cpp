#include "library/ChassisController.h"
#include <iostream>

#define PI 3.1415926

Chassis::Chassis(pros::Motor_Group& left, pros::Motor_Group& right, pros::Imu& gyro) {
    Chassis::left = &left;
    Chassis::right = &right;
    Chassis::gyro = &gyro;

    Chassis::unit = Chassis::drive_units::en;
    Chassis::wheelRadius = 0;

    Chassis::pid = PID(1, 0, 0);
    Chassis::turnPid = PID(4, 0, 0);
}

Chassis::Chassis(pros::Motor_Group& left, pros::Motor_Group& right, pros::Imu& gyro, drive_units units, double wheelRadius) {
    Chassis::left = &left;
    Chassis::right = &right;
    Chassis::gyro = &gyro;
    
    Chassis::unit = units;
    Chassis::wheelRadius = wheelRadius;

    Chassis::pid = PID(1, 0, 0);
    Chassis::turnPid = PID(4, 0, 0);
}

Chassis::Chassis(pros::Motor_Group& left, pros::Motor_Group& right, pros::Imu& gyro, PID& pid, PID& turnPid) {
    Chassis::left = &left;
    Chassis::right = &right;
    Chassis::gyro = &gyro;

    Chassis::unit = Chassis::drive_units::en;
    Chassis::wheelRadius = 0;

    Chassis::pid = pid;
    Chassis::turnPid = turnPid;
}

Chassis::Chassis(pros::Motor_Group& left, pros::Motor_Group& right, pros::Imu& gyro, PID& pid, PID& turnPid, drive_units units, double wheelRadius) {
    Chassis::left = &left;
    Chassis::right = &right;
    Chassis::gyro = &gyro;

    Chassis::unit = units;
    Chassis::wheelRadius = wheelRadius;

    Chassis::pid = pid;
    Chassis::turnPid = turnPid;
}

void Chassis::DriveArcade(double drive, double turn, float turn_expo, float drive_expo) {
     //apply expo for turning
     turn /= 127;
     bool negative = turn<0;
     turn = pow(turn, turn_expo);

     if(turn>0 && negative)
        turn*=-1;

    turn *= 127;

    //apply expo for driving
    drive /= 127;
    negative = drive<0;
    drive = pow(drive, drive_expo);

     if(drive>0 && negative)
        drive*=-1;

    drive *= 127;

    int rightSpeed = int(drive-turn);
    int leftSpeed = int(drive+turn);

    right->move(rightSpeed);
    left->move(leftSpeed);
}

void Chassis::DriveTank(double left, double right) {
    Chassis::right->move(right);
    Chassis::left->move(left);
}

void Chassis::MovePid(double distance, float speed_m, float slewrate, bool inertialLock) {
    resetMotors();

    double slew=0;
    double averageMotorSpeed = 99999;
    double a_right = 0;
    double a_left = 0;

    speed_m = std::abs(speed_m);

    if(inertialLock)
        gyro->tare();

    while(std::abs(distance-getAveragePosition(true)) > 5 || std::abs(averageMotorSpeed)>1) {
        double s = pid.calculate(distance-getAveragePosition(true));
        double t = inertialLock ? gyro->get_rotation() * 5 : 0;

        if(std::abs(s) > 127)
            s = 127 * (s<0 ? -1 : 1);

        s*=speed_m;

        if(std::abs(s) < MIN_MOTOR_SPEED)
            s = MIN_MOTOR_SPEED * (s<0 ? -1 : 1);
            
        if(slewrate>0 && std::abs(slew)<std::abs(s)) {
            s = slew;
            slew += slewrate * (distance<0 ? -1 : 1);
        }
        
        right->move(s + t);
        left->move(s - t);

        a_right = std::abs(average(right->get_actual_velocities()));
        a_left = std::abs(average(left->get_actual_velocities()));
        averageMotorSpeed = (a_right + a_left) / 2;
    }

    right->brake();
    left->brake();
}

void Chassis::Move(double distance, int speed, float slewrate, int timeout, bool inertialLock) {
    resetMotors();

    double slew=0;
    int elapsedTime = 0;
    speed = std::abs(speed);

    if(inertialLock)
        gyro->tare();

    while(distanceToEncoder(std::abs(distance)) > std::abs(getAveragePosition(true))) {
        //handle timeout
        if(timeout != -1) {
            elapsedTime += 20;

            if(elapsedTime >= timeout)
                break;
        }

        int s = speed * (distance<0?-1:1);
        int t = inertialLock ? gyro->get_rotation() * 5 : 0; 

        if(slewrate>0 && slew<s) {
            s = (int)slew;
            slew += slewrate * (distance<0?-1:1);
        }

        right->move(s + t);
        left->move(s - t);

        pros::delay(20);
    }

    right->brake();
    left->brake();
}

void Chassis::TurnPid(int degrees, float speed_m, int disable) {
    gyro->tare_rotation();
    double averageMotorSpeed = 99999;
    double a_right = 0;
    double a_left = 0;

    speed_m = std::abs(speed_m);

    while(std::abs(degrees - gyro->get_rotation()) > 4 || std::abs(averageMotorSpeed) > 1) {
        std::cout << std::abs(degrees - gyro->get_rotation()) << std::endl;
        double s = turnPid.calculate(degrees - gyro->get_rotation());
        if(std::abs(s) > 127)
            s = 127 * (s<0 ? -1 : 1);

        s*=speed_m;

        if(std::abs(s) < MIN_MOTOR_SPEED)
            s = MIN_MOTOR_SPEED * (s<0 ? -1 : 1);

        if(disable != 1)
            right->move(-s);
        if(disable != 0)
            left->move(s);

        a_right = std::abs(average(right->get_actual_velocities()));
        a_left = std::abs(average(left->get_actual_velocities()));
        averageMotorSpeed = (a_right + a_left) / 2;
    }

    right->brake();
    left->brake();
}

void Chassis::Turn(int degrees, int speed, int disable) {
    gyro->tare_rotation();
    speed = std::abs(speed);

    while(std::abs(gyro->get_rotation()) < std::abs(degrees)) {
        double s = speed * (degrees<0?-1:1);

        if(disable != 1)
            right->move(-s);
        if(disable != 0)
            left->move(s);

        pros::delay(20);
    }

    right->brake();
    left->brake();
}

double Chassis::getAverageRightPosition(bool rawEncoder) {
    std::vector<double> positions = right->get_positions();
    double total = 0;
    for(int i=0; i<positions.size(); i++)
        total += positions[i];
    total /= positions.size();

    //translate the distances into the correct units
    if(!rawEncoder && unit == drive_units::in || unit == drive_units::cm)
        return (total / 360) * (2*PI*wheelRadius);
    
    return total;
}

double Chassis::getAverageLeftPosition(bool rawEncoder) {
    std::vector<double> positions = left->get_positions();
    double total = 0;
    for(int i=0; i<positions.size(); i++)
        total += positions[i];
    total /= positions.size();

    //translate the distances into the correct units
    if(!rawEncoder && unit == drive_units::in || unit == drive_units::cm)
        return (total / 360) * (2*PI*wheelRadius);

    return total;
}

double Chassis::getAveragePosition(bool rawEncoder) {
    return (getAverageRightPosition(rawEncoder) + getAverageLeftPosition(rawEncoder)) / 2;
}

void Chassis::resetMotors() {
    right->tare_position();
    left->tare_position();
}

int Chassis::distanceToEncoder(double distance) {
    if (unit == drive_units::in || unit == drive_units::cm)
        return (int)((distance / (2*PI*wheelRadius)) * 360);
    else
        return (int)distance;
}

double Chassis::average(std::vector<double> values) {
    double sum = 0;
    for(int i=0; i<values.size(); i++)
        sum += values.at(i);
    return sum / values.size();
}