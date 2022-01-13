#pragma once
#include "menu.h"

//Global UI constants
bool readyToLaunch = false;
bool autonTestFlag = false;
bool liftAlignmentFlag = false;
bool hotkeyMode = false;
int targetAutonSide = Right;
std::string targetAutonSideLabel = "DF";
int targetAutonStrategy = DirectRush;
std::string targetAutonStrategyLabel = "DF";
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
MenuItem optionSelectDirectRush([] {targetAutonStrategy = DirectRush; targetAutonStrategyLabel = "DR"; manager.GotoLevel("Main"); }, "Direct Rush");
MenuItem optionSelectMiddleRush([] {targetAutonStrategy = MiddleRush; targetAutonStrategyLabel = "MR"; manager.GotoLevel("Main"); }, "Middle Rush");
MenuItem optionSelectComplexRush([] {targetAutonStrategy = ComplexRush; targetAutonStrategyLabel = "CR"; manager.GotoLevel("Main"); }, "Complex Rush");
std::vector<MenuItem> strategySelectionItems
{
  {optionSelectSimpleRush}, {optionSelectDirectRush}, {optionSelectMiddleRush}, {optionSelectComplexRush}
};
MenuLevel strategySelectionLevel(strategySelectionItems, "AutonStrategySelection");

MenuItem optionAutoAlignment([] { liftAlignmentFlag = true; readyToLaunch = true; }, "Auto-alignment");
MenuItem optionAutonTest([] { autonTestFlag = true; readyToLaunch = true; }, "Auton Test");
MenuItem optionSkillsAuton([] { targetAutonSide = Skills; manager.GotoLevel("Home"); }, "Skills Auton");
MenuItem optionDisableAuton([] { targetAutonSide = Null, manager.GotoLevel("Home"); }, "Disable auton");
std::vector<MenuItem> utilitiesItems
{
  {optionAutoAlignment}, {optionAutonTest}, {optionSkillsAuton}, {optionDisableAuton}
};
MenuLevel utilitiesLevel(utilitiesItems, "Utilities");

std::vector<MenuLevel> levels
{
  {mainLevel}, {sideSelectionLevel}, {strategySelectionLevel}, {utilitiesLevel}
};

void menuPrint(pros::Controller master)
{
    if(!pros::competition::is_connected()){pros::delay(50);}
    else{pros::delay(10);}
    master.set_text(0, 0, "> " + manager.GetCurrentItemName() + "                                        end");
}

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

    if(master.get_digital_new_press(DIGITAL_RIGHT)) { hotkeyMode = !hotkeyMode; }
    if(master.get_digital_new_press(DIGITAL_LEFT) && hotkeyMode) { liftAlignmentFlag = true; return; }
    if(master.get_digital_new_press(DIGITAL_UP) && hotkeyMode)
    {
      autonTestFlag = true;
      pros::delay(50);
      master.rumble("-");
      pros::delay(1000);
      master.rumble("-");
      pros::delay(1000);
      master.rumble("---");
      pros::delay(1000);
      return;
    }

    if(displayUpdateFlag) { displayUpdateFlag = false; menuPrint(master); }
    pros::delay(2);
  }
  return;
}
