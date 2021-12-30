#ifndef UI_GLOBALS_H
#define UI_GLOBALS_H

//Global UI constants
bool ready = false;
bool auton_test = false;

std::string home_options[] = { "Run!", "Auton Select" , "Utilities", "Exit", "Left", "Right", "Direct Rush", "Middle Rush", "Auton OK?", "Abort auton",
"Hotkey mode", "Auto-alignment", "Auton test", "Skills auton", "Disable auton"};

int auton_menustage = Home;
int array_lowerbound = 0;
int array_upperbound = 3;
int array_index = 0;
bool updateneeded = true;

int auton_side = Right; //1 indicates left, 2 indicates right
std::string auton_sidelabel = "DF";
int auton_variant = DirectRush; //1-?, indicates what moves to make
std::string auton_varlabel = "DF";

float multiplier = 1.0f;

#endif
