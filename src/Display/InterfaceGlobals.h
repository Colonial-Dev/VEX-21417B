#pragma once
#define LVOBJ lv_obj_t *
#define SCREEN lv_scr_act()
#define HIDE(x) lv_obj_set_hidden(x, true);
#define SHOW(x) lv_obj_set_hidden(x, false);

//Global state constants
bool postStatus = false;
bool readyToLaunch = false;
bool autonJumpFlag = false;
bool driverJumpFlag = false;
bool hotkeyMode = false;
int targetAutonSide = Right;
int targetAutonStrategy = DoubleRush;
std::string targetAutonSideLabel = "";
std::string targetAutonStrategyLabel = "";

//Global LVGL objects/data
static const char * bar_map[] = {"Home", "Logs", "Odom", "Debug", ""}; 
static const char * side_map[] = {"#0000FF Left#", "#FF0000 Right#", "\n", 
                                  "#FFFF00 Skills#", "#808080 None#", ""}; 
static const char * strat_map[] = {"Alpha", "Beta", "\n", 
                                   "Gamma", "Delta", ""}; 
static const char * mode_map[] = {"Tacit", "Verbose", ""}; 

lv_theme_t * theme =  lv_theme_alien_init(180, NULL);
LVOBJ menu_bar;

LVOBJ current_page;
LVOBJ home_page;
LVOBJ logs_page;
LVOBJ odom_page;
LVOBJ debug_page;

LVOBJ status_display;
LVOBJ status_text;

LVOBJ auton_selector;
LVOBJ side_buttons;
LVOBJ strat_buttons;
LVOBJ selector_cancel;
LVOBJ selector_finalized;

LVOBJ console;
LVOBJ tacit_log;
LVOBJ verbose_log;
LVOBJ console_mode_buttons;

LVOBJ motor_data;
LVOBJ inertial_data;
LVOBJ threewire_data;
LVOBJ placeholder_data;
