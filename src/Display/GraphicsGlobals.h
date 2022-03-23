#pragma once
#define LVOBJ lv_obj_t *
#define SCREEN lv_scr_act()
#define HIDE(x) lv_obj_set_hidden(x, true);
#define SHOW(x) lv_obj_set_hidden(x, false);

//Button enums
enum AutonSelectionStage
{
  Side,
  Strategy,
  Complete
};

enum LeftPanelSwitcher
{
  Status,
  Controls
};

enum ModeControls
{
  Driver,
  Autonomous,
  Disable,
  Terminate
};

//Global state constants
int targetAutonSide = Right;
int targetAutonStrategy = DoubleRush;
std::string targetAutonSideLabel = "";
std::string targetAutonStrategyLabel = "";

//Global LVGL objects/data
static const char * switcher_map[] = {"Status", "Controls", ""}; 
static const char * controls_map[] = {"#00ff00 Driver#", "\n",
                                      "#0000ff Autonomous#", "\n",
                                      "#808080 Disable#", "\n",
                                      "#ff0000 Terminate#", ""}; 
static const char * side_map[] = {"#0000FF Left#", "#FF0000 Right#", "\n", 
                                  "#FFFF00 Skills#", "#808080 None#", ""}; 
static const char * strat_map[] = {"Alpha", "Beta", "\n", 
                                   "Gamma", "Delta", ""}; 

lv_theme_t * theme =  lv_theme_alien_init(180, NULL);
static lv_style_t status_style;
static lv_style_t confirm_style;

LVOBJ home_page;

LVOBJ left_panel;
LVOBJ status_page;
LVOBJ controls_page;
LVOBJ status_text;
LVOBJ status_mode_switcher;
LVOBJ mode_controls;
LVOBJ autonomous_mask;

LVOBJ right_panel;
LVOBJ side_page;
LVOBJ strat_page;
LVOBJ ready_page;
LVOBJ waiting_label;
LVOBJ side_buttons;
LVOBJ strat_buttons;
LVOBJ selector_cancel;
LVOBJ selector_finalized;
