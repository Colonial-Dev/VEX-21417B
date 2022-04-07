#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "pros/apix.h"
#include "robokauz/Display/DisplayGlobals.hpp"
#include "robokauz/ROBOT.hpp"

//Global state variables
int targetAutonSide = Right;
int targetAutonStrategy = DoubleRush;
std::string targetAutonSideLabel = "";
std::string targetAutonStrategyLabel = "";

lv_theme_t * theme =  lv_theme_alien_init(180, NULL);
lv_style_t status_style;
lv_style_t confirm_style;

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
