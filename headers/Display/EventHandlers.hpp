#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "pros/apix.h"
#include "DisplayUtils.hpp"
#include "DisplayGlobals.hpp"

std::string precise_string(double value, const int n);

void statusUpdateTask(void*);


void switchSelectorStage(int targetStage);

void switchLeftPanePage(int targetPage);

lv_res_t handleSideSelect(lv_obj_t * obj, const char *txt);

lv_res_t handleStratSelect(lv_obj_t * obj, const char *txt);

lv_res_t handleSelectionCancel(lv_obj_t * obj);

lv_res_t handleStatusModeSwitcher(lv_obj_t * obj, const char *txt);

lv_res_t handleControls(lv_obj_t * obj, const char *txt);