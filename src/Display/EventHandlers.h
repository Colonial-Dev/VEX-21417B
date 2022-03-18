#pragma once

void statusUpdateTask(void*)
{
    std::string statusReadout = "";
    statusReadout += "POST OK!\n";
    statusReadout += "Mode: Operator\n";
    statusReadout += "Battery: " + to_string(int (pros::battery::get_capacity())) + "%\n";
    statusReadout += to_string(right_encoder.get_value()) + " | " + to_string(middle_encoder.get_value()) + " | " + to_string(left_encoder.get_value()) + "\n";
    statusPrint(statusReadout);
}

lv_res_t handlePagination(lv_obj_t * obj, const char *txt)
{
    int activated = lv_btnm_get_pressed(obj);
    
    HIDE(current_page)
    if(activated == Home) { SHOW(home_page) HIDE(verbose_log) SHOW(tacit_log) }
    else if(activated == Logs) { SHOW(logs_page) current_page = logs_page; }
    else if(activated == Odom) { SHOW(odom_page) current_page = odom_page; }
    else if(activated == Debug) { SHOW(debug_page) current_page = debug_page; }
    return LV_RES_OK;
}

lv_res_t handleConsoleModeSwitcher(lv_obj_t * obj, const char *txt)
{
    int activated = lv_btnm_get_pressed(obj);
    if(activated == 1) { HIDE(tacit_log) SHOW(verbose_log) }
    else if(activated == 0) { HIDE(verbose_log) SHOW(tacit_log) }
    return LV_RES_OK;
}

void switchSelectorStage(int targetStage)
{
    switch(targetStage)
    {
        case Side:
        {
            HIDE(selector_cancel)
            HIDE(strat_buttons)
            SHOW(side_buttons)
            break;
        }
        case Strategy:
        {
            HIDE(side_buttons)
            SHOW(selector_cancel)
            SHOW(strat_buttons)
            break;
        }
        case Complete:
        {
            break;
        }
    }
}

lv_res_t handleSideSelect(lv_obj_t * obj, const char *txt)
{
    int activated = lv_btnm_get_pressed(obj);
    switch(activated)
    {
        case Right:
        {
            targetAutonSide = Right;
            targetAutonSideLabel = "#FF0000 R#-";
            switchSelectorStage(Strategy);
            break;
        }
        case Left:
        {
            targetAutonSide = Left;
            targetAutonSideLabel = "#0000FF L#-";
            switchSelectorStage(Strategy);
            break;
        }
        case Skills:
        {
            targetAutonSide = Skills;
            targetAutonSideLabel = "#00FFFF Skills#";
            switchSelectorStage(Complete);
            break;
        }
        case None:
        {
            targetAutonSide = None;
            targetAutonSideLabel = "#808080 None#";
            switchSelectorStage(Complete);
            break;
        }
    }

    return LV_RES_OK;
}

lv_res_t handleStratSelect(lv_obj_t * obj, const char *txt)
{
    return LV_RES_OK;
}

lv_res_t handleSelectionCancel(lv_obj_t * obj)
{
    switchSelectorStage(Side);
    return LV_RES_OK;
}