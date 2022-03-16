lv_res_t handlePagination(lv_obj_t * obj, const char *txt)
{
    int activated = lv_btnm_get_pressed(obj);
    
    HIDE(current_page)
    if(activated == Home) { SHOW(home_page) HIDE(verbose_log) SHOW(tacit_log) }
    else if(activated == Hardware) { SHOW(hardware_page) }
    else if(activated == Odom) { SHOW(odom_page) }
    else if(activated == Debug) { SHOW(debug_page) }
    return LV_RES_OK;
}

lv_res_t handleConsoleModeSwitcher(lv_obj_t * obj, const char *txt)
{
    int activated = lv_btnm_get_pressed(obj);
    if(activated == 1) { HIDE(tacit_log) SHOW(verbose_log) }
    else if(activated == 0) { HIDE(verbose_log) SHOW(tacit_log) }
    return LV_RES_OK;
}

lv_res_t handleSideSelect(lv_obj_t * obj, const char *txt)
{
    int activated = lv_btnm_get_pressed(obj);
    if(activated == Right)
    {
        targetAutonSide = Right;
        targetAutonSideLabel = "R-";
        SHOW(strat_buttons)
    }
    else if(activated == Left)
    {
        targetAutonSide = Left;
        targetAutonSideLabel = "L-";
        SHOW(strat_buttons)
    }
    else if(activated == Skills)
    {
        targetAutonSide = Skills;
        targetAutonSideLabel = "Skills";
    }
    else if(activated == None)
    {
        targetAutonSide = None;
        targetAutonSideLabel = "None";
    }

    HIDE(side_buttons) 
    return LV_RES_OK;
}

lv_res_t handleStratSelect(lv_obj_t * obj, const char *txt)
{
    return LV_RES_OK;
}