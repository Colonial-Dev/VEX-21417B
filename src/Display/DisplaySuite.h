//Note: Rendering area is 480x239 pixels
#pragma once
#include "pros/apix.h"
#include "InterfaceGlobals.h"
#include "GraphicsUtils.h"
#include "EventHandlers.h"

#define TEAM "VEX 21417B #00ff00 Intercept#"
#define VERSION "\nCompetition Suite v3.0.0"
#define CODENAME " \"Worldsbound\""
#define COMPILED "\nCompiled on " __DATE__ " at " __TIME__ "\n"

void configurePages()
{
    menu_bar = createMatrix(bar_map, 480, 25, SCREEN, LV_ALIGN_IN_TOP_LEFT, 0, 0, *handlePagination);
    home_page = createPage(SCREEN, 480, 215, LV_ALIGN_IN_TOP_MID, 0, 25);
    current_page = home_page;
    logs_page = createPage(SCREEN, 480, 215, LV_ALIGN_IN_TOP_MID, 0, 25, true);
    odom_page = createPage(SCREEN, 480, 215, LV_ALIGN_IN_TOP_MID, 0, 25, true);
    debug_page = createPage(SCREEN, 480, 215, LV_ALIGN_IN_TOP_MID, 0, 25, true);
}

void setupHomePage()
{
    status_display = createContainer(home_page, 240, 214, LV_ALIGN_IN_TOP_LEFT, 120, 0, LV_LAYOUT_COL_L);
    status_text = createLabel(status_display, "Initializing...", LV_LABEL_ALIGN_LEFT, LV_ALIGN_IN_TOP_LEFT, 0, 0);
    lv_label_set_long_mode(status_text, LV_LABEL_LONG_BREAK);
    lv_obj_set_size(status_text, 240, 10);


    auton_selector = createContainer(home_page, 240, 214, LV_ALIGN_IN_RIGHT_MID, -120, 0, LV_LAYOUT_COL_M);
    side_buttons = createMatrix(side_map, 240, 189, auton_selector, LV_ALIGN_IN_TOP_LEFT, 0, 0, *handleSideSelect);
    strat_buttons = createMatrix(strat_map, 240, 189, auton_selector, LV_ALIGN_IN_TOP_LEFT, 0, 0, *handleStratSelect);
    HIDE(strat_buttons)
    selector_cancel = createButton(auton_selector, "#FF0000 Cancel#", 240, 25, LV_ALIGN_IN_BOTTOM_MID, 0, 0, *handleSelectionCancel);
    HIDE(selector_cancel)
}

void setupLogsPage()
{
    console = createContainer(logs_page, 480, 214, LV_ALIGN_IN_TOP_LEFT, 240, 0, LV_LAYOUT_COL_M);
    tacit_log = createTextArea(console, 480, 189, LV_ALIGN_IN_TOP_MID, 0, 0, LV_CURSOR_NONE, "Initializing...");
    verbose_log = createTextArea(console, 480, 189, LV_ALIGN_IN_TOP_MID, 0, 0, LV_CURSOR_NONE, "Initializing...");
    HIDE(verbose_log)
    console_mode_buttons = createMatrix(mode_map, 480, 25, console, LV_ALIGN_IN_BOTTOM_MID, 0, 0, *handleConsoleModeSwitcher);
}

void splashDisplay()
{
    configureTheming();
    configurePages();
    setupHomePage();
    setupLogsPage();
    static uint32_t user_data = 10;
    lv_task_t * status_update_task = lv_task_create(statusUpdateTask, 50, LV_TASK_PRIO_MID, &user_data);
}