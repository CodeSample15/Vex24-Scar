#pragma once

#include <vector>
#include <string>
#include "api.h"

//basic utility class to be able to abstract a little of the code for the 
class AutonSelector 
{
public:
  AutonSelector();
  AutonSelector(int start);
  ~AutonSelector();

  void display_autons();
  void iterate(); //called when a user presses a button to change the auton selection by 1
  void add(std::string name, std::string description="", std::string description2="");

  int getSelected();

private:
  int selected;
  int numberOfAutons;

  std::vector<std::string> autons;
  std::vector<std::vector<std::string>> descriptions;
};