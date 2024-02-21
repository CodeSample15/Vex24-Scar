/*
  Author: Luke Crimi
  File: AutonSelector.cpp
  Date Created: 1/30/2022
  
  Use this class to easily select autons in the preauton function
*/

#include <vector>
#include <cstring>
#include "AutonSelector.h"
#include "robot.h"

AutonSelector::AutonSelector() 
{
  AutonSelector::selected = 0;
  AutonSelector::numberOfAutons = 0;
}

AutonSelector::AutonSelector(int start)
{
  AutonSelector::selected = start;
  AutonSelector::numberOfAutons = 0;
}

AutonSelector::~AutonSelector() 
{
  //clear vectors
  AutonSelector::autons.clear();
  AutonSelector::descriptions.clear();
}

//return the selected auton
int AutonSelector::getSelected() 
{
  return AutonSelector::selected;
}

void AutonSelector::add(std::string name, std::string description, std::string description2)
{
  AutonSelector::numberOfAutons++;

  AutonSelector::autons.push_back(name);

  std::vector<std::string> stringVector;
  stringVector.push_back(description);
  stringVector.push_back(description2);
  AutonSelector::descriptions.push_back(stringVector);
}

//change the current selected auton and give haptic feedback to the driver
void AutonSelector::iterate() 
{
  AutonSelector::selected++;

  if(AutonSelector::selected >= AutonSelector::numberOfAutons)
    AutonSelector::selected = 0;

  master.rumble(".");
}

//display the current auton on the remote screen
void AutonSelector::display_autons() 
{
  master.clear();
  pros::delay(50);
  master.clear();
  pros::delay(50);

  master.print(0, 0, "%s", AutonSelector::autons[AutonSelector::selected].c_str());
  pros::delay(50);
  master.print(1, 0, "%s", AutonSelector::descriptions[AutonSelector::selected][0].c_str());
  pros::delay(50);
  master.print(2, 0, "%s", AutonSelector::descriptions[AutonSelector::selected][1].c_str());
  pros::delay(50);
}