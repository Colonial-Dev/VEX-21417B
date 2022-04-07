#pragma once

std::string precise_string(double value, const int n = 2)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << value;
    return out.str();
}

void statusUpdateTask(void*)
{
    std::string statusReadout;
    std::string connectionMode;
    std::string operatingMode;
    std::string inertialHeading;

    if(pros::competition::is_connected()) { connectionMode = "[#00ff00 SWITCH#]"; } 
    else if(!pros::competition::is_connected() && master.is_connected()) { connectionMode = "[#0000ff RADIO#]"; }
    else { connectionMode = "[#808080 NONE#]"; }

    if(!pros::competition::is_disabled() && pros::competition::is_autonomous()) { operatingMode = "[#ff0000 AUTONOMOUS#]"; }
    else if(!pros::competition::is_disabled() && !pros::competition::is_autonomous()) { operatingMode = "[#00ff00 DRIVER#]"; }
    else if(pros::competition::is_disabled() && !pros::competition::is_autonomous()) { operatingMode = "[#808080 DISABLED#]"; }

    if(inertial_sensor.get_heading() == INFINITY) { inertialHeading = "Calibrating..."; }
    else{ inertialHeading = to_string(inertial_sensor.get_heading()); }


    statusReadout += "\n LINK " + connectionMode + " | MODE " + operatingMode + "\n";
    statusReadout += " BATTERY [" + to_string(int (pros::battery::get_capacity())) + "% | " + precise_string(pros::battery::get_voltage() / 1000.0, 3) + "V]\n";
    statusReadout += " POSITION [" + precise_string(drive_train->getState().x.convert(foot)) + " | " + precise_string(drive_train->getState().y.convert(foot)) + "]\n";
    statusReadout += " IMU POSITION [" + precise_string(imu_odometer.getPosition().x.convert(foot)) + " | " + precise_string(imu_odometer.getPosition().y.convert(foot)) + "]\n";
    statusReadout += " HEADING [" + precise_string(drive_train->getState().theta.convert(degree)) + " | " + inertialHeading + "]\n";
    statusReadout += " ENCODERS [L " + to_string(left_encoder.get_value()) + " | M " + to_string(middle_encoder.get_value()) + " | R " + to_string(right_encoder.get_value()) + "]\n";
    statusPrint(statusReadout);
}


void switchSelectorStage(int targetStage)
{
    switch(targetStage)
    {
        case Side:
        {
            HIDE(strat_page)
            HIDE(ready_page)
            SHOW(side_page)
            break;
        }
        case Strategy:
        {
            HIDE(side_page)
            HIDE(ready_page)
            SHOW(strat_page)
            break;
        }
        case Complete:
        {
            HIDE(side_page)
            HIDE(strat_page)
            SHOW(ready_page)
            break;
        }
    }
}

void switchLeftPanePage(int targetPage)
{
    switch(targetPage)
    {
        case Status:
        {
            HIDE(controls_page)
            SHOW(status_page)
            break;
        }
        case Controls:
        {
            HIDE(status_page)
            SHOW(controls_page)
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

lv_res_t handleStatusModeSwitcher(lv_obj_t * obj, const char *txt)
{
    int activated = lv_btnm_get_pressed(obj);
    switch(activated)
    {
        case Status:
        {
            switchLeftPanePage(Status);
            break;
        }
        case Controls:
        {
            switchLeftPanePage(Controls);
            break;
        }
    }
    return LV_RES_OK;
}

lv_res_t handleControls(lv_obj_t * obj, const char *txt)
{
    int toggled = lv_btnm_get_pressed(obj);
    switch(toggled)
    {
        case Driver:
        {
            overwatch.resumeDriverControl();
            break;
        }
        case Autonomous:
        {
            pros::Task autonomous_async(autonomousAsync);
            switchLeftPanePage(Status);
            break;
        }
        case Reset:
        {
            overwatch.reinitialize();
            break;
        }
        case Kill:
        {
            abort();
            break;
        }
    }
    return LV_RES_OK;
}