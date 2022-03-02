#pragma once
#include "ControllerMenu.h"
#include "DisplaySuite.h"
#include <string>

//Global UI constants
bool readyToLaunch = false;
bool autonTestFlag = false;
bool hotkeyMode = false;
int targetAutonSide = Right;
std::string targetAutonSideLabel = "R";
int targetAutonStrategy = DoubleRush;
std::string targetAutonStrategyLabel = "SpR";
float throttleMultiplier = 1.0f;

MenuManager manager;

MenuItem optionRun([] { readyToLaunch = true; }, "Run!");
MenuItem optionAutonSelect([] { manager.GotoLevel("AutonSideSelection"); }, "Auton Select");
MenuItem optionUtilities([] { manager.GotoLevel("Utilities"); }, "Utilities");
std::vector<MenuItem> mainItems
{
  {optionRun}, {optionAutonSelect}, {optionUtilities}
};
MenuLevel mainLevel(mainItems, "Main");

MenuItem optionSelectLeft([] { targetAutonSide = Left; targetAutonSideLabel = "L"; manager.GotoLevel("AutonStrategySelection"); }, "Left");
MenuItem optionSelectRight([] { targetAutonSide = Right; targetAutonSideLabel="R"; manager.GotoLevel("AutonStrategySelection"); }, "Right");
std::vector<MenuItem> sideSelectionItems
{
  {optionSelectLeft}, {optionSelectRight}
};
MenuLevel sideSelectionLevel(sideSelectionItems, "AutonSideSelection");

MenuItem optionSelectSimpleRush([] {targetAutonStrategy = SimpleRush; targetAutonStrategyLabel = "SR"; manager.GotoLevel("Main"); }, "Simple Rush");
MenuItem optionSelectDoubleRush([] {targetAutonStrategy = DoubleRush; targetAutonStrategyLabel = "DR"; manager.GotoLevel("Main"); }, "Double Rush");
MenuItem optionSelectStackRush([] {targetAutonStrategy = StackRush; targetAutonStrategyLabel = "StR"; manager.GotoLevel("Main"); }, "Stack Rush");
std::vector<MenuItem> strategySelectionItems
{
  {optionSelectDoubleRush}, {optionSelectStackRush}, {optionSelectSimpleRush}
};
MenuLevel strategySelectionLevel(strategySelectionItems, "AutonStrategySelection");

MenuItem optionAutonTest([] { autonTestFlag = true; readyToLaunch = true; }, "Auton Test");
MenuItem optionSkillsAuton([] { targetAutonSide = Skills; manager.GotoLevel("Main"); }, "Skills Auton");
MenuItem optionDisableAuton([] { targetAutonSide = Null; manager.GotoLevel("Main"); }, "Disable Auton");
std::vector<MenuItem> utilitiesItems
{
  {optionAutonTest}, {optionSkillsAuton}, {optionDisableAuton}
};
MenuLevel utilitiesLevel(utilitiesItems, "Utilities");

MenuItem hotkeyModeItem([] { manager.GotoLevel("Main"); }, "Hotkey mode");
std::vector<MenuItem> hotkeyModeItems { {hotkeyModeItem} };
MenuLevel hotkeyModeLevel(hotkeyModeItems, "Hotkey");

std::vector<MenuLevel> levels
{
  {mainLevel}, {sideSelectionLevel}, {strategySelectionLevel}, {utilitiesLevel}, {hotkeyModeLevel}
};

//R1/L1 to page through options
//A to select
void advanced_auton_select(pros::Controller master)
{
  readyToLaunch = false;
  while(!readyToLaunch)
  {
    if(pros::competition::is_connected()) {return;}
    if(master.get_digital_new_press(DIGITAL_R1)) { manager.Page(true); }
    if(master.get_digital_new_press(DIGITAL_L1)) { manager.Page(false); }
    if(master.get_digital_new_press(DIGITAL_A)) { manager.DoOperation(); }
    if(master.get_digital_new_press(DIGITAL_B)) { manager.GotoLevel("Main"); }

    if(master.get_digital_new_press(DIGITAL_RIGHT)) 
    { 
      if(manager.InHotkeyMode()) { manager.GotoLevel("Main"); }
      else { manager.GotoLevel("Hotkey"); }
    }
    if(master.get_digital_new_press(DIGITAL_UP) && manager.InHotkeyMode())
    {
      autonTestFlag = true;
      pros::delay(50);
      master.rumble("-");
      pros::delay(1000);
      master.rumble("-");
      pros::delay(1000);
      master.rumble("---");
      pros::delay(1000);
      master.set_text(0, 0, "Auton test START!" + spacerText);
      return;
    }
    if(master.get_digital_new_press(DIGITAL_DOWN) && manager.InHotkeyMode()) { abort(); }

    if(readyToLaunch) { return; }
    if(displayUpdateFlag) { displayUpdateFlag = false; controllerPrint(master, manager.GetCurrentItemName(), "> "); }
    pros::delay(2);
  }
  return;
}
