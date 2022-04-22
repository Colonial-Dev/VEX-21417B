#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "robokauz/ROBOT.hpp"
#include "pros/apix.h"
#include "robokauz/Display/EventHandlers.hpp"
#include "robokauz/Display/DisplayGlobals.hpp"
#include "robokauz/Display/DisplayUtils.hpp"
#include "robokauz/Other/BotManager.hpp"

void statusUpdateTask(void*)
{
    std::string statusReadout;

    statusReadout += "\n LINK " + overwatch.getPrettyConnectionMode() + " | MODE " + overwatch.getPrettyOperatingMode() + "\n";
    statusReadout += " BATTERY " + overwatch.getPrettyBattery() + "\n";
    statusReadout += " TEMPS " + overwatch.getPrettyTemperatures() + "\n";
    statusReadout += " #ff0000 ODO# " + overwatch.getPrettyOdomState() + "\n";
    statusReadout += " #0000ff IMU# " + imu_odometer.getPrettyPosition() + "\n";
    statusReadout += " ENCODERS " + overwatch.getPrettyEncoders() + "\n";
    statusReadout += " POTENTIOMETER [" + precise_string(arm_controller.getLiftAngle().convert(degree)) + "]\n";
    
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
            overwatch.suspendDriverControl();
            overwatch.setBrakeMode(pros::E_MOTOR_BRAKE_COAST);    
            lv_btnm_set_map(mode_controls, controls_map_unlocked);
            break;
        }
        case Lock:
        {
            overwatch.resumeDriverControl();
            lv_btnm_set_map(mode_controls, controls_map_locked); 
            overwatch.setBrakeMode(pros::E_MOTOR_BRAKE_HOLD);   
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