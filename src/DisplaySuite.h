#pragma once
#include "pros/apix.h"
#define TEAM "VEX 21417B #00ff00 Intercept#"
#define VERSION "\nCompetition Suite v3.0.0"
#define CODENAME " \"Worldsbound\""
#define COMPILED "\nCompiled on " __DATE__ " at " __TIME__ "\n"
std::string spacerText = "                                                            ";

int motorPorts [8] = {12, 15, 16, 19, 6, 9, 8, 1};
std::vector<int> failedMotors;

lv_obj_t * outputLabel =  lv_label_create(lv_scr_act(), NULL);
lv_obj_t * debugLabel = lv_label_create(lv_scr_act(), NULL);

bool POST()
{
    return true;
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

    }
    else
    {
        
    }
}