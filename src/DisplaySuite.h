#pragma once
#include "pros/apix.h"
#define TEAM "VEX 21417B #00ff00 Intercept#"
#define VERSION "\nCompetition Suite v3.0.0"
#define CODENAME " \"Worldsbound\""
#define COMPILED "\nCompiled on " __DATE__ " at " __TIME__ "\n"

static const char * btnm_map[] = {"Home", "Hardware", "Odom", "Debug", ""}; 

void splashDisplay()
{
    lv_theme_t * th = lv_theme_alien_init(180, NULL);
    th->btnm.bg->body.radius = 0;
    th->btnm.btn.pr->body.radius = 0;
    th->page.bg->body.radius = 0;
    th->cont->body.border.width=1;
    lv_theme_set_current(th); 

    lv_obj_t * btnm1 = lv_btnm_create(lv_scr_act(), NULL);
    lv_btnm_set_map(btnm1, btnm_map);
    lv_obj_set_size(btnm1, 480, 25);
    lv_obj_align(btnm1, NULL, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    lv_obj_t * page = lv_page_create(lv_scr_act(), NULL);
    lv_obj_set_size(page, 480, 247);
    lv_obj_align(page, NULL, LV_ALIGN_CENTER, 0, 25);

    lv_obj_t * cont = lv_cont_create(lv_scr_act(), page);
    lv_obj_align_origo(cont, page, LV_ALIGN_IN_LEFT_MID, 0, 0); 
    lv_cont_set_layout(cont, LV_LAYOUT_COL_M);

    lv_obj_t * cont2 = lv_cont_create(lv_scr_act(), page);
    lv_obj_align_origo(cont2, page, LV_ALIGN_IN_RIGHT_MID, 0, 0); 
    lv_cont_set_layout(cont2, LV_LAYOUT_COL_M);

    lv_obj_t * cont3 = lv_cont_create(lv_scr_act(), page);
    lv_obj_align_origo(cont3, page, LV_ALIGN_IN_BOTTOM_LEFT, 0, 0); 
    lv_cont_set_layout(cont3, LV_LAYOUT_COL_M);

    lv_obj_t * label;
    label = lv_label_create(cont, NULL);
    lv_label_set_text(label, "Short text");
}