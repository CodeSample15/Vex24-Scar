#pragma once
#include "robot.h"

#if ORANGE_BOT
void frontWingsOut();
void frontWingsIn();
#endif

void leftBackWingOut();
void leftBackWingIn();
void rightBackWingOut();
void rightBackWingIn();

void backWingsIn();
void backWingsOut();

void startIntake(bool in=true);
void stopIntake();