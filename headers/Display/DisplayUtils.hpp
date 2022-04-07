#pragma once
#include "robokauz/PROS.hpp"
#include "robokauz/COMMON.hpp"
#include "pros/apix.h"
#include "DisplayGlobals.hpp"

void configureTheming();

void configureStyling();

lv_obj_t * createButton(lv_obj_t *parent, std::string label, int size_x, int size_y, lv_align_t alignment, int x_offset, int y_offset, lv_action_t action);

lv_obj_t * createMatrix(const char **map, int size_x, int size_y, lv_obj_t *parent, lv_align_t alignment, int x_offset, int y_offset, lv_btnm_action_t action);

lv_obj_t * createPage(lv_obj_t *parent, int x_size, int y_size, lv_align_t alignment, int x_offset, int y_offset, bool start_hidden);

lv_obj_t * createContainer(lv_obj_t *parent, int x_size, int y_size, lv_align_t alignment, int x_offset, int y_offset, lv_layout_t layout);

lv_obj_t * createTextArea(lv_obj_t *parent, int x_size, int y_size, lv_align_t alignment, int x_offset, int y_offset, lv_cursor_type_t cursor, std::string initialText);

lv_obj_t * createLabel(lv_obj_t *parent, std::string initialText, lv_label_align_t text_alignment, lv_align_t alignment, int x_offset, int y_offset);

void statusPrint(std::string new_content);