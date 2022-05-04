#pragma once
#include "robokauz/PRELUDE.hpp"
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
  Unlock,
  Lock,
  Autonomous,
  Reset,
  Kill
};

//Global state variables
extern int targetAutonSide;
extern int targetAutonStrategy;

//Global LVGL objects/data
static const char * switcher_map[] = {"Status", "Controls", ""}; 

static const char * controls_map_unlocked[] = {"\261#808080 Unlocked#", "#ffff00 Lock#" , "\n",
                                      "\221#0000ff Run Autonomous#", "\n",
                                      "#FFA500 Reinitialize#", "\n",
                                      "#ff0000 E-Stop#", ""}; 

static const char * controls_map_locked[] = {"#00ff00 Unlock#", "\261#808080 Locked#" , "\n",
"\221#0000ff Run Autonomous#", "\n",
"#FFA500 Reinitialize#", "\n",
"#ff0000 E-Stop#", ""}; 

static const char * side_map[] = {"#0000FF Left#", "#FF0000 Right#", "\n", 
                                  "#FFFF00 Skills#", "#808080 None#", ""}; 
static const char * strat_map[] = {"Win Point (Full)", "\n",
                                   "Win Point (Half)", "\n", 
                                   "Single Rush", "\n",
                                   "Double Rush", ""}; 

extern lv_theme_t * theme;
extern lv_style_t status_style;
extern lv_style_t confirm_style;
extern lv_style_t symbol_style;

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
extern LVOBJ symbol_label;
extern LVOBJ ready_label;
extern LVOBJ ready_container;
extern LVOBJ waiting_label;
extern LVOBJ side_buttons;
extern LVOBJ strat_buttons;
extern LVOBJ selector_cancel;
extern LVOBJ selector_finalized;
