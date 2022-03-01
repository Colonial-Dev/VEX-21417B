#pragma once
#include "pros/apix.h"
#define TEAM "VEX 21417A #ff0000 Invictus#"
#define VERSION "\nCompetition Suite v2.2.0"
#define CODENAME " \"Six-Wheel Monstrosity\""
#define COMPILED "\nCompiled on " __DATE__ " at " __TIME__ "\n"
std::string spacerText = "                                                            ";

int motorPorts [8] = {12, 15, 16, 19, 6, 9, 8, 1};
std::vector<int> failedMotors;

int holdCycleCount = 0;
std::string holdCycleMessages [3] = {"Holding...", "Press DOWN or", "plug in comp switch."};

lv_obj_t * outputLabel =  lv_label_create(lv_scr_act(), NULL);
lv_obj_t * debugLabel = lv_label_create(lv_scr_act(), NULL);

bool POST()
{
    for(int i = 0; i < 8; i++)
    {
        pros::Motor motor (motorPorts[i]);
        motor.get_power();
        if(errno == ENODEV)
        {
            failedMotors.push_back(motorPorts[i]);
            errno = 0;
        }
    }

    if(!failedMotors.empty())
    {
        return false;
    }
    else
    {
        return true;
    }
}

void brainPrint(std::string str)
{
    std::string newlined = str + "\n";
    lv_label_ins_text(outputLabel, LV_LABEL_POS_LAST, newlined.c_str());
}

void controllerPrint(pros::Controller master, std::string content, std::string prefix = "")
{
  if(!pros::competition::is_connected()){pros::delay(50);}
  else{pros::delay(10);}
  master.set_text(0, 0, prefix + content + spacerText);
}

void holdCycle()
{
  pros::Controller master(pros::E_CONTROLLER_MASTER);
  while(true)
  {
    holdCycleCount++;
    if(holdCycleCount > 2) { holdCycleCount = 0; }
    master.set_text(0, 0, holdCycleMessages[holdCycleCount] + spacerText);
    pros::delay(2500);
  }
}

void debugPrint(std::string str)
{
    lv_label_set_text(debugLabel, str.c_str());
}

void splashDisplay()
{
    lv_label_set_text(outputLabel, TEAM VERSION CODENAME COMPILED);
    lv_label_set_text(debugLabel, "Debug line ready.");

    lv_obj_align(outputLabel, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_obj_align(debugLabel, NULL, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0);

    lv_label_set_recolor(outputLabel, true);  

    if(POST() == false)
    {
        brainPrint("#ff0000 [ERR]# Power-On Self Test failure!");
        std::string failedPortNums = "Failed ports: ";
        for(int i = 0; i < failedMotors.size(); i++)
        {
            failedPortNums = failedPortNums + std::to_string(failedMotors.at(i)) + " ";
        }
        brainPrint(failedPortNums);
    }
    else
    {
        brainPrint("Power-On Self Test #00ff00 [OK]#");
    }
}