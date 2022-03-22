//Note: Rendering area is 480x239 pixels
#pragma once
#include "pros/apix.h"
#include "InterfaceSetup.h"
#include "GraphicsUtils.h"
#include "EventHandlers.h"

#define TEAM "VEX 21417B #00ff00 Intercept#"
#define VERSION "\nCompetition Suite v3.0.0"
#define CODENAME " \"Worldsbound\""
#define COMPILED "\nCompiled on " __DATE__ " at " __TIME__ "\n"

void setupHomePage()
{
    left_panel = createContainer(home_page, 240, 239, LV_ALIGN_IN_TOP_LEFT, 120, 0, LV_LAYOUT_OFF);
    status_mode_switcher = createMatrix(switcher_map, 240, 25, left_panel, LV_ALIGN_IN_BOTTOM_MID, 0, -1, *handleStatusModeSwitcher);
    
    status_page = createPage(left_panel, 240, 214, LV_ALIGN_IN_TOP_LEFT, 0, 0, false);
    status_text = createLabel(status_page, "Initializing...", LV_LABEL_ALIGN_LEFT, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_long_mode(status_text, LV_LABEL_LONG_BREAK);
    lv_obj_set_style(status_text, &status_style);
    lv_obj_set_size(status_text, 260, 214);

    controls_page = createPage(left_panel, 240, 214, LV_ALIGN_IN_TOP_LEFT, 0, 0, true);
    mode_controls = createMatrix(controls_map, 240, 214, controls_page, LV_ALIGN_IN_TOP_MID, 0, 0, *handleControls);

    right_panel = createContainer(home_page, 240, 239, LV_ALIGN_IN_RIGHT_MID, -120, 0, LV_LAYOUT_COL_M);
    
    side_page = createPage(right_panel, 240, 239, LV_ALIGN_IN_TOP_LEFT, 0, 0, false);
    side_buttons = createMatrix(side_map, 240, 214, side_page, LV_ALIGN_IN_TOP_LEFT, 0, 0, *handleSideSelect);
    waiting_label = createLabel(side_page, "Waiting...", LV_LABEL_ALIGN_CENTER, LV_ALIGN_IN_BOTTOM_MID, 0, 23);
    
    strat_page = createPage(right_panel, 240, 239, LV_ALIGN_IN_TOP_LEFT, 0, 0, true);
    strat_buttons = createMatrix(strat_map, 240, 214, strat_page, LV_ALIGN_IN_TOP_LEFT, 0, 0, *handleStratSelect);
    selector_cancel = createButton(strat_page, "#FF0000 Cancel#", 240, 25, LV_ALIGN_IN_BOTTOM_MID, 0, 24, *handleSelectionCancel);

    ready_page = createPage(right_panel, 240, 239, LV_ALIGN_IN_TOP_LEFT, 0, 0, true);
}

void splashDisplay()
{
    configureTheming();
    home_page = createPage(SCREEN, 480, 239, LV_ALIGN_IN_TOP_MID, 0, 0);
    lv_style_copy(&status_style, home_page->style_p);
    lv_style_copy(&confirm_style, home_page->style_p);
    configureStyling();
    setupHomePage();

    static uint32_t user_data = 10;
    lv_task_t * status_update_task = lv_task_create(statusUpdateTask, 50, LV_TASK_PRIO_MID, &user_data);
}