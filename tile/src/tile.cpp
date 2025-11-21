#include "tile.h"
#include "lvgl.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include "pin_config.h"

static void update_text_subscriber_cb(lv_event_t *e);
static void update_touch_point_subscriber_cb(lv_event_t *e);
static void timer_task(lv_timer_t *t);
static lv_obj_t *dis;

void switch_page(void)
{
    static uint8_t n;
    n++;
    lv_obj_set_tile_id(dis, 0, n % UI_PAGE_COUNT, LV_ANIM_ON);
}

void tilepage()
{
    dis = lv_tileview_create(lv_scr_act());
    lv_obj_align(dis, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(dis, LV_PCT(100), LV_PCT(100));
    lv_obj_remove_style(dis, 0, LV_PART_SCROLLBAR);

    lv_obj_t *tile1 = lv_tileview_add_tile(dis, 0, 0, LV_DIR_VER);
    lv_obj_t *tile2 = lv_tileview_add_tile(dis, 0, 1, LV_DIR_VER);
    /* page 1 */
    lv_obj_t *main_view1 = lv_obj_create(tile1);
    lv_obj_set_size(main_view1,LV_PCT(100),LV_PCT(100));
    lv_obj_set_align(main_view1,LV_ALIGN_CENTER);
    lv_obj_set_style_border_width(main_view1,0,0);
    lv_obj_clear_flag(main_view1,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(main_view1,UI_BG_COLOR,0);

    lv_obj_t *big_box = lv_obj_create(main_view1);
    lv_obj_set_size(big_box,LV_PCT(55),LV_PCT(105));
    lv_obj_align(big_box,LV_ALIGN_LEFT_MID,LV_PCT(-2),0);
    lv_obj_clear_flag(big_box, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_border_width(big_box,0,0);
    lv_obj_set_style_bg_color(big_box, UI_FRAME_COLOR, 0);

    lv_obj_t *small_box = lv_obj_create(main_view1);
    lv_obj_set_size(small_box,LV_PCT(46),LV_PCT(70));
    lv_obj_align(small_box,LV_ALIGN_BOTTOM_RIGHT,LV_PCT(2),LV_PCT(3));
    lv_obj_set_style_border_width(small_box,0,0);
    lv_obj_clear_flag(small_box, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(small_box, UI_FRAME_COLOR, 0);

    lv_obj_t *status_bar = lv_obj_create(main_view1);
    lv_obj_set_size(status_bar,LV_PCT(46),LV_PCT(30));
    lv_obj_align(status_bar,LV_ALIGN_TOP_RIGHT,LV_PCT(2),LV_PCT(-2));
    lv_obj_set_style_border_width(status_bar,0,0);
    lv_obj_clear_flag(status_bar, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(status_bar, UI_BG_COLOR, 0);

    lv_obj_t *time = lv_label_create(status_bar);
    lv_obj_set_style_text_font(time, &lv_font_montserrat_20,0);
    lv_obj_set_align(time,LV_ALIGN_RIGHT_MID);
    lv_obj_clear_flag(time, LV_OBJ_FLAG_SCROLLABLE);
    lv_label_set_text(time,"100% 12:34");

    /* page 2 */
    lv_obj_t *main_view2 = lv_obj_create(tile2);
    lv_obj_set_size(main_view2,LV_PCT(100),LV_PCT(100));
    lv_obj_set_style_border_width(main_view2,0,0);
    lv_obj_clear_flag(main_view2,LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(main_view2,UI_BG_COLOR,0);

    /*
    lv_obj_t *small_sub = lv_obj_create(main_view2);
    lv_obj_set_size(small_sub, LV_PCT(40), LV_PCT(90));
    lv_obj_align(small_sub,LV_ALIGN_LEFT_MID,LV_PCT(3),0);
    lv_obj_clear_flag(small_sub, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_bg_color(small_sub, UI_FRAME_COLOR, 0);
    */

}