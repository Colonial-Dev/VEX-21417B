#pragma once
#define LVOBJ lv_obj_t *
#define SCREEN lv_scr_act()
#define HIDE(x) lv_obj_set_hidden(x, true);
#define SHOW(x) lv_obj_set_hidden(x, false); current_page = x;

//Global state constants
bool readyToLaunch = false;
bool autonJumpFlag = false;
bool driverJumpFlag = false;
bool hotkeyMode = false;
int targetAutonSide = Right;
int targetAutonStrategy = DoubleRush;
std::string targetAutonSideLabel = "";
std::string targetAutonStrategyLabel = "";

//Global LVGL objects/data
static const char * bar_map[] = {"Home", "Hardware", "Odom", "Debug", ""}; 
static const char * side_map[] = {"Right", "Left", "\n", 
                                  "Skills", "None", ""}; 
static const char * strat_map[] = {"Alpha", "Beta", "\n", 
                                   "Gamma", "Delta", ""}; 
static const char * mode_map[] = {"Tacit", "Verbose", ""}; 

lv_theme_t * theme =  lv_theme_alien_init(180, NULL);
LVOBJ menu_bar;

LVOBJ current_page;
LVOBJ home_page;
LVOBJ hardware_page;
LVOBJ odom_page;
LVOBJ debug_page;

LVOBJ auton_selector;
LVOBJ side_buttons;
LVOBJ strat_buttons;
LVOBJ selector_progress;

LVOBJ console;
LVOBJ tacit_log;
LVOBJ verbose_log;
LVOBJ console_mode_buttons;
