#pragma once
#include "pros/apix.h"
#define TEAM "VEX 21417A "
#define VERSION "v2.0.0"
#define CODENAME " \"Multi-file Revolution\""
#define COMPILED "\n\nCompiled on " __DATE__ " at " __TIME__

void SplashDisplay()
{
    /*Create a Label on the currently active screen*/
    lv_obj_t * label1 =  lv_label_create(lv_scr_act(), NULL);

    /*Modify the Label's text*/
    lv_label_set_text(label1, TEAM VERSION CODENAME COMPILED);

    /* Align the Label to the center
     * NULL means align on parent (which is the screen now)
     * 0, 0 at the end means an x, y offset after alignment*/
    lv_obj_align(label1, NULL, LV_ALIGN_CENTER, 0, 0); 

    LV_IMG_DECLARE(rklogo);
    lv_obj_t * img_src = lv_img_create(lv_scr_act(), NULL); /*Crate an image object*/
    lv_img_set_src(img_src, &rklogo);  /*Set the created file as image*/
    lv_obj_set_pos(img_src, 10, 10);      /*Set the positions*/
    lv_obj_set_drag(img_src, true); 
}