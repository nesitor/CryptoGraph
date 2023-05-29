/*
MIT License

Copyright (c) 2022 Sukesh Ashok Kumar

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "ota.h"
#include "widgets/tux_panel.h"
#include <fmt/core.h>
#include <fmt/format.h>
#include "OpenWeatherMap.hpp"
#include "CoinMarketCap.hpp"
#include "apps/weather/weathericons.h"
#include "events/gui_events.hpp"
#include <esp_partition.h>

LV_IMG_DECLARE(dev_bg)
LV_IMG_DECLARE(img_btc)
//LV_IMG_DECLARE(tux_logo)

// LV_FONT_DECLARE(font_7seg_64)
// LV_FONT_DECLARE(font_7seg_60)
// LV_FONT_DECLARE(font_7seg_58)
LV_FONT_DECLARE(font_7seg_56)

//LV_FONT_DECLARE(font_robotomono_12)
LV_FONT_DECLARE(font_robotomono_13)

LV_FONT_DECLARE(font_fa_14)
#define FA_SYMBOL_BLE "\xEF\x8A\x94"      // 0xf294
#define FA_SYMBOL_SETTINGS "\xEF\x80\x93" // 0xf0ad

/*********************
 *      DEFINES
 *********************/
#define HEADER_HEIGHT 30
#define FOOTER_HEIGHT 30

/******************
 *  LV DEFINES
 ******************/
static const lv_font_t *font_small;
static const lv_font_t *font_large;
static const lv_font_t *font_normal;
static const lv_font_t *font_symbol;
static const lv_font_t *font_fa;
static const lv_font_t *font_xl;
static const lv_font_t *font_xxl;

static lv_obj_t *panel_header;
static lv_obj_t *panel_title;
static lv_obj_t *panel_status; // Status icons in the header
static lv_obj_t *content_container;
static lv_obj_t *screen_container;
static lv_obj_t *qr_status_container;

// Screen Definitions

static lv_obj_t * ui_mainScreen;
static lv_obj_t * ui_cryptoPanel;
static lv_obj_t * ui_titleCryptoPanel;
static lv_obj_t * ui_cryptoImage;
static lv_obj_t * ui_cryptoLabel;
static lv_obj_t * ui_priceLabel;
static lv_obj_t * ui_assetsCryptoPanel;
static lv_obj_t * ui_amountLabel;
static lv_obj_t * ui_changesCryptoPanel1;
static lv_obj_t * ui_changesLabel1;
static lv_obj_t * ui_divisorLabel1;
static lv_obj_t * ui_changesLabel2;
static lv_obj_t * ui_divisorLabel2;
static lv_obj_t * ui_changesLabel3;
static lv_obj_t * ui_cryptoChart;
static lv_obj_t * ui_toolPanel;
static lv_obj_t * ui_configButton;
static lv_obj_t * ui_configScreen;
static lv_obj_t * ui_configPanel;
static lv_obj_t * ui_closeButton;
static lv_obj_t * ui_ipLabel;
static lv_obj_t * ui_qrImage;
static lv_obj_t * ui_wifiResetButton;

// TUX Panels
static lv_obj_t *tux_clock_weather;
static lv_obj_t *chart_panel;

static lv_obj_t *island_wifi;
static lv_obj_t *island_ota;
static lv_obj_t *island_devinfo;
static lv_obj_t *prov_qr;

static lv_obj_t *label_title;
static lv_obj_t *label_message;
static lv_obj_t *lbl_version;
static lv_obj_t *lbl_update_status;
static lv_obj_t *lbl_scan_status;

static lv_obj_t *lbl_device_info;

static lv_obj_t *icon_storage;
static lv_obj_t *icon_wifi;
static lv_obj_t *icon_ble;
static lv_obj_t *icon_battery;

/* Date/Time */
static lv_obj_t *lbl_time;
static lv_obj_t *lbl_ampm;
static lv_obj_t *lbl_date;

/* Weather */
static lv_obj_t *lbl_weathericon;
static lv_obj_t *lbl_temp;
static lv_obj_t *lbl_hl;

static lv_obj_t *lbl_wifi_status;

static lv_coord_t screen_h;
static lv_coord_t screen_w;

/* Crypto Panel */

static lv_obj_t *island_crypto;

static lv_obj_t * ui_Screen1;
static lv_obj_t * ui_Chart2;
static lv_obj_t * ui_Label2;
static lv_obj_t * ui_Image2;

static lv_obj_t * chart;
static lv_chart_series_t * ser;
static lv_chart_cursor_t * cursor;

/******************
 *  LVL STYLES
 ******************/
static lv_style_t style_content_bg;

static lv_style_t style_message;
static lv_style_t style_title;
static lv_style_t style_iconstatus;
static lv_style_t style_storage;
static lv_style_t style_wifi;
static lv_style_t style_ble;
static lv_style_t style_battery;

static lv_style_t style_ui_island;

static lv_style_t style_glow;

static lv_style_t style_win;
static lv_style_t style_loose;
static lv_style_t style_change;
static lv_style_t style_change_divisor;
static lv_style_t style_price;
static lv_style_t style_amount;

/******************
 *  LVL ANIMATION
 ******************/
static lv_anim_t anim_labelscroll;

void anim_move_left_x(lv_obj_t * TargetObject, int start_x, int end_x, int delay);
void tux_anim_callback_set_x(lv_anim_t * a, int32_t v);

void anim_move_left_y(lv_obj_t * TargetObject, int start_y, int end_y, int delay);
void tux_anim_callback_set_y(lv_anim_t * a, int32_t v);

void anim_fade_in(lv_obj_t * TargetObject, int delay);
void anim_fade_out(lv_obj_t * TargetObject, int delay);

void tux_anim_callback_set_opacity(lv_anim_t * a, int32_t v);
/******************
 *  LVL FUNCS & EVENTS
 ******************/
static void create_page_home(lv_obj_t *parent);
static void create_page_settings(lv_obj_t *parent);
static void create_page_updates(lv_obj_t *parent);
static void create_page_crypto(lv_obj_t *parent);

// Home page islands
static void tux_panel_clock_weather(lv_obj_t *parent);
static void tux_panel_config(lv_obj_t *parent);

// Setting page islands
static void tux_panel_devinfo(lv_obj_t *parent);
static void tux_panel_ota(lv_obj_t *parent);
static void tux_panel_wifi(lv_obj_t *parent);

static void create_header(lv_obj_t *parent);
static void create_footer(lv_obj_t *parent);

static void footer_message(const char *fmt, ...);
static void create_splash_screen();
static void switch_theme(bool dark);
static void qrcode_ui(lv_obj_t *parent);
static void show_ui();

static const char* get_firmware_version();

static void rotate_event_handler(lv_event_t *e);
static void theme_switch_event_handler(lv_event_t *e);
static void espwifi_event_handler(lv_event_t* e);
//static void espble_event_handler(lv_event_t *e);
static void checkupdates_event_handler(lv_event_t *e);
static void home_clicked_eventhandler(lv_event_t *e);
static void status_clicked_eventhandler(lv_event_t *e);
static void footer_button_event_handler(lv_event_t *e);
static void graph_event_handler(lv_event_t *e);

// static void new_theme_apply_cb(lv_theme_t * th, lv_obj_t * obj);

/* MSG Events */
void datetime_event_cb(lv_event_t * e);
void weather_event_cb(lv_event_t * e);
void crypto_event_cb(lv_event_t * e);

static void status_change_cb(void * s, lv_msg_t *m);
static void lv_update_battery(uint batval);
static void set_weather_icon(string weatherIcon);

static int current_page = 0;

void lv_setup_styles()
{
    font_symbol = &lv_font_montserrat_14;
    font_normal = &lv_font_montserrat_14;
    font_large = &lv_font_montserrat_16;
    font_small = &lv_font_montserrat_10;
    font_xl = &lv_font_montserrat_24;
    font_xxl = &lv_font_montserrat_32;
    font_fa = &font_fa_14;

    screen_h = lv_obj_get_height(lv_scr_act());
    screen_w = lv_obj_get_width(lv_scr_act());

    /* CONTENT CONTAINER BACKGROUND */
    lv_style_init(&style_content_bg);
    lv_style_set_bg_opa(&style_content_bg, LV_OPA_0);
    lv_style_set_radius(&style_content_bg, 0);

    // DASHBOARD TITLE
    lv_style_init(&style_title);
    lv_style_set_text_font(&style_title, font_large);
    lv_style_set_align(&style_title, LV_ALIGN_LEFT_MID);
    lv_style_set_pad_left(&style_title, 15);
    lv_style_set_border_width(&style_title, 0);
    lv_style_set_size(&style_title, LV_SIZE_CONTENT);

    // HEADER STATUS ICON PANEL
    lv_style_init(&style_iconstatus);
    lv_style_set_size(&style_iconstatus, LV_SIZE_CONTENT);
    lv_style_set_pad_all(&style_iconstatus, 0);
    lv_style_set_border_width(&style_iconstatus, 0);
    lv_style_set_align(&style_iconstatus, LV_ALIGN_RIGHT_MID);
    lv_style_set_pad_right(&style_iconstatus, 15);

    lv_style_set_layout(&style_iconstatus, LV_LAYOUT_FLEX);
    lv_style_set_flex_flow(&style_iconstatus, LV_FLEX_FLOW_ROW);
    lv_style_set_flex_main_place(&style_iconstatus, LV_FLEX_ALIGN_CENTER);
    lv_style_set_flex_track_place(&style_iconstatus, LV_FLEX_ALIGN_SPACE_BETWEEN);
    lv_style_set_pad_row(&style_iconstatus, 3);

    // BATTERY
    lv_style_init(&style_battery);
    lv_style_set_text_font(&style_battery, font_symbol);
    lv_style_set_align(&style_battery, LV_ALIGN_RIGHT_MID);
    lv_style_set_text_color(&style_battery, lv_palette_main(LV_PALETTE_RED));

    // SD CARD
    lv_style_init(&style_storage);
    lv_style_set_text_font(&style_storage, font_symbol);
    lv_style_set_align(&style_storage, LV_ALIGN_RIGHT_MID);

    // WIFI
    lv_style_init(&style_wifi);
    lv_style_set_text_font(&style_wifi, font_symbol);
    lv_style_set_align(&style_wifi, LV_ALIGN_RIGHT_MID);

    // BLE
    lv_style_init(&style_ble);
    lv_style_set_text_font(&style_ble, font_fa);
    lv_style_set_align(&style_ble, LV_ALIGN_RIGHT_MID);

    // FOOTER MESSAGE & ANIMATION
    lv_anim_init(&anim_labelscroll);
    //lv_anim_set_delay(&anim_labelscroll, 1000);        // Wait 1 second to start the first scroll
    lv_anim_set_repeat_delay(&anim_labelscroll, 1000); // Repeat the scroll 3 seconds after the label scrolls back to the initial position

    lv_style_init(&style_message);
    lv_style_set_anim(&style_message, &anim_labelscroll); // Set animation for the style
    // lv_style_set_text_color(&style_message, lv_palette_main(LV_PALETTE_RED));
    lv_style_set_opa(&style_message, LV_OPA_COVER);
    lv_style_set_text_font(&style_message, font_normal);
    lv_style_set_align(&style_message, LV_ALIGN_LEFT_MID);
    lv_style_set_pad_left(&style_message, 15);
    lv_style_set_pad_right(&style_message, 15);

    // UI ISLANDS
    lv_style_init(&style_ui_island);
    lv_style_set_bg_color(&style_ui_island, bg_theme_color);
    lv_style_set_bg_opa(&style_ui_island, LV_OPA_80);
    lv_style_set_border_color(&style_ui_island, bg_theme_color);
    //lv_style_set_border_opa(&style_ui_island, LV_OPA_80);
    lv_style_set_border_width(&style_ui_island, 1);
    lv_style_set_radius(&style_ui_island, 10);

    // PRICE STYLES
    lv_style_init(&style_win);
    lv_style_set_text_color(&style_win, lv_color_hex(0x00FF00));

    lv_style_init(&style_loose);
    lv_style_set_text_color(&style_loose, lv_color_hex(0xFF0000));

    lv_style_init(&style_change);
    lv_style_set_width(&style_change, LV_SIZE_CONTENT);
    lv_style_set_height(&style_change, LV_SIZE_CONTENT);
    lv_style_set_align(&style_change, LV_ALIGN_RIGHT_MID);
    lv_style_set_text_opa(&style_change, 255);
    lv_style_set_text_align(&style_change, LV_TEXT_ALIGN_LEFT);
    lv_style_set_text_font(&style_change, font_small);

    lv_style_init(&style_change_divisor);
    lv_style_set_width(&style_change_divisor, LV_SIZE_CONTENT);
    lv_style_set_height(&style_change_divisor, LV_SIZE_CONTENT);
    lv_style_set_align(&style_change_divisor, LV_ALIGN_RIGHT_MID);
    lv_style_set_text_color(&style_change_divisor, lv_color_hex(0x5D5D5D));
    lv_style_set_text_opa(&style_change_divisor, 255);
    lv_style_set_text_align(&style_change_divisor, LV_TEXT_ALIGN_LEFT);
    lv_style_set_text_font(&style_change_divisor, font_small);

    lv_style_init(&style_price);
    lv_style_set_width(&style_price, LV_SIZE_CONTENT);
    lv_style_set_height(&style_price, LV_SIZE_CONTENT);
    lv_style_set_align(&style_price, LV_ALIGN_RIGHT_MID);
    lv_style_set_text_color(&style_price, lv_color_hex(0x999999));
    lv_style_set_text_opa(&style_price, 255);

    lv_style_init(&style_amount);
    lv_style_set_align(&style_amount, LV_ALIGN_LEFT_MID);
    lv_style_set_text_color(&style_amount, lv_color_hex(0x999999));
    lv_style_set_text_opa(&style_amount, 255);
    lv_style_set_text_align(&style_amount, LV_TEXT_ALIGN_AUTO);
    lv_style_set_pad_left(&style_amount, 0);
    lv_style_set_pad_right(&style_amount, 0);
    lv_style_set_pad_top(&style_amount, 0);
    lv_style_set_pad_bottom(&style_amount, 0);

    // FOOTER NAV BUTTONS
    //lv_style_init(&style_glow);
    //lv_style_set_bg_opa(&style_glow, LV_OPA_COVER);
    //lv_style_set_border_width(&style_glow,0);
    //lv_style_set_bg_color(&style_glow, lv_palette_main(LV_PALETTE_RED));

    /*Add a shadow*/
    // lv_style_set_shadow_width(&style_glow, 10);
    // lv_style_set_shadow_color(&style_glow, lv_palette_main(LV_PALETTE_RED));
    // lv_style_set_shadow_ofs_x(&style_glow, 5);
    // lv_style_set_shadow_ofs_y(&style_glow, 5);
}

static void create_header(lv_obj_t *parent)
{
    // HEADER PANEL
    panel_header = lv_obj_create(parent);
    lv_obj_set_size(panel_header, LV_PCT(100), HEADER_HEIGHT);
    lv_obj_set_style_pad_all(panel_header, 0, 0);
    lv_obj_set_style_radius(panel_header, 0, 0);
    lv_obj_set_align(panel_header, LV_ALIGN_TOP_MID);
    lv_obj_set_scrollbar_mode(panel_header, LV_SCROLLBAR_MODE_OFF);

    // HEADER TITLE PANEL
    panel_title = lv_obj_create(panel_header);
    lv_obj_add_style(panel_title, &style_title, 0);
    lv_obj_set_scrollbar_mode(panel_title, LV_SCROLLBAR_MODE_OFF);

    // HEADER TITLE
    label_title = lv_label_create(panel_title);
    lv_label_set_text(label_title, LV_SYMBOL_HOME " CryptoGraph");

    // HEADER STATUS ICON PANEL
    panel_status = lv_obj_create(panel_header);
    lv_obj_add_style(panel_status, &style_iconstatus, 0);
    lv_obj_set_scrollbar_mode(panel_status, LV_SCROLLBAR_MODE_OFF);

    // BLE
    icon_ble = lv_label_create(panel_status);
    lv_label_set_text(icon_ble, FA_SYMBOL_BLE);
    lv_obj_add_style(icon_ble, &style_ble, 0);

    // WIFI
    icon_wifi = lv_label_create(panel_status);
    lv_label_set_text(icon_wifi, LV_SYMBOL_WIFI);
    lv_obj_add_style(icon_wifi, &style_wifi, 0);

    // SD CARD
    icon_storage = lv_label_create(panel_status);
    lv_label_set_text(icon_storage, LV_SYMBOL_SD_CARD);
    lv_obj_add_style(icon_storage, &style_storage, 0);

    // BATTERY
    icon_battery = lv_label_create(panel_status);
    lv_label_set_text(icon_battery, LV_SYMBOL_CHARGE);
    lv_obj_add_style(icon_battery, &style_battery, 0);

    lv_obj_add_event_cb(panel_title, home_clicked_eventhandler, LV_EVENT_CLICKED, NULL);
    lv_obj_add_event_cb(panel_status, status_clicked_eventhandler, LV_EVENT_CLICKED, NULL);
}

static void create_footer(lv_obj_t *parent)
{
    lv_obj_t *panel_footer = lv_obj_create(parent);
    lv_obj_set_size(panel_footer, LV_PCT(100), FOOTER_HEIGHT);
    // lv_obj_set_style_bg_color(panel_footer, bg_theme_color, 0);
    lv_obj_set_style_pad_all(panel_footer, 0, 0);
    lv_obj_set_style_radius(panel_footer, 0, 0);
    lv_obj_set_align(panel_footer, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_scrollbar_mode(panel_footer, LV_SCROLLBAR_MODE_OFF);

/*
    // Create Footer label and animate if text is longer
    label_message = lv_label_create(panel_footer); // full screen as the parent
    lv_obj_set_width(label_message, LV_PCT(100));
    lv_label_set_long_mode(label_message, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_obj_add_style(label_message, &style_message, LV_STATE_DEFAULT);
    lv_obj_set_style_align(label_message,LV_ALIGN_BOTTOM_LEFT,0);

    // Show LVGL version in the footer
    footer_message("A Touch UX Template using LVGL v%d.%d.%d", lv_version_major(), lv_version_minor(), lv_version_patch());
*/

    /* REPLACE STATUS BAR WITH BUTTON PANEL FOR NAVIGATION */
    //static const char * btnm_map[] = {LV_SYMBOL_HOME " HOME", LV_SYMBOL_BARS " CRYPTO", FA_SYMBOL_SETTINGS " SETTINGS", LV_SYMBOL_DOWNLOAD " UPDATE", NULL};
    static const char * btnm_map[] = {LV_SYMBOL_HOME, LV_SYMBOL_BARS, FA_SYMBOL_SETTINGS, LV_SYMBOL_DOWNLOAD,  NULL};
    lv_obj_t * footerButtons = lv_btnmatrix_create(panel_footer);
    lv_btnmatrix_set_map(footerButtons, btnm_map);
    lv_obj_set_style_text_font(footerButtons,&lv_font_montserrat_16,LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(footerButtons,LV_OPA_TRANSP,0);
    lv_obj_set_size(footerButtons,LV_PCT(100),LV_PCT(100));
    lv_obj_set_style_border_width(footerButtons,0,LV_PART_MAIN | LV_PART_ITEMS);
    lv_btnmatrix_set_btn_ctrl_all(footerButtons, LV_BTNMATRIX_CTRL_CHECKABLE);

    //lv_obj_set_style_align(footerButtons,LV_ALIGN_TOP_MID,0);
    lv_btnmatrix_set_one_checked(footerButtons, true);   // only 1 button can be checked
    lv_btnmatrix_set_btn_ctrl(footerButtons,0,LV_BTNMATRIX_CTRL_CHECKED);

    // Very important but weird behavior
    lv_obj_set_height(footerButtons,FOOTER_HEIGHT+20);
    lv_obj_set_style_radius(footerButtons,0,LV_PART_ITEMS);
    lv_obj_set_style_bg_opa(footerButtons,LV_OPA_TRANSP,LV_PART_ITEMS);
    lv_obj_add_style(footerButtons, &style_glow,LV_PART_ITEMS | LV_BTNMATRIX_CTRL_CHECKED); // selected

    lv_obj_align(footerButtons, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_event_cb(footerButtons, footer_button_event_handler, LV_EVENT_ALL, NULL);

}

static void tux_panel_clock_weather(lv_obj_t *parent)
{
    tux_clock_weather = tux_panel_create(parent, "", 130);
    lv_obj_add_style(tux_clock_weather, &style_ui_island, 0);

    lv_obj_t *cont_panel = tux_panel_get_content(tux_clock_weather);
    lv_obj_set_flex_flow(tux_clock_weather, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(tux_clock_weather, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // ************ Date/Time panel
    lv_obj_t *cont_datetime = lv_obj_create(cont_panel);
    lv_obj_set_size(cont_datetime,180,120);
    lv_obj_set_flex_flow(cont_datetime, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_scrollbar_mode(cont_datetime, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(cont_datetime,LV_ALIGN_LEFT_MID,0,0);
    lv_obj_set_style_bg_opa(cont_datetime,LV_OPA_TRANSP,0);
    lv_obj_set_style_border_opa(cont_datetime,LV_OPA_TRANSP,0);
    //lv_obj_set_style_pad_gap(cont_datetime,10,0);
    lv_obj_set_style_pad_top(cont_datetime,20,0);

    // MSG - MSG_TIME_CHANGED - EVENT
    lv_obj_add_event_cb(cont_datetime, datetime_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(MSG_TIME_CHANGED, cont_datetime, NULL);

    // Time
    lbl_time = lv_label_create(cont_datetime);
    lv_obj_set_style_align(lbl_time, LV_ALIGN_TOP_LEFT, 0);
    lv_obj_set_style_text_font(lbl_time, &font_7seg_56, 0);
    lv_label_set_text(lbl_time, "00:00");

    // AM/PM
    lbl_ampm = lv_label_create(cont_datetime);
    lv_obj_set_style_align(lbl_ampm, LV_ALIGN_TOP_LEFT, 0);
    lv_label_set_text(lbl_ampm, "AM");

    // Date
    lbl_date = lv_label_create(cont_datetime);
    lv_obj_set_style_align(lbl_date, LV_ALIGN_BOTTOM_MID, 0);
    lv_obj_set_style_text_font(lbl_date, font_large, 0);
    lv_obj_set_height(lbl_date,30);
    lv_label_set_text(lbl_date, "waiting for update");

    // ************ Weather panel (panel widen with weekly forecast in landscape)
    lv_obj_t *cont_weather = lv_obj_create(cont_panel);
    lv_obj_set_size(cont_weather,100,115);
    lv_obj_set_flex_flow(cont_weather, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(cont_weather, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_scrollbar_mode(cont_weather, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align_to(cont_weather,cont_datetime,LV_ALIGN_OUT_RIGHT_MID,0,0);
    lv_obj_set_style_bg_opa(cont_weather,LV_OPA_TRANSP,0);
    lv_obj_set_style_border_opa(cont_weather,LV_OPA_TRANSP,0);

    // MSG - MSG_WEATHER_CHANGED - EVENT
    lv_obj_add_event_cb(cont_weather, weather_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(MSG_WEATHER_CHANGED, cont_weather, NULL);

    // This only for landscape
    // lv_obj_t *lbl_unit = lv_label_create(cont_weather);
    // lv_obj_set_style_text_font(lbl_unit, font_normal, 0);
    // lv_label_set_text(lbl_unit, "Light rain");

    // Weather icons
    lbl_weathericon = lv_label_create(cont_weather);
    lv_obj_set_style_text_font(lbl_weathericon, &font_fa_weather_42, 0);
    // "F:/weather/cloud-sun-rain.bin");//10d@2x.bin"
    lv_label_set_text(lbl_weathericon, FA_WEATHER_SUN);
    lv_obj_set_style_text_color(lbl_weathericon,lv_palette_main(LV_PALETTE_ORANGE),0);

    // Temperature
    lbl_temp = lv_label_create(cont_weather);
    //lv_obj_set_style_text_font(lbl_temp, &lv_font_montserrat_32, 0);
    lv_obj_set_style_text_font(lbl_temp, font_xl, 0);
    lv_obj_set_style_align(lbl_temp, LV_ALIGN_BOTTOM_MID, 0);
    lv_label_set_text(lbl_temp, "0°C");

    lbl_hl = lv_label_create(cont_weather);
    lv_obj_set_style_text_font(lbl_hl, font_normal, 0);
    lv_obj_set_style_align(lbl_hl, LV_ALIGN_BOTTOM_MID, 0);
    lv_label_set_text(lbl_hl, "H:0° L:0°");
}

static lv_obj_t * slider_label;
static void slider_event_cb(lv_event_t * e)
{
    lv_obj_t * slider = lv_event_get_target(e);
    lv_label_set_text_fmt(slider_label,"Brightness : %d",(int)lv_slider_get_value(slider));
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);
    lcd.setBrightness((int)lv_slider_get_value(slider));
}

static void tux_panel_config(lv_obj_t *parent)
{
    /******** CONFIG & TESTING ********/
    lv_obj_t *island_2 = tux_panel_create(parent, LV_SYMBOL_EDIT " CONFIG", 200);
    lv_obj_add_style(island_2, &style_ui_island, 0);

    // Get Content Area to add UI elements
    lv_obj_t *cont_2 = tux_panel_get_content(island_2);

    lv_obj_set_flex_flow(cont_2, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_style_pad_row(cont_2, 10, 0);
    //lv_obj_set_style_pad_column(cont_2, 5, 0);
    lv_obj_set_flex_align(cont_2, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END);

    // Screen Brightness
    /*Create a label below the slider*/
    slider_label = lv_label_create(cont_2);
    lv_label_set_text_fmt(slider_label, "Brightness : %d", lcd.getBrightness());

    lv_obj_t * slider = lv_slider_create(cont_2);
    lv_obj_center(slider);
    lv_obj_set_size(slider, LV_PCT(90), 20);
    lv_slider_set_range(slider, 50 , 255);
    lv_obj_add_event_cb(slider, slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_align_to(slider_label, slider, LV_ALIGN_OUT_TOP_MID, 0, 30);
    lv_bar_set_value(slider,lcd.getBrightness(),LV_ANIM_ON);

    // THEME Selection
    lv_obj_t *label = lv_label_create(cont_2);
    lv_label_set_text(label, "Theme : Dark");
    //lv_obj_set_size(label, LV_PCT(90), 20);
    lv_obj_align_to(label,slider,LV_ALIGN_OUT_TOP_MID,0,15);
    //lv_obj_center(slider);

    lv_obj_t *sw = lv_switch_create(cont_2);
    lv_obj_add_event_cb(sw, theme_switch_event_handler, LV_EVENT_ALL, label);
    lv_obj_align_to(label, sw, LV_ALIGN_OUT_TOP_MID, 0, 20);
    //lv_obj_align(sw,LV_ALIGN_RIGHT_MID,0,0);

    // Rotate to Portait/Landscape
    lv_obj_t *btn2 = lv_btn_create(cont_2);
    lv_obj_align(btn2, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(btn2, LV_SIZE_CONTENT, 30);
    lv_obj_add_event_cb(btn2, rotate_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_t *lbl2 = lv_label_create(btn2);
    lv_label_set_text(lbl2, "Rotate to Landscape");
    //lv_obj_center(lbl2);
    lv_obj_align_to(btn2, sw, LV_ALIGN_OUT_BOTTOM_MID, 0, 15);
}

// Provision WIFI
static void tux_panel_wifi(lv_obj_t *parent)
{
    /******** PROVISION WIFI ********/
    island_wifi = tux_panel_create(parent, LV_SYMBOL_WIFI " WIFI STATUS", 270);
    lv_obj_add_style(island_wifi, &style_ui_island, 0);
    // tux_panel_set_title_color(island_wifi, lv_palette_main(LV_PALETTE_BLUE));

    // Get Content Area to add UI elements
    lv_obj_t *cont_1 = tux_panel_get_content(island_wifi);
    lv_obj_set_flex_flow(cont_1, LV_FLEX_FLOW_COLUMN_WRAP);
    lv_obj_set_flex_align(cont_1, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lbl_wifi_status = lv_label_create(cont_1);
    lv_obj_set_size(lbl_wifi_status, LV_SIZE_CONTENT, 30);
    lv_obj_align(lbl_wifi_status, LV_ALIGN_LEFT_MID, 0, 0);
    lv_label_set_text(lbl_wifi_status, "Waiting for IP");

    // Reset wi-fi settings button
    lv_obj_t *btn_unprov = lv_btn_create(cont_1);
    lv_obj_set_size(btn_unprov, LV_SIZE_CONTENT, 40);
    lv_obj_align(btn_unprov, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t *lbl2 = lv_label_create(btn_unprov);
    lv_label_set_text(lbl2, "Reset Wi-Fi Settings");
    lv_obj_center(lbl2);

    /* ESP QR CODE inserted here */
    qr_status_container = lv_obj_create(cont_1);
    lv_obj_set_size(qr_status_container, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(qr_status_container, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_ver(qr_status_container, 3, 0);
    lv_obj_set_style_border_width(qr_status_container, 0, 0);
    lv_obj_set_flex_flow(qr_status_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(qr_status_container, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_add_event_cb(btn_unprov, espwifi_event_handler, LV_EVENT_CLICKED, NULL);

    /* QR CODE */
    lv_color_t bg_color = lv_palette_lighten(LV_PALETTE_GREY, 4);
    lv_color_t fg_color = lv_palette_darken(LV_PALETTE_BLUE, 4);

    prov_qr = lv_qrcode_create(qr_status_container, 100, fg_color, bg_color);

    /* Set data - format of BLE provisioning data */
    // {"ver":"v1","name":"TUX_4AA440","pop":"abcd1234","transport":"ble"}
    const char *qrdata = "https://github.com/nesitor/CryptoChart";
    lv_qrcode_update(prov_qr, qrdata, strlen(qrdata));

    /*Add a border with bg_color*/
    lv_obj_set_style_border_color(prov_qr, bg_color, 0);
    lv_obj_set_style_border_width(prov_qr, 5, 0);

    lbl_scan_status = lv_label_create(qr_status_container);
    lv_obj_set_size(lbl_scan_status, LV_SIZE_CONTENT, 30);
    lv_label_set_text(lbl_scan_status, "Scan to learn about CryptoChart");

}

static void tux_panel_ota(lv_obj_t *parent)
{
    /******** OTA UPDATES ********/
    island_ota = tux_panel_create(parent, LV_SYMBOL_DOWNLOAD " OTA UPDATES", 180);
    lv_obj_add_style(island_ota, &style_ui_island, 0);

    // Get Content Area to add UI elements
    lv_obj_t *cont_ota = tux_panel_get_content(island_ota);
    lv_obj_set_flex_flow(cont_ota, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(cont_ota, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    // Current Firmware version
    lbl_version = lv_label_create(cont_ota);
    lv_obj_set_size(lbl_version, LV_SIZE_CONTENT, 30);
    lv_obj_align(lbl_version, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text_fmt(lbl_version, "Firmware Version %s",get_firmware_version());

    // Check for Updates button
    lv_obj_t *btn_checkupdates = lv_btn_create(cont_ota);
    lv_obj_set_size(btn_checkupdates, LV_SIZE_CONTENT, 40);
    lv_obj_align(btn_checkupdates, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t *lbl2 = lv_label_create(btn_checkupdates);
    lv_label_set_text(lbl2, "Check for Updates");
    lv_obj_center(lbl2);
    lv_obj_add_event_cb(btn_checkupdates, checkupdates_event_handler, LV_EVENT_ALL, NULL);

    lv_obj_t *esp_updatestatus = lv_obj_create(cont_ota);
    lv_obj_set_size(esp_updatestatus, LV_PCT(100), LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(esp_updatestatus, LV_OPA_10, 0);
    lv_obj_set_style_border_width(esp_updatestatus, 0, 0);

    lbl_update_status = lv_label_create(esp_updatestatus);
    lv_obj_set_style_text_color(lbl_update_status, lv_palette_main(LV_PALETTE_YELLOW), 0);
    lv_obj_align(lbl_update_status, LV_ALIGN_CENTER, 0, 0);
    lv_label_set_text(lbl_update_status, "Click to check for updates");
}

static void tux_panel_devinfo(lv_obj_t *parent)
{
    island_devinfo = tux_panel_create(parent, LV_SYMBOL_TINT " DEVICE INFO", 200);
    lv_obj_add_style(island_devinfo, &style_ui_island, 0);

    // Get Content Area to add UI elements
    lv_obj_t *cont_devinfo = tux_panel_get_content(island_devinfo);

    lbl_device_info = lv_label_create(cont_devinfo);
    // Monoaspace font for alignment
    lv_obj_set_style_text_font(lbl_device_info,&font_robotomono_13,0);
}

static void graph_event_handler(lv_event_t * e)
{
    static int32_t last_id = -1;
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        last_id = lv_chart_get_pressed_point(obj);
        if(last_id != LV_CHART_POINT_NONE) {
            lv_chart_set_cursor_point(obj, cursor, NULL, last_id);
        }
    }
    else if(code == LV_EVENT_DRAW_PART_END) {
        lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
        if(dsc->part == LV_PART_CURSOR && dsc->p1 && dsc->p2 && dsc->p1->y == dsc->p2->y && last_id >= 0) {
            lv_coord_t * data_array = lv_chart_get_y_array(chart, ser);
            lv_coord_t v = data_array[last_id];
            char buf[16];
            lv_snprintf(buf, sizeof(buf), "%d", v);

            lv_point_t size;
            lv_txt_get_size(&size, buf, LV_FONT_DEFAULT, 0, 0, LV_COORD_MAX, LV_TEXT_FLAG_NONE);

            lv_area_t a;
            a.y2 = dsc->p1->y - 5;
            a.y1 = a.y2 - size.y - 10;
            a.x1 = dsc->p1->x + 10;
            a.x2 = a.x1 + size.x + 10;

            lv_draw_rect_dsc_t draw_rect_dsc;
            lv_draw_rect_dsc_init(&draw_rect_dsc);
            draw_rect_dsc.bg_color = lv_palette_main(LV_PALETTE_BLUE);
            draw_rect_dsc.radius = 3;

            lv_draw_rect(dsc->draw_ctx, &draw_rect_dsc, &a);

            lv_draw_label_dsc_t draw_label_dsc;
            lv_draw_label_dsc_init(&draw_label_dsc);
            draw_label_dsc.color = lv_color_white();
            a.x1 += 5;
            a.x2 -= 5;
            a.y1 += 5;
            a.y2 -= 5;
            lv_draw_label(dsc->draw_ctx, &draw_label_dsc, &a, buf, NULL);
        }
    }
}

static void create_page_home(lv_obj_t *parent)
{
    /* HOME PAGE PANELS */
    tux_panel_clock_weather(parent);
    //tux_panel_devinfo(parent);
}

///////////////////// SCREENS ////////////////////
void create_page_crypto(lv_obj_t *parent)
{
    ui_cryptoPanel = lv_obj_create(parent);
    lv_obj_set_size(ui_cryptoPanel, 440, screenHeight - HEADER_HEIGHT - 10);
    lv_obj_set_align(ui_cryptoPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_cryptoPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_cryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_cryptoPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_cryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_cryptoPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_cryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_titleCryptoPanel = lv_obj_create(ui_cryptoPanel);
    lv_obj_set_width(ui_titleCryptoPanel, 440);
    lv_obj_set_height(ui_titleCryptoPanel, 25);
    lv_obj_set_x(ui_titleCryptoPanel, 0);
    lv_obj_set_y(ui_titleCryptoPanel, -119);
    lv_obj_set_align(ui_titleCryptoPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_titleCryptoPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_titleCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_titleCryptoPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_titleCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_titleCryptoPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_titleCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_cryptoImage = lv_img_create(ui_titleCryptoPanel);
    lv_img_set_src(ui_cryptoImage, &img_btc);
    lv_obj_set_width(ui_cryptoImage, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_cryptoImage, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_cryptoImage, -32);
    lv_obj_set_y(ui_cryptoImage, 0);
    lv_obj_set_align(ui_cryptoImage, LV_ALIGN_LEFT_MID);
    lv_obj_add_flag(ui_cryptoImage, LV_OBJ_FLAG_ADV_HITTEST);     /// Flags
    lv_obj_clear_flag(ui_cryptoImage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_img_set_zoom(ui_cryptoImage, 100);

    ui_cryptoLabel = lv_label_create(ui_titleCryptoPanel);
    lv_obj_set_width(ui_cryptoLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_cryptoLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_cryptoLabel, 20);
    lv_obj_set_y(ui_cryptoLabel, 0);
    lv_obj_set_align(ui_cryptoLabel, LV_ALIGN_LEFT_MID);
    lv_label_set_text(ui_cryptoLabel, "Bitcoin / USD");
    lv_obj_set_style_text_color(ui_cryptoLabel, lv_color_hex(0xFF8C00), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_cryptoLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_priceLabel = lv_label_create(ui_titleCryptoPanel);
    lv_obj_add_style(ui_priceLabel, &style_price, 0);
    lv_obj_add_style(ui_priceLabel, &style_loose, 0);
    lv_label_set_text(ui_priceLabel, "$ 25.253");

    // MSG - MSG_CRYPTO_CHANGED - EVENT
    lv_obj_add_event_cb(ui_cryptoPanel, crypto_event_cb, LV_EVENT_MSG_RECEIVED, NULL);
    lv_msg_subsribe_obj(MSG_CRYPTO_CHANGED, ui_cryptoPanel, NULL);

    ui_assetsCryptoPanel = lv_obj_create(ui_cryptoPanel);
    lv_obj_set_width(ui_assetsCryptoPanel, 440);
    lv_obj_set_height(ui_assetsCryptoPanel, 40);
    lv_obj_set_x(ui_assetsCryptoPanel, 0);
    lv_obj_set_y(ui_assetsCryptoPanel, -91);
    lv_obj_set_align(ui_assetsCryptoPanel, LV_ALIGN_CENTER);
    lv_obj_clear_flag(ui_assetsCryptoPanel, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_assetsCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_assetsCryptoPanel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_assetsCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_assetsCryptoPanel, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_assetsCryptoPanel, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_amountLabel = lv_label_create(ui_assetsCryptoPanel);
    lv_obj_set_width(ui_amountLabel, 220);
    lv_obj_set_height(ui_amountLabel, 15);
    lv_obj_set_x(ui_amountLabel, -10);
    lv_obj_set_y(ui_amountLabel, 0);
    lv_obj_add_style(ui_amountLabel, &style_amount, 0);
    lv_label_set_text(ui_amountLabel, "0.03536482 BTC | 2.205 US$");

    ui_changesCryptoPanel1 = lv_obj_create(ui_assetsCryptoPanel);
    lv_obj_set_width(ui_changesCryptoPanel1, 190);
    lv_obj_set_height(ui_changesCryptoPanel1, 15);
    lv_obj_set_align(ui_changesCryptoPanel1, LV_ALIGN_RIGHT_MID);
    lv_obj_set_flex_flow(ui_changesCryptoPanel1, LV_FLEX_FLOW_ROW_WRAP);
    lv_obj_set_flex_align(ui_changesCryptoPanel1, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_clear_flag(ui_changesCryptoPanel1, LV_OBJ_FLAG_SCROLLABLE);      /// Flags
    lv_obj_set_style_radius(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(ui_changesCryptoPanel1, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(ui_changesCryptoPanel1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_top(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_bottom(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_row(ui_changesCryptoPanel1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(ui_changesCryptoPanel1, 2, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_changesLabel1 = lv_label_create(ui_changesCryptoPanel1);
    lv_obj_add_style(ui_changesLabel1, &style_change, 0);
    lv_obj_add_style(ui_changesLabel1, &style_win, 0);
    lv_label_set_text(ui_changesLabel1, "1H:  - 23%");

    ui_divisorLabel1 = lv_label_create(ui_changesCryptoPanel1);
    lv_obj_add_style(ui_divisorLabel1, &style_change_divisor, 0);
    lv_label_set_text(ui_divisorLabel1, " | ");

    ui_changesLabel2 = lv_label_create(ui_changesCryptoPanel1);
    lv_obj_add_style(ui_changesLabel2, &style_change, 0);
    lv_obj_add_style(ui_changesLabel2, &style_loose, 0);
    lv_label_set_text(ui_changesLabel2, " 1D: + 1.05%");

    ui_divisorLabel2 = lv_label_create(ui_changesCryptoPanel1);
    lv_obj_add_style(ui_divisorLabel2, &style_change_divisor, 0);
    lv_label_set_text(ui_divisorLabel2, " | ");

    ui_changesLabel3 = lv_label_create(ui_changesCryptoPanel1);
    lv_obj_add_style(ui_changesLabel3, &style_change, 0);
    lv_obj_add_style(ui_changesLabel3, &style_win, 0);
    lv_label_set_text(ui_changesLabel3, "7D: - 50%");

    // CHART
    chart = lv_chart_create(ui_cryptoPanel);
    lv_obj_set_size(chart, 420, 190);
    lv_obj_align(chart, LV_ALIGN_CENTER, 0, 35);

    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 0, 0, 0, true, 50);
    lv_chart_set_axis_tick(chart, LV_CHART_AXIS_PRIMARY_X, 0, 0, 5, 2, true, 50);

    lv_chart_set_update_mode(chart, LV_CHART_UPDATE_MODE_SHIFT);
    lv_chart_set_point_count(chart, 16);

    lv_obj_add_event_cb(chart, graph_event_handler, LV_EVENT_ALL, NULL);
    lv_obj_refresh_ext_draw_size(chart);

    cursor = lv_chart_add_cursor(chart, lv_palette_main(LV_PALETTE_BLUE), LV_DIR_LEFT | LV_DIR_BOTTOM);

    ser = lv_chart_add_series(chart, lv_color_hex(0xFF8C00), LV_CHART_AXIS_PRIMARY_Y);
    uint32_t i;
    for(i = 0; i <= 15; i++) {
        lv_chart_set_next_value(chart, ser, lv_rand(20,80));
    }

    lv_obj_set_style_radius(chart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(chart, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(chart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_color(chart, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_opa(chart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_width(chart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_outline_pad(chart, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

    lv_obj_set_style_bg_color(chart, lv_color_hex(0x000000), LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_opa(chart, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_size(chart, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

}

static void create_page_settings(lv_obj_t *parent)
{
    /* SETTINGS PAGE PANELS */
    tux_panel_wifi(parent);
    //tux_panel_config(parent);
}

static void create_page_updates(lv_obj_t *parent)
{
    /* OTA UPDATES PAGE PANELS */
    tux_panel_ota(parent);
    tux_panel_devinfo(parent);
}

static void create_splash_screen()
{
    lv_obj_t * splash_screen = lv_scr_act();
    lv_obj_set_style_bg_color(splash_screen, lv_color_black(),0);
    lv_obj_t * splash_img = lv_img_create(splash_screen);
    lv_img_set_src(splash_img, "F:/bg/tux-logo.bin"); //&tux_logo);
    lv_obj_align(splash_img, LV_ALIGN_CENTER, 0, 0);

    //lv_scr_load_anim(splash_screen, LV_SCR_LOAD_ANIM_FADE_IN, 5000, 10, true);
    lv_scr_load(splash_screen);
}

static void show_ui()
{
    // screen_container is the root of the UX
    screen_container = lv_obj_create(NULL);

    lv_obj_set_size(screen_container, LV_PCT(100), LV_PCT(100));
    lv_obj_set_style_pad_all(screen_container, 0, 0);
    lv_obj_align(screen_container, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_border_width(screen_container, 0, 0);
    lv_obj_set_scrollbar_mode(screen_container, LV_SCROLLBAR_MODE_OFF);

    // HEADER & FOOTER
    create_header(screen_container);
    //create_footer(screen_container);

    // CONTENT CONTAINER
    content_container = lv_obj_create(screen_container);
    lv_obj_set_size(content_container, screen_w, screen_h - HEADER_HEIGHT);
    lv_obj_align(content_container, LV_ALIGN_TOP_MID, 0, HEADER_HEIGHT);
    lv_obj_set_style_border_width(content_container, 0, 0);
    lv_obj_set_style_bg_opa(content_container, LV_OPA_TRANSP, 0);

    lv_obj_set_flex_flow(content_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_clear_flag(content_container, LV_OBJ_FLAG_SCROLLABLE);

    // Show Home Page
    create_page_crypto(content_container);
    create_page_settings(content_container);
    //create_page_crypto(content_container);

    // Load main screen with animation
    //lv_scr_load(screen_container);
    lv_scr_load_anim(screen_container, LV_SCR_LOAD_ANIM_FADE_IN, 1000,100, true);

    // Status subscribers
    lv_msg_subsribe(MSG_WIFI_PROV_MODE, status_change_cb, NULL);
    lv_msg_subsribe(MSG_WIFI_CONNECTED, status_change_cb, NULL);
    lv_msg_subsribe(MSG_WIFI_DISCONNECTED, status_change_cb, NULL);
    lv_msg_subsribe(MSG_OTA_STATUS, status_change_cb, NULL);
    lv_msg_subsribe(MSG_SDCARD_STATUS, status_change_cb, NULL);
    lv_msg_subsribe(MSG_BATTERY_STATUS, status_change_cb, NULL);
    lv_msg_subsribe(MSG_DEVICE_INFO, status_change_cb, NULL);

    // Send default page load notification => HOME
    lv_msg_send(MSG_PAGE_HOME,NULL);
}

static void rotate_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    lv_obj_t *label = lv_obj_get_child(btn, 0);

    if (code == LV_EVENT_CLICKED)
    {
        lvgl_acquire();

        if (lv_disp_get_rotation(disp) == LV_DISP_ROT_270)
            lv_disp_set_rotation(disp, LV_DISP_ROT_NONE);
        else
            lv_disp_set_rotation(disp, (lv_disp_rot_t)(lv_disp_get_rotation(disp) + 1));

        if (LV_HOR_RES > LV_VER_RES)
            lv_label_set_text(label, "Rotate to Portrait");
        else
            lv_label_set_text(label, "Rotate to Landscape");

        lvgl_release();

        // Update
        screen_h = lv_obj_get_height(lv_scr_act());
        screen_w = lv_obj_get_width(lv_scr_act());
        lv_obj_set_size(content_container, screen_w, screen_h - HEADER_HEIGHT - FOOTER_HEIGHT);

        // footer_message("%d,%d",screen_h,screen_w);
    }
}

static void theme_switch_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    lv_obj_t *udata = (lv_obj_t *)lv_event_get_user_data(e);

    if (code == LV_EVENT_VALUE_CHANGED)
    {
        LV_LOG_USER("State: %s\n", lv_obj_has_state(obj, LV_STATE_CHECKED) ? "On" : "Off");
        if (lv_obj_has_state(obj, LV_STATE_CHECKED))
        {
            switch_theme(false);
            lv_label_set_text(udata, "Theme : Light");

            // Pass the new theme info
            // ESP_ERROR_CHECK(esp_event_post(TUX_EVENTS, TUX_EVENT_THEME_CHANGED, NULL,NULL, portMAX_DELAY));
        }
        else
        {
            switch_theme(true);
            lv_label_set_text(udata, "Theme : Dark");

            // Pass the new theme info
            // ESP_ERROR_CHECK(esp_event_post(TUX_EVENTS, TUX_EVENT_THEME_CHANGED, NULL,NULL, portMAX_DELAY));
        }
    }
}

static void footer_message(const char *fmt, ...)
{
    char buffer[200];
    va_list args;
    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    lv_label_set_text(label_message, buffer);
    va_end(args);
}

static void home_clicked_eventhandler(lv_event_t *e)
{
    // footer_message("Home clicked!");
    //  Clean the content container first
    lv_obj_clean(content_container);
    create_page_crypto(content_container);
}

static void status_clicked_eventhandler(lv_event_t *e)
{
    // footer_message("Status icons touched but this is a very long message to show scroll animation!");
    //  Clean the content container first
    lv_obj_clean(content_container);
    create_page_settings(content_container);
    //create_page_crypto(content_container);
}

void switch_theme(bool dark)
{
    if (dark)
    {
        theme_current = lv_theme_default_init(disp, lv_palette_main(LV_PALETTE_BLUE),
                                              lv_palette_main(LV_PALETTE_GREEN),
                                              1, /*Light or dark mode*/
                                              &lv_font_montserrat_14);
        bg_theme_color = lv_palette_darken(LV_PALETTE_GREY, 5);
        lv_disp_set_theme(disp, theme_current);
        //bg_theme_color = theme_current->flags & LV_USE_THEME_DEFAULT ? lv_palette_darken(LV_PALETTE_GREY, 5) : lv_palette_lighten(LV_PALETTE_GREY, 2);
        // lv_theme_set_apply_cb(theme_current, new_theme_apply_cb);

        lv_style_set_bg_color(&style_ui_island, bg_theme_color);
        //lv_style_set_bg_opa(&style_ui_island, LV_OPA_80);

        ESP_LOGI(TAG,"Dark theme set");
    }
    else
    {
        theme_current = lv_theme_default_init(disp,
                                              lv_palette_main(LV_PALETTE_BLUE),
                                              lv_palette_main(LV_PALETTE_RED),
                                              0, /*Light or dark mode*/
                                              &lv_font_montserrat_14);
        //bg_theme_color = lv_palette_lighten(LV_PALETTE_GREY, 5);    // #BFBFBD
        // bg_theme_color = lv_color_make(0,0,255);
        bg_theme_color = lv_color_hex(0xBFBFBD); //383837


        lv_disp_set_theme(disp, theme_current);
        // lv_theme_set_apply_cb(theme_current, new_theme_apply_cb);
        lv_style_set_bg_color(&style_ui_island, bg_theme_color);
        ESP_LOGI(TAG,"Light theme set");

    }
}

// /*Will be called when the styles of the base theme are already added
//   to add new styles*/
// static void new_theme_apply_cb(lv_theme_t * th, lv_obj_t * obj)
// {
//     LV_UNUSED(th);

//     if(lv_obj_check_type(obj, &tux_panel_class)) {
//         lv_obj_add_style(obj, &style_ui_island, 0);
//         //lv_style_set_bg_color(&style_ui_island,theme_current->color_primary);
//     }

// }

static void espwifi_event_handler(lv_event_t* e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        bool provisioned = false;
        ESP_ERROR_CHECK(wifi_prov_mgr_is_provisioned(&provisioned));
        if (provisioned) {
            wifi_prov_mgr_reset_provisioning();     // reset wifi

            // Reset device to start provisioning
            lv_label_set_text(lbl_wifi_status, "Wi-Fi Disconnected!");
            lv_obj_set_style_text_color(lbl_wifi_status, lv_palette_main(LV_PALETTE_YELLOW), 0);
            lv_label_set_text(lbl_scan_status, "Restart device to provision WiFi.");
            lv_obj_add_state( btn, LV_STATE_DISABLED );  /* Disable */
        }
    }
}

inline void checkupdates_event_handler(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *btn = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        /*Get the first child of the button which is the label and change its text*/
        // Maybe disable the button and enable once completed
        //lv_obj_t *label = lv_obj_get_child(btn, 0);
        //lv_label_set_text_fmt(label, "Checking for updates...");
        LV_LOG_USER("Clicked");
        lv_msg_send(MSG_OTA_INITIATE,NULL);
    }
}

static const char* get_firmware_version()
{
    const esp_partition_t *running = esp_ota_get_running_partition();
    esp_app_desc_t running_app_info;
    if (esp_ota_get_partition_description(running, &running_app_info) == ESP_OK) {
        return fmt::format("{}",running_app_info.version).c_str();
    }
    return "0.0.0";
}

void datetime_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    //lv_event_get_target(e) => cont_datetime
    lv_msg_t * m = lv_event_get_msg(e);

    // Not necessary but if event target was button or so, then required
    if (code == LV_EVENT_MSG_RECEIVED)
    {
        struct tm *dtinfo = (tm*)lv_msg_get_payload(m);
        // Date & Time formatted
        char strftime_buf[64];
        // strftime(strftime_buf, sizeof(strftime_buf), "%c %z", dtinfo);
        // ESP_LOGW(TAG,"Triggered:datetime_event_cb %s",strftime_buf);

        // Date formatted
        strftime(strftime_buf, sizeof(strftime_buf), "%a, %e %b %Y", dtinfo);
        lv_label_set_text_fmt(lbl_date,"%s",strftime_buf);

        // Time in 12hrs
        strftime(strftime_buf, sizeof(strftime_buf), "%I:%M", dtinfo);
        lv_label_set_text_fmt(lbl_time, "%s", strftime_buf);

        // 12hr clock AM/PM
        strftime(strftime_buf, sizeof(strftime_buf), "%p", dtinfo);
        lv_label_set_text_fmt(lbl_ampm, "%s", strftime_buf);
    }
}

void weather_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_msg_t * m = lv_event_get_msg(e);

    // Not necessary but if event target was button or so, then required
    if (code == LV_EVENT_MSG_RECEIVED)
    {
        OpenWeatherMap *e_owm = NULL;
        e_owm = (OpenWeatherMap*)lv_msg_get_payload(m);
        //ESP_LOGW(TAG,"weather_event_cb %s",e_owm->LocationName.c_str());

        // set this according to e_owm->WeatherIcon
        set_weather_icon(e_owm->WeatherIcon);

        lv_label_set_text(lbl_temp,fmt::format("{:.1f}°{}",e_owm->Temperature,e_owm->TemperatureUnit).c_str());
        lv_label_set_text(lbl_hl,fmt::format("H:{:.1f}° L:{:.1f}°",e_owm->TemperatureHigh,e_owm->TemperatureLow).c_str());
    }
}

void crypto_event_cb(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_msg_t * m = lv_event_get_msg(e);

    // Not necessary but if event target was button or so, then required
    if (code == LV_EVENT_MSG_RECEIVED)
    {
        CoinMarketCap *e_cmc = NULL;
        e_cmc = (CoinMarketCap*)lv_msg_get_payload(m);

        float e_change1h = e_cmc->Change1h;
        float e_change24h = e_cmc->Change24h;
        float e_change7d = e_cmc->Change7d;

        if (e_change1h > 0) {
            lv_obj_add_style(ui_priceLabel, &style_win, 0);
            lv_obj_add_style(ui_changesLabel1, &style_win, 0);
            lv_obj_add_style(ui_amountLabel, &style_win, 0);
        } else {
            lv_obj_add_style(ui_priceLabel, &style_loose, 0);
            lv_obj_add_style(ui_changesLabel1, &style_loose, 0);
            lv_obj_add_style(ui_amountLabel, &style_loose, 0);
        }

        if (e_change24h > 0) {
            lv_obj_add_style(ui_changesLabel2, &style_win, 0);
        } else {
            lv_obj_add_style(ui_changesLabel2, &style_loose, 0);
        }

        if (e_change7d > 0) {
            lv_obj_add_style(ui_changesLabel3, &style_win, 0);
        } else {
            lv_obj_add_style(ui_changesLabel3, &style_loose, 0);
        }

        lv_label_set_text(ui_cryptoLabel,fmt::format("{} ({}) / {}", e_cmc->Name, e_cmc->Symbol, e_cmc->Quote).c_str());
        lv_label_set_text(ui_priceLabel,fmt::format("$ {:.2f}", e_cmc->Price).c_str());
        lv_label_set_text(ui_amountLabel,fmt::format("{:.6f} {} | {:.2f} US$", e_cmc->Amount, e_cmc->Symbol, e_cmc->AmountValue).c_str());
        lv_label_set_text(ui_changesLabel1,fmt::format("1H: {:+.2f}%", e_change1h).c_str());
        lv_label_set_text(ui_changesLabel2,fmt::format("1D: {:+.2f}%", e_change24h).c_str());
        lv_label_set_text(ui_changesLabel3,fmt::format("7D: {:+.2f}%", e_change7d).c_str());

        int minim = 0;
        int maxim = 0;
        for(int value : e_cmc->ChartValues)
        {
            int fixed_value = value;
            lv_chart_set_next_value(chart, ser, fixed_value);
            if (fixed_value > maxim) {
                maxim = fixed_value;
            }
            if (fixed_value < minim || minim == 0) {
                minim = fixed_value;
            }
        }
        int max_scale = maxim + (int) ((float) maxim * (float) 0.01);
        int min_scale = minim - (int) ((float) minim * (float) 0.01);
        ESP_LOGI(TAG,"Updating chart scale: %d min & %d max", min_scale, max_scale);
        lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, min_scale, max_scale);
        lv_chart_set_range(chart, LV_CHART_AXIS_SECONDARY_Y, min_scale, max_scale);
    }
}

static void footer_button_event_handler(lv_event_t * e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t * obj = lv_event_get_target(e);

    if(code == LV_EVENT_VALUE_CHANGED) {
        uint32_t page_id = lv_btnmatrix_get_selected_btn(obj);
        const char * txt = lv_btnmatrix_get_btn_text(obj, page_id);
        printf("[%ld] %s was pressed\n", page_id,txt);

        // Do not refresh the page if its not changed
        if (current_page != page_id) current_page = page_id;
        else return;    // Skip if no page change

        // HOME
        if (page_id==MSG_PAGE_HOME)  {
            lv_obj_clean(content_container);
            create_page_home(content_container);
            anim_move_left_x(content_container,screen_w,0,200);
            lv_msg_send(MSG_PAGE_HOME,NULL);
        }
        // REMOTE
        else if (page_id == MSG_PAGE_REMOTE) {
            lv_obj_clean(content_container);
            create_page_crypto(content_container);
            anim_move_left_x(content_container,screen_w,0,200);
            lv_msg_send(MSG_PAGE_REMOTE,NULL);
        }
        // SETTINGS
        else if (page_id == MSG_PAGE_SETTINGS) {
            lv_obj_clean(content_container);
            create_page_settings(content_container);
            anim_move_left_x(content_container,screen_w,0,200);
            lv_msg_send(MSG_PAGE_SETTINGS,NULL);
        }
        // OTA UPDATES
        else if (page_id == MSG_PAGE_OTA) {
            lv_obj_clean(content_container);
            create_page_updates(content_container);
            anim_move_left_x(content_container,screen_w,0,200);
            lv_msg_send(MSG_PAGE_OTA,NULL);
        }
    }
}

static void status_change_cb(void * s, lv_msg_t *m)
{
    LV_UNUSED(s);
    unsigned int msg_id = lv_msg_get_id(m);
    const char * msg_payload = (const char *)lv_msg_get_payload(m);
    const char * msg_data = (const char *)lv_msg_get_user_data(m);

    switch (msg_id)
    {
        case MSG_WIFI_PROV_MODE:
        {
            ESP_LOGW(TAG,"[%d] MSG_WIFI_PROV_MODE",msg_id);
            // Update QR code for PROV and wifi symbol to red?
            lv_style_set_text_color(&style_wifi, lv_palette_main(LV_PALETTE_GREY));
            lv_label_set_text(icon_wifi, LV_SYMBOL_WIFI);

            char qr_data[150] = {0};
            snprintf(qr_data,sizeof(qr_data),"%s",(const char*)lv_msg_get_payload(m));
            lv_qrcode_update(prov_qr, qr_data, strlen(qr_data));
            lv_label_set_text(lbl_scan_status, "Install 'ESP SoftAP Prov' App & Scan");
        }
        break;
        case MSG_WIFI_CONNECTED:
        {
            ESP_LOGW(TAG,"[%d] MSG_WIFI_CONNECTED",msg_id);
            lv_style_set_text_color(&style_wifi, lv_palette_main(LV_PALETTE_BLUE));
            lv_label_set_text(icon_wifi, LV_SYMBOL_WIFI);

            if (lv_msg_get_payload(m) != NULL) {
                char ip_data[20]={0};
                // IP address in the payload so display
                snprintf(ip_data,sizeof(ip_data),"%s",(const char*)lv_msg_get_payload(m));
                lv_label_set_text_fmt(lbl_wifi_status, "IP Address: %s",ip_data);
            }
        }
        break;
        case MSG_WIFI_DISCONNECTED:
        {
            ESP_LOGW(TAG,"[%d] MSG_WIFI_DISCONNECTED",msg_id);
            lv_style_set_text_color(&style_wifi, lv_palette_main(LV_PALETTE_GREY));
            lv_label_set_text(icon_wifi, LV_SYMBOL_WIFI);
        }
        break;
        case MSG_OTA_STATUS:
        {
            ESP_LOGW(TAG,"[%d] MSG_OTA_STATUS",msg_id);
            // Shows different status during OTA update
            char ota_data[150] = {0};
            snprintf(ota_data,sizeof(ota_data),"%s",(const char*)lv_msg_get_payload(m));
            lv_label_set_text(lbl_update_status, ota_data);
        }
        break;
        case MSG_SDCARD_STATUS:
        {
            bool sd_status = *(bool *)lv_msg_get_payload(m);
            ESP_LOGW(TAG,"[%d] MSG_SDCARD_STATUS %d",msg_id,sd_status);
            if (sd_status) {
                lv_style_set_text_color(&style_storage, lv_palette_main(LV_PALETTE_GREEN));
            } else {
                lv_style_set_text_color(&style_storage, lv_palette_main(LV_PALETTE_RED));
            }
        }
        break;
        case MSG_BATTERY_STATUS:
        {
            int battery_val = *(int *)lv_msg_get_payload(m);
            //ESP_LOGW(TAG,"[%d] MSG_BATTERY_STATUS %d",msg_id,battery_val);
            lv_update_battery(battery_val);
        }
        break;
        case MSG_DEVICE_INFO:
        {
            ESP_LOGW(TAG,"[%d] MSG_DEVICE_INFO",msg_id);
            char devinfo_data[300] = {0};
            snprintf(devinfo_data,sizeof(devinfo_data),"%s",(const char*)lv_msg_get_payload(m));
            lv_label_set_text(lbl_device_info,devinfo_data);
        }
        break;
    }
}

static void lv_update_battery(uint batval)
{
    if (batval < 20)
    {
        lv_style_set_text_color(&style_battery, lv_palette_main(LV_PALETTE_RED));
        lv_label_set_text(icon_battery, LV_SYMBOL_BATTERY_EMPTY);
    }
    else if (batval < 50)
    {
        lv_style_set_text_color(&style_battery, lv_palette_main(LV_PALETTE_RED));
        lv_label_set_text(icon_battery, LV_SYMBOL_BATTERY_1);
    }
    else if (batval < 70)
    {
        lv_style_set_text_color(&style_battery, lv_palette_main(LV_PALETTE_DEEP_ORANGE));
        lv_label_set_text(icon_battery, LV_SYMBOL_BATTERY_2);
    }
    else if (batval < 90)
    {
        lv_style_set_text_color(&style_battery, lv_palette_main(LV_PALETTE_GREEN));
        lv_label_set_text(icon_battery, LV_SYMBOL_BATTERY_3);
    }
    else
    {
        lv_style_set_text_color(&style_battery, lv_palette_main(LV_PALETTE_GREEN));
        lv_label_set_text(icon_battery, LV_SYMBOL_BATTERY_FULL);
    }
}

/********************** ANIMATIONS *********************/
void anim_move_left_x(lv_obj_t * TargetObject, int start_x, int end_x, int delay)
{
    lv_anim_t property_anim;
    lv_anim_init(&property_anim);
    lv_anim_set_time(&property_anim, 200);
    lv_anim_set_user_data(&property_anim, TargetObject);
    lv_anim_set_custom_exec_cb(&property_anim, tux_anim_callback_set_x);
    lv_anim_set_values(&property_anim, start_x, end_x);
    lv_anim_set_path_cb(&property_anim, lv_anim_path_overshoot);
    lv_anim_set_delay(&property_anim, delay + 0);
    lv_anim_set_playback_time(&property_anim, 0);
    lv_anim_set_playback_delay(&property_anim, 0);
    lv_anim_set_repeat_count(&property_anim, 0);
    lv_anim_set_repeat_delay(&property_anim, 0);
    lv_anim_set_early_apply(&property_anim, true);
    lv_anim_start(&property_anim);
}

void tux_anim_callback_set_x(lv_anim_t * a, int32_t v)
{
    lv_obj_set_x((lv_obj_t *)a->user_data, v);
}

void anim_move_left_y(lv_obj_t * TargetObject, int start_y, int end_y, int delay)
{
    lv_anim_t property_anim;
    lv_anim_init(&property_anim);
    lv_anim_set_time(&property_anim, 300);
    lv_anim_set_user_data(&property_anim, TargetObject);
    lv_anim_set_custom_exec_cb(&property_anim, tux_anim_callback_set_y);
    lv_anim_set_values(&property_anim, start_y, end_y);
    lv_anim_set_path_cb(&property_anim, lv_anim_path_overshoot);
    lv_anim_set_delay(&property_anim, delay + 0);
    lv_anim_set_playback_time(&property_anim, 0);
    lv_anim_set_playback_delay(&property_anim, 0);
    lv_anim_set_repeat_count(&property_anim, 0);
    lv_anim_set_repeat_delay(&property_anim, 0);
    lv_anim_set_early_apply(&property_anim, true);
    lv_anim_start(&property_anim);
}

void tux_anim_callback_set_y(lv_anim_t * a, int32_t v)
{
    lv_obj_set_y((lv_obj_t *)a->user_data, v);
}

void anim_fade_in(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t property_anim;
    lv_anim_init(&property_anim);
    lv_anim_set_time(&property_anim, 3000);
    lv_anim_set_user_data(&property_anim, TargetObject);
    lv_anim_set_custom_exec_cb(&property_anim, tux_anim_callback_set_opacity);
    lv_anim_set_values(&property_anim, 0, 255);
    lv_anim_set_path_cb(&property_anim, lv_anim_path_ease_out);
    lv_anim_set_delay(&property_anim, delay + 0);
    lv_anim_set_playback_time(&property_anim, 0);
    lv_anim_set_playback_delay(&property_anim, 0);
    lv_anim_set_repeat_count(&property_anim, 0);
    lv_anim_set_repeat_delay(&property_anim, 0);
    lv_anim_set_early_apply(&property_anim, false);
    lv_anim_start(&property_anim);

}
void anim_fade_out(lv_obj_t * TargetObject, int delay)
{
    lv_anim_t property_anim;
    lv_anim_init(&property_anim);
    lv_anim_set_time(&property_anim, 1000);
    lv_anim_set_user_data(&property_anim, TargetObject);
    lv_anim_set_custom_exec_cb(&property_anim, tux_anim_callback_set_opacity);
    lv_anim_set_values(&property_anim, 255, 0);
    lv_anim_set_path_cb(&property_anim, lv_anim_path_ease_in_out);
    lv_anim_set_delay(&property_anim, delay + 0);
    lv_anim_set_playback_time(&property_anim, 0);
    lv_anim_set_playback_delay(&property_anim, 0);
    lv_anim_set_repeat_count(&property_anim, 0);
    lv_anim_set_repeat_delay(&property_anim, 0);
    lv_anim_set_early_apply(&property_anim, false);
    lv_anim_start(&property_anim);

}
void tux_anim_callback_set_opacity(lv_anim_t * a, int32_t v)
{
    lv_obj_set_style_opa((lv_obj_t *)a->user_data, v, 0);
}

static void set_weather_icon(string weatherIcon)
{
    /*
        https://openweathermap.org/weather-conditions#Weather-Condition-Codes-2
        d = day / n = night
        01 - clear sky
        02 - few clouds
        03 - scattered clouds
        04 - broken clouds
        09 - shower rain
        10 - rain
        11 - thunderstorm
        13 - snow
        50 - mist
    */
    // lv_color_make(red, green, blue);

    if (weatherIcon.find('d') != std::string::npos) {
        // set daytime color
        // color = whitesmoke = lv_color_make(245, 245, 245)
        // Ideally it should change for each weather - light blue for rain etc...
        lv_obj_set_style_text_color(lbl_weathericon,lv_color_make(241, 235, 156),0);
    } else {
        // set night time color
        lv_obj_set_style_text_color(lbl_weathericon,lv_palette_main(LV_PALETTE_BLUE_GREY),0);
    }

    if (weatherIcon.find("50") != std::string::npos) {     // mist - need icon
        lv_label_set_text(lbl_weathericon,FA_WEATHER_DROPLET);
        return;
    }

    if (weatherIcon.find("13") != std::string::npos) {
        lv_label_set_text(lbl_weathericon,FA_WEATHER_SNOWFLAKES);
        return;
    }

    if (weatherIcon.find("11") != std::string::npos) {
        lv_label_set_text(lbl_weathericon,FA_WEATHER_CLOUD_SHOWERS_HEAVY);
        return;
    }

    if (weatherIcon.find("10") != std::string::npos) {
        lv_label_set_text(lbl_weathericon,FA_WEATHER_CLOUD_RAIN);
        return;
    }

    if (weatherIcon.find("09") != std::string::npos) {
        lv_label_set_text(lbl_weathericon,FA_WEATHER_CLOUD_RAIN);
        return;
    }

    if (weatherIcon.find("04") != std::string::npos) {
        lv_label_set_text(lbl_weathericon,FA_WEATHER_CLOUD);
        return;
    }

    if (weatherIcon.find("03") != std::string::npos) {
        lv_label_set_text(lbl_weathericon,FA_WEATHER_CLOUD);
        return;
    }

    if (weatherIcon.find("02") != std::string::npos) {
        lv_label_set_text(lbl_weathericon,FA_WEATHER_CLOUD);
        return;
    }

    if (weatherIcon.find("01") != std::string::npos) {
        lv_label_set_text(lbl_weathericon,FA_WEATHER_SUN);
        return;
    }


    // default
    lv_label_set_text(lbl_weathericon,FA_WEATHER_CLOUD_SHOWERS_HEAVY);
    lv_obj_set_style_text_color(lbl_weathericon,lv_palette_main(LV_PALETTE_BLUE_GREY),0);


}
