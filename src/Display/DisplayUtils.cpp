#include "robokauz/PRELUDE.hpp"
#include "robokauz/COMMON.hpp"
#include "pros/apix.h"
#include "display/lv_fonts/lv_font_builtin.h"
#include "robokauz/Display/DisplayGlobals.hpp"
#include "robokauz/Display/DisplayUtils.hpp"

void configureTheming()
{
    theme->btnm.bg->body.border.color = LV_COLOR_GRAY;
    theme->btnm.bg->body.radius = 0;
    theme->btnm.btn.pr->body.radius = 0;
    theme->btnm.btn.rel->body.main_color = LV_COLOR_BLACK;
    theme->btnm.btn.rel->body.radius = 0;
    theme->btnm.btn.rel->body.border.color = LV_COLOR_GRAY;
    theme->btnm.btn.rel->body.border.width = 1;

    theme->page.bg->body.radius = 0;
    theme->page.bg->body.main_color = LV_COLOR_BLACK;
    theme->page.bg->body.grad_color = LV_COLOR_BLACK;
    theme->page.bg->body.padding.hor = 0;
    theme->page.bg->body.padding.ver = 0;

    theme->cont->body.border.width=1;
    theme->cont->body.padding.inner=0;

    theme->btn.pr->body.radius=0;
    theme->btn.rel->body.radius=0;
    theme->btn.rel->body.border.color = LV_COLOR_GRAY;

    lv_theme_set_current(theme); 
}

void configureStyling()
{
    confirm_style.text.font = &lv_font_dejavu_40;
    status_style.text.font = &lv_font_monospace_8;
    symbol_style.text.font = &lv_font_symbol_40;
}

lv_obj_t * createButton(lv_obj_t *parent, std::string label, int size_x, int size_y, lv_align_t alignment, int x_offset, int y_offset, lv_action_t action = NULL)
{
    lv_obj_t * newButton = lv_btn_create(parent, NULL);
    lv_obj_set_size(newButton, size_x, size_y);
    lv_obj_align(newButton, NULL, alignment, x_offset, y_offset);
    if(action != NULL) { lv_btn_set_action(newButton, LV_BTN_ACTION_CLICK, action); }

    lv_obj_t * buttonLabel = lv_label_create(newButton, NULL);
    lv_label_set_recolor(buttonLabel, true);
    lv_label_set_text(buttonLabel, label.c_str());
    lv_obj_align(buttonLabel, NULL, LV_ALIGN_IN_TOP_MID, 0, 5);

    return newButton;
}

lv_obj_t * createMatrix(const char **map, int size_x, int size_y, lv_obj_t *parent, lv_align_t alignment, int x_offset, int y_offset, lv_btnm_action_t action = NULL)
{
    lv_obj_t * newMatrix = lv_btnm_create(parent, NULL);
    lv_btnm_set_map(newMatrix, map);
    lv_obj_set_size(newMatrix, size_x, size_y);
    lv_obj_align(newMatrix, NULL, alignment, x_offset, y_offset);
    lv_btnm_set_recolor(newMatrix, true);
    if(action != NULL) { lv_btnm_set_action(newMatrix, action); }
    return newMatrix;
}

lv_obj_t * createPage(lv_obj_t *parent, int x_size, int y_size, lv_align_t alignment, int x_offset, int y_offset, bool start_hidden = false)
{
    lv_obj_t * newPage = lv_page_create(parent, NULL);
    lv_obj_set_size(newPage, x_size, y_size);
    lv_obj_align(newPage, NULL, alignment, x_offset, y_offset);
    if(start_hidden) { HIDE(newPage) }

    return newPage;
}

lv_obj_t * createContainer(lv_obj_t *parent, int x_size, int y_size, lv_align_t alignment, int x_offset, int y_offset, lv_layout_t layout)
{
    lv_obj_t * newContainer = lv_cont_create(parent, NULL);
    lv_obj_set_size(newContainer, x_size, y_size);
    lv_obj_align_origo(newContainer, NULL, alignment, x_offset, y_offset); 
    lv_cont_set_layout(newContainer, layout);

    return newContainer;
}

lv_obj_t * createTextArea(lv_obj_t *parent, int x_size, int y_size, lv_align_t alignment, int x_offset, int y_offset, lv_cursor_type_t cursor, std::string initialText = "")
{
    lv_obj_t * newTA = lv_ta_create(parent, NULL);
    lv_obj_set_size(newTA, x_size, y_size);
    lv_obj_align(newTA, NULL, alignment, x_offset, y_offset);
    lv_ta_set_cursor_type(newTA, cursor);
    lv_ta_set_text(newTA, initialText.c_str());

    return newTA; 
}

lv_obj_t * createLabel(lv_obj_t *parent, std::string initialText, lv_label_align_t text_alignment, lv_align_t alignment, int x_offset, int y_offset)
{
    lv_obj_t * newLabel = lv_label_create(parent, NULL);
    lv_label_set_align(newLabel, text_alignment);
    lv_label_set_text(newLabel, initialText.c_str());
    lv_obj_align(newLabel, NULL, alignment, x_offset, y_offset);
    lv_label_set_recolor(newLabel, true);  
    return newLabel;
}

void statusPrint(std::string new_content)
{
    lv_label_set_text(status_text, new_content.c_str());
}