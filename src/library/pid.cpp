#include "library/pid.h"
#include "api.h"

#include <iostream>

//the robot will use the I term if the error is less than IMax, but greater than IMin
PID::PID(double Kp, double Ki, double Kd, double dt, double IMax, double IMin, double MaxI)
{
  PID::_Kp = Kp;
  PID::_Ki = Ki;
  PID::_Kd = Kd;
  PID::_dt = dt;
  PID::_IMin = IMin;
  PID::_IMax = IMax;
  PID::_MaxI = MaxI;

  PID::_integral = 0;
  PID::_prev_error = 0;
}

double PID::calculate(double error, bool delay)
{
    double p = error * _Kp;

    if(std::abs(error) > _IMax || std::abs(error) < _IMin)
      _integral = 0;
    else
      _integral += error;

    if(std::abs(_integral) > _MaxI)
        _integral = _MaxI * (_integral<0 ? -1:1);

    double i = _integral * _Ki;
    double d = error - _prev_error;
    _prev_error = error;

    if(delay)
      pros::delay(_dt);
      
    return p + i + d;
}