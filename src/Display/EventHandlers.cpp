#include "robokauz/PRELUDE.hpp"
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
    statusReadout += " TEMPS " + overwatch.getPrettyTemperatures() + "\n\n";
    statusReadout += " #0000ff IMU# " + imu_odometer.getPrettyPosition() + "\n";
    statusReadout += " ENCODERS " + overwatch.getPrettyEncoders() + "\n";
    statusReadout += " POTENTIOMETER [" + precise_string(arm_controller.getLiftAngle().convert(degree)) + "]\n";
    statusReadout += " PURE PURSUIT " + wayfarer.getPrettyStatus() + "\n";
    statusPrint(statusReadout);
}

void routineUpdateTask(void*)
{
    std::string routineReadout;

    routineReadout += "#00AA00 Ready!#\n";

    switch(targetAutonSide)
    {
        case Left:
        {
            routineReadout += "#0000FF Left#\n";
            break;
        }
        case Right:
        {
            routineReadout += "#FF0000 Right#\n";
            break;
        }
        case Skills:
        {
            routineReadout += "#FFFF00 Skills#";
            break;
        }
        case None:
        {
            routineReadout += "#808080 None#";
            break;
        }
    }

    if(targetAutonSide != Left && targetAutonSide != Right)
    {
        lv_label_set_text(ready_label, routineReadout.c_str());
        return;
    }

    switch(targetAutonStrategy)
    {
        case FullWinPoint:
        {
            routineReadout += "Clip Rush";
            break;
        }
        case HalfWinPoint:
        {
            routineReadout += "WP Half";
            break;
        }
        case SingleGoalRush:
        {
            routineReadout += "Single Rush";
            break;
        }
        case MiddleGoalRush:
        {
            routineReadout += "Middle Rush";
            break;
        }
    }
    
    lv_label_set_text(ready_label, routineReadout.c_str());
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
            switchSelectorStage(Strategy);
            break;
        }
        case Left:
        {
            targetAutonSide = Left;
            switchSelectorStage(Strategy);
            break;
        }
        case Skills:
        {
            targetAutonSide = Skills;
            switchSelectorStage(Complete);
            break;
        }
        case None:
        {
            targetAutonSide = None;
            switchSelectorStage(Complete);
            break;
        }
    }

    return LV_RES_OK;
}

lv_res_t handleStratSelect(lv_obj_t * obj, const char *txt)
{
    int activated = lv_btnm_get_pressed(obj);
    switch(activated)
    {
        case FullWinPoint:
        {
            targetAutonStrategy = FullWinPoint;
            switchSelectorStage(Complete);
            break;
        }
        case HalfWinPoint:
        {
            targetAutonStrategy = HalfWinPoint;
            switchSelectorStage(Complete);
            break;
        }
        case SingleGoalRush:
        {
            targetAutonStrategy = SingleGoalRush;
            switchSelectorStage(Complete);
            break;
        }
        case MiddleGoalRush:
        {
            targetAutonStrategy = MiddleGoalRush;
            switchSelectorStage(Complete);
            break;
        }
    }

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
            driver_control_gate.closeGate();
            lv_btnm_set_map(mode_controls, controls_map_unlocked);
            overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_COAST);
            overwatch.setManipulatorBrakeModes(pros::E_MOTOR_BRAKE_COAST); 
            break;
        }
        case Lock:
        {
            driver_control_gate.openGate();
            lv_btnm_set_map(mode_controls, controls_map_locked); 
            overwatch.setDriveBrakeMode(pros::E_MOTOR_BRAKE_HOLD);
            overwatch.setManipulatorBrakeModes(pros::E_MOTOR_BRAKE_HOLD);      
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