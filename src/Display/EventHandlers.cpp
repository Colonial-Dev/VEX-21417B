#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "pros/apix.h"
#include "robokauz/Display/EventHandlers.hpp"
#include "robokauz/Display/DisplayGlobals.hpp"
#include "robokauz/Display/DisplayUtils.hpp"
#include "robokauz/BotManager.hpp"

std::string precise_string(double value, const int n)
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

    if(pros::competition::is_connected()) { connectionMode = "[#00ff00 SWITCH#]"; } 
    else if(!pros::competition::is_connected() && master.is_connected()) { connectionMode = "[#0000ff RADIO#]"; }
    else { connectionMode = "[#808080 NONE#]"; }

    if(!pros::competition::is_disabled() && pros::competition::is_autonomous()) { operatingMode = "[#ff0000 AUTONOMOUS#]"; }
    else if(!pros::competition::is_disabled() && !pros::competition::is_autonomous()) { operatingMode = "[#00ff00 DRIVER#]"; }
    else if(pros::competition::is_disabled() && !pros::competition::is_autonomous()) { operatingMode = "[#808080 DISABLED#]"; }

    statusReadout += "\n LINK " + connectionMode + " | MODE " + operatingMode + "\n";
    statusReadout += " BATTERY [" + std::to_string(int (pros::battery::get_capacity())) + "% | " + precise_string(pros::battery::get_voltage() / 1000.0, 3) + "V]\n";
    statusReadout += " #ff0000 ODO# [" + precise_string(drive_train->getState().x.convert(foot)) + " | " + precise_string(drive_train->getState().y.convert(foot)) + " | " + precise_string(drive_train->getState().theta.convert(degree), 3) + "]\n";
    statusReadout += " #0000ff IMU# " + imu_odometer.getPrettyPosition() + "\n";
    statusReadout += " ENCODERS [L " + std::to_string(left_encoder.get_value()) + " | M " + std::to_string(middle_encoder.get_value()) + " | R " + std::to_string(right_encoder.get_value()) + "]\n";
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
        case Unlock:
        {
            overwatch.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
            overwatch.resumeDriverControl();
            lv_btnm_set_map(mode_controls, controls_map_unlocked);    
            break;
        }
        case Lock:
        {
            overwatch.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            overwatch.suspendDriverControl();
            lv_btnm_set_map(mode_controls, controls_map_locked);    
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
            switchLeftPanePage(Status);
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