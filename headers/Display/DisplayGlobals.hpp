#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "pros/apix.h"

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
  Reset,
  Kill
};

//Global state variables
extern int targetAutonSide;
extern int targetAutonStrategy;
extern std::string targetAutonSideLabel;
extern std::string targetAutonStrategyLabel;

//Global LVGL objects/data
static const char * switcher_map[] = {"Status", "Controls", ""}; 
static const char * controls_map[] = {"#00ff00 Driver#", "\n",
                                      "#0000ff Autonomous#", "\n",
                                      "#FFA500 Reset#", "\n",
                                      "#ff0000 Abort#", ""}; 
static const char * side_map[] = {"#0000FF Left#", "#FF0000 Right#", "\n", 
                                  "#FFFF00 Skills#", "#808080 None#", ""}; 
static const char * strat_map[] = {"Alpha", "Beta", "\n", 
                                   "Gamma", "Delta", ""}; 

extern lv_theme_t * theme;
extern lv_style_t status_style;
extern lv_style_t confirm_style;

extern LVOBJ home_page;
 
extern LVOBJ left_panel;
extern LVOBJ status_page;
extern LVOBJ controls_page;
extern LVOBJ status_text;
extern LVOBJ status_mode_switcher;
extern LVOBJ mode_controls;
extern LVOBJ autonomous_mask;
 
extern LVOBJ right_panel;
extern LVOBJ side_page;
extern LVOBJ strat_page;
extern LVOBJ ready_page;
extern LVOBJ waiting_label;
extern LVOBJ side_buttons;
extern LVOBJ strat_buttons;
extern LVOBJ selector_cancel;
extern LVOBJ selector_finalized;
